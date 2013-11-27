#include "Angel.h"
#include <vector>
#include <iostream>

#include "utils.h"

using namespace std;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#ifndef CAMERA_H
#define CAMERA_H 

#define quaternion glm::gtx::quaternion

class Camera
{
private:
	void constrict_angles();
public:
	Camera(GLuint program_id);
	~Camera();

	void set_at(vec4 _at);
	void set_up(vec4 _up);

	void update_proj(); // given the camera properties, send a new matrix to the graphics card
	void update_view();

	GLuint 		projection_loc, 
				view_loc;

	mat4 		projection_mat4, 
				view_mat4;

	GLfloat  	fovy = 50.0;  // Field-of-view in Y direction angle (in degrees)
	GLfloat  	aspect = 1.1;       // Viewport aspect ratio
	GLfloat  	zNear = 0.5, zFar = 30000.0;

	point4  	eye;
    point4  	at;
    vec4    	up;

    /* camera orientation properties */
	float 	yaw;                 // rotation around the y axis
	float 	pitch;               // rotation around the x axis
	float 	roll;                // rotation around the z axis

	float 	theta_x;		/// using these and NOT yaw ptich roll...
	float 	theta_z;
	float 	theta_y;

	// aka: orient
	void adjust(vec4 direction); // vec4 orientation

    // walk function,
    // increments current position
    // in that direction..........
    // (essentially moves the world)
    void walk(vec4 direction);

    // walk rate:
    float dx = 0.01, dy = 0.01, dz = 0.01;

    // turn function:
    // turns the camera (essentially rotates the world)
    void offsetOrientation(vec2 direction);
    // turn rate:
    float dTheta = 4.5, dPhi = 0.5;
};

#endif