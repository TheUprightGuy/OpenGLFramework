#pragma once
#include "utils.h"


class CTerrain
{
public:
	CTerrain();
	~CTerrain();
private:
	// Get the heightmap from the RAW file
	void loadHeightMap();

	// Compute the average height of IJ
	float average(int i, int j);

	//Smooth using this ^
	void smooth();

	std::vector<float> mHeightmap;

public:
};

