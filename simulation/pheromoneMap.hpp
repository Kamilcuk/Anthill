#ifndef PHEROMONE_MAP_H
#define PHEROMONE_MAP_H

#include <vector>

#include "updatable.hpp"
#include "point.hpp"

class World;
class PheromoneMap : public Updatable
{
public:
	enum Type {
		ToFood,
		FromFood
	};
private:
	using Grid2D = std::vector<std::vector<float> >;
	Grid2D map_;

	PheromoneMap::Type type_;

	// trying to achieve (1-e^-(t/To))
	float decay_coeff_;
	float decay(float p_current_strength);

	// clips map edges
	void addStrengthClipping(unsigned p_x, unsigned p_y, 
		float p_strength);
	void addStrength(unsigned p_x, unsigned p_y, float p_strength);

public:


	PheromoneMap(World& world, PheromoneMap::Type type, unsigned p_size_x, unsigned p_size_y, float decay_coeff);
	~PheromoneMap();

	void step(int p_delta_time);

	// resets all pheromone map squares to zero
	void reset();

	// Determines how fast the smell decays. Must be 0-1.
	void setDecayCoeff(float p_decay_coeff);

	// Creates a circular blob of pheromone at specified
	// position. Maximum strength (in the middle of the blob)
	// is specified as p_initial_strength param. Pheromone
	// strength goes down to zero towards the edges of the blob.
	void createBlob(const Point& p_pos, const float p_radius, 
		float p_initial_strength);

	const Grid2D& getMap();
	Grid2D getMapCopy();
	float getStrengthAtPosition(const Point& p_pos);
	Point getStrongestAtArea(const Point& middle, const float radius);
	PheromoneMap::Type getType() const;
};

#endif // PHEROMONE_MAP_H
