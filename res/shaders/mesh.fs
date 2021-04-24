#version 330

out vec4 frag_color;


struct material_t {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct light_t {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

  
uniform vec3 u_view_pos;
uniform material_t u_material;
uniform light_t u_light;

in vec3 frag_pos;
in vec3 normal;
in vec2 uv;

uniform sampler2D my_texture;

void main()
{
    // ambient
    vec3 ambient = u_light.ambient * u_material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(u_light.position - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = u_light.diffuse * (diff * u_material.diffuse);
    
    // specular
    vec3 view_dir = normalize(u_view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * (spec * u_material.specular);  
    
    vec3 result = ambient + diffuse + specular;

    vec4 tex = texture(my_texture, uv);
    frag_color = tex * vec4(result, 1.0);
} 