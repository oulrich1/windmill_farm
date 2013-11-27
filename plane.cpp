
#include "plane.h"

Plane::Plane(Camera* _cam){
	max_throttle 	= THROTTLE_MAX;
	cur_throttle 	= 0;
	min_throttle 	= 0;

	thetas			= vec4(0,0,0,0);
	direction 		= vec4(0,0,0,0);

	cam = _cam;
	delegated_camera_control = (cam ? true: false); 
	
	cam_frozen 		= false;
	plane_frozen 	= false;

}
Plane::~Plane(){

}

bool Plane::fly(){
	if (plane_frozen == false) {
		if (delegated_camera_control){
			cam->walk(vec4(0,0,-((float)cur_throttle/max_throttle) * MAX_SPEED, 0));
		}
		return true;
	}
	return false;
}
bool Plane::pause(){
	if (plane_frozen == false) {
		plane_frozen = true;
		return false;
	}
	return true;
}

bool Plane::unpause(){
	if (plane_frozen == true) {
		plane_frozen = false;
		return false;
	}
	return true;
}

string Plane::throttleMessage(){
	return "Throttle: " + toString((float)cur_throttle/max_throttle);
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


/* STRAFE:	FORWARD(w), BACKWARDS(s), 
			LEFT(a), RIGHT(d), 
			UP(x), DOWN(z),  		*/
bool Plane::controlDirection(vec4 dir){
	// set parameters
	// with parameters set:
	//   control the camera:
	cout << "dir      : " << dir << endl;

	direction = dir;
	//direction = normalize(direction);

	if(delegated_camera_control){
		// do camera stuff here...
		cam->walk(this->direction);
	}
	return true;
}

/* ROLL, PITCH, YAW */
bool Plane::controlStick(vec4 angle_dir){
	// set parameters
	// with parameters set:
	//   control the camera:
	cout << "angle_dir: " << angle_dir << endl;

	thetas = angle_dir;
	//thetas = normalize(thetas);

	if(delegated_camera_control){
		// do camera stuff here...

		cam->adjust(this->thetas);
	}

	return true;
}