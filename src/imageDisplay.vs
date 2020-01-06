#version 430 core
layout (location = 0) in vec2 aPos;

out vec4 position;

void main() {
    position = vec4(aPos.xy, 0.0, 1.0);
    
    gl_Position =  vec4(aPos.xy, 0.0, 1.0);
}