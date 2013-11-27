

csci566 Project 3: Camera Flight over the Windmill Farm :)
==========================================

Completed!

About:
------------------------------------------
    Fly over a windmill farm

To Compile and Run:
------------------------------------------
    make
    ./flight_sim

Dependancies:
------------------------------------------
    -lglut -lGLEW -lGL -lGLU -lm 
    (as usual)


Controls:
------------------------------------------
    Windmill Farm:
    [  :   Resets the windmill sim
    o/O   :   Rotates about the Y-Axis in only one direction CW
    p/P   :   Star/Stop the windmill 

    Camera Controls:
    w:  Move Forward
    a:  Move Backward
    s:  Strafe Left
    d:  Strafe Right

    x:  Thrust Up (Strafe Up)
    z:  Thrust Down (Strafe Down)

    q:  Yaw Left
    e:  Yaw Right

    LEFT_ARROW  :   Roll Left
    RIGHT_ARROW :   Roll Right
    UP_ARROW    :   Pitch Down
    DOWN_ARROW  :   Pitch Up


Code:
------------------------------------------
    (Overview of file/design heirarchy)
    Main.cpp -> Game -> Windmill -> Geometry -> Angel.h
                     -> Camera


Challenges:
------------------------------------------
    -> Camera.. that was the challenge..
    -> Trying to figure out how to orient 
        Camera was the most difficult:
            Specifically: components of the thetas

    -> Change the basis of the system and found 
        angle components from that...

    -> Working on the random Landscape





    