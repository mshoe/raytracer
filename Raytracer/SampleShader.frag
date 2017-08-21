#version 430
in vec3 outColor;
in vec2 texCoord;

out vec4 fc;

uniform sampler2D img_output;

void main()
{
    //fc = vec4(1.0 ,0.0,0.0,1.0);
	fc = texture(img_output, texCoord);
}