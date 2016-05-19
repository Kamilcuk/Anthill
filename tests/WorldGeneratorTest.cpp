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
    BOOST_CHECK_EQUAL(world.getEntityMap().lock()->getAllEntities().size(), 1);
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
    BOOST_CHECK_EQUAL(world.getEntityMap().lock()->getAllEntities().size(), 11); 
        // (+ anthill)
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
    test_placeMoreAntsThanFreeSpace_shouldCreateAsMuchAsPossibleAndThrow, 
    Fixture)
{
    const Point centre(world_dim_x / 2, world_dim_y / 2);
    
    WorldGenerator::placeAnthill(&world);
    auto anthill = world.getSimulationObjects<Anthill>()[0];
    anthill->setPos(centre);
    
    AntsParams params;
    params.quantity = world_dim_x * world_dim_y;
    
    world.setDimensions(world_dim_x, world_dim_y);
    
    // create some obstacles
    int num_obstacles = 0;
    for(const auto& p : ShapeGenerator::GenerateCircle(centre, 25))
    {
        world.addSimulationObject<Obstacle>(
            boost::make_shared<Obstacle>(&world, p));
        num_obstacles++;
    }

    const int expected_num = world_dim_x * world_dim_y - num_obstacles;
    
    BOOST_CHECK_THROW(WorldGenerator::placeAnts(&world, params), 
        std::runtime_error);
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 
        expected_num);
}

BOOST_FIXTURE_TEST_CASE(
    test_placeAntWhenNoImmediateEmptySpace_shouldExpandRadiusAndFit, Fixture)
{
    const Point centre(world_dim_x / 2, world_dim_y / 2);
    WorldGenerator::placeAnthill(&world);
    
    auto anthill = world.getSimulationObjects<Anthill>()[0];
    anthill->setPos(centre);
    
    const float obstacle_radius = 15;
    
    // spam a blob of obstacles around the anthill
    for(const auto& p : ShapeGenerator::GenerateCircle(centre, obstacle_radius))
        world.addSimulationObject<Obstacle>(
            boost::make_shared<Obstacle>(&world, p));
       
    const unsigned original_max_radius = 5;
    AntsParams params;
    params.quantity = 1;
    params.min_dist_from_anthill = 2;
    params.max_dist_from_anthill = original_max_radius;
    
    WorldGenerator::placeAnts(&world, params);
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 1);
    BOOST_CHECK_LT(original_max_radius, (unsigned)params.max_dist_from_anthill);
}

BOOST_FIXTURE_TEST_CASE(
    test_userFriendlyAntCreation_shouldSpawnAsSpecified, Fixture)
{
    WorldGenerator::placeAnthill(&world);
    AntsParams params;
    params.quantity = 1;
    params.ratio_scouts = 0.2;
    params.applyNumAnts(50);
    BOOST_CHECK_EQUAL(params.quantity, 50);
    params.applyRatioScouts(25);
    BOOST_CHECK_CLOSE(params.ratio_scouts, 0.25, 0.001);
}

BOOST_FIXTURE_TEST_CASE(test_placeAntsWithoutAnthill_shouldThrow, Fixture)
{
    AntsParams params;
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Anthill>().size(), 0);
    BOOST_CHECK_THROW(WorldGenerator::placeAnts(&world, params), 
        std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(test_placeAntsInvalidParams_shouldThrow, Fixture)
{
    WorldGenerator::placeAnthill(&world);
    AntsParams params;
    params.quantity = 10;
    params.min_dist_from_anthill = 5;
    params.max_dist_from_anthill = 2;
    BOOST_CHECK_THROW(WorldGenerator::placeAnts(&world, params), 
        std::runtime_error);
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
    BOOST_CHECK(world.getEntityMap().lock()->getAllEntities().size() > 0);
}

BOOST_FIXTURE_TEST_CASE(test_placeObstaclesInvalidParams_shouldThrow, Fixture)
{
    ObstaclesParams params;
    params.quantity_per_100_by_100 = 10001;
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
    BOOST_CHECK(world.getEntityMap().lock()->getAllEntities().size() > 0);
}

BOOST_FIXTURE_TEST_CASE(test_placeFoodsInvalidParams_shouldThrow, Fixture)
{
    FoodsParams params;
    params.quantity_per_100_by_100 = 10001;
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
    WorldGenerator::initPheromoneMaps(&world);
    BOOST_CHECK(world.getSimulationObjects<PheromoneMap>().size() == 
        num_phero_maps);
    BOOST_CHECK(world.getEntityMap().lock()->getAllEntities().size() == 0); 
        // phero maps arent entities
}

BOOST_FIXTURE_TEST_CASE(test_setCoefficientsFromGUIKnobs, Fixture)
{
    PheromoneParams params;
    params.scale = 0.1;
    params.to_food_decay_rate = 0.5;
    params.applyCoefficients(20, 30, 40);
    BOOST_CHECK_CLOSE(params.to_food_decay_rate, 0.02, 0.0001);
}

BOOST_FIXTURE_TEST_CASE(test_invalidRandomBlobParams_shouldThrow, Fixture)
{
    ObstaclesParams params;

    params.blob.line_size_min = 10;
    params.blob.line_size_max = 5;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);    
    params.blob.line_size_min = 2;

    params.blob.circle_radius_min = 10;
    params.blob.circle_radius_max = 5;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);    
    params.blob.circle_radius_min = 2;
        
    params.blob.chance_line = -1;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);    
    params.blob.chance_line = 2;
    BOOST_CHECK_THROW(WorldGenerator::placeObstacles(&world, params), 
        std::runtime_error);
}

} // namespace WorldGeneratorTest