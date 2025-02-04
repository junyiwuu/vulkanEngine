#version 450
//tell which glsl you are using


layout (location = 0) in vec2 position;
//tell GPU to read position data from location 0 of the vertex buffer
layout (location = 1) in vec3 color;  //从vertex buffer读入位置数据


layout (push_constant) uniform Push{
    vec2 offset;
    vec3 color;
}push;


//this main function is going to be executed once for each vertex  we have
void main(){
    //assign a value to act as our output , 4d vector.. 
    //top left is (-1,-1), buttom right is (1,1), so center is (0,0)
    gl_Position = vec4( position + push.offset , 0.0, 1.0 );

}