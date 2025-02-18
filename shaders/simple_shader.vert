#version 450
//tell which glsl you are using


layout (location = 0) in vec3 position;
//tell GPU to read position data from location 0 of the vertex buffer
layout (location = 1) in vec3 color;  //从vertex buffer读入位置数据

layout(location = 0) out vec3 fragColor;


layout (push_constant) uniform Push{

    mat4 transform;
    vec3 color;
}push;


//this main function is going to be executed once for each vertex  we have
void main(){
    gl_Position = push.transform * vec4(position, 1.0 );
    fragColor = color;

}