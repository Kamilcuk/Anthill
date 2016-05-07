#include "pheromoneMap.hpp"

#include <stdexcept>
#include <math.h>

PheromoneMap::PheromoneMap(World* world, PheromoneMap::Type type, 
	unsigned p_size_x, unsigned p_size_y, float decay_coeff)
	:
    Updatable(world), type_(type), decay_coeff_(decay_coeff)
{
	resize(p_size_x, p_size_y);
}

PheromoneMap::PheromoneMap(World* world) : 
	Updatable(world), type_(Type::None), decay_coeff_(0)
{
}

PheromoneMap::PheromoneMap(const PheromoneMap& other)
	:
    Updatable(other.world_), map_(other.map_), type_(other.type_),
	decay_coeff_(other.decay_coeff_)
{
}

PheromoneMap::~PheromoneMap()
{
}

PheromoneMap& PheromoneMap::operator=(const PheromoneMap& other)
{
	map_ = other.map_;
	type_ = other.type_;
	decay_coeff_ = other.decay_coeff_;
    return *this;
}

bool PheromoneMap::operator==(const PheromoneMap& other)
{
    return *this == other;
}

bool PheromoneMap::operator!=(const PheromoneMap& other)
{
    return *this != other;
}

PheromoneMap::Type PheromoneMap::getType() const
{
	return type_;
}

void PheromoneMap::reset()
{
	for(auto& row : map_) for(auto& cell : row)
	{
		cell = 0;
	}
}

void PheromoneMap::resize(unsigned p_size_x, unsigned p_size_y)
{
	map_ = std::vector<std::vector<float>>
		(p_size_x, std::vector<float>(p_size_y, 0.0));
}

float PheromoneMap::decay(float p_current_strength)
{
	float new_strength = p_current_strength * (1 - decay_coeff_);
	if(new_strength < 0.1 * decay_coeff_)
		new_strength = 0;
	return new_strength;
}


void PheromoneMap::addStrengthClipping(unsigned p_x, unsigned p_y, 
	float p_strength)
{
	// only add if within map boundaries
	if(p_x >= 0 && p_y >= 0 && p_x < map_.size() && p_y < map_[0].size())
		addStrength(p_x, p_y, p_strength);
}

void PheromoneMap::addStrength(unsigned p_x, unsigned p_y, float p_strength)
{
	map_[p_x][p_y] += p_strength;
}

void PheromoneMap::step(int p_delta_time)
{
    if(!p_delta_time){
        return;
    }

	for(auto& row : map_){
        for(auto& cell : row)
        {
            cell = decay(cell);
        }
    }
}

const PheromoneMap::Grid2D& PheromoneMap::getMap()
{
	return map_;
}

float PheromoneMap::getStrengthAtPosition(const Point& p_pos)
{
	if((unsigned)p_pos.posX() > map_.size() || (unsigned)p_pos.posY() >
		map_[0].size())
		throw std::runtime_error("getStrength: Specified position exceeds pheromone map.");
	return map_[(unsigned)p_pos.posX()][(unsigned)p_pos.posY()];
}

Point PheromoneMap::getStrongestAtArea(const Point &middle, const float radius)
{
	// for now its a quadrant
	float strongest = -1;
	Point ret(-1,-1);
	for(int i=-radius;i<radius;++i) for(int j=-radius;j<radius;++j) 
	{
		const unsigned int x = middle.posX()+i;
		const unsigned int y = middle.posY()+i;
		if ( x >= 0 && y >= 0 && x < map_.size() && y < map_.size() ) 
		{
			float strength = getStrengthAtPosition(Point(x, y));
			if ( strength > strongest ) 
			{
				ret = Point(x,y);
				strongest = strength;
			}
		}
	}
	return ret;
}

void PheromoneMap::createBlob(const Point& p_pos, const float p_radius, 
	float p_initial_strength)
{
	if((unsigned)p_pos.posX() > map_.size() || 
			(unsigned)p_pos.posY() > map_[0].size() ||
			(unsigned)p_pos.posX() < 0 || (unsigned)p_pos.posY() < 0)
		throw std::runtime_error("createBlob: Specified position exceeds pheromone map.");
	
	// consider each cell in a square
	for(unsigned x = p_pos.posX() - p_radius; x < p_pos.posX() + p_radius; x++)
		for(unsigned y = p_pos.posY() - p_radius; y < p_pos.posY() + p_radius;
			y++)
		{			
			// strength falls off linearly with distance from center
			const float dist_from_center = sqrt((p_pos.posX() - x) * 
				(p_pos.posX() - x) + (p_pos.posY() - y) * (p_pos.posY() - y));

			if(dist_from_center <= p_radius)
			{
				const float local_strength = p_initial_strength * 
					(1 - dist_from_center / p_radius);
				addStrengthClipping(x, y, local_strength);
			}
		}
}
