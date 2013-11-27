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
    glutInitWindowPosition(WindowPosX, WindowPosY);
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


   glutSetCursor(GLUT_CURSOR_INHERIT);  //GLUT_CURSOR_INHERIT, GLUT_CURSOR_NONE
   glutWarpPointer(CurrentWidth/2, CurrentHeight/2); //for every frame as well
 

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

        for (int i = 0; i < windmills.size(); ++i){
            WM(windmills[i])->display();
        }

    
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

    cam->aspect = GLfloat(width)/height;
    cam->update_proj();   

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
    case '!': case '1':
        exit( EXIT_SUCCESS );
        break;

    case 'o':  
        WM(windmill)->rotate(toRadians(5));
        for (int i = 0; i < windmills.size(); ++i){
            WM(windmills[i])->rotate(toRadians(5));
        }
    break;
    case 'O':
        WM(windmill)->rotate(toRadians(-5));
        for (int i = 0; i < windmills.size(); ++i){
            WM(windmills[i])->rotate(toRadians(-5));
        }
    break;
    case 'p': {
        // start/stop windmill turning
        uint wm_mode = WM(windmill)->switch_mode();    

        for (int i = 0; i < windmills.size(); ++i){
            WM(windmills[i])->switch_mode();
        }

        if(wm_mode == WindmillMode::OFF){
            printf("Windmill Mode Off\n");
        } else {
            printf("Windmill Mode On\n");
        }
        break;
    }
    case 'P': {
        // start/stop windmill turning
        int wm_direction = WM(windmill)->switch_turbine_direction();   
        for (int i = 0; i < windmills.size(); ++i){
            WM(windmills[i])->switch_turbine_direction();
        } 
        if(wm_direction == 1){
            printf("Windmill rotating CW\n");
        } else {
            printf("Windmill rotating CCW\n");
        }
        break;
    }

    /* plane controls: set throttle.. */
    case 't': {
        plane->increaseThrottle();
        break;
    }
    case 'T': {
        plane->decreaseThrottle();
        break;
    }

    /* plane controls: forward,backward, strafe: sides: left,right */
    case 'w': {
        //cam->walk(vec4(0,0,-1,0));
        plane->controlDirection(vec4(0,0,-1,0));
        break;
    }
    case 'a': {
        plane->controlDirection(vec4(-1,0,0,0));
        break;
    }
    case 's': {
       plane->controlDirection(vec4(0,0,1,0));
        break;
    }
    case 'd': {
       plane->controlDirection(vec4(1,0,0,0));
        break;
    }

    /* plane controls : strafe UP/DOWN */
    case 'x': {
       plane->controlDirection(vec4(0,1,0,0));
        break;
    }
    case 'z': {
       plane->controlDirection(vec4(0,-1,0,0));
        break;
    }

    /* plane controls: YAW  */
    case 'q': {
        plane->controlStick(vec4(0,-M_PI/32,0,0));
        break;
    }
    case 'e': {
        plane->controlStick(vec4(0,M_PI/32,0,0));
        break;
    }
    case '[': {  // reset values to their defaults
        WM(windmill)->init();
        for (int i = 0; i < windmills.size(); ++i){
            WM(windmills[i])->init();
        }
        break;
    }
    case ' ':
        plane->resetOrientation();
        break;
    }
    glutPostRedisplay();
}

void Game::keyboardWrapper(unsigned char keycode, int x, int y) {
    callbackInstance->keyboard(keycode, x, y);
}


