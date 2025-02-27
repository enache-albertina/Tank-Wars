#version 330


layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec3 v_normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


out vec3 color;

void main()
{
    color = v_color;
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}