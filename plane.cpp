
#include "plane.h"

Plane::Plane(Camera* _cam){
	max_throttle = THROTTLE_MAX;
	cur_throttle = 0;
	min_throttle = 0;

	cam = _cam;
	delegated_camera_control = (cam ? true: false); 

}
Plane::~Plane(){

}

bool Plane::increaseThrottle(){
	if (cur_throttle < max_throttle) {
		cur_throttle += INCR_THROTTLE_FACTOR;
		if (cur_throttle > max_throttle){
			cur_throttle = max_throttle;
		}
		return true;
	} 
	return false;
}

bool Plane::decreaseThrottle(){
	if (cur_throttle > min_throttle){
		cur_throttle -= INCR_THROTTLE_FACTOR;
		if (cur_throttle < min_throttle){
			cur_throttle = min_throttle;
		}
		return true;
	}
	return false;
}

float Plane::getThrottle(){
	return cur_throttle;
}

bool Plane::setThrottle(float throttle){
	if (throttle < min_throttle){
		cur_throttle = min_throttle;
	} else if (throttle > max_throttle){
		cur_throttle = max_throttle;
	} else{
		cur_throttle = throttle;
		return true;
	}
	return false;
}

bool Plane::controlStick(vec4 dir){
	// set parameters
	// with parameters set:
	//   control the camera:


	if(delegated_camera_control){
		// do camera stuff here...
		cam ->walk(vec4(0,0,-1,0));
	}
	return true;
}