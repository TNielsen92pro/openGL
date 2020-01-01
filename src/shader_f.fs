#version 430 core
layout(std430, binding = 2) buffer points {
        float data_SSBO[];
};
in float colorSetFrag;
in vec4 position;
out vec4 FragColor;
uniform vec3 color;
uniform mat4 trans;

void main()
{
    FragColor = vec4(color, 1.0f);
    if(colorSetFrag == 1){
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    } else if (colorSetFrag == 2){
        FragColor = vec4(0.5f, 0.8f, 0.5f, 1.0f);
    } else if (colorSetFrag == 3){
        FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }
    if(data_SSBO[3] == data_SSBO[1] && data_SSBO[3] == 2891){ // WTF??
        //FragColor = vec4(position.xy, 1.0f, 1.0f);
    }
    for(int i = 0; i < data_SSBO.length() - 2; i++){
        FragColor = vec4(data_SSBO[i], data_SSBO[i+1], data_SSBO[i+2], 1.0);
        FragColor = vec4(position.xy, 1.0f, 1.0f);
    /*float distance = sqrt((data_SSBO[i] - position.x) * (data_SSBO[i] - position.x)
                   + (data_SSBO[i + 1] - position.y) * (data_SSBO[i + 1] - position.y));
        if(distance < 0.05){
            FragColor = vec4(0.0f, 0.0f, 0.0f);
        }*/
    }
};