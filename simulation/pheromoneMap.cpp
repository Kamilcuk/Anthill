#include "pheromoneMap.hpp"

PheromoneMap::PheromoneMap(World& world, PheromoneMap::Type type, unsigned p_size_x, unsigned p_size_y) :
	Updatable(world),
	type_(type),
	decay_coeff_(0.05)
{
	map_ = std::vector<std::vector<float>>
		(p_size_x, std::vector<float>(p_size_y, 0.0));
}


PheromoneMap::~PheromoneMap()
{

}


PheromoneMap::Type PheromoneMap::getType() const
{
	return type_;
}

float PheromoneMap::decay(float p_current_strength)
{
	return p_current_strength * (1 - decay_coeff_);
}


void PheromoneMap::addStrengthClipping(unsigned p_x, unsigned p_y, 
	float p_strength)
{
	if(p_x < 0) p_x = 0;
	if(p_x > map_.size() - 1) p_x = map_.size() - 1;
	if(p_y < 0) p_y = 0;
	if(p_y > map_[0].size() - 1) p_y = map_.size() - 1;
	addStrength(p_x, p_y, p_strength);
}


void PheromoneMap::addStrength(unsigned p_x, unsigned p_y, float p_strength)
{
	map_[p_x][p_y] = p_strength;
}


void PheromoneMap::step()
{
	for(auto& row : map_) for(auto& cell : row)
	{
		cell = decay(cell);
	}
}

const PheromoneMap::Grid2D& PheromoneMap::getMap()
{
	return map_;
}
PheromoneMap::Grid2D PheromoneMap::getMapCopy()
{
	return map_;
}

float PheromoneMap::getStrengthAtPosition(const Point& p_pos)
{
	if((unsigned)p_pos.posX() > map_.size() || (unsigned)p_pos.posY() > map_[0].size())
		throw std::runtime_error("Specified position exceeds pheromone map.");
	return map_[(unsigned)p_pos.posX()][(unsigned)p_pos.posY()];
}

Point PheromoneMap::getStrongestAtArea(const Point &middle, const float radius)
{
	/** for now its a quadrant */
	float strongest = -1;
	Point ret(-1,-1);
	for(int i=-radius;i<radius;++i) {
		for(int j=-radius;j<radius;++j) {
			unsigned int x = middle.posX()+i;
			unsigned int y = middle.posY()+i;
			if ( x >= 0 && y >= 0 && x < map_.size() && y < map_.size() ) {
				float strength = getStrengthAtPosition(Point(x, y));
				if ( strength > strongest ) {
					ret = Point(x,y);
					strongest = strength;
				}
			}
		}
	}
	return ret;
}


void PheromoneMap::createBlob(const Point& p_pos, const float p_radius, 
	float p_initial_strength)
{
	// consider each cell in a square
	for(unsigned x = p_pos.posX() - p_radius; x < p_pos.posX() + p_radius; x++)
		for(unsigned y = p_pos.posY() - p_radius; y < p_pos.posY() + p_radius; y++)
		{
			// strength falls off linearly with distance from center
			const float dist_from_center = sqrt((p_pos.posX() - x) * (p_pos.posX() - x) + 
					(p_pos.posY() - y) * (p_pos.posY() - y));
			const float local_strength = p_initial_strength * (1 - dist_from_center / p_radius);
			
			addStrengthClipping(x, y, local_strength);
		}
}
