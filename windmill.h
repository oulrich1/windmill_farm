/* windmill.h
 *
 * Base class for the windmill
 */

#include "geometry.h"

#ifndef WINDMILL_H
#define WINDMILL_H 

#define PTS_N_CUBE      8

 // this is the length of one synmetic side of the unit cube..
#define UNIT_CUBE_D     0.035      

// this is half the length in usable coordinates when plotting cube points         
#define U_CUBE_C        UNIT_CUBE_D/2

#define TURN_RATE_DEGREES_PER_FRAME 8
 

enum WindmillMode {
    OFF = 0,
    ON = 1
};



class Windmill : public Geometry
{
    /* PRIVATE */
private:
    // private variables and functions

        GLuint  program_id,   // shader program id
            vao,        // vertex  array object id
            vbo,        // vertex buffer object id
            ibo;        // index  buffer object id

        // Model transformation parameters
        struct {       
            float theta;
            float gamma;
            float phi;
        } model_angles;              // angles from the default
                        // model coordinates

        // Viewing transformation parameters

        struct {
            float x;
            float y;
            float z;
        } view_angles;

        GLfloat radius = 1.0;   // ??
        GLfloat theta = 0.0;    // horizontal angle
        GLfloat phi = 0.0;      // vertical angle

        const GLfloat  dr = 5.0 * DegreesToRadians;

        GLuint  model_view;  // model-view matrix uniform shader variable location

        /* Color Blocks */
        GLuint new_color;


        /* ---------------- */
        static const int NumWindmillComponents = 5; // 4 blades and 1 stem..

        mat4 model_view_matricies[NumWindmillComponents]; // the model view matrix data for the windmill
        mat4 models[NumWindmillComponents];    // array of model matricies for each blade and the stem
        mat4 views[NumWindmillComponents];

        uint    windmill_mode;
        int    windmill_turbine_direction;
        float   current_blade_angle; // degrees

        struct {
            float width;
            float height;
        } blade;

        struct{
            float width;
            float height;
        } post;

    /* helpers */
            /* These helpers are from Angel's example code */
    void quad( int a, int b, int c, int d );
    void colorcube();

    void init_logic();
    void init_data();
    void init_gl();

    void init_models();
    void init_views();


    void update_mv_matricies();

    int Index = 0;

    /* PUBLIC */
public:
    Windmill(GLuint _program_id, vec4 _offset = vec4(0,0,0,0));
    ~Windmill();
    
    void init();

    // one iteration or "tick" for a windmill.. 
    // called within timer func in applcation
    void tick();
    void display();               // engages the gpu to display the primatives

    uint switch_mode();                 // switch the state, on or off
                                        // returns current state
    int switch_turbine_direction();    // returns current direction 

    void rotate(float rad); // rotates the entire body by radians
    void turn_blades(float rad);    //turns the blades as a whole (4 total)

    static const int NumVertices = 36;

    /* Data set for GL to draw with*/
    point4 points[NumVertices];
    color4 colors[NumVertices];

    /* standard unit cube, with 8 pts */
    point4 vertices[PTS_N_CUBE];
    color4 vertex_colors[PTS_N_CUBE];

    vec4 offset; // offsets this instance of a windmill

};


#endif
