#version 460 core

#define POINT_LIGHT_N 8

in vec3 position;
in vec3 normal;
in vec2 texCoord;

uniform vec3 uViewPosition;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material uMaterial;

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight uDirectionalLight;

struct PointLight
{
    int casts[POINT_LIGHT_N];

    vec3 positions[POINT_LIGHT_N];

    float constants[POINT_LIGHT_N];
    float linears[POINT_LIGHT_N];
    float quadratics[POINT_LIGHT_N];

    vec3 ambient[POINT_LIGHT_N];
    vec3 diffuse[POINT_LIGHT_N];
    vec3 specular[POINT_LIGHT_N];
};

uniform PointLight uPointLight;

out vec4 outColor;

vec3 calculateDirectionalLight(vec3 viewDir)
{
    vec3 lightDir = normalize(-uDirectionalLight.direction);

    float diffuseCoeff = max(dot(normal, lightDir), 0.0f);

    vec3 reflectDir = reflect(-lightDir, normal);
    float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), uMaterial.shininess);

    vec3 ambient = uDirectionalLight.ambient * uMaterial.diffuse; 
    vec3 diffuse = diffuseCoeff * uMaterial.diffuse * uDirectionalLight.diffuse;
    vec3 specular = specularCoeff * uMaterial.specular * uDirectionalLight.specular;

    return (ambient + diffuse + specular);
}

vec3 calculatePointLight(int iLight, vec3 viewDir)
{
    if (0 == uPointLight.casts[iLight])
    {
        return vec3(0.0f);
    }

    vec3 lightDir = normalize(uPointLight.positions[iLight] - position);

    float diffuseCoeff = max(dot(normal, lightDir), 0.0f);

    vec3 reflectDir = reflect(-lightDir, normal);
    float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), uMaterial.shininess);

    float distance = length(uPointLight.positions[iLight] - position);
    float constant = uPointLight.constants[iLight];
    float linear = uPointLight.linears[iLight];
    float quadratic = uPointLight.quadratics[iLight];
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec3 ambient = uPointLight.ambient[iLight] * uMaterial.ambient;
    vec3 diffuse = uPointLight.diffuse[iLight] * uMaterial.diffuse * diffuseCoeff;
    vec3 specular = uPointLight.specular[iLight] * uMaterial.diffuse * specularCoeff;
    
    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec3 viewDir = normalize(uViewPosition - position);

    vec3 color = calculateDirectionalLight(viewDir);

    for (int iLight = 0; iLight < POINT_LIGHT_N; iLight++)
    {
	color += calculatePointLight(iLight, viewDir);
    }

    outColor = vec4(color, 1.0f);

    // For debug purposes.
    // outColor = vec4(1.0f);
}

