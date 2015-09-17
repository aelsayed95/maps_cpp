#pragma once
#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include "m2.h"

#define TURN_TIME 0.25
#define MAX_TRAVEL_TIME 5500000
#define SIZE_OF_CLOSEST_POI_LIST 3
#define MAX_SPEED 90
#define TIME_FACTOR 1000
#define UNITS_CONVERSION 0.06

// Returns a path (route) between the start intersection and the end 
// intersection, if one exists. If no path exists, this routine returns 
// an empty (size == 0) vector. If more than one path exists, the path 
// with the shortest travel time is returned. The path is returned as a vector 
// of street segment ids; traversing these street segments, in the given order,
// would take one from the start to the end intersection.
std::vector<unsigned> find_path_between_intersections(unsigned 
                   intersect_id_start, unsigned intersect_id_end);


// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can 
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.
double compute_path_travel_time(const std::vector<unsigned>& path);


// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.
std::vector<unsigned> find_path_to_point_of_interest (unsigned 
                   intersect_id_start, std::string point_of_interest_name);


unsigned find_closest_intersection(LatLon poi_position);

void populate_adj_intersections_unordered_map();

unsigned find_street_segment(unsigned first_id, unsigned second_id);