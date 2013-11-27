
#include "geometry.h"
#include "utils.h"

typedef std::vector<float>                  VF; 
typedef std::vector<std::vector<float> >    VVF; 

class Terrain  // : public Geometry
{

    /* = = = = PRIVATE = = = =*/

private:

    uint    init_terrainlogic();
    uint    init_terraindata();

    uint    generateRandomTerrain (float MAX_X, float MAX_Y, float MAX_Z);
    float   generateCompletelyRandomHeight (float interval_width);

    uint    mergeTerrainDataIntoPoints(); // from the VVF terrain_data -> merge into points buffer


    uint    init_gl();
    void    init_models();
    void    init_views();


    /* GL STUFF: */
        GLuint  program_id,   // shader program id
                vao,        // vertex  array object id
                vbo,        // vertex buffer object id
                ibo;        // index  buffer object id


        GLuint  model_view;  // model-view matrix uniform shader variable location

        /* SWITCH.. (hack...) */
        GLuint USE_COLOR_BUFFER;


        /* ---------------- */
       
        static const int NUM_COMPONENTS = 1; // 1 random terrain..
        static const int NUM_VERTICIES = 10000; // a bunch of points

        // this is what goes to GL..
        // because 6 pins per square and that many squares
        // extra 6 because mistakes...
    Angel::vec4 points[(NUM_VERTICIES-100)*6 + 6];
    Angel::vec4 colors[(NUM_VERTICIES-100)*6 + 6];

        mat4 model_view_matrix;   //  concatentation of V * M;

        mat4 model;     // explains the scale, shear.. and other 
                        // transformations of the shape of the collection
        mat4 view;      // exaplains how it is oriented to the user



    /*  INIT :
            logic/member variables..
            generate terrain...
            send data to GPU      */


    // 2d array of floats, 
    // that represent height...
    VVF     terrain_data;


    float   percent_random;
    float   land_level_offset;
    float   amplitude;



    /* = = = = PUBLIC = = = = */

public:
    Terrain(GLuint _program_id);
    ~Terrain();

    uint    collides(Geometry* collidable);

    // called by the main application, (once)
    // possibly once if nothing changes..
    uint    display(); 

    float   getHeightAt(int x, int y);

    float   width;
    float   max_height;



};