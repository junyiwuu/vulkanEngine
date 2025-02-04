#version 450
//tell which glsl you are using


layout (location = 0) in vec2 position;
//tell GPU to read position data from location 0 of the vertex buffer
layout (location = 1) in vec3 color;  //从vertex buffer读入位置数据


layout (push_constant) uniform Push{
    mat2 transform;
    vec2 offset;
    vec3 color;
}push;


//this main function is going to be executed once for each vertex  we have
void main(){
    gl_Position = vec4( push.transform * position + push.offset , 0.0, 1.0 );

}