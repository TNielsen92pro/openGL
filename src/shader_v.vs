#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in float colorSet;
layout (std430, binding = 2) buffer points {
        float data_SSBO[];
};

uniform mat4 trans;
out float colorSetFrag;
out vec4 position;

void main() {
    
    colorSetFrag = colorSet;
    position = trans * vec4(aPos.xy, 0.0, 1.0);
    if(data_SSBO.length() == 0){
        //position = vec4(0, 0, 0.0, 1.0);
    }
    gl_Position = position;
}