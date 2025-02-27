#version 330 core

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texcoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D heightMap;

out vec4 out_color;

void main() {

    float height = texture(heightMap, frag_texcoord).r;


    vec4 color1 = texture(texture1, frag_texcoord);
    vec4 color2 = texture(texture2, frag_texcoord);

    float blendFactor = smoothstep(0.2, 0.8, height);
    vec4 blendedColor = mix(color1, color2, blendFactor);

    out_color = blendedColor;
}
