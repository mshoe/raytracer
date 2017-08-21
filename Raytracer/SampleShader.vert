#version 430
layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture;

out vec3 outColor; // Output a color to the fragment shader
out vec2 texCoord;

void main()
{
	gl_Position = vec4(position, 1.0);
	texCoord = texture;
	outColor = color;
}