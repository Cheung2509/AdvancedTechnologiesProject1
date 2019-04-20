#version 400 core

in vec4 fragmentColour;
in mat4 model;
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
	vec3 surfaceToLight = normalize(u_light.pos - fragPos);
	float brightness = dot(normal, surfaceToLight);
	brightness = clamp(brightness,0,1);

	//Calculating ambient
	vec3 ambient = u_light.ambient * fragmentColour.rgb * 1;
	
	//Calculating diffuse
	float diffuseCoefficient = max(0.0, brightness);
	vec3 diffuse = diffuseCoefficient * fragmentColour.rgb * 1;

	//Calculating specula
	vec3 incedence = -surfaceToLight;
	vec3 reflection = reflect(incedence, normal);
	vec3 surfaceToCamera = normalize(u_viewPos - fragPos);
	float angle = max(0, dot(surfaceToCamera, reflection));
	
	float specCoefficient = 0.0;
	if(diffuseCoefficient > 0.0)
	{
		specCoefficient = pow(angle, u_material.shininess);
	}
	vec3 specular = specCoefficient * u_material.specular * 1.0;

	vec4 result = vec4(diffuse + ambient + specular, 1.0);
	
	colour = result;
};