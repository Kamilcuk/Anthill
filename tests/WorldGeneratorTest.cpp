#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include <stdlib.h>
#include <time.h>

#include <boost/make_shared.hpp>

#include "../simulation/world.hpp"
#include "../simulation/worldGenerator.hpp"
#include "../simulation/shapeGenerator.hpp"
#include "../simulation/anthill.hpp"
#include "../simulation/obstacle.hpp"
#include "../simulation/ant.hpp"

namespace WorldGeneratorTest
{
    
struct Fixture 
{
    const unsigned world_dim_x = 100, world_dim_y = 100;
    const float generation_random_epsilon = 30; // in percent
    Fixture() { srand(time(NULL)); }
    World world;
};    

BOOST_FIXTURE_TEST_CASE(test_placeAnthill_shouldBeAnthill, Fixture)
{
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Anthill>().size(), 0);
    AnthillParams params;
    params.quantity = 1;    
    WorldGenerator::placeAnthill(&world, params);
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Anthill>().size(), 1);
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 1);
}

BOOST_FIXTURE_TEST_CASE(test_placeAnts_shouldBeAnts, Fixture)
{
    WorldGenerator::placeAnthill(&world);    
    
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 0);    
    AntsParams params;
    params.quantity = 10;
    params.min_dist_from_anthill = 2;
    params.max_dist_from_anthill = 5;
    WorldGenerator::placeAnts(&world, params);
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 10);
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 11); // (+ anthill)
}

BOOST_FIXTURE_TEST_CASE(
    test_placeAntsWhenAnthillNearEdge_shouldNotPlaceAntsOutsideMap, Fixture)
{
    WorldGenerator::placeAnthill(&world);
    const unsigned size_x = 100, size_y = 100;
    world.setDimensions(size_x, size_y);
    auto anthill = world.getSimulationObjects<Anthill>()[0];
    const Point near_margin(1, 10);
    anthill->setPos(near_margin);
    
    AntsParams params;
    params.quantity = 10;
    params.min_dist_from_anthill = 2;
    params.max_dist_from_anthill = 5;
    WorldGenerator::placeAnts(&world, params);
    
    auto ants = world.getSimulationObjects<Creature>();
    for (auto a : ants)
        BOOST_CHECK(a->getPos().isInBounds(size_x, size_y));
}

BOOST_FIXTURE_TEST_CASE(
    test_placeManyAntsInSmallRadius_shouldThrow, Fixture)
{
    WorldGenerator::placeAnthill(&world);

    AntsParams params;
    const unsigned to_spawn = 1000;
    params.quantity = to_spawn;
    params.min_dist_from_anthill = 2;
    params.max_dist_from_anthill = 5;
   
    BOOST_CHECK_THROW(WorldGenerator::placeAnts(&world, params), 
        std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(
    test_placeAntsWhenNoEmptySpace_shouldThrow, Fixture)
{
    WorldGenerator::placeAnthill(&world);
    auto anthill = world.getSimulationObjects<Anthill>()[0];
    auto anthill_pos = anthill->getPos();
    
    const float radius = 5;
    
    // spam a blob of obstacles around the anthill
    for(const auto& p : ShapeGenerator::GenerateCircle(anthill_pos, radius))
        world.addSimulationObject<Obstacle>(
            boost::make_shared<Obstacle>(&world, p));
    
    AntsParams params;
    params.quantity = 1;
    params.min_dist_from_anthill = 2;
    params.max_dist_from_anthill = radius;
    BOOST_CHECK_THROW(WorldGenerator::placeAnts(&world, params), 
        std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(test_placeAntsInvalidParams_shouldThrow, Fixture)
{
    AntsParams params;
    params.quantity = -10;
    BOOST_CHECK_THROW(WorldGenerator::placeAnts(&world, params), 
        std::runtime_error);
    params.quantity = 10;
    params.min_dist_from_anthill = -2;
    BOOST_CHECK_THROW(WorldGenerator::placeAnts(&world, params), 
        std::runtime_error);
    params.min_dist_from_anthill = 2;
    params.max_dist_from_anthill = -5;
    BOOST_CHECK_THROW(WorldGenerator::placeAnts(&world, params), 
        std::runtime_error);
    params.max_dist_from_anthill = 5;
}

BOOST_FIXTURE_TEST_CASE(
    test_userFriendlyAntCreation_shouldCreateSpecifiedNumAnts, Fixture)
{
    WorldGenerator::placeAnthill(&world);    
    
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 0);    
    AntsParams params;
    const unsigned to_spawn = 20;
    params.applyNumAnts(to_spawn);
    WorldGenerator::placeAnts(&world, params);
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), to_spawn);
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 1 + to_spawn); // + anthill
}

