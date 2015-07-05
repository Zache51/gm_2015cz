#include "MeshObject.hpp"



/******************************* Private *******************************/



bool MeshObject::loadObj(std::string filename, std::string mtlFileName,
	std::vector<Point>& points, std::vector<GLuint>& indices)
{
	std::vector < glm::vec3 > vertices;
	std::vector < glm::vec2 > uvs;
	std::vector < glm::vec3 > normals;

	FILE * file;
	fopen_s(&file, filename.data(), "r");

	fprintf(stdout, "Reading file\n");

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

			//mtlFileName = std::string(mtlName);
		}
		else if (strcmp(lineHeader, "v") == 0)
		{
			// Push vertices into vector < myVec3 >

			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
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
				Point temp;

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

	fprintf(stdout, "Generating vertices\n");

	//Succes!
	fclose(file);
	return true;
}



/***********************************************************************/
/******************************* Public ********************************/



MeshObject::MeshObject(std::string filename)
{
	std::string mtlFilename = "";

	std::vector<Point> points = std::vector<Point>();
	std::vector<GLuint> indicies = std::vector<GLuint>();

	MtlContainer mtl;

	loadObj(MESH_FOLDER + filename, mtlFilename, points, indicies);
}

MeshObject::MeshObject(std::vector<Point> points)
{
	std::string mtlFilename = "";

	this->points = points;
	std::vector<GLuint> indicies = std::vector<GLuint>();

	MtlContainer mtl;
}

std::vector<Point> MeshObject::GetPoints() const
{
	return points;
}

std::vector<GLuint> MeshObject::GetIndicies() const
{
	return indicies;
}

MtlContainer MeshObject::GetMtl() const
{
	return mtl;
}
