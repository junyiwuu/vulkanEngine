#version 450
//tell which glsl you are using

//vec2 is built in function, 2 dimenstion float vector 
vec2 positions[3] = vec2[] ( 
    vec2(0.0, -0.5) , vec2(0.5, 0.5) , vec2(-0.5, 0.5));

//this main function is going to be executed once for each vertex  we have
void main(){
    //assign a value to act as our output , 4d vector.. 
    //top left is (-1,-1), buttom right is (1,1), so center is (0,0)
    gl_Position = vec4( positions [gl_VertexIndex ] , 0.0, 1.0 );


}