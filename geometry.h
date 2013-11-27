/* Geometry.h
 *
 * Base class for geometric objects.
 */
#include "Angel.h"
#include <vector>
#include <iostream>

using namespace std;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#ifndef GEOMETRY_H
#define GEOMETRY_H

#define toRadians(degrees) (degrees * DegreesToRadians)

class Geometry {
private:

        // common state variables
    
protected:
    mat4 I;

public:
    Geometry();
    virtual ~Geometry();

    /*
         Pure virtual function to display the Geometry. Classes
         inheriting from Geometry need to provide a definition
         for display().
    */
    virtual void init_data() = 0;   // initializes this object's data
    virtual void init_gl( ) = 0;        // initializes the GPU 
                                    // with any data we care about sending over
    virtual void display() = 0;     // switches to the object's vao/vbo and 
                                    // invokes postRedisplay

    // rotates the entire body by radians
    virtual void rotate(float rad) = 0;

};




#endif
