#include <vector>
#include <cassert>
#include "StreetsDatabaseAPI.h"
#include <iostream>
#include "path_verify.h"
using namespace std;

namespace ece297test {

bool path_is_legal(unsigned start_intersection, unsigned end_intersection, const std::vector<unsigned>& path) {
    //'start_intersection' is the intersection id of the starting intersection
    //'end_intersection' is the intersection id of the ending intersection
    //'path' is a vector street segment id's

    if (path.size() < 1) {
       cout << "path size = 0\n";
        return false; //If it is a valid path it must contain at-least one segment

    } else {
        //General case
        //To verify the path by walking along each segment checking:
        //  * That we are going a legal direction (e.g. not against one-ways)
        //  * That each segment is connected to the previous intersection
        //  * That the final segment is connected to the end_intersection
        //We start our intersection at the start_intersection

        assert(path.size() >= 1);

        unsigned curr_intersection = start_intersection;
        for (int i = 0; i < (int) path.size(); i++) {
            StreetSegmentEnds seg_endpoints = getStreetSegmentEnds(path[i]);


            //Are we moving forward or back along the segment?
            bool seg_traverse_forward;
            if (seg_endpoints.to == curr_intersection) {
                seg_traverse_forward = false;

            } else if (seg_endpoints.from == curr_intersection) {
                seg_traverse_forward = true;

            } else {
                cout << "segment " << i << endl;
                //cout << path.size() << endl;
                
                cout << "This segment isn't connected to the current intersection " << curr_intersection<< endl;
                //This segment isn't connected to the current intersection
                return false;
            }

            //Are we going the wrong way along the segment?
            if (!seg_traverse_forward && getStreetSegmentOneWay(path[i])) {
                cout << "failing at the segment " << path[i] << endl;
                cout << seg_endpoints.from << " " << seg_endpoints.to << endl; 
                cout << "Are we going the wrong way along the segment\n";
                return false;
            }

            //Advance to the next intersection
            curr_intersection = (seg_traverse_forward) ? seg_endpoints.to : seg_endpoints.from;

        }

        //We should be at the end intersection
        if (curr_intersection != end_intersection) {
            cout << "We should be at the end intersection\n";
            return false;
        }

    }

    //Everything validated
    return true;
}

}
