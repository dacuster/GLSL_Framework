#version 330

layout (location = 0) in vec3 aPosition;

out vec4 vertexColor;

uniform mat4 uModel;

uniform mat4 uProjection;

void main()
{
	
	gl_Position = uProjection * uModel * vec4(aPosition, 1.0);
	vertexColor = vec4(clamp(aPosition, 0.0, 1.0), 1.0);
}