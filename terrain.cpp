#include "terrain.h"

/* by default it generates:
    RANDOM TERRAIN    */
Terrain::Terrain(){
    uint err = 0; 

    err = init_terrainlogic();  // sets member verables n stuff
        err = err << 1;

    err |= init_terraindata();      // initializes the terrain data.. (Does not send to the GPU )

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
    land_level_offset   = 10;
    amplitude           = 1;

    max_height = 1;
    width = 100;

    return 0;
}
uint Terrain::init_terraindata(){
    // load data...
    // set data in memory...

    //generate data..
    generateRandomTerrain(width, width, max_height); // 100 by 100 by 1 : landscape

    return 0;
}
uint Terrain::init_gl(){
    // send data to the gpu

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

    for(auto terrain_row : terrain_data){
        for(auto h : terrain_row){
            cout << h << " ";
        }
        cout << endl;
    }

    return 0;
}


uint Terrain::display(){
    /* generate MV matrix:
        send to GPU..
        Display.. render...
    (REALLY ONLY NEEDS TO HAPPEN ONCE...) */
    return 0;
}

uint Terrain::collides(Geometry* collidable){
    return 0;
}