BOOST_FIXTURE_TEST_CASE(
    test_userFriendlyAntCreationPlaceManyAnts_shouldExpandToFitAll, Fixture)
{
    WorldGenerator::placeAnthill(&world);    
    
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 0);    
    AntsParams params;
    const unsigned to_spawn = 2000, original_max_radius = 5;
    params.quantity = to_spawn;
    params.max_dist_from_anthill = original_max_radius;
    
    params.applyNumAnts(to_spawn);
    BOOST_CHECK(original_max_radius < (unsigned)params.max_dist_from_anthill);
    
    WorldGenerator::placeAnts(&world, params);
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), to_spawn);
}

BOOST_FIXTURE_TEST_CASE(
    test_placeObstacles_shouldBeAroundSpecifiedDensityObstacles, Fixture)
{
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Obstacle>().size(), 0);
    world.setDimensions(world_dim_x, world_dim_y);
    ObstaclesParams params;
    WorldGenerator::placeObstacles(&world, params);
    
    const float actual_num = world.getSimulationObjects<Obstacle>().size(),
        expected_num =
            params.quantity_per_100_by_100 * world_dim_x * world_dim_y / 10000;
    
    BOOST_CHECK_CLOSE(actual_num / expected_num, 1, generation_random_epsilon);
    BOOST_CHECK(world.getEntityPtrs().size() > 0);
}

BOOST_FIXTURE_TEST_CASE(test_placeObstaclesInvalidParams_shouldThrow, Fixture)
{
    ObstaclesParams params;
    params.quantity_per_100_by_100 = -5;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params),
        std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(
    test_userFriendlyObstacleCreation_shouldUseLinearScale0IsHalf100IsDouble, 
    Fixture)
{
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Obstacle>().size(), 0);    
    ObstaclesParams params;
    const unsigned original = params.quantity_per_100_by_100;
    params.applyObstacleFrequency(50);
    BOOST_CHECK_EQUAL(params.quantity_per_100_by_100, original);
    params.applyObstacleFrequency(25);
    BOOST_CHECK_EQUAL(params.quantity_per_100_by_100, (original / 2));
}

BOOST_FIXTURE_TEST_CASE(
    test_placeFoods_shouldBeAroundSpecifiedDensityFoods, Fixture)
{
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Food>().size(), 0);
    world.setDimensions(world_dim_x, world_dim_y);
    FoodsParams params;
    WorldGenerator::placeFoods(&world, params);
    
    const float actual_num = world.getSimulationObjects<Food>().size(),
        expected_num =
            params.quantity_per_100_by_100 * world_dim_x * world_dim_y / 10000;
    
    BOOST_CHECK_CLOSE(actual_num / expected_num, 1, generation_random_epsilon);
    BOOST_CHECK(world.getEntityPtrs().size() > 0);
}

BOOST_FIXTURE_TEST_CASE(test_placeFoodsInvalidParams_shouldThrow, Fixture)
{
    FoodsParams params;
    params.quantity_per_100_by_100 = -5;
    BOOST_CHECK_THROW(WorldGenerator::placeFoods(&world, params),
        std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(
    test_userFriendlyFoodCreation_shouldUseLinearScale0IsHalf100IsDouble, 
    Fixture)
{
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Food>().size(), 0);    
    FoodsParams params;
    const unsigned original = params.quantity_per_100_by_100;
    params.applyFoodGenerosity(50);
    BOOST_CHECK_EQUAL(params.quantity_per_100_by_100, original);
    params.applyFoodGenerosity(25);
    BOOST_CHECK_EQUAL(params.quantity_per_100_by_100, (original / 2));
}

BOOST_FIXTURE_TEST_CASE(test_initPheromoneMaps_shouldBeMaps, Fixture)
{
    BOOST_CHECK_EQUAL(world.getSimulationObjects<PheromoneMap>().size(), 0);  
    const unsigned num_phero_maps = 3; 
    const float to_food_decay_rate = 0.5, from_food_decay_rate = 0.5, 
        anthill_decay_rate = 0.5;
    WorldGenerator::initPheromoneMaps(&world, to_food_decay_rate,
        from_food_decay_rate, anthill_decay_rate);
    BOOST_CHECK(world.getSimulationObjects<PheromoneMap>().size() == 
        num_phero_maps);
    BOOST_CHECK(world.getEntityPtrs().size() == 0); // phero maps arent entities
}

BOOST_FIXTURE_TEST_CASE(test_invalidRandomBlobParams_shouldThrow, Fixture)
{
    ObstaclesParams params;
    params.blob.line_size_min = -1;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);    
    params.blob.line_size_min = 1;

    params.blob.line_size_max = -1;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);    
    params.blob.line_size_max = 10;
    
        params.blob.line_size_min = 15;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);    
    params.blob.line_size_min = 1;
    
        params.blob.circle_radius_min = -1;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);    
    params.blob.circle_radius_min = 1;
    
        params.blob.circle_radius_max = -1;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);    
    params.blob.circle_radius_max = 10;
    
        params.blob.circle_radius_min = 15;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);    
    params.blob.circle_radius_min = 1;
    
    params.blob.chance_line = -1;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);    
    params.blob.chance_line = 2;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);
    
}

} // namespace WorldGeneratorTest