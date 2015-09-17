
#include "m3.h"
#include<iostream>
#include<queue>
#include <debug/unordered_map>
using namespace std;

//unsigned total_intersections;
extern unordered_map <unsigned, vector<unsigned> > adjacent_intersections;
extern std::unordered_map<string,vector<unsigned>> POI_list;     //used to map a poi names to a vector of its IDs
extern std::unordered_map<unsigned,unsigned> POI_inter_id;     //used to map a poi to its closest intersection
extern vector< vector<two_vectors> > positions; //positions intersections and point of interests in map

extern double min_lat, min_lon, max_lat, max_lon;
extern unsigned intersections_count;
extern unsigned streets_count;
extern unsigned poi_count;
extern unsigned street_segment_count;
extern unsigned lat_range, lon_range;
extern unordered_map<unsigned long long, unsigned> best_seg_id;
extern std::unordered_map<unsigned,double> seg_travel_time;

bool notFound;

extern unordered_map<unsigned,unsigned> street_seg_street_id;

class intersection_node{
public:
    LatLon position;
    unsigned id;
    double priority;
    double total_cost; // their level
    intersection_node(){
        id = -1;
        position.lat = -1;
        position.lon = -1;
        priority = -1;
        total_cost = -1;
    }
    
    intersection_node(unsigned id_i, LatLon position_i, double priority_i, double total_cost_i){
        id = id_i;
        position = position_i;
        priority = priority_i;
        total_cost = total_cost_i;
    }
    void updatePriority(LatLon destination){
        priority = total_cost + estimate(destination);
    }
    ///void nextLevel()
    // Estimate the remaining distance from the current position to the destination
    double estimate(LatLon & destination){
        double d;
        d = find_distance_between_two_points(destination,position);
        ////cout << "here-------------------------------------------------------------5657675896\n";
        
//        double xd, yd, d_squared;
//        xd = destination.lon - position.lon;
//        yd = destination.lat - position.lat;
//        //////cout << xd << " " << yd << endl;
//        //Distance squared between the destination and the intersection position
//        d_squared = xd*xd + yd*yd;  
//        d = sqrt(d_squared);
//        //////cout << "d = " << d << endl;

        //return time taken to travel the total length (euclidian distance)
        return d*UNITS_CONVERSION/MAX_SPEED;
    }
};

// Lower priority number means the intersection is closer to the destination
bool operator<(const intersection_node & a, const intersection_node & b){
    return a.priority > b.priority;
}

unsigned find_street_segment(unsigned first_id, unsigned second_id){
    //find all the segments connected to the parent intersection
    vector<unsigned> adjacent_segs = find_intersection_street_segments(first_id);
    StreetSegmentEnds sEnds;
    unsigned k, best_segment_id = street_segment_count;
    double time, best_time = MAX_TRAVEL_TIME;

    
    for(k=0;k<adjacent_segs.size();k++){
        sEnds = getStreetSegmentEnds(adjacent_segs[k]);
        bool oneWay = getStreetSegmentOneWay(adjacent_segs[k]);
        //check if its a legal segment connected to the second intersection
        if((sEnds.from==first_id && sEnds.to==second_id) || ((sEnds.to==first_id && !oneWay && sEnds.from==second_id)) )
        {
            //if there are more than one segement connecting them, choose the segement which takes smallest time
            time = seg_travel_time[adjacent_segs[k]];//find_street_segment_length(adjacent_segs[k])/getStreetSegmentSpeedLimit(adjacent_segs[k]);
            if(time < best_time){
                best_time = time;
                best_segment_id = adjacent_segs[k];
            }
        }
    }
    return best_segment_id;
}

//A star algorithm!!!!! :D
// Returns a path (route) between the start intersection and the end 
// intersection, if one exists. If no path exists, this routine returns 
// an empty (size == 0) vector. If more than one path exists, the path 
// with the shortest travel time is returned. The path is returned as a vector 
// of street segment ids; traversing these street segments, in the given order,
// would take one from the start to the end intersection.


