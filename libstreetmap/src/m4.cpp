#include "m4.h"
#include <fstream>
#include <iomanip>

extern unordered_map<unsigned long long, unsigned> best_seg_id;

// This routine takes in a vector of N intersections that must be 
// traversed in a path, and another vector of M intersections that 
// are legal start and end points for the path. The first vector 
// gives the intersection ids of a set of locations at which 
// deliveries must be made. The second vector gives the intersection
// ids of courier company depots containing trucks; you start at any
// one of these depots and end at any one of the depots.
// You can assume that N is always at least 1, and M is always at 
// least one (i.e. both input vectors are non-empty).  You can also
// assume that no intersection is repeated in either list and that
// no intersection is both a delivery location and a depot (i.e. no
// intersection will appear in both vectors).
// This routine returns a vector of street segment ids that form a 
// path, where the first street segment id is connected to a depot
// intersection, and the last street segment id also connects to a
// depot intersection.  If no path connecting all the delivery locations
// and a start and end depot exists, this routine should return an 
// empty (size == 0) vector.

double calculate_goodness(double time, double newTime, double temperature){
    if(newTime < time){
        return 1;
    }
//    cout << "time-newTime " << time-newTime << endl;
    cout << "temp = " << temperature << endl;
    //return 100*pow(5,(time-newTime)/temperature)*pow(5,-temperature);//exp((time-newTime)/temperature);
    return exp(-MAX_TEMP/temperature)*3;
}

/*
vector<unsigned> add_depots(vector<unsigned> inter_route, vector<unsigned> depots){
    //find the best start depot
    unsigned first_inter_id = inter_route[0];
    unsigned last_inter_id = inter_route[inter_route.size()-1];
    
    LatLon first_position = getIntersectionPosition(first_inter_id);
    LatLon last_position = getIntersectionPosition(last_inter_id);
    
    double best_distance, distance, time, best_time;
    vector<unsigned> path, best_path;
    unsigned first_depot;
    
    double best_distance2, best_time2;
    vector<unsigned> best_path2;
    unsigned last_depot;
    
    for(unsigned i=0;i<depots.size();i++){
        //find the starting depot
        distance = find_distance_between_two_points(first_position,getIntersectionPosition(depots[i]));
        if(distance < best_distance){
            path = find_path_between_intersections(first_inter_id,depots[i]);
            if(path.size()!=0){
                time = compute_path_travel_time(path);
                if(time < best_time){
                    best_time = time;
                    best_path = path;
                    best_distance = distance;
                    first_depot = depots[i];
                }
            }
            
        }
        
        //find the ending depot
        distance = find_distance_between_two_points(last_position,getIntersectionPosition(depots[i]));
        if(distance < best_distance2){
            path = find_path_between_intersections(last_inter_id,depots[i]);
            if(path.size()!=0){
                time = compute_path_travel_time(path);
                if(time < best_time2){
                    best_time2 = time;
                    best_path2 = path;
                    best_distance2 = distance;
                    last_depot = depots[i];
                }
            }
            
        }
    }
    inter_route.insert(inter_route.begin(),first_depot);
    inter_route.push_back(last_depot);
    
    //find the best end depot
    return inter_route;
}
 * */

unordered_map<unsigned,unsigned> best_start_depot_id;
unordered_map<unsigned,unsigned> best_end_depot_id;

unordered_map<unsigned,vector<unsigned>> best_start_depot_path;
unordered_map<unsigned,vector<unsigned>> best_end_depot_path;

unordered_map<unsigned,double> best_start_depot_time;
unordered_map<unsigned,double> best_end_depot_time;

unordered_map<unsigned long long, vector<unsigned>> best_path_between_intersections;
unordered_map<unsigned long long, double> best_time_between_intersections;

//Given a vector of all intersections that need to be traversed and all depot locations,
//finds the closest depot to each intersection. Also, fills in the unordered maps with the best time and path.
//The closest depot is calculated for two scenarios:
//intersection->depot and depot->intersection

