#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 texCoord;
//vec3 normals = aColor;
vec3 normals = vec3(0.0,0.0,0.0);
uniform vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 vertexColor;
out vec3 fragPosition;
out vec3 Normal;
out vec2 tex;


void main() {
    fragPosition = vec3(model * vec4(aPosition,1.0));
    Normal = inverse(transpose(mat3(model))) * normals;
    vertexColor = aColor;
    gl_Position =  projection * view * model*vec4(aPosition, 1.0);
    tex = texCoord;
}