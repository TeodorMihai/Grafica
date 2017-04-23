#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int mix_texture;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function

	vec4 color1 = texture2D(texture_1, texcoord);
	vec4 color2 = texture2D(texture_2, texcoord);
	vec4 color;

	if (mix_texture == 1) {
		color = mix(color1, color2, 0.5f);
	}
	else {
		color = color1;
	}
	if (color1.a < 0.5f) discard; 

	out_color = color;

}