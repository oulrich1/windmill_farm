

#include "camera.h"     //very important 

#ifndef PLANE_H
#define PLANE_H 

#define INCR_THROTTLE_FACTOR 1.51235 //RANDOM!
#define THROTTLE_MAX 200
#define MAX_SPEED    100


class Plane
{
public:
    Plane(Camera* _cam = NULL);
    ~Plane();

    // control masks:
    static const int YAW_LEFT = 0x0001;
    static const int YAW_RIGHT = 0x0002;
    static const int ROLL_LEFT = 0x0004;
    static const int ROLL_RIGHT = 0x0008;
    static const int PITCH_DOWN = 0x0010;
    static const int PITCH_UP = 0x0020;

    float ROTATE_FACTOR;

    int     current_control_state;

    bool    fly();
    bool    pause();
    bool    unpause();

    string  throttleMessage();

    bool    increaseThrottle();
    bool    decreaseThrottle();

    bool    setThrottle(float throttle);
    float   getThrottle();

    void    setControlState(int state);
    void    unsetControlState(int state);

    bool    controlStick(vec4);
    bool    controlDirection(vec4); // control the stick in certain direction...

    bool    resetOrientation(vec4 up = vec4(0,1,0,0));
    bool    collided();

    /* - - - - data  - - - - */

    // THROTTLES // 
    // the max the throttle can ever be.. 
    // this determiens accelerations
    float   max_throttle = 100;

    float   cur_throttle = 0;

    float   min_throttle = 0; 

    vec4    thetas;
    vec4    direction;
    /* - - - - - - - - - - - */
private:
    /* Since the camera is just a camera:
        And the plane happens to be  
        controlled by the user..
        The plane needs to have some control
        of the camera... */
    bool    delegated_camera_control;
    Camera* cam;

    bool    cam_frozen;
    bool    plane_frozen;

    float   drag_factor;
    float   mass;
};

#endif