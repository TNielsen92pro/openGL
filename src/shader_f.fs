#version 430 core
layout (std430, binding = 2) buffer points {
        vec4 data_SSBO[]; // Make sure first element is the size of remaining elements
};
in float colorSetFrag;
in vec4 position;
out vec4 FragColor;
uniform vec3 color;
uniform mat4 trans;
uniform int ssbolength;

const float e = 2.71828;
const float pi = 3.1415;


float KDE(float x, float b){
    float r = x / b; 
	return (1.0f / b) * (1.0f / (sqrt(2.0f * pi))) * pow(e, -(r*r) / (2.0 ));
}

void main()
{
    FragColor = vec4(color, 1.0f);
    
    int nrOfPoints = 0;
    float bandwidth = 0.5;
    float kernelSum = 0.0f;
    for(int i = 0; i < ssbolength; i++){
	    int offset = 4 * i;
		vec3 point = data_SSBO[i].xyz;
    //FragColor = vec4(data_SSBO[i], data_SSBO[i+1], 1.0, 0.0);
        float distance = length(point.xy - position.xy); //sqrt(((trans * data_SSBO[i]) - position.x) * ((trans * data_SSBO[i]) - position.x) + ((trans * data_SSBO[i + 1]) - position.y) * ((trans * data_SSBO[i + 1]) - position.y));
        if(point.z < 1.001f){
            kernelSum += KDE(distance, bandwidth);
			nrOfPoints++;
        }
    }
    float densityValue = (1.0f / nrOfPoints) * kernelSum;
	
    FragColor = vec4(densityValue , 0, 0,0);
};