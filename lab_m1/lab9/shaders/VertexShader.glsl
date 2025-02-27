#version 330 core
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texcoord;

uniform sampler2D heightMap;
uniform float heightScale;

out vec2 texcoord;

void main() {
    vec4 heightData = texture(heightMap, in_texcoord);
    float height = heightData.r * heightScale;
    vec3 newPosition = in_position + vec3(0.0, height, 0.0);
    gl_Position = projection * view * model * vec4(newPosition, 1.0);
    texcoord = in_texcoord;
}
