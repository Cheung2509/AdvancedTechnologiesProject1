#version 400 core

in vec4 fragmentColour;
in vec3 normal;
in vec3 fragPos;
in vec2 uv;

out vec4 colour;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 pos;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light u_light;
uniform Material u_material;
uniform vec3 u_viewPos;

void main()
{
	//Calculate ambient light
	vec3 ambient = u_light.ambient * u_material.ambient;

	//Calculate diffuse light
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(u_light.pos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_light.diffuse * (diff * u_material.diffuse);

	//Calculate specular light
	vec3 viewDir = normalize(u_viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	vec3 specular = u_light.specular * (spec * u_material.specular);

	vec3 result = ambient + diffuse + specular;
	colour = vec4(result, 1.0);
};