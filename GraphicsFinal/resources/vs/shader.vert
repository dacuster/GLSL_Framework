#version 330

layout (location = 0) in vec3 aPosition;

uniform float xMove;

void main()
{
	vec3 position = aPosition * 0.4;
	
	position.x += xMove;
	
	gl_Position = vec4(position, 1.0);
}