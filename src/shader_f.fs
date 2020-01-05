#version 460 core
layout (std430, binding = 2) buffer points {
        float data_SSBO[]; // Make sure first element is the size of remaining elements
};
in float colorSetFrag;
in vec4 position;
out vec4 FragColor;
uniform vec3 color;
uniform mat4 trans;

void main()
{
    FragColor = vec4(color, 1.0f);
    
    /*int nrOfPoints = 0;
    float bandwidth = 30;
    float kernelSum = 0.0f;
    for(int i = 1; i < data_SSBO[0]+1; i = i + 3){
    //FragColor = vec4(data_SSBO[i], data_SSBO[i+1], 1.0, 0.0);
            float distance = sqrt(((trans * data_SSBO[i]) - position.x) * ((trans * data_SSBO[i]) - position.x) + ((trans * data_SSBO[i + 1]) - position.y) * ((trans * data_SSBO[i + 1]) - position.y));
        if(distance < 0.2 && data_SSBO[i+2] == 1){
            kernelSum = kernelSum + ((1.0f/bandwidth) * (1.0f/sqrt(2.0f*3.1415f)) * pow(2.71828f, -(distance*distance) / (2.0f*0.9f)));
            nrOfPoints++;
        }
    }
    float densityValue = (1.0f / nrOfPoints) * kernelSum;
    
    */
    if(colorSetFrag == 1){
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    } else if (colorSetFrag == 2){
        FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    } else if (colorSetFrag == 3){
        FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }
    /*if(densityValue > 0.0132){
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }*/
};