std::vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, unsigned intersect_id_end){
    vector<unsigned> path(0);
    notFound = false;
    if(intersect_id_start>intersections_count || intersect_id_end>intersections_count){
        notFound = true;
        return path;
    }
    vector<unsigned> visited(intersections_count,0);
    vector<double> discovered(intersections_count,0);
    vector<unsigned> parent_intersection(intersections_count,0);
   
    LatLon intersection_start_position, intersection_dest_position;
    priority_queue<intersection_node> pq[2];
    path.clear();
    unsigned pqi = 0; //priority queue index
    intersection_node temp;
    unordered_map<unsigned long long, unsigned>::iterator it;
    unsigned long long joined_id;
    
    intersection_start_position = getIntersectionPosition(intersect_id_start);
    intersection_dest_position = getIntersectionPosition(intersect_id_end);
    
    //add the start node
    
    //unsigned id_i, LatLon position_i, double priority_i, double total_cost_i
    intersection_node n0 = intersection_node(intersect_id_start, intersection_start_position, 0, 0);
    n0.updatePriority(intersection_dest_position);
    discovered[intersect_id_start] = n0.priority;
    pq[pqi].push(n0);

    //parent of intersect id start?------------------------------------------------------------5438997583475
    
    unsigned prev_street_id = 0, prev_seg_id, adj_street_id;
    unsigned adjacent_count, valid_adj_segment;
    ////cout << intersect_id_start << endl;
    ////cout << intersect_id_end << endl;
    while(!pq[pqi].empty()){
        //visit the top node
        intersection_node top_node = pq[pqi].top();
        n0 = intersection_node(top_node.id, top_node.position, top_node.priority, top_node.total_cost);
        visited[top_node.id] = 1;
        discovered[top_node.id] = 0;
        pq[pqi].pop();
        
        //check if we have arrived at the destination
        if(top_node.id==intersect_id_end){
            path.clear();
            
            unsigned cur_id = intersect_id_end;
            while(cur_id!=intersect_id_start){
                /*
                it = best_seg_id.find(joined_id);
                if(it!=best_seg_id.end()){
                    valid_adj_segment = it->second;
                }
                else{
                    valid_adj_segment = find_street_segment(parent_intersection[cur_id],cur_id);
                    best_seg_id.insert(make_pair(joined_id,valid_adj_segment));
                }*/
                joined_id = parent_intersection[cur_id]*PUSH_LEFT+cur_id;
                valid_adj_segment = best_seg_id[joined_id];
                if(valid_adj_segment==street_segment_count){
                    //cout << "should never happen-------------------------------------58943759834758\n";
                }
                else{
                    path.push_back(valid_adj_segment);
                }
                cur_id = parent_intersection[cur_id];
            }

            while(!pq[pqi].empty())pq[pqi].pop();
            reverse(path.begin(),path.end());
            //best_seg_id.clear();
            return path;
        }
        
        //find the street on which the segment (parent[top_node], top_node)
        if(top_node.id != intersect_id_start){
            /*
            joined_id = parent_intersection[top_node.id]*PUSH_LEFT+top_node.id;
            it = best_seg_id.find(joined_id);
            if(it!=best_seg_id.end()){
                prev_seg_id = it->second;
            }
            else{
                prev_seg_id = find_street_segment(parent_intersection[top_node.id], top_node.id);
                best_seg_id.insert(make_pair(joined_id,prev_seg_id));
            }*/
            joined_id = parent_intersection[top_node.id]*PUSH_LEFT+top_node.id;
            prev_seg_id = best_seg_id[joined_id];
            prev_street_id = street_seg_street_id[prev_seg_id];
        }

        
        //add adjacent nodes to the queue
        adjacent_count = adjacent_intersections[top_node.id].size();
        for(unsigned i=0;i<adjacent_count;i++){
            if(visited[ adjacent_intersections[top_node.id][i] ]==0){
                //check if there exists a valid segment between top node and adjacent node
                //1- check one way
                //2- check time
                unsigned adj_node_id = adjacent_intersections[top_node.id][i];
                joined_id = top_node.id*PUSH_LEFT+adj_node_id;
                /*
                it = best_seg_id.find(joined_id);
                if(it!=best_seg_id.end()){
                    valid_adj_segment = it->second;
                }
                else{
                    valid_adj_segment = find_street_segment(top_node.id,adj_node_id);
                    best_seg_id.insert(make_pair(joined_id,valid_adj_segment));
                }*/
                valid_adj_segment = best_seg_id[joined_id];
                if(valid_adj_segment==street_segment_count){
                    //cout << "didnt find a valid segment\n";
                    continue; // because we do not want to consider this segment since there is no valid segment between top node and adj node
                }
                
                //calculate the cost of the adjacent segment in minutes
                double time = seg_travel_time[valid_adj_segment];
                 

                //if the adjacent segment lies on a different street from the top node, add the turning cost
                adj_street_id = street_seg_street_id[valid_adj_segment];
                if(top_node.id!=intersect_id_start && adj_street_id!=prev_street_id){
                    //add a turning pentality
                    time += TURN_TIME;
                }
//                else if (adj_street_id!=prev_street_id)
//                    time-=TURN_TIME/10;
                
                LatLon adj_position = getIntersectionPosition(adj_node_id);
                intersection_node m0 = intersection_node(adj_node_id, adj_position, n0.priority, n0.total_cost);
                //m0 = new 
                m0.total_cost += time;
                m0.updatePriority(intersection_dest_position);
                
                //push the adjacent node to the stack
                if(discovered[adj_node_id]==0){
                    //hasn't been discovered
                    discovered[adj_node_id] = m0.priority;
                    parent_intersection[adj_node_id] = top_node.id;
                    pq[pqi].push(m0);
                }
                else if(discovered[adj_node_id] > m0.priority){
                    discovered[adj_node_id] = m0.priority;
                    parent_intersection[adj_node_id] = top_node.id;
                    
                    /*
                    temp = pq[pqi].top();
                    while(temp.id!=adj_node_id){
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                        temp = pq[pqi].top();
                    }
                    pq [pqi].pop();
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi = 1-pqi;
                    while(!pq[pqi].empty()){
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi = 1-pqi;
                     * */
                    pq[pqi].push(m0);
                }
            }
            
        }

    }
    ////cout << "cant find the path\n";
    //path.resize(street_segment_count);
    notFound = true;
    //best_seg_id.clear();
    // if it reaches here... path is empty
    return path;
}


// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can 
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.
double compute_path_travel_time(const std::vector<unsigned>& path){
    
    double total_path_time = 0.0;
    unsigned cur_street_ID, last_street_ID = -1;
    
    for(unsigned i=0;i<path.size();i++){
        
        cur_street_ID = street_seg_street_id[path[i]];
        if(i!=0)
            if(cur_street_ID!=last_street_ID)total_path_time+=TURN_TIME;
        
        total_path_time += seg_travel_time[path[i]];
        last_street_ID = cur_street_ID;
    }
    
    //subtract TURN_TIME because at i=0, it thinks it made a turn on a new street but it is actually just the first street
    return total_path_time;//-TURN_TIME;
}


unsigned find_closest_intersection(LatLon poi_position){
    unsigned lon_index, lat_index; 
    vector<unsigned> inters_in_range;
    LatLon intersection_position;
    unsigned inter_ID = intersections_count; // max number of intersections
    double distance, shortest_distance = EARTH_RADIUS_IN_METERS;
    lon_index = abs(poi_position.lon - min_lon)*1000;
    lat_index = abs(poi_position.lat - min_lat)*1000;
    
    for (int i = -1; i<=1; i++){
        if (lon_index + i > 0 && lon_index + i < lon_range){
            for (int j=-1; j<=1;j++){
                if (lat_index + j > 0 && lat_index + j < lat_range){
                    inters_in_range = positions[lon_index + i][lat_index + j].inters;
                    for(unsigned k =0 ; k<inters_in_range.size(); k++){
                        intersection_position = getIntersectionPosition(inters_in_range[k]);
                        distance = find_distance_between_two_points(poi_position,intersection_position);

                        if(distance<shortest_distance){
                            shortest_distance = distance;
                            inter_ID = inters_in_range[k];
                        }
                    }
                }
            }
        }
    }
    //cout<<" this is the distance" <<shortest_distance << " at poi ";
    //cout<<inter_ID<<" at poi "<<endl;
    return inter_ID;
}

// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.

struct poi_info{
    unsigned id;
    double distance;
    
    bool operator<(const poi_info & a) const
    {
        return distance < a.distance;
    }
};

