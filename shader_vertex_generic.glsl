#version 150 

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

uniform mat4 model_view;
uniform mat4 camera_view;
uniform mat4 projection;

uniform vec4 new_color;

uniform int USE_COLOR_BUFFER;

void main() 
{

	// model_view matrix {object.view * object.model}
	// (depends on the object itself..) 
	//   TODO: shouldnt be uniform:

	// the view for the camera, (effects all world objects..)

    gl_Position =  projection * camera_view * model_view * vPosition;

    // the following is a little "get to work" 
    // hack since i forgot about the solid colors being used for the windmill
    if(USE_COLOR_BUFFER == 1){
        color = vColor; //vColor
    } else {
        color = new_color;
    }

} 