
#include "geometry.h"
#include "utils.h"

typedef std::vector<float>  				VF; 
typedef std::vector<std::vector<float> > 	VVF; 

class Terrain
{
public:
	Terrain();
	~Terrain();

	uint collides(Geometry* collidable);

	// called by the main application, (once)
	// possibly once if nothing changes..
	uint display(); 

	float width;
	float max_height;


private:
	/* 	INIT :
			logic/member variables..
			generate terrain...
			send data to GPU      */

	uint init_terrainlogic();
	uint init_terraindata();
	uint init_gl();

	uint generateRandomTerrain (float MAX_X, float MAX_Y, float MAX_Z);
	float generateCompletelyRandomHeight (float interval_width);

	// 2d array of floats, 
	// that represent height...
	VVF terrain_data;


	float percent_random;
	float land_level_offset;
	float amplitude;


	/* data */
};