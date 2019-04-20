#version 400 core

layout(location = 0) in vec3 lPos;
layout(location = 1) in vec3 lNormal;
layout(location = 2) in vec2 lUV;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_MVP;
uniform vec3 u_colour;

out vec3 fragmentColour;
out mat4 model;
out vec3 normal;
out vec3 fragPos;
out vec2 uv;

void main()
{
	fragmentColour = u_colour.xyz;
	model = u_model;
	normal = normalize(transpose(inverse(mat3(u_view * u_model))) * lNormal);
	//normal = lNormal;
	fragPos = vec3(u_model * vec4(lPos,1.0));
	uv = lUV;

	gl_Position = u_MVP * vec4(lPos, 1);
};