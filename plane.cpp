
#include "plane.h"

Plane::Plane(Camera* _cam){
    max_throttle    = THROTTLE_MAX;
    cur_throttle    = 0;
    min_throttle    = 0;

    thetas          = vec4(0,0,0,0);
    direction       = vec4(0,0,0,0);

    cam = _cam;
    delegated_camera_control = (cam ? true: false); 

    current_control_state = 0; // the state mask that is used to control the orientation of the camera..
    
    cam_frozen      = false;
    plane_frozen    = false;

    drag_factor     = 0.034123; // random
    ROTATE_FACTOR   = 0.03f;

}
Plane::~Plane(){

}


/* simulates the flying.. */
bool Plane::fly(){

    if(current_control_state & YAW_LEFT) {
        controlStick(vec4(0,-ROTATE_FACTOR,0,0));
    }

   if(current_control_state & YAW_RIGHT) {
        controlStick(vec4(0,ROTATE_FACTOR,0,0));
    }

    if(current_control_state & ROLL_LEFT) {
        controlStick(vec4(0,0,-ROTATE_FACTOR, 0));
    }
    if(current_control_state & ROLL_RIGHT) {
        controlStick(vec4(0,0,ROTATE_FACTOR, 0));
    }
    if(current_control_state & PITCH_DOWN) {
        controlStick(vec4(-ROTATE_FACTOR,0,0, 0));
    }
   if(current_control_state & PITCH_UP) {
        controlStick(vec4(ROTATE_FACTOR,0,0, 0));
    }

    
    if (plane_frozen == false) {
        if (delegated_camera_control){
            cam->walk(vec4(0,0,-((float)cur_throttle/max_throttle) * MAX_SPEED, 0) + this->direction );
            cam->adjust(thetas);
        }
        this->thetas -=  this->thetas * drag_factor; // drag the change in angle..
        this->direction -= this->direction * drag_factor;
        return true;
    }
    return false;
}

bool Plane::resetOrientation(vec4 up){
    thetas = vec4(0,0,0,0);
    cam->set_up(up);    
    return true;
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


void Plane::setControlState(int state){
    current_control_state = state;
}

/* STRAFE:  FORWARD(w), BACKWARDS(s), 
            LEFT(a), RIGHT(d), 
            UP(x), DOWN(z),         */
bool Plane::controlDirection(vec4 dir){

    direction = dir;

    if(delegated_camera_control){
        //  cam->walk(this->direction);
    }
    return true;
}

/* ROLL, PITCH, YAW */
bool Plane::controlStick(vec4 angle_dir){

    thetas += angle_dir;

    return true;
}

bool Plane::collided(){
    if (cam->eye.y < -13){
        resetOrientation();
        cam->eye.y = -5;
        return true;
    }
    return false;
}