//void find_closest_depots(vector<unsigned> intersections_dest, vector<unsigned> depots){ //--@%$#%^#$^#$ redundant calculations if the m4 functions are called multiple times and theres overlap between the list of intersections and depots
//    
//    unsigned num_of_inters = intersections_dest.size();
//    unsigned num_of_depots = depots.size();
//    LatLon inter_position;
//    vector<unsigned> path1, path2;
//    double time1, time2;
//    double distance, best_distance, second_best_distance;
//    unsigned best_id = 0, second_best_id = 0;
//    
//    //Loops through all intersections, to find the closest two depots using the Euclidean distance
//    for(unsigned j=0;j<num_of_inters;j++){
//        
//        inter_position = getIntersectionPosition(intersections_dest[j]);
//        
//        best_distance = EARTH_RADIUS_IN_METERS;
//        second_best_distance = EARTH_RADIUS_IN_METERS;
//        
//        for(unsigned i=0;i<num_of_depots;i++){ //for each depot
//            distance = find_distance_between_two_points(inter_position,getIntersectionPosition(depots[i]));
//            if(distance < best_distance){
//                best_distance = distance;
//                best_id = depots[i];
//            }
//            else if(distance < second_best_distance){
//                second_best_distance = distance;
//                second_best_id = depots[i];
//            }
//        }
//        
//        //Two shortest distances were found
//        if(second_best_distance<EARTH_RADIUS_IN_METERS){
//            //for going from start depot to inter
//            path1 = find_path_between_intersections(best_id,intersections_dest[j]);
//            time1 = compute_path_travel_time(path1);
//            path2 = find_path_between_intersections(second_best_id,intersections_dest[j]);
//            time2 = compute_path_travel_time(path2);
//            
//            // Insert one of the two possible depots based on time
//            if(time2 < time1){   //------------------------------------------------------%$#^$%&^&9768 what if path size is 0??? what are you gonna do? huh?
//                best_start_depot_id.insert(make_pair(intersections_dest[j],second_best_id));
//                best_start_depot_path.insert(make_pair(intersections_dest[j],path2));
//                best_start_depot_time.insert(make_pair(intersections_dest[j],time2));
//            }
//            else{
//                best_start_depot_id.insert(make_pair(intersections_dest[j],best_id));
//                best_start_depot_path.insert(make_pair(intersections_dest[j],path1));
//                best_start_depot_time.insert(make_pair(intersections_dest[j],time1));
//            }
//            
//            //for going from inter to end depot
//            
//            path1 = find_path_between_intersections(intersections_dest[j],best_id);
//            time1 = compute_path_travel_time(path1);
//            path2 = find_path_between_intersections(intersections_dest[j],second_best_id);
//            time2 = compute_path_travel_time(path2);
//            
//            if(time2 < time1){   //------------------------------------------------------%$#^$%&^&9768 what if path size is 0??? what are you gonna do? huh?
//                best_end_depot_id.insert(make_pair(intersections_dest[j],second_best_id));
//                best_end_depot_path.insert(make_pair(intersections_dest[j],path2));
//                best_end_depot_time.insert(make_pair(intersections_dest[j],time2));
//            }
//            else{
//                best_end_depot_id.insert(make_pair(intersections_dest[j],best_id));
//                best_end_depot_path.insert(make_pair(intersections_dest[j],path1));
//                best_end_depot_time.insert(make_pair(intersections_dest[j],time1));
//            }
//        }
//        else{ // path1 is the best one for sure (second path not found/does not exist)
//            
//            //for going from start depot to inter
//            path1 = find_path_between_intersections(best_id,intersections_dest[j]);
//            time1 = compute_path_travel_time(path1);
//            best_start_depot_id.insert(make_pair(intersections_dest[j],best_id));
//            best_start_depot_path.insert(make_pair(intersections_dest[j],path1));
//            best_start_depot_time.insert(make_pair(intersections_dest[j],time1));
//            
//            //for going from inter to end depot
//            path1 = find_path_between_intersections(intersections_dest[j],best_id);
//            time1 = compute_path_travel_time(path1);
//            best_end_depot_id.insert(make_pair(intersections_dest[j],best_id));
//            best_end_depot_path.insert(make_pair(intersections_dest[j],path1));
//            best_end_depot_time.insert(make_pair(intersections_dest[j],time1));
//        }
//    }
//    return;
//}

