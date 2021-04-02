#version 330 core
out vec4 frag_color;

in vec3 normal;  
in vec3 frag_pos;  
  
uniform float u_ambient_strength;
uniform float u_specular_strength;
uniform int  u_resolution;
uniform vec3 u_light_pos; 
uniform vec3 u_view_pos; 
uniform vec3 u_light_color;
uniform vec3 u_object_color;

void main()
{
    // ambient
    vec3 ambient = u_ambient_strength * u_light_color;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(u_light_pos - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * u_light_color;
    
    // specular
    vec3 view_dir = normalize(u_view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_resolution);
    vec3 specular = u_specular_strength * spec * u_light_color;  
        
    vec3 result = (ambient + diffuse + specular) * u_object_color;
    frag_color = vec4(result, 1.0);
} 