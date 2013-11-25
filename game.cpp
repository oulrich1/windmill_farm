/* game.cpp
 *
 * Implementation of Game class.
 *
 */

#include "game.h"
#include <cstdlib>
#include <GL/glut.h>

// static initializer
Game* Game::callbackInstance(NULL);

Game::Game() {
}

Game::Game(int* argcp, char** argv) {
    PREV_KEY = ' '; //keeps track of the key pressed 

    printf("Creating instance of game\n"); 

    glut_initialize(argcp, argv);       //initializes the window and display..

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

Game::~Game() {
    // do any cleanup
}

void Game::glut_initialize(int* argcp, char** argvp) {
    // GLUT init code here...
    GLenum GlewInitResult;
     CurrentWidth = WINDOW_WIDTH;
     CurrentHeight = WINDOW_HEIGHT;

    /*  - - - - - - -  */
    glutInit(argcp, argvp);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(CurrentWidth, CurrentHeight);
    glutInitContextVersion(4, 0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );
    
    WindowHandle = 0;
    WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

    if(WindowHandle < 1) {
        fprintf(
            stderr,
            "ERROR: Could not create a new rendering window.\n"
        );
        exit(EXIT_FAILURE);
    }


    // event handlers are set when the game is run..

    /*  - - - - - - -  */

    glewExperimental = GL_TRUE;
    GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult) {
        fprintf(
            stderr,
            "ERROR: %s\n",
            glewGetErrorString(GlewInitResult)
        );
        exit(EXIT_FAILURE);
    }
    
    
    fprintf(
        stdout,
        "   INFO: OpenGL Version: %s\n",
        glGetString(GL_VERSION)
    );

    printf("\n0.) Initialized: GLUT\n");

}




/* --------------------------------------------------------------------------  */
/* CaLLBACK FUNCTIONS: hopefulling listed in the order that they were registred*/


/* Render */
void Game::display() {
    // // display code here...

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* call draw arrays for each of the vbo's */
        windmill->display();
    
    glutSwapBuffers();   // swap double buffers
    glutPostRedisplay(); // mark the window to be redrawn
}

void Game::displayWrapper() {
    callbackInstance->display();
}



void Game::resize(int width, int height) {
    // resize code here...
    CurrentWidth = width;
    CurrentHeight = height;
    glutInitWindowSize(CurrentWidth, CurrentHeight);
    glViewport(0, 0, CurrentWidth, CurrentHeight);
    glutPostRedisplay();
}

void Game::resizeWrapper(int width, int height) {
    callbackInstance->resize(width, height);
}



void Game::keyboard(unsigned char key, int x, int y) {
      // keyboard code here...
    
    switch( key ) {
    case 033: // Escape Key
    case 'q': case 'Q':
        exit( EXIT_SUCCESS );
        break;

    case 'y':  
        WM(windmill)->rotate(toRadians(5));
    break;
    case 'Y':
        WM(windmill)->rotate(toRadians(-5));
    break;
    case 's': {
        // start/stop windmill turning
        uint wm_mode = WM(windmill)->switch_mode();    
        if(wm_mode == WindmillMode::OFF){
            printf("Windmill Mode Off\n");
        } else {
            printf("Windmill Mode On\n");
        }
        break;
    }
    case 'S': {
        // start/stop windmill turning
        int wm_direction = WM(windmill)->switch_turbine_direction();    
        if(wm_direction == 1){
            printf("Windmill rotating CW\n");
        } else {
            printf("Windmill rotating CCW\n");
        }
        break;
    }
    case 'r': 
    case ' ':  // reset values to their defaults
        WM(windmill)->init();
        break;
    }
    glutPostRedisplay();
}

void Game::keyboardWrapper(unsigned char keycode, int x, int y) {
    callbackInstance->keyboard(keycode, x, y);
}




void Game::mouseMotionFunc(int x, int y){
    //printf("%d %d\n", x, y);
    //paddle->set_position(vec2( ((2.0*x)/CurrentWidth - 0.82) ,  ((-2.0*y)/CurrentHeight) + 1.76) );

}

void Game::mouseMotionFuncWrapper(int x, int y){
    callbackInstance->mouseMotionFunc(x, y);
}


void Game::timerFunc(int value){
    WM(windmill)->tick();   // calcuate next position (if relevant)

    // perform animations and calculations regarding animations here
    glutPostRedisplay();
}


void Game::timerFuncWrapper(int value){
    callbackInstance->timerFunc(value);

   
    glutTimerFunc(16, timerFuncWrapper, ++value);
    
}


void Game::idleFunc(void){
    //glutPostRedisplay();
}

void Game::idleFuncWrapper(void){
    callbackInstance->idleFunc();
}





void Game::closeCleanup(void){
    // clean up stuff when window is closed..
    // call this's destructor..
}

void Game::closeCleanupWrapper(void){
    callbackInstance->closeCleanup();
}

/* -------------------------------------------------- */

void Game::initGame() {
    if (callbackInstance == NULL)     {
        callbackInstance = this;
    }   

    /* - - - - - - - - - - - - - - - - - - - - */
    /*      Shader init                        */

    //GLuint programId  = InitShader(  "shader_vertex_generic.glsl", 
                        //            "shader_fragment_generic.glsl" );

    printf("1.) Initialized: Shaders\n");


    /* - - - - - - - - - - - - - - - - - - - - */
    /*      game init code here...             */

    // initializes the object data points 
    //    && sends data to GPU
    windmill = new Windmill(); 

    
    printf("2.) Initialized: Geometry Objects\n");
}

/* -------------------------------------------------  */



/* regeisters the glut callbacks.. */
void Game::registerCallbacks() {
    glutDisplayFunc(displayWrapper);        // calls PostRedisplay
    glutReshapeFunc(resizeWrapper);         // inits the window when window is modified
    glutKeyboardFunc(keyboardWrapper);      // keyboard callback

    glutPassiveMotionFunc(mouseMotionFuncWrapper);

    glutTimerFunc(0, timerFuncWrapper, 0);  // timer function callback for animation
    glutIdleFunc(idleFuncWrapper);          // additional calculations

    glutCloseFunc(closeCleanupWrapper);     // on close: cleanup

}



void Game::run() {
    
    //the glut callbacks depend on this..
    this->initGame();               // sets up the game instance: 


    // now we can register c-style callback functions
    this->registerCallbacks();  
    printf("3.) Initialized: Various Callbacks, including Display()\n\n");
    

    glClearColor( 1.0, 1.0, 1.0, 1.0 );


    printf("Last.) Running the game..\n");
    glutMainLoop();
    printf("End.) Game end..\n");
}

// game specific functions here...




