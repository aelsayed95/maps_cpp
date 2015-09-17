//#include <iostream>
//#include <string>
//#include <random>
//#include <unittest++/UnitTest++.h>
//
//#include "m1.h"
//#include "m4.h"
//
//std::string map_name = "/cad2/ece297s/public/maps/saint_helena.bin";
//
//int main(int argc, char** argv) {
//    //Run the unit tests
//    int num_failures = UnitTest::RunAllTests();
//
//    return num_failures;
//}
//
//struct MapFixture {
//    MapFixture() {
//        //Load the test map
//        if(!load_map(map_name)) {
//            std::cout << "ERROR: Could not load map file: '" << map_name << "'!";
//            std::cout << " Subsequent tests will likely fail." << std::endl;
//            //Don't abort tests, since we still want to show that all
//            //tests fail.
//        }
//    }
//
//    ~MapFixture() {
//        //Cleanup
//        close_map();
//    }
//
//	std::minstd_rand rng;
//	std::uniform_int_distribution<unsigned> rand_intersection;
//	std::uniform_int_distribution<unsigned> rand_poi;
//};
//
//
//TEST_FIXTURE(MapFixture, ValgrindExerciseFunctions) {
//    std::vector<unsigned> delivery_ids = {87918, 100030, 101323};
//    std::vector<unsigned> depot_ids = {1756, 120812};
//
//    std::vector<unsigned> tour;
//    tour = traveling_salesman(delivery_ids, depot_ids);
//
//    double tour_cost = compute_path_travel_time(tour);
//
//}
//
