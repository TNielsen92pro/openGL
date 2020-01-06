#version 430 core

out vec4 out_color;
in vec4 position;
uniform sampler2D image;

void main(){
	out_color = vec4(texture2D(image, (position.xy + 1) / 2).rgb, 1.0);
}