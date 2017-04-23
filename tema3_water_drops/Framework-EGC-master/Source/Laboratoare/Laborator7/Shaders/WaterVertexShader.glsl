#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;


uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float dim;

uniform float currentTime;

struct Wave {
	float amplitude;
	float waveLength;
	float speed;
	vec3 pos;
	float startTime;
	float baseTimeM;
	float baseTimeS;
};

uniform Wave waves[500];
uniform int len;

// Output value to fragment shader
out vec3 world_position;
out vec3 world_normal;

float pi = 3.14159265358;

vec3 computeHeight(float xx, float zz) {

	float dd = 0;

	for(int i = 0 ; i < len ; ++i) {

		vec2 posi = vec2(xx, zz);
		vec2 drop_posi = vec2(waves[i].pos.x, waves[i].pos.z);
		vec2 post = posi - drop_posi;
		vec2 Di = (drop_posi - posi) / abs(length(drop_posi - posi));
		float dist = length(post);
		float time_release = currentTime - waves[i].startTime;

		if(dist *  3.2 - waves[i].waveLength * 2.4  < time_release * waves[i].speed * 2 * pi + waves[i].baseTimeM) 
			if(dist * 3.2 + waves[i].waveLength * 2.4 > time_release  * waves[i].speed * 2 * pi + waves[i].baseTimeM)
			 dd += waves[i].amplitude * (sin( dot(Di, post) * pi / waves[i].waveLength + 
							time_release * (waves[i].speed * 2 * pi / waves[i].waveLength) + waves[i].baseTimeS ) + 1);
	}

	return vec3(xx, dd, zz);
}


vec3 computeNormal(vec3 p1, vec3 p2, vec3 p3) {
	
	return  normalize(cross(p2 - p1, p3 - p1));

}

void main()
{
	world_position = (Model * vec4(v_position, 1)).xyz;

	float dd = 0;

	for(int i = 0 ; i < len ; ++i) {

		vec2 posi = vec2(v_position.x, v_position.z);
		vec2 drop_posi = vec2(waves[i].pos.x, waves[i].pos.z);
		vec2 post = posi - drop_posi;
		vec2 Di = (drop_posi - posi) / abs(length(drop_posi - posi));
		float dist = length(post);
		float time_release = currentTime - waves[i].startTime;

		if(dist *  3.2 - waves[i].waveLength * 2.4  < time_release * waves[i].speed * 2 * pi + waves[i].baseTimeM) 
			if(dist * 3.2 + waves[i].waveLength * 2.4 > time_release  * waves[i].speed * 2 * pi + waves[i].baseTimeM)
			 dd += waves[i].amplitude * (sin( dot(Di, post) * pi / waves[i].waveLength + 
							time_release * (waves[i].speed * 2 * pi / waves[i].waveLength) + waves[i].baseTimeS ) + 1);
	}
	
	vec3 p =  vec3(v_position.x, dd, v_position.z);
	vec3 p1 = computeHeight(v_position.x, v_position.z + dim);
	vec3 p2 = computeHeight(v_position.x + dim, v_position.z);
	vec3 p3 = computeHeight(v_position.x, v_position.z - dim);
	vec3 p4 = computeHeight(v_position.x - dim, v_position.z);

	vec3 norm = vec3(0, 0 ,0);
	norm += computeNormal(p, p1, p2);
	norm += computeNormal(p, p2, p3);
	norm += computeNormal(p, p3, p4);
	norm += computeNormal(p, p4, p1);

	norm /= 4;
	norm = normalize(norm);
	world_normal = normalize( mat3(Model) * norm);

	gl_Position = Projection * View * Model * vec4(p, 1.0);

}
