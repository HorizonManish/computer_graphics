// Fragment shader source code

#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;		//which texture unit is used

void main()
{
	FragColor = texture(tex0, texCoord);
}