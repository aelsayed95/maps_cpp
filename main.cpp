#include "input.h"
#include "m4.h"

extern vector<unsigned> path_test;

int main () {
    //start_AHA();
//    std::vector<unsigned> delivery_ids = {1176, 4800, 43655, 66335, 121022};
//    std::vector<unsigned> depot_ids = {64314, 111573, 112996, 122728};
//    std::vector<unsigned> tour = traveling_salesman(delivery_ids, depot_ids);
		

    load_map("/cad2/ece297s/public/maps/toronto.bin");
    //std::vector<unsigned> path = find_path_between_intersections(23162, 58114);
    std::vector<unsigned> delivery_ids = {334, 4904, 6868, 9693, 11584, 16073, 19450, 22197, 23666, 24352, 24756, 24837, 26373, 28349, 28760, 29864, 33378, 44271, 45997, 53528, 54101, 59155, 59257, 59823, 63019, 67387, 68370, 69112, 71050, 71395, 75438, 85427, 86482, 97085, 103839, 105650, 109689, 119610, 122014, 129738, 137740, 140247, 141616, 143665, 143872, 144521, 152618, 157705, 159225, 163352};
    std::vector<unsigned> depot_ids = {6604, 15321, 30063, 57131, 77774, 97918, 102219, 103141, 160373, 160522};

    std::vector<unsigned> tour = traveling_salesman(delivery_ids, depot_ids);
    close_map();
    return 0;
}