vector<poi_info> find_closest_POIs(LatLon intersection_position, string point_of_interest_name){
    ////cout << "here !\n";
    unordered_map<string,vector<unsigned>>::iterator it;
    it = POI_list.find(point_of_interest_name);
    vector<poi_info> poi_list(0);
    if(it==POI_list.end())return poi_list;
    unsigned count = POI_list[point_of_interest_name].size();
    double distance;
    //vector<unsigned> closest_poi_list;
    //vector<double> distances;
    LatLon poi_position;
    
    for(unsigned i=0;i<count;i++){
        poi_position = getPointOfInterestPosition(POI_list[point_of_interest_name][i]);
        distance = find_distance_between_two_points(intersection_position, poi_position);
        poi_info info;
        info.id = POI_list[point_of_interest_name][i];
        info.distance = distance;
        poi_list.push_back(info);
    }
    sort(poi_list.begin(),poi_list.end());
    return poi_list;
}

/*
vector<unsigned> find_closest_POIs(LatLon intersection_position, string point_of_interest_name){
    unsigned count = POI_list[point_of_interest_name].size();
    double distance;
    vector<unsigned> closest_poi_list;
    vector<double> distances;
    
    LatLon poi_position;
    unsigned j;
    
    for(unsigned i=0;i<count;i++){
        poi_position = getPointOfInterestPosition(POI_list[point_of_interest_name][i]);
        distance = find_distance_between_two_points(intersection_position, poi_position);
        
        for(j=0;j<distances.size();j++){
            if(distance < distances[j]){
              
                if(distances.size()==SIZE_OF_CLOSEST_POI_LIST){
                    ////////cout << "here\n";
                    closest_poi_list.pop_back();
                    distances.pop_back();
                    ////////cout << "updated distances size " << distances.size() << endl;
                }
                closest_poi_list.insert(closest_poi_list.begin()+j,POI_list[point_of_interest_name][i]);
                distances.insert(distances.begin()+j,distance);
                ////////cout << "j = " << j << endl;
                ////////cout << "updated distances size is" << distances.size() << endl;
                break;
            }
        }
        if(j==distances.size() && j<SIZE_OF_CLOSEST_POI_LIST){
            distances.push_back(distance);
            closest_poi_list.push_back(POI_list[point_of_interest_name][i]);
        }
    }
    return closest_poi_list;
}*/

std::vector<unsigned> find_path_to_point_of_interest (unsigned intersect_id_start, std::string point_of_interest_name){
    //cout << "first\n";
    vector<unsigned> path, shortest_path(0);
    if(intersect_id_start>intersections_count)return shortest_path;
    LatLon intersection_position = getIntersectionPosition(intersect_id_start);
    //cout << "second\n";
    vector<poi_info> POI_ids = find_closest_POIs(intersection_position,point_of_interest_name); 
    //cout << "third\n";
    vector<unsigned> POI_intersections;
    double shortest_path_time = MAX_TRAVEL_TIME, path_time;

    ////////cout << POI_ids.size();
    ////cout << "we start at " << intersect_id_start << endl;
    //cout << POI_ids.size() << endl;
    for(unsigned i=0;i<POI_ids.size() && i<SIZE_OF_CLOSEST_POI_LIST;i++){
        //POI_intersections.push_back(find_closest_intersection(getPointOfInterestPosition(POI_ids[i])));
        POI_intersections.push_back(POI_inter_id[POI_ids[i].id]);
        path = find_path_between_intersections(intersect_id_start,POI_intersections[i]);
//        ////cout << "for the poi with inter id = " << POI_intersections[i] << endl;
//        ////cout << "path size is "<<path.size() << endl;
//        ////cout << "beginning at "<<getStreetSegmentEnds(path[0]).from << " to " << getStreetSegmentEnds(path[path.size()-1]).to << endl;
        if(notFound)continue;
        path_time = compute_path_travel_time(path);
      //  //cout<<" the poi id is "<<POI_ids[i]<<endl;
        //cout<<path_time<<" path time is this "<<endl;
        if(path_time < shortest_path_time){
            shortest_path_time = path_time;
            shortest_path = path;
        }
        ////////cout << "for poi " << POI_ids[i] << " closest intersect is " << POI_intersections[i] << endl;
    }
    //////cout << "done\n";
    return shortest_path;
}