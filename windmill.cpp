

#include "windmill.h"

Windmill::Windmill(GLuint _program_id, vec4 _offset) : Geometry(){

    this->offset = _offset;
    this->program_id = _program_id;

    this->init_logic();
    this->init_data();  // initializes this object's data
    this->init_gl();    // initializes the GPU with any data we care about sending over


}

Windmill::~Windmill(){

}

void Windmill::init(){


    this->init_logic();
    this->init_data();  // initializes this object's data

    // DO NOT SEND TO GPU AGAIN..


}

void Windmill::init_logic(){
    windmill_mode = WindmillMode::OFF;

    current_blade_angle = 0;
    windmill_turbine_direction = 1; //arbitrary direction -1 or 1
    windmill_mode = 0;  // (ON or OFF) just 0 or 1 

    view_angles.x = 0;  // in radians for windmill rotations
    view_angles.y = 0;  // TODO: rename the variable to something better
    view_angles.z = 0;
}

// switch the state of the windmill, on or off
uint Windmill::switch_mode(){
    windmill_mode ^= 1;
    return windmill_mode;
}

// switch the direction of the blades, CW or CCW
int Windmill::switch_turbine_direction(){
    windmill_turbine_direction ^= 0xFFFFFFFE;
    return windmill_turbine_direction;
}

/*  TICK
    public: called when application needs to 
            iterate the windmill for one 
            iteration 

    only called within the application's timer 
    function every 16 ms or something
*/
void Windmill::tick(){
    // engage the turbine, turn the fan..
    if (windmill_mode == WindmillMode::ON) {
        this->turn_blades(toRadians(TURN_RATE_DEGREES_PER_FRAME));
    }
}

/* 
    TURN_BLADES
    rotates the blades about the model's z axis
        to do so:   change the rotation factor 
        for each model view matrix */
void Windmill::turn_blades(float rad){
    // turn the blades
    //printf(" >> Turning Windmill Blades By: %f rad.\n", rad);
    
    update_mv_matricies();

    current_blade_angle += rad / DegreesToRadians;
    if (current_blade_angle >= 360){
        current_blade_angle = 0; // since its actually a float, cant use '%'
    } 
}

/* rotate:
    turns the entire windmill about
    the world model y axis.. */
void Windmill::rotate(float rad){ // rad is a change in radians
    view_angles.y += rad;
    if (view_angles.y >= 2* M_PI){
        view_angles.y = 0;
    }
    update_mv_matricies();
}

void Windmill::update_mv_matricies(){
    mat4 rotateY = RotateY(view_angles.y / DegreesToRadians);

    // UPDATE: perform the transformation on the object at the origin...
    model_view_matricies[0] = views[0] * Translate(offset) * rotateY * Translate(-offset) * models[0];

    for (int i = 1; i < NumWindmillComponents; ++i) {
        // gotta rotate the blades themselves 
        // around their relative z asix
        mat4 ctm =  rotateY *
                Translate(0, blade.height, 0)  *
                RotateZ( (current_blade_angle + 180*(i-1)) 
                         * windmill_turbine_direction) *
                Translate(0, -blade.height, 0);

        // UPDATE: perform the transformation on the object at the origin...
        model_view_matricies[i] = views[i] * Translate(offset) * ctm * Translate(-offset) * models[i];
    }
}


/*  
    initializes the unit cube ...
    before creating the verticies that 
    the gpu can use to create primatives 
*/
void Windmill::init_data(){

    /* initializing the points and colors for the base cube.. */
    int i = 0;

    vertices[i] = point4( -U_CUBE_C, -U_CUBE_C,  U_CUBE_C, 1.0 ); i++;
    vertices[i] = point4( -U_CUBE_C,  U_CUBE_C,  U_CUBE_C, 1.0 ); i++;
    vertices[i] = point4(  U_CUBE_C,  U_CUBE_C,  U_CUBE_C, 1.0 ); i++;
    vertices[i] = point4(  U_CUBE_C, -U_CUBE_C,  U_CUBE_C, 1.0 ); i++;
    vertices[i] = point4( -U_CUBE_C, -U_CUBE_C, -U_CUBE_C, 1.0 ); i++;
    vertices[i] = point4( -U_CUBE_C,  U_CUBE_C, -U_CUBE_C, 1.0 ); i++;
    vertices[i] = point4(  U_CUBE_C,  U_CUBE_C, -U_CUBE_C, 1.0 ); i++;
    vertices[i] = point4(  U_CUBE_C, -U_CUBE_C, -U_CUBE_C, 1.0 ); //end


    i = 0;

    vertex_colors[i] = color4( 0.0, 0.0, 0.0, 1.0 ); i++;  // black
    vertex_colors[i] = color4( 1.0, 0.0, 0.0, 1.0 ); i++;  // red
    vertex_colors[i] = color4( 1.0, 1.0, 0.0, 1.0 ); i++;  // yellow
    vertex_colors[i] = color4( 0.0, 1.0, 0.0, 1.0 ); i++;  // green
    vertex_colors[i] = color4( 0.0, 0.0, 1.0, 1.0 ); i++;  // blue
    vertex_colors[i] = color4( 1.0, 0.0, 1.0, 1.0 ); i++; // magenta
    vertex_colors[i] = color4( 1.0, 1.0, 1.0, 1.0 ); i++;  // white
    vertex_colors[i] = color4( 0.0, 1.0, 1.0, 1.0 );   // cyan
    /* colors and points for the unit cube have been initialized */
    printf("1.a.) Initialized: Windmill Geometry\n");

    i = 0;  


    post.width = 2 * UNIT_CUBE_D;
    post.height = 20 * UNIT_CUBE_D;

    blade.width = 3 * UNIT_CUBE_D;
    blade.height = 10 * UNIT_CUBE_D;

    // INITIALIZE MODELS:   (translation, scalling, and rotation, etc)

    init_models();

    // INITIALZE VIEWS: ("position" matrix at which the "camera" views the objects)

    init_views();

    /* - - -   CREATE:    MODEL VIEW MATRICIES  - - -  */

    for (int i = 0; i < NumWindmillComponents; ++i) {
        model_view_matricies[i] =  views[i] * models[i];
    }

}