void Game::specialKeys(int key, int x, int y) {
      // keyboard code here...

    switch( key ) {
        /* plane controls : ROLL and PITCH */
        case GLUT_KEY_LEFT: {
            plane->controlStick(vec4(0,0,-M_PI/32, 0));
            //cam->walk(vec4(-1,0,0,0));
            break;
        }
        case GLUT_KEY_RIGHT: {
            plane->controlStick(vec4(0,0,M_PI/32, 0));
            //cam->walk(vec4(1,0,0,0));
            break;
        }
        case GLUT_KEY_UP: {
            //cam->walk(vec4(0,0,0,0));
            plane->controlStick(vec4(-M_PI/32,0,0, 0));
            break;
        }
        case GLUT_KEY_DOWN: {
            //cam->walk(vec4(0,0,0,0));
            plane->controlStick(vec4(M_PI/32,0,0, 0));
            break;
        }

    }
    glutPostRedisplay();
}

void Game::specialKeysWrapper(int keycode, int x, int y){
    callbackInstance->specialKeys(keycode, x, y);
}



/* MOUSE CALLBACKS */
void Game::mouse(int button, int state, int x, int y){
    
}

void Game::mouseWrapper(int button, int state, int x, int y){
    callbackInstance->mouse(button, state, x, y);
}


void Game::mouseMotionFunc(int x, int y){
    currentMouseX = x;
    currentMouseY = y;
    //printf("%d %d\n", x, y);
    //paddle->set_position(vec2( ((2.0*x)/CurrentWidth - 0.82) ,  ((-2.0*y)/CurrentHeight) + 1.76) );
}

void Game::mouseMotionFuncWrapper(int x, int y){
    callbackInstance->mouseMotionFunc(x, y);
}
/* - - END - - - - - - MOUSE CALLBACKS */


void Game::getMousePos(int *mouseX, int *mouseY){
    *mouseX = currentMouseX;
    *mouseY = currentMouseY;
}


void Game::timerFunc(int value){
    const float mouseSensitivity = 0.05;
    int mouseX, mouseY;
    getMousePos(&mouseX, &mouseY);
    mouseX = mouseX - CurrentWidth/2;
    mouseY = mouseY - CurrentHeight/2;
    cam->offsetOrientation(vec2(mouseSensitivity * mouseX, mouseSensitivity * mouseY));
    //glutWarpPointer(CurrentWidth/2, CurrentHeight/2);


    WM(windmill)->tick();   // calcuate next position (if relevant)

    for (int i = 0; i < windmills.size(); ++i){
        WM(windmills[i])->tick();
    }

    plane->fly();
    //cout << plane->throttleMessage() << endl;

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

        // member variable "program_id"
    program_id = InitShader(  "shader_vertex_generic.glsl", 
                              "shader_fragment_generic.glsl" );
    glUseProgram( program_id );




    printf("1.) Initialized: Shaders\n");


    /* - - - - - - - - - - - - - - - - - - - - */
    /*      game init code here...             */

    // initializes the object data points 
    //    && sends data to GPU
    //
    windmill = new Windmill(program_id); 
    windmills = std::vector<Geometry*>(100, NULL);  

    srand(time(NULL));
    for(int i = 0; i < windmills.size(); i++){
        vec4 offset = vec4(rand_f() * 30 - 15, rand_f()-1, rand_f() * 30 - 15, 0);
        cout << offset << endl;
        windmills[i] = new Windmill(program_id, offset);
    }
    cout << endl;

    /* - - - - - - - - - - - - - - - - - - - - */
    /*      camera init code here...  */
    /* What this part does is updates the shaders so that the
       uniform variables 'camera_view and projection' are set */
    cam = new Camera(program_id);

    // the user controls the plane.
    plane = new Plane(cam);


    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
    
    printf("2.) Initialized: Game Objects\n");
}

/* -------------------------------------------------  */



/* regeisters the glut callbacks.. */
void Game::registerCallbacks() {
    glutDisplayFunc(displayWrapper);        // calls PostRedisplay
    glutReshapeFunc(resizeWrapper);         // inits the window when window is modified
    glutKeyboardFunc(keyboardWrapper);      // keyboard callback
    glutSpecialFunc(specialKeysWrapper);

    glutMouseFunc(mouseWrapper);
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




