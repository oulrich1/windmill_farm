
#include "Angel.h"
#include <vector>
#include <iostream>

#include "camera.h"		//very important 

#ifndef PLANE_H
#define PLANE_H 

#define INCR_THROTTLE_FACTOR 1.51235 //RANDOM!
#define THROTTLE_MAX 123


class Plane
{
public:
	Plane(Camera* _cam);
	~Plane();

	bool	increaseThrottle();
	bool 	decreaseThrottle();

	bool 	setThrottle(float throttle);
	float 	getThrottle();


	bool 	controlStick();

	/* - - - - data  - - - - */

	// THROTTLES //	
	// the max the throttle can ever be.. 
	// this determiens accelerations
	float 	max_throttle = 100;

	float 	cur_throttle = 0;

	float 	min_throttle = 0; 

	/* - - - - - - - - - - - */
private:
	/* Since the camera is just a camera:
		And the plane happens to be  
		controlled by the user..
		The plane needs to have some control
		of the camera... */
	bool 	delegated_camera_control;
	Camera* cam;
};

#endif