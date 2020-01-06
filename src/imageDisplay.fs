#version 430 core

out vec4 out_color;
in vec4 position;
uniform float max_density;
uniform vec3 color;
uniform sampler2D image;

void main(){
	vec3 density_vec = texture2D(image, (position.xy + 1) / 2).rgb;
	float density = density_vec.r / max_density;
	float alpha = 0;
	const float thresh = 0.5;
	gl_FragDepth = 0.0;
	if (density > thresh) {
		alpha = 0.8;

		if (density < thresh + 0.05) {
			alpha = 1.0;
			gl_FragDepth = 1.0;
		} 

	} else {
		alpha = density;
	}
	out_color = vec4(color, alpha);
}