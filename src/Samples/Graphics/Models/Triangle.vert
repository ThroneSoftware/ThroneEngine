#version 450

layout (set = 0, binding = 0) uniform globalBuffer
{
	mat4 viewProjection;
};

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 textureCoords;

layout (location = 0) out vec2 out_textureCoords;

void main()
{
	gl_Position = viewProjection * vec4(vertexPosition, 1.0f);

	out_textureCoords = textureCoords;
}