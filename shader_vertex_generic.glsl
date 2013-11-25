#version 150 

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 new_color;

void main() 
{
    // projection*
    gl_Position = model_view*vPosition/vPosition.w;
    color = new_color; //vColor
} 