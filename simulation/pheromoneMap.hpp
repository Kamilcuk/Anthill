#ifndef PHEROMONE_MAP_H
#define PHEROMONE_MAP_H

#include <vector>
#include <stdexcept>
#include <math.h>

#include "updatable.h"
#include "point.hpp"


class PheromoneMap : public Updatable
{
	using Grid2D = std::vector<std::vector<float>>;
	Grid2D map_;

	// trying to achieve (1-e^-(t/To))
	float decay_coeff_;
	float decay(float p_current_strength);

	// clips map edges
	void addStrengthClipping(unsigned p_x, unsigned p_y, 
		float p_strength);
	void addStrength(unsigned p_x, unsigned p_y, float p_strength);

public:
	PheromoneMap(unsigned p_size_x, unsigned p_size_y);
	~PheromoneMap();

	void step();

	// Determines how fast the smell decays. Must be 0-1.
	void setDecayCoeff(float p_decay_coeff);

	// Creates a circular blob of pheromone at specified
	// position. Maximum strength (in the middle of the blob)
	// is specified as p_initial_strength param. Pheromone
	// strength goes down to zero towards the edges of the blob.
	void createBlob(const Point& p_pos, const float p_radius, 
		float p_initial_strength);

	const Grid2D& getMap();
	float getStrengthAtPosition(const Point& p_pos);
};

#endif // PHEROMONE_MAP_H
