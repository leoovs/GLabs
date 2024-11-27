#version 460 core

#define POINT_LIGHT_N 5
#define LIGHT_KIND_POINT 0
#define LIGHT_KIND_SPOT  1

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
    int kinds[POINT_LIGHT_N];
    int casts[POINT_LIGHT_N];

    vec3 positions[POINT_LIGHT_N];
    vec3 directions[POINT_LIGHT_N];

    float cutOff[POINT_LIGHT_N];
    float outerCutOff[POINT_LIGHT_N];
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
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-uDirectionalLight.direction);

    float diffuseCoeff = max(dot(norm, lightDir), 0.0f);

    vec3 reflectDir = reflect(-lightDir, norm);
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

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(uPointLight.positions[iLight] - position);

    float diffuseCoeff = max(dot(norm, lightDir), 0.0f);

    vec3 reflectDir = reflect(-lightDir, norm);
    float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), uMaterial.shininess);

    float distance = length(uPointLight.positions[iLight] - position);
    float constant = uPointLight.constants[iLight];
    float linear = uPointLight.linears[iLight];
    float quadratic = uPointLight.quadratics[iLight];
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec3 ambient = uPointLight.ambient[iLight] * uMaterial.ambient;
    vec3 diffuse = uPointLight.diffuse[iLight] * uMaterial.diffuse * diffuseCoeff;
    vec3 specular = uPointLight.specular[iLight] * uMaterial.specular * specularCoeff;
    
    return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateSpotLight(int iLight, vec3 viewDir)
{
    vec3 lightDir = normalize(uPointLight.positions[iLight] - position);
    vec3 spotLightDir = uPointLight.directions[iLight];

    float theta = dot(lightDir, normalize(-spotLightDir));
    float epsilon = uPointLight.cutOff[iLight] - uPointLight.outerCutOff[iLight];
    float intensity = clamp((theta - uPointLight.outerCutOff[iLight]) / epsilon, 0.0f, 1.0f);

    return intensity * calculatePointLight(iLight, viewDir);
}

void main()
{
    vec3 viewDir = normalize(uViewPosition - position);

    vec3 color = calculateDirectionalLight(viewDir);

    for (int iLight = 0; iLight < POINT_LIGHT_N; iLight++)
    {
	vec3 addUp;

	switch (uPointLight.kinds[iLight])
	{
	case LIGHT_KIND_POINT:
	    addUp = calculatePointLight(iLight, viewDir);
	    break;
	case LIGHT_KIND_SPOT:
	    addUp = calculateSpotLight(iLight, viewDir);
	    break;
	}

	color += addUp;
    }

    outColor = vec4(color, 1.0f);

    // For debug purposes.
    // outColor = vec4(1.0f);
}

