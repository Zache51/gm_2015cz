#include "MeshObject.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/******************************* Private *******************************/

bool MeshObject::loadObj(std::string filename, float scale, std::string& mtlFileName,
	std::vector<Point_Obj>& points, std::vector<GLuint>& indices)
{
	std::vector < glm::vec3 > vertices;
	std::vector < glm::vec2 > uvs;
	std::vector < glm::vec3 > normals;

	std::string out = "Reading data from " + filename + "\n";
	fprintf(stdout, out.data());

	FILE * file;
	fopen_s(&file, filename.data(), "r");

	GLuint index = 0;
	while (1){
		char lineHeader[128];

		// load a row into char array
		char res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));

		// Check for End of file
		if (res == EOF) break;


		if (strcmp(lineHeader, "mtllib") == 0)
		{
			char mtlName[100];
			fscanf_s(file, "%s", mtlName, sizeof(mtlName));

			mtlFileName = std::string(mtlName);
		}
		else if (strcmp(lineHeader, "v") == 0)
		{
			// Push vertices into vector < myVec3 >

			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex*glm::vec3(scale));
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			// Push uvs into vector < myVec2 >

			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			// Push normals into vector < myVec3 >

			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			struct face
			{
				GLuint v[3];
				GLuint uv[3];
				GLuint vn[3];
			};

			face faceL;
			fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &faceL.v[0], &faceL.uv[0], &faceL.vn[0], &faceL.v[1], &faceL.uv[1], &faceL.vn[1], &faceL.v[2], &faceL.uv[2], &faceL.vn[2]);

			for (GLuint i = 0; i < 3; i++)
			{
				faceL.v[i] --;
				faceL.uv[i] --;
				faceL.vn[i] --;
			}

			// Put the v/vt/vn into the right order and push into vector<objBuffer> 
			for (size_t j = 0; j < 3; j++)
			{
				Point_Obj temp;

				// Copying vertices
				temp.ver.x = vertices[faceL.v[j]].x;
				temp.ver.y = vertices[faceL.v[j]].y;
				temp.ver.z = vertices[faceL.v[j]].z;

				// Copying texture normals
				temp.uvs[0] = uvs[faceL.uv[j]].x;
				temp.uvs[1] = uvs[faceL.uv[j]].y;

				// Copying vertex normals
				temp.vns[0] = normals[faceL.vn[j]].x;
				temp.vns[1] = normals[faceL.vn[j]].y;
				temp.vns[2] = normals[faceL.vn[j]].z;

				points.push_back(temp);

				// push indexes into index buffer :-)
				indices.push_back(index++);
			}
		}
	}

	fprintf(stdout, "Vertices generated\n");

	//Succes!
	fclose(file);
	return true;
}

bool MeshObject::loadMtl(std::string filename, MtlContainer& mtl)
{
	FILE * file;
	fopen_s(&file, filename.data(), "r");

	std::string out = "Reading data from " + filename + "\n";
	fprintf(stdout, out.data());

	while (1){
		char lineHeader[128];

		// load a row into char array
		char res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));

		// Check for End Of File
		if (res == EOF) break;

		if (strcmp(lineHeader, "Ns") == 0)
		{
			fscanf_s(file, "%f\n", &mtl.Ns);
		}
		else if (strcmp(lineHeader, "Ka") == 0)
		{
			fscanf_s(file, "%f %f %f\n", &mtl.Ka[0], &mtl.Ka[1], &mtl.Ka[2]);
		}
		else if (strcmp(lineHeader, "Kd") == 0)
		{
			fscanf_s(file, "%f %f %f\n", &mtl.Kd[0], &mtl.Kd[1], &mtl.Kd[2]);
		}
		else if (strcmp(lineHeader, "Ks") == 0)
		{
			fscanf_s(file, "%f %f %f\n", &mtl.Ks[0], &mtl.Ks[1], &mtl.Ks[2]);
		}
		else if (strcmp(lineHeader, "map_Kd") == 0)
		{
			char fileName[100];
			fscanf_s(file, "%s", fileName, sizeof(fileName));
			mtl.filenames.push_back(MESH_FOLDER + std::string(fileName));
		}
		/*******************************************************************************/
		// d (ignore)
		/*******************************************************************************/
		/*******************************************************************************/
		// illum (ignore)
		/*******************************************************************************/
	}

	fclose(file);

	fprintf(stdout, "Mtl read\n");

	// Succes!
	return true;
}

bool MeshObject::loadTexture(MtlContainer& mtl)
{
	GLint wi;
	GLint he;
	GLint nrOfBytes;
	
	int nrOfTextures = mtl.filenames.size();

	for (int i = 0; i < nrOfTextures; i++)
	{
		std::string out = "Reading data from " + mtl.filenames[i] + "\n";
		fprintf(stdout, out.data());

		unsigned char* image = stbi_load(mtl.filenames[i].data(), &wi, &he, &nrOfBytes, 3);

		if (image == nullptr)
		{
			std::string toPrint = "Failed to load image " + mtl.filenames[i];
			fprintf(stdout, toPrint.data());
			return false;
		}

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		mtl.textureIDs.push_back(textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (nrOfBytes == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wi, he, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			fprintf(stdout, "Texture read\n");
		}
		else
		{
			fprintf(stdout, "Unexpected number of bytes in texture");
			return false;
		}

		stbi_image_free(image);
	}

	return true;
}

/***********************************************************************/
/******************************* Public ********************************/



MeshObject::MeshObject(std::string filename, float scale)
{
	std::string mtlFilename = "";

	points = std::vector<Point_Obj>();
	indicies = std::vector<GLuint>();

	loadObj(MESH_FOLDER + filename, scale, mtlFilename, points, indicies);
	if (mtlFilename != "")
	{
		fprintf(stdout, "Mtl data found\n");
		loadMtl(MESH_FOLDER + mtlFilename, mtl);
		if (!mtl.filenames.empty())
		{
			fprintf(stdout, "Texture found\n");
			loadTexture(mtl);
		}
	}

	fprintf(stdout, "\n");

	numberOfPoints = points.size();
	numberOfIndicies = indicies.size();
}

void MeshObject::FreeMemory()
{
	points.clear();
	indicies.clear();
}

void MeshObject::SetOffset(GLuint offset)
{
	this->offset = offset;
}

void MeshObject::SetOffsetInd(GLuint offset)
{
	this->offsetInd = offset;
}

void MeshObject::AddIndicies(GLuint add)
{
	for (GLuint i = 0; i < numberOfIndicies; i++)
	{
		indicies[i] += add;
	}
}

GLuint MeshObject::GetNumberOfPoint() const
{
	return numberOfPoints;
}

GLuint MeshObject::GetOffset() const
{
	return offset;
}

GLuint MeshObject::GetOffsetInd() const
{
	return offsetInd;
}

void* MeshObject::GetPointsData()
{
	return points.data();
}

GLuint MeshObject::GetFloatAmount() const
{
	return numberOfPoints * sizeof(Point_Obj);
}


