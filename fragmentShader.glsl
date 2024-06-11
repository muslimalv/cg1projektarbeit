#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vertexColor;
in vec3 Normal;
in vec2 tex;
in vec3 fragPosition;
uniform Material material;
uniform Light light;

vec3 lightPos = vec3(0.0,10.0,0.0);
vec3 lightColor = vec3(1.0,1.0,1.0);
vec3 viewPos =vec3(10.0,0.0,0.0);
uniform vec3 camera;

layout (location = 0) out vec4 fragColor;
uniform sampler2D ourTexture;
void main() {
    //ambient
    float ambientStrength = 0.6;
    vec3 ambient = light.ambient * material.ambient;
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - fragPosition);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = light.diffuse *(diff * material.diffuse);
    //specular
    float SpecularStrength = 0.5;
    //vec3 viewDir = normalize(viewPos -fragPosition);
    vec3 viewDir = normalize(camera -fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = light.specular *(spec *material.specular);

    vec3 result = ambient + diffuse + specular;
    //fragColor = vec4(result, 1.0);
    //fragColor = vec4(vertexColor,1.0);
    
    //fragColor = texture(ourTexture,tex);
    //fragColor = vec4(Normal,1.0);
    vec4 t = vec4(result,1.0f);
    vec4 z = vec4(vertexColor,1.0);
    fragColor = mix(t,z, 0.5);
}