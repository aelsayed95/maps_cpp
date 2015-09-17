#pragma once //protects against multiple inclusions of this header file

#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include<sstream>
#include <iterator>
#include <set>
#include "StreetsDatabaseAPI.h"
using namespace std;


//use these defines whenever you need earth radius 
//or conversion from degrees to radians
#define EARTH_RADIUS_IN_METERS 6372797.560856
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define STREET_SEG_INTERVAL 500
#define PUSH_LEFT 1000000
/*
 When searching for the nearest POI, after the inspection of google maps and understanding
   the nature of latitude and longitude geographical system, we learned the following:
   Maximum value of Latitude is 90 degrees (at poles) 
   Minimum value of Latitude is 0 degree (at Equator) 
   Maximum value of Longitude is 180 degrees ( eastward from Greenwich) 
   Minimum value of Longitude is -180 degree ( westward from Greenwich)
   Thus, we defined the min search range to be 5 degrees around the starting point
   and the max search range to be 100 degrees around the starting point
 */
#define MIN_SEARCH_RANGE 5
#define MAX_SEARCH_RANGE 20

struct two_vectors{
    vector<unsigned> inters;
    vector<unsigned> poi;
};

//function to load bin or osm map
bool load_map(std::string map_name);

//close the loaded map
void close_map();

//function to return intersection id for an intersection name
unsigned find_intersection_id_from_name(std::string intersection_name);

//function to return street id for an intersection name
unsigned find_street_id_from_name(std::string street_name);

//function to return the street segments for a given intersection 
std::vector<unsigned> find_intersection_street_segments(std::string intersection_name);
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id);

//function to return street names at an intersection
std::vector<std::string> find_intersection_street_names(std::string intersection_name);
std::vector<std::string> find_intersection_street_names(unsigned intersection_id);

//can you get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)
bool are_directly_connected(std::string intersection_name1, std::string intersection_name2);

//find all intersections connected by one street segment from given intersection (hint: check for 1-way streets too)
std::vector<unsigned> find_adjacent_intersections(std::string intersection_name);

//find all intersections connected by one street segment from given intersection id
std::vector<unsigned> find_adjacent_intersections(unsigned intersectionID);

//for a given street, return all the street segments
std::vector<unsigned> find_street_street_segments(std::string street_name);

//for a given street, find all the intersections
std::vector<unsigned> find_all_street_intersections(std::string street_name);

//find distance between two coordinates
double find_distance_between_two_points(LatLon point1, LatLon point2);

//find the length of a given street segments
double find_street_segment_length(unsigned street_segment_id);

//find the length of a whole street
double find_street_length(std::string street_name);

//find the travel time to drive a street segment (time(minutes) = distance(Km)/speed_limit(Km/h)*60)
double find_segment_travel_time(unsigned street_segment_id);

//find the nearest point of interest (by name) to a given position
std::string find_closest_point_of_interest(LatLon my_position);

unsigned number_of_street_segments_in_street_by_segmentID(unsigned segmentID);

unsigned number_of_street_segments_in_street(unsigned streetID);

double find_speed_limit_street_segmentID(unsigned segmentID);

void draw_street_name(unsigned streetID, string streetName,double initial_area);

StreetSegmentEnds find_segment_ends(unsigned streetID, unsigned segmentID);

unsigned get_segmentID_by_streetID_segmentNum(unsigned streetID, unsigned segmentIndex);

vector<LatLon> find_POI_Locations_by_name(string POI_name, LatLon intersection_id_start);

std::vector<unsigned> find_intersection_street_segments_one_way(unsigned intersection_id);

bool compare_in_lower(string s1, string s2);