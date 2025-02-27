#version 330

// Get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform vec3 light_position2;
uniform vec3 light_direction2;
uniform int type_of_light2;
uniform float cut_off_angle2;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform int type_of_light;
uniform float cut_off_angle;

layout(location = 0) out vec4 out_color;

void main()
{
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);

    // Define ambient light component (global)
    float ambient_light = 0.25;

    // Compute diffuse light component for first light
    float diffuse_light = material_kd * max(dot(N, L), 0.0);

    // Compute specular light component for first light
    float specular_light = 0.0;

    if (diffuse_light > 0.0)
    {
        specular_light = material_ks * pow(max(dot(N, H), 0.0), material_shininess);
    }

    // Compute total light for first light source
    float light = 0.0;

    if (type_of_light == 1)
    {
        float cut_off_rad       = radians(cut_off_angle);
        float spot_light        = dot(-L, light_direction);
        float spot_light_limit  = cos(cut_off_rad);
        
        if (spot_light > spot_light_limit)
        {    
            // Quadratic attenuation
            float linear_att       = (spot_light - spot_light_limit) / (1.0 - spot_light_limit);
            float light_att_factor = pow(linear_att, 2.0);
            light                  = light_att_factor * (diffuse_light + specular_light);
        }
    }
    else
    {
        light = diffuse_light + specular_light;
    }

    // Compute lighting for the second light source
    vec3 L2 = normalize(light_position2 - world_position);
    vec3 H2 = normalize(L2 + V);

    float diffuse_light2 = material_kd * max(dot(N, L2), 0.0);

    float specular_light2 = 0.0;

    if (diffuse_light2 > 0.0)
    {
        specular_light2 = material_ks * pow(max(dot(N, H2), 0.0), material_shininess);
    }

    float light2 = 0.0;

    if (type_of_light2 == 1)
    {
        float cut_off_rad2      = radians(cut_off_angle2);
        float spot_light2       = dot(-L2, light_direction2);
        float spot_light_limit2 = cos(cut_off_rad2);

        if (spot_light2 > spot_light_limit2)
        {
            float linear_att2       = (spot_light2 - spot_light_limit2) / (1.0 - spot_light_limit2);
            float light_att_factor2 = pow(linear_att2, 2.0);
            light2                  = light_att_factor2 * (diffuse_light2 + specular_light2);
        }
    }
    else
    {
        light2 = diffuse_light2 + specular_light2;
    }

    // Compute total lighting
    float total_light = ambient_light + light + light2;

    // Write pixel out color
    vec3 colour = object_color * total_light;

    out_color = vec4(colour, 1.0);
}