void find_closest_depots(unsigned intersections_dest, vector<unsigned> depots){ //--@%$#%^#$^#$ redundant calculations if the m4 functions are called multiple times and theres overlap between the list of intersections and depots
    
    unsigned num_of_depots = depots.size();
    LatLon inter_position;
    vector<unsigned> path1, path2;
    double time1, time2;
    double distance, best_distance, second_best_distance;
    unsigned best_id = 0, second_best_id = 0;
    
    inter_position = getIntersectionPosition(intersections_dest);

    best_distance = EARTH_RADIUS_IN_METERS;
    second_best_distance = EARTH_RADIUS_IN_METERS;

    bool do_calculations_start = false;
    bool do_calculations_end = false;
    
    unordered_map<unsigned,unsigned>::iterator it;
    it = best_start_depot_id.find(intersections_dest);
    if(it==best_start_depot_id.end())
        do_calculations_start = true;
    
    it = best_end_depot_id.find(intersections_dest);
    if(it==best_end_depot_id.end())
        do_calculations_end = true;
    
    if(do_calculations_start==false && do_calculations_end==false)return;
    
    
    for(unsigned i=0;i<num_of_depots;i++){ //for each depot
        distance = find_distance_between_two_points(inter_position,getIntersectionPosition(depots[i]));
        if(distance < best_distance){
            best_distance = distance;
            best_id = depots[i];
        }
        else if(distance < second_best_distance){
            second_best_distance = distance;
            second_best_id = depots[i];
        }
    }

    
    
    //Two shortest distances were found
    if(second_best_distance<EARTH_RADIUS_IN_METERS){
        //for going from start depot to inter
        if(do_calculations_start){
            path1 = find_path_between_intersections(best_id,intersections_dest);
            time1 = compute_path_travel_time(path1);
            path2 = find_path_between_intersections(second_best_id,intersections_dest);
            time2 = compute_path_travel_time(path2);

            // Insert one of the two possible depots based on time
            if(time2 < time1){   //------------------------------------------------------%$#^$%&^&9768 what if path size is 0??? what are you gonna do? huh?
                best_start_depot_id.insert(make_pair(intersections_dest,second_best_id));
                best_start_depot_path.insert(make_pair(intersections_dest,path2));
                best_start_depot_time.insert(make_pair(intersections_dest,time2));
            }
            else{
                best_start_depot_id.insert(make_pair(intersections_dest,best_id));
                best_start_depot_path.insert(make_pair(intersections_dest,path1));
                best_start_depot_time.insert(make_pair(intersections_dest,time1));
            }
        }

        //for going from inter to end depot
        if(do_calculations_end){
            path1 = find_path_between_intersections(intersections_dest,best_id);
            time1 = compute_path_travel_time(path1);
            path2 = find_path_between_intersections(intersections_dest,second_best_id);
            time2 = compute_path_travel_time(path2);

            if(time2 < time1){   //------------------------------------------------------%$#^$%&^&9768 what if path size is 0??? what are you gonna do? huh?
                best_end_depot_id.insert(make_pair(intersections_dest,second_best_id));
                best_end_depot_path.insert(make_pair(intersections_dest,path2));
                best_end_depot_time.insert(make_pair(intersections_dest,time2));
            }
            else{
                best_end_depot_id.insert(make_pair(intersections_dest,best_id));
                best_end_depot_path.insert(make_pair(intersections_dest,path1));
                best_end_depot_time.insert(make_pair(intersections_dest,time1));
            }
        }
    }
    else{ // path1 is the best one for sure (second path not found/does not exist)

        //for going from start depot to inter
        if(do_calculations_start){
            path1 = find_path_between_intersections(best_id,intersections_dest);
            time1 = compute_path_travel_time(path1);
            best_start_depot_id.insert(make_pair(intersections_dest,best_id));
            best_start_depot_path.insert(make_pair(intersections_dest,path1));
            best_start_depot_time.insert(make_pair(intersections_dest,time1));
        }
        if(do_calculations_end){
            //for going from inter to end depot
            path1 = find_path_between_intersections(intersections_dest,best_id);
            time1 = compute_path_travel_time(path1);
            best_end_depot_id.insert(make_pair(intersections_dest,best_id));
            best_end_depot_path.insert(make_pair(intersections_dest,path1));
            best_end_depot_time.insert(make_pair(intersections_dest,time1));
        }
    }
    return;
}

