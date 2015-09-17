//#include <vector>
//#include <unittest++/UnitTest++.h>
//#include "unit_test_util.h"
//#include "m3.h"
//#include "m4.h"
//#include "tour_verify.h"
//
//SUITE(Very_Hard) {
//	TEST(Test_Very_Hard_0) {
//		std::vector<unsigned> delivery_ids = {159, 6118, 11108, 13397, 15488, 16525, 17019, 19072, 22986, 24242, 24567, 25122, 27591, 32835, 40283, 47653, 51513, 52181, 53180, 60687, 63494, 68860, 74010, 75486, 76352, 82903, 87781, 99109, 99257, 100334, 106475, 138162, 143340, 144891, 150279, 151704, 152207, 153603, 161514, 164537};
//		std::vector<unsigned> depot_ids = {12487, 17593, 22983, 31837, 45118, 54428, 62587, 88803, 108746, 160177};
//
//		std::vector<unsigned> tour;
//		{
//			UNITTEST_TIME_CONSTRAINT(30000);
//			tour = traveling_salesman(delivery_ids, depot_ids);
//		}
//
//		bool tour_is_legal = ece297test::tour_is_legal(delivery_ids, depot_ids, tour);
//		CHECK(tour_is_legal == true);
//
//		if(tour_is_legal) {
//			double tour_cost = compute_path_travel_time(tour);
//			std::cout << "Test_Very_Hard_0 Cost: " << tour_cost << std::endl;
//		} else {
//			std::cout << "Test_Very_Hard_0 Cost: N/A (illegal)" << std::endl;
//		}
//	}
//}
