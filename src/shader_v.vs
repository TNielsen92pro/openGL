#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in float colorSet;

uniform mat4 trans;
out float colorSetFrag;
out vec4 position;

void main() {
    
    colorSetFrag = colorSet;
    position = vec4(aPos.xy, 0.0, 1.0);
    
    gl_Position =  vec4(aPos.xy, 0.0, 1.0);
}