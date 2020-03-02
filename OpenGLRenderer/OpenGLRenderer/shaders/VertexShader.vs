#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec2  textureCord;
out vec3  normal;
out vec3  vertexPosition;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


void main()
{
	vertexPosition = (model * vec4(pos, 1.0f)).xyz; 
	gl_Position    = projection * view * vec4(vertexPosition, 1.0f);
	textureCord    = tex;
	normal         = mat3(transpose(inverse(model))) * norm ;
}