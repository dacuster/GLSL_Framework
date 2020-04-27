#version 330

layout (location = 0) in vec3 aPosition;

void main()
{
	vec3 position = aPosition * 0.4;
	gl_Position = vec4(position, 1.0);
}