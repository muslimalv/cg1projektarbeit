#version 330 core
out vec4 FragColor;
in vec3 texCoords;
uniform samplerCube skybox;
void main() {
    FragColor = texture(skybox, texCoords);
    //FragColor = vec4(1.0,0.0,0.0,1.0);
}