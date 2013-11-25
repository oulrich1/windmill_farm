

csci566 Project 2: Windmill
==========================================

Completed!

To Compile and Run:
------------------------------------------
    make

    ./spinning_windmill


Controls:
------------------------------------------
    r   :   Resets the windmill sim
    y   :   Rotates about the Y-Axis in only one direction CW
    s   :   Star/Stop the windmill


Code:
------------------------------------------
    (Overview of file/design heirarchy)
    Main.cpp -> Game -> Windmill -> Geometry -> Angel.h

    Windmill is the bulk of the program; handles sending it's own data
        to the GPU. It also handles the display functions

    Game.cpp is essentially the object's controller.

    Main just starts the Windmill simulation.


Challenges:
------------------------------------------
    -> Much less GL challenge, and more of design challenge
    -> Decisions about implementation were the most challenging:
        ->> Decision between rotation about Y vs
        ->> (vs) changing the view matrix.. but it turned out
        ->> changing the models with the view matrix changed clipping plane
            => todo: eventually figure out why it was doing that

        ->> blade rotation wasn't too bad
            => however: i might have to rethink how i am doing the 
                matrix cacluation





    