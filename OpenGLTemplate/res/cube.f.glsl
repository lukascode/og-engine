#version 330 core

out vec4 outColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;


in vec3 Normal;
in vec3 FragmentPosition;

//lighting factors
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength; 
uniform int concentrationFactor; 

void main()
{
    //Ambient lighting component
    vec3 ambient = ambientStrength * lightColor;

    //Diffuse lighting component
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragmentPosition);
    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 diffuse = diffuseStrength * diff * lightColor;

	//Specular lighting component
	vec3 viewDir = normalize(cameraPosition - FragmentPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), concentrationFactor);
	vec3 specular = specularStrength * spec * lightColor;


    vec3 resultColor = (ambient + diffuse + specular) * objectColor;
    outColor = vec4(resultColor, 1.0f);
}