void Windmill::init_models() {
    models[0] = mat4({2,0,0,0}, {0,20,0,0}, {0,0,2,0}, {0,0,0,1});  //post
    models[0] =  Translate(offset) * models[0];

    for (int i = 1; i < NumWindmillComponents; ++i) {
        models[i] = mat4({3,0,0,0}, 
                        {0,10,0, blade.width/2 + blade.height/2}, 
                        {0,0,1,0}, 
                        {0,0,0,1}); //blades:
    }


    for (int i = 1; i < NumWindmillComponents; ++i) {
        models[i] =  RotateZ(90*(i-1)) * models[i];// ROTATE
        models[i] =  Translate(0,blade.height, -post.width) * models[i];// TRANSLATE
        models[i] =  Translate(offset) * models[i];
    }

}


// create the views.. currently not necssary
void Windmill::init_views(){
    //     point4  eye( radius*sin(theta)*cos(phi),
    //          radius*sin(theta)*sin(phi),
    //          radius*cos(theta),
    //          1.0 );
    //     point4  at( 0.0, 0.0, 0.0, 1.0 );
    //     vec4    up( 0.0, 1.0, 0.0, 0.0 );

    // mat4  view = LookAt( eye, at, up );
    mat4 view = mat4({1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1});
    for (int i = 0; i < NumWindmillComponents; ++i){
        views[i] = view;
    }
}

/* 
    Creates the VAO and VBO 
    Sends data to the GPU
    Creates the shader program  
*/

void Windmill::init_gl(){
    colorcube();    //generates a colored cube..

    // for (int i = 0; i < 6; ++i)
    // {
    //  printf("%f %f %f %f\n", 
    //      points[i].x , points[i].y , points[i].z , points[i].w);
    // }

    glGenVertexArrays( 1, &(this->vao) );   
    glBindVertexArray( this->vao );         // bind the vertex array object

    glGenBuffers( 1, &(this->vbo) );        
    glBindBuffer( GL_ARRAY_BUFFER, this->vbo );// bind the array buffer object

printf("1.b.1.) Initialized: GPU VAO && VBO specifically for Windmill\n");

    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
          NULL, GL_STATIC_DRAW );
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
        glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

printf("1.b.2.) Initialized: GPU with Windmill Geometry Data\n");
    
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
    new_color = glGetUniformLocation( program_id, "new_color" );

    /* belay the possiblity of misuse */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}


/* 
    Uses this program to make sure the context is correct:

    Binds the vao.. (same reason)

    sends the mv matricies to the GPU at the location in the GPU memory 
    given by the loation of the uniform variables

    draws all of the verticies in the buffer array given the MV matrix
*/

void Windmill::display(){
    
    mat4 mv;

    glUseProgram( program_id );
    glBindVertexArray( this->vao );         // bind the vertex array object
    glBindBuffer( GL_ARRAY_BUFFER, this->vbo );// bind the array buffer object

// printf("%f %f %f %f\n", eye.x, eye.y, eye.z, eye.w);

    // windmill post
        glUniformMatrix4fv( model_view, 1, GL_TRUE, model_view_matricies[0] );
        glUniform4fv(new_color, 1, color4( 1.0, 0.0, 0.0, 1.0 ));
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

    // blade 1

        glUniformMatrix4fv( model_view, 1, GL_TRUE, model_view_matricies[1] );
        glUniform4fv(new_color, 1, color4( 1.0, 1.0, 0.0, 1.0 ));
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

    // blade 2

        glUniformMatrix4fv( model_view, 1, GL_TRUE, model_view_matricies[2] );
        glUniform4fv(new_color, 1, color4( 0.0, 1.0, 0.0, 1.0 ));
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    
    // blade 3

        glUniformMatrix4fv( model_view, 1, GL_TRUE, model_view_matricies[3] );
        glUniform4fv(new_color, 1, color4( 0.0, 0.0, 1.0, 1.0 ));
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    
    // blade 4

        glUniformMatrix4fv( model_view, 1, GL_TRUE, model_view_matricies[4] );
        glUniform4fv(new_color, 1, color4( 0.0, 1.0, 1.0, 1.0 ));
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    
}



/*   - - - -    Private Helpers   - - - -    */

/*  
    Creating the verticies that the 
    gpu uses to create primatives
*/

void Windmill::quad( int a, int b, int c, int d ) {
    
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++; 
}

void Windmill::colorcube() {
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}
