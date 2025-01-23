#version 450


layout(location=0) out vec4 outColor;
//layout qualifier, which taks a location value
//fragment is capable to output to multiple different locations
///for now we are only using location 0
//finally declare the variable's name and what type it is 

void main(){
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}