double compute_route_travel_time(vector<unsigned> route){
    unsigned route_size = route.size()-1;
    vector<unsigned> path;
    double total_time = 0, time;
    unordered_map<unsigned long long, vector<unsigned>>::iterator it;
    
    for(unsigned i=0;i<route_size;i++){
        unsigned long long joined_id = route[i]*PUSH_LEFT+route[i+1];
        it = best_path_between_intersections.find(joined_id);
        if(it!=best_path_between_intersections.end()){
            path = it->second;
            time = best_time_between_intersections[joined_id];
        }
        else{// not calculated before
            path = find_path_between_intersections(route[i],route[i+1]);
            time = compute_path_travel_time(path);
            best_path_between_intersections.insert(make_pair(joined_id,path));
            best_time_between_intersections.insert(make_pair(joined_id,time));
        }
        total_time+=time;
    }
    return total_time;
}

vector<unsigned> add_depots(vector<unsigned> inter_route, unsigned first_depot_id, unsigned last_depot_id){
    vector<unsigned> combined_route(inter_route.size()+1);
    combined_route[0] = first_depot_id;
    //cout << "fdi " << first_depot_id << endl;
    if(combined_route.size()!=0)
        move (inter_route.begin(), inter_route.end(), combined_route.begin()+1);
    /*for(unsigned i=0;i<inter_route.size();i++)
        combined_route.push_back(inter_route[i]);*/
    combined_route.push_back(last_depot_id);
    /*
    for(unsigned i=0;i<inter_route.size();i++)
        cout << inter_route[i]<<" "; 
    cout << endl;
    for(unsigned i=0;i<combined_route.size();i++)
        cout << combined_route[i]<< " "; 
    cout << endl;
    getchar();
    */
    return combined_route;
}

extern unsigned intersections_count;
extern unsigned streets_count;
extern unsigned poi_count;
extern unsigned street_segment_count;

void print_route(vector<unsigned> route){
    for(unsigned i=0;i<route.size();i++){
        cout << route[i] << " ";
    }
    cout << endl;
    getchar();
    return;
}


double best_distance;

vector<unsigned>greedy_order(vector<unsigned>inters_id){
    double distance;
    unsigned num_of_inters = inters_id.size(), best_id = 0;
    vector<unsigned> sol;
    sol.push_back(inters_id[0]);
    vector<bool> visited(num_of_inters,false);
    visited[0] = true;
    unsigned i = 0;
    
    while(sol.size()<num_of_inters){
        best_distance = EARTH_RADIUS_IN_METERS;
        LatLon pos1 = getIntersectionPosition(inters_id[i]);
        for(unsigned j=0;j<num_of_inters;j++)
            if(i!=j && visited[j]==false)
            {
                LatLon pos2 = getIntersectionPosition(inters_id[j]);
                distance = find_distance_between_two_points(pos1, pos2);
                if(distance < best_distance){
                    best_distance = distance;
                    best_id = j;
                }
            }
        //cout<<"done\n";
        sol.push_back(inters_id[best_id]);
        visited[best_id] = true;
        i = best_id;
    }
    return sol;
}


// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
unsigned orientation(LatLon p, LatLon q, LatLon r)
{
    // See 10th slides from following link for derivation of the formula
    // http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
    double val = (q.lat - p.lat) * (r.lon - q.lon) - (q.lon - p.lon) * (r.lat - q.lat);
 
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2;  // clock or counterclock wise
}

bool doIntersect(LatLon p1, LatLon q1, LatLon p2, LatLon q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
 
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
 
    // Special Cases
    /*
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
 
    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
 
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
 
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
 */
    return false; // Doesn't fall in any of the above cases
}

std::vector<unsigned> traveling_salesman(std::vector<unsigned> intersections_to_traverse, std::vector<unsigned> depot_locations){
    ofstream fout;
    fout.open("output.txt");
    clock_t start = clock(), diff;
    vector<unsigned> current_route, current_route2, current_route3;
    vector<unsigned> next_route;
    vector<unsigned> best_route;
    
    
    for(unsigned i=0;i<depot_locations.size();i++)
        if(depot_locations[i]>intersections_count)
            return current_route;
    
    for(unsigned i=0;i<intersections_to_traverse.size();i++)
        if(intersections_to_traverse[i]>intersections_count)
            return current_route;
    
    double temperature = MAX_TEMP;
    unsigned route_size = intersections_to_traverse.size();
    
    if(route_size <= SMALL_INPUT){
       ;
    }
    else{
        
        current_route.resize(route_size);
        next_route.resize(route_size);
        best_route.resize(route_size);

        srand (time(NULL));

        //calculate all best depots and paths for all destination intersections
        //find_closest_depots(intersections_to_traverse,depot_locations);

        current_route = intersections_to_traverse; //lets start by assuming that the order in which the intersections are passed is the best
        /*
        //modify the initial intersection vector so it is ordered greedily
        unsigned id2, id3;
        double d1, d2, d3;
        current_route2 = current_route;
        current_route3 = current_route;
        current_route = greedy_order(current_route);
        d1 = best_distance;
        id2 = rand()%route_size;
        id3 = rand()%route_size;
        while(id3==id2){
            id3 = rand()%route_size;
        }
        swap(current_route2[0],current_route2[id2]);
        current_route2 = greedy_order(current_route2);
        d2 = best_distance;
        swap(current_route3[0],current_route3[id3]);
        current_route3 = greedy_order(current_route3);
        d3 = best_distance;
        
        if(d3<d2){
            copy(current_route3.begin(), current_route3.end(), current_route2.begin());
            //current_route2 = current_route3;
            d2 = d3;
        }
        if(d2<d1){
            copy(current_route2.begin(), current_route2.end(), current_route.begin());
            //current_route = current_route2;
            //d1 = d2;
        }
        */
        //best_route = current_route;
        copy(current_route.begin(), current_route.end(), best_route.begin());
        
        //best_route = add_depots(best_route, depot_locations);             //calculate the time for the best route
        //best_route = add_depots(best_route,best_depot_map[best_route[0]],best_depot_map[best_route[route_size-1]]);

        double best_time = compute_route_travel_time(best_route);
        //double start_inter_time, end_inter_time;
        while(temperature > LOWEST_TEMP){
            next_route.clear();
            next_route.resize(route_size);
            copy(current_route.begin(), current_route.end(), next_route.begin());

            unsigned inter_index_1 = rand()%route_size;
            unsigned inter_index_2 = rand()%route_size;

            while(inter_index_1==inter_index_2){
                inter_index_2 = rand()%route_size;
            }

            swap(next_route[inter_index_1],next_route[inter_index_2]);

            //for current route
            find_closest_depots(current_route[0],depot_locations);
            find_closest_depots(current_route[route_size-1],depot_locations);

            //for the next route
            find_closest_depots(next_route[0],depot_locations);
            find_closest_depots(next_route[route_size-1],depot_locations);

            double current_time = compute_route_travel_time(current_route) + best_start_depot_time[current_route[0]] + best_end_depot_time[current_route[route_size-1]];
            double next_time = compute_route_travel_time(next_route)  + best_start_depot_time[next_route[0]] + best_end_depot_time[next_route[route_size-1]];

            double c1 = calculate_goodness(current_time, next_time, temperature);
            double c2 = ((double) rand() / (RAND_MAX));
            cout << setw(5) << c1 << "     " << c2 << endl;
            //cout << (c1 > c2) << endl;
            if(c1 > c2){
                //cout << "HEREEEEEEEEEEEEEEEEEEEEE\n";
                current_route.clear();
                current_route.resize(route_size);
                copy(next_route.begin(), next_route.end(), current_route.begin());
                current_time = next_time;
                //current_route = next_route;
            }
            
            fout << next_time << " " << current_time << " ";

            if(current_time < best_time){
                cout << "updating the freakin best time\n";
                //best_route = current_route;
                best_route.clear();
                best_route.resize(route_size);
                copy(current_route.begin(), current_route.end(), best_route.begin());
                best_time = current_time;
            }
            fout << best_time << endl;

            temperature *= (1-COOLING_RATE);

            diff = clock() - start;
            int msec = diff * 1000 / CLOCKS_PER_SEC;
            if(msec >= MAX_CALCULATION_TIME)
                break;
        }
        
        //removing crossings
//        for(unsigned i=0;i<route_size-1;i++){          
//            LatLon pos1s = getIntersectionPosition(best_route[i]);
//            LatLon pos1e = getIntersectionPosition(best_route[i+1]);
//            
//            for(unsigned j=i+2;j<=route_size-1;j++)
//            {                    
//                LatLon pos2s = getIntersectionPosition(best_route[j]);
//                LatLon pos2e = getIntersectionPosition(best_route[j+1]);
//                if(doIntersect(pos1s,pos1e,pos2s,pos2e)){
//                    swap (best_route[i+1], best_route[j]);
//                }
//            }
//        }
//        
        /*
        double xi, xim1, xj, xjm1;
        double yi, yip1, yj, yjp1;
        double A, B, s, t;
        for(unsigned i=1;i<route_size-1;i++){          
            LatLon pos1s = getIntersectionPosition(best_route[i-1]);
            LatLon pos1m = getIntersectionPosition(best_route[i]);
            LatLon pos1e = getIntersectionPosition(best_route[i+1]);
            
            for(unsigned j=i+2;j<=route_size-1;j++)
                //if(i!=j && j!=i+1)//the intersections do not represent the same segment or the next segment in the array (as they will never intersect)
                {                    
                    LatLon pos2s = getIntersectionPosition(best_route[j-1]);
                    LatLon pos2m = getIntersectionPosition(best_route[j]);
                    LatLon pos2e = getIntersectionPosition(best_route[j+1]);
                    
                    xi = pos1m.lon;
                    yi = pos1m.lat;

                    yip1 = pos1e.lat;
                    xim1 = pos1s.lon;
                    
                    xj = pos2m.lon;
                    yj = pos2m.lat;
                                       
                    xjm1 = pos2s.lon;
                    yjp1 = pos2e.lat;
                    
                    //the grand equation
                    A = (yi - yj) + ((yip1 - yi)*(xj - xi)/(xim1 - xi));
                    B = (yjp1 - yj) - ((xjm1 - xj)*(yip1-yi)/(xim1 - xi));
                    
                    s = A/B;
                    t = ((xj - xi)+(xjm1 - xj)*s)/(xim1 - xi);
                    if (s<=1 && s>= 0 && t<= 1 && t>=0){
                        //they are intersecting. 
                        //best_route[i+1] transposed with best_route[j]
                        swap (best_route[i+1], best_route[j]);
                    }            

                }
        }*/
        best_route = add_depots(best_route,best_start_depot_id[best_route[0]],best_end_depot_id[best_route[route_size-1]]);
    }

    
    // Connect the intersections with valid street segment ids (path) before returning
    vector<unsigned> final_path, consecutive_intersections_path;
    double consecutive_intersections_time;
    unordered_map<unsigned long long, vector<unsigned>>::iterator it;
    for (unsigned i=0; i< best_route.size()-1;i++)
    {
        unsigned long long joined_id = best_route[i]*PUSH_LEFT + best_route[i+1];
        it = best_path_between_intersections.find(joined_id);
        if(it!=best_path_between_intersections.end()){  // Finds the path between consecutive intersections
            consecutive_intersections_path = it->second;
            final_path.insert(final_path.end(), consecutive_intersections_path.begin(), consecutive_intersections_path.end());
        }
        else{// not calculated before
            consecutive_intersections_path = find_path_between_intersections(best_route[i],best_route[i+1]);
            consecutive_intersections_time = compute_path_travel_time(consecutive_intersections_path);
            best_path_between_intersections.insert(make_pair(joined_id,consecutive_intersections_path));
            best_time_between_intersections.insert(make_pair(joined_id,consecutive_intersections_time));
            final_path.insert(final_path.end(), consecutive_intersections_path.begin(), consecutive_intersections_path.end());
        }
    }

    //draw_tour(final_path,best_route,depot_locations,"/cad2/ece297s/public/maps/toronto.bin");
    return final_path;
}
