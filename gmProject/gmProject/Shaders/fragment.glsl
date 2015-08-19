#version 400
in vec2 texture_normal_VS;
uniform sampler2D texSampler;
out vec4 fragment_color;
void main () {
	vec4 mySample = texture(texSampler, vec2(texture_normal_VS.s, 1-texture_normal_VS.t));
	fragment_color = vec4( mySample.rgb, 1.0 );
}