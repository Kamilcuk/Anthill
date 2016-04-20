#ifndef PHEROMONE_MAP_H
#define PHEROMONE_MAP_H

#include <vector>

#include "updatable.hpp"
#include "point.hpp"

class World;

/**
 * Represents 2D grid-like a map of pheromones, that ants use to leave smell 
 * trails. There can be many types of pheromones, each with different decay 
 * rates.
 */
class PheromoneMap : public Updatable
{
	using Grid2D = std::vector<std::vector<float>>;
public:
	/// Different types of pheromones.
	enum class Type 
	{
		None,
		ToFood,
		FromFood
	};

	PheromoneMap(World& world, PheromoneMap::Type type, unsigned p_size_x, 
		unsigned p_size_y, float decay_coeff);
	~PheromoneMap();

	/// Implemented from Updatable.
	void step(int p_delta_time);

	/// Resets all pheromone map squares to zero.
	void reset();

	/// Determines how fast the smell decays. Must be 0-1.
	void setDecayCoeff(float p_decay_coeff);

	/// Creates a circular blob of pheromone at specified position. Maximum 
	/// strength (in the middle of the blob) is specified as p_initial_strength 
	/// param. Pheromone strength goes down to zero towards the edges of the 
	/// blob.
	void createBlob(const Point& p_pos, const float p_radius, 
		float p_initial_strength);

	/// Returns pheromone strength at specified position.
	float getStrengthAtPosition(const Point& p_pos);
	
	/// Returns position of strongest pheromone in a square area specified in 
	/// params.
	Point getStrongestAtArea(const Point& middle, const float radius);
	
	const Grid2D& getMap();
	Grid2D getMapCopy();
	PheromoneMap::Type getType() const;
	
private:
	Grid2D map_;

	const PheromoneMap::Type type_;

	// trying to achieve (1-e^-(t/To))
	float decay_coeff_;
	float decay(float p_current_strength);

	// adds smell strength to strength at specified position
	void addStrength(unsigned p_x, unsigned p_y, float p_strength);
	
	// same as addStrength but doesn't do anything if specified position is 
	// outside of map
	void addStrengthClipping(unsigned p_x, unsigned p_y, 
		float p_strength);
		
};

#endif // PHEROMONE_MAP_H
