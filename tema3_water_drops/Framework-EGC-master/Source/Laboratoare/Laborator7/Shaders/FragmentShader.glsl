#version 330

//received interpolated from vertexShader
in vec3 world_position;
in vec3 world_normal;

uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform	uint material_shininess;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	
	float ambient_light = (material_kd + material_ks) / 7;
	
	float diffuse_light = material_kd * max(dot(L, world_normal), 0);
	
	float specular_light = 0;
	
	if (diffuse_light > 0)
		specular_light = material_ks * pow( max(dot(world_normal, H), 0), material_shininess);
	
	float d = distance(light_position, world_position);
	float atenuare = 1 / (0.2 + 0.3 * d + 0.6 * d * d);

	float light_color = min(ambient_light + atenuare * (diffuse_light + specular_light), 1);

	out_color = vec4( object_color * light_color , 1);
}