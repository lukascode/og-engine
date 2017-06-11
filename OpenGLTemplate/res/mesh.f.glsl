#version 330 core

out vec4 color;

struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;

	sampler2D texture_ambient1;
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;

};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPosition;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;


void main()
{
/*
	//ambient
	vec3 ambient = light.ambient * material.ambient;

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//specular
	vec3 viewDir = normalize(viewPosition - FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	//color = vec4(result, 1.0);
*/
	//color = texture(material.texture_diffuse1, TexCoord);
	color = vec4(texture(material.texture_diffuse1, TexCoord).rgb, 1.0);


}