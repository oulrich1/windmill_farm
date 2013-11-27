#include "terrain.h"

/* by default it generates:
    RANDOM TERRAIN    */
Terrain::Terrain(GLuint _program_id){
    program_id = _program_id;

    uint err = 0; 

    err = init_terrainlogic();  // sets member verables n stuff
        err = err << 1;

    err |= init_terraindata();      // initializes the terrain data.. (Does not send to the GPU )

     /* now the points data will be sent to the GPU */
        err = err << 1;
    err |=init_gl();                // Sends the terrain data points to the GPU, nicely formated

    if (err){
        cerr << "There was an error with the terrain. (Code: " << err << ").." << endl;
    }

}
Terrain::~Terrain(){

}



uint Terrain::init_terrainlogic(){
    percent_random      = 1.0;
    land_level_offset   = 10; // downward..
    amplitude           = 5;

    max_height = 1;
    width = (int)sqrt(NUM_VERTICIES);

    return 0;
}
uint Terrain::init_terraindata(){
    // load data...
    // set data in memory...

    //generate data..
    generateRandomTerrain(width, width, max_height); // 100 by 100 by 1 : landscape

    // terrain stored in 2D array terrain_data
    mergeTerrainDataIntoPoints();

    init_models();

    init_views();

    model_view_matrix = view * model; //terrain model view

    return 0;
}


void Terrain::init_models(){
    model = mat4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1});   //stretched
    model = Scale(vec3(100,10,10)) * Translate(vec4(8, 7, -50, 0)) * RotateZ(180) * model;  // translated
}

void Terrain::init_views(){
    view = mat4({1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1});  //Identity
}


uint Terrain::init_gl(){
    // send data to the gpu

    glGenVertexArrays( 1, &(this->vao) );   
    glBindVertexArray( this->vao );         // bind the vertex array object

    glGenBuffers( 1, &(this->vbo) );        
    glBindBuffer( GL_ARRAY_BUFFER, this->vbo );// bind the array buffer object

printf("1.b.1.) Initialized: GPU VAO && VBO specifically for Windmill\n");
    
    // int number_of_floats_per_vertex = 4;

    // int number_of_floats = NUM_VERTICIES * (sizeof(float) * number_of_floats_per_vertex);

    glBufferData( GL_ARRAY_BUFFER,  sizeof(points) + sizeof(colors),
          NULL, GL_STATIC_DRAW );
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
        glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

printf("1.b.2.) Initialized: GPU with TERRAIN Geometry Data\n");
    
        GLuint vPosition = glGetAttribLocation( program_id, "vPosition" );
        glEnableVertexAttribArray( vPosition );
        glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                   BUFFER_OFFSET(0) );


        GLuint vColor = glGetAttribLocation( program_id, "vColor" ); 
        glEnableVertexAttribArray( vColor );
        glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                   BUFFER_OFFSET(sizeof(points)) );


    /* find location of the mv and proj matrix vars in vshader */
    model_view = glGetUniformLocation( program_id, "model_view" );
    USE_COLOR_BUFFER =  glGetUniformLocation( program_id, "USE_COLOR_BUFFER" );

    /* belay the possiblity of misuse */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    return 0;
}

float Terrain::generateCompletelyRandomHeight (float interval_width){
    float randomHeight = ((rand_f() * interval_width) - (interval_width/2));

    randomHeight =  amplitude * 
                        (randomHeight - (1-percent_random) * randomHeight) 
                    + land_level_offset;

    return randomHeight;
}

uint Terrain::generateRandomTerrain(float MAX_X, float MAX_Y, float MAX_Z){
    terrain_data.clear();
    terrain_data.resize(MAX_X);     // MAX_X number of elements height 0
    //srand(time(NULL));
    for (VVF::iterator iter = terrain_data.begin(); iter != terrain_data.end(); ++iter) {
        float randomHeight = generateCompletelyRandomHeight(MAX_Z);
        (*iter).resize(MAX_Y, randomHeight);
    }

    return 0;
}

uint Terrain::mergeTerrainDataIntoPoints(){
    int Index = 0;
    for (int i = 0; i < width-1; i++) {
        for (int j = 0; j <= width-2; ++j) {
            //     colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
            // points[Index++] = vec4(i, j, terrain_data[i][j], 1);
            //     colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
            // points[Index++] = vec4(i+1, j, terrain_data[i+1][j], 1);
            //     colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
            // points[Index++] = vec4(i, j+1, terrain_data[i][j+1], 1);
            //         colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
            //     points[Index++] = vec4(i+1, j, terrain_data[i+1][j], 1);
            //         colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
            //     points[Index++] = vec4(i, j+1, terrain_data[i][j+1], 1);
            //         colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
            //     points[Index++] = vec4(i+1, j+1, terrain_data[i+1][j+1], 1);

                colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
            points[Index++] = vec4(i, terrain_data[i][j], j,  1);

                colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
            points[Index++] = vec4(i+1,terrain_data[i+1][j], j,  1);

                colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
            points[Index++] = vec4(i, terrain_data[i][j+1], j+1,  1);

                    colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
                points[Index++] = vec4(i+1, terrain_data[i+1][j], j,  1);

                    colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
                points[Index++] = vec4(i, terrain_data[i][j+1], j+1,  1);

                    colors[Index] = vec4(rand_f(), rand_f(), rand_f(), 1);
                points[Index++] = vec4(i+1,  terrain_data[i+1][j+1], j+1, 1);
        }
    }
    // Index === 10000-200 * 6 ... + 6  // dont know where the extra 6 are coming from....
    return 0;
}


float Terrain::getHeightAt(int x, int y){
    if (x >= 0 && x < width && y >= 0 && y < width){
       return terrain_data[x][y];
    }
    return 0;
}


uint Terrain::display(){
    /* generate MV matrix:
        send to GPU..
        Display.. render...
    (REALLY ONLY NEEDS TO HAPPEN ONCE...) */

    glUseProgram( program_id );
    glBindVertexArray( this->vao );             // bind the vertex array object
    glBindBuffer( GL_ARRAY_BUFFER, this->vbo ); // bind the array buffer object


        glUniformMatrix4fv( model_view, 1, GL_TRUE, model_view_matrix );
        glUniform1i(USE_COLOR_BUFFER, 1);
        //glUniform4fv(new_color, 1, color4( rand_f(), rand_f(), rand_f(), 1.0 ));
    glDrawArrays( GL_TRIANGLES, 0, NUM_VERTICIES ); // draws the terrain..

    glUniform1i(USE_COLOR_BUFFER, 0);

    return 0;
}

uint Terrain::collides(Geometry* collidable){
    // if the current quadrant contains the collidable..
    // then if then nearest part of that quadrant is close to the collidable
    //      then  check if they collided...
    //          if they did then return TRUE!
    //  also: cause the ground to ripple..
    return 0;
}