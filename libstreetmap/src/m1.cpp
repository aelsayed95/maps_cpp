#include "m1.h"
#include "m3.h"

#include "input.h"
//Declare data structures used to store data from the given map
//Using unordered_map STL container because the hash table in an unordered map
//only costs O(1) to to map a key to a value
typedef std::unordered_map<string,unsigned> Mymap;
Mymap intersections;    //used to intersections names to intersection IDs
Mymap intersections_lowercase;    //used to intersections names to intersection IDs
Mymap streets;          //used to map street names to street IDs
Mymap POI;              //used to map POI names to POI IDs

unordered_map <unsigned, vector<unsigned> > adjacent_intersections;
std::unordered_map<string,vector<unsigned>> POI_list;     //used to map a poi names to a vector of its IDs
std::unordered_map<unsigned,unsigned> POI_inter_id;     //used to map a poi to its closest intersection
std::unordered_map<unsigned,double> seg_travel_time;
std::unordered_map<string, string>poi_lower_to_upper_names;

double min_lat, min_lon, max_lat, max_lon, init_area;
unsigned lat_range, lon_range;
unsigned intersections_count;
unsigned streets_count;
unsigned poi_count;
unsigned street_segment_count;
//vector<string> intersection_names;
vector<string> intersection_names;
vector<string> poi_names;
//vector<const char*> street_names;
//vector<const char*> street_names_compare;
vector<const char*>intersection_name_in_order;

typedef std::unordered_map<unsigned,vector<unsigned>> streetProperties;
streetProperties streetToStreetSegments; //used to map a street to the segments that lie on that street

typedef std::unordered_map<unsigned,double> streetSegSpeed ;
streetSegSpeed streetSegToSpeedLimit;

unordered_map<unsigned long long, unsigned> best_seg_id;

unordered_map<unsigned,unsigned> street_seg_street_id;

vector< vector<two_vectors> > positions;

double area_of_screen(){   
    min_lat = 1000, min_lon = 1000, max_lat = -10000, max_lon = -10000;
    double area;
    StreetSegmentEnds ends;
    LatLon st_p,  en_p, poi_position;
    for(unsigned i=0;i<street_segment_count;i++){
        ends = getStreetSegmentEnds(i);
        st_p = getIntersectionPosition(ends.from);
        en_p = getIntersectionPosition(ends.to);
        min_lat = (st_p.lat < min_lat)? st_p.lat : min_lat;
        min_lon = (st_p.lon < min_lon)? st_p.lon : min_lon;
        max_lat = (st_p.lat > max_lat)? st_p.lat : max_lat;
        max_lon = (st_p.lon > max_lon)? st_p.lon : max_lon;
        
        min_lat = (en_p.lat < min_lat)? en_p.lat : min_lat;
        min_lon = (en_p.lon < min_lon)? en_p.lon : min_lon;
        max_lat = (en_p.lat > max_lat)? en_p.lat : max_lat;
        max_lon = (en_p.lon > max_lon)? en_p.lon : max_lon;
    }
    for(unsigned i = 0; i<poi_count; i++){
        poi_position = getPointOfInterestPosition(i);
        min_lat = (poi_position.lat < min_lat)? poi_position.lat : min_lat;
        min_lon = (poi_position.lon < min_lon)? poi_position.lon : min_lon;
        max_lat = (poi_position.lat > max_lat)? poi_position.lat : max_lat;
        max_lon = (poi_position.lon > max_lon)? poi_position.lon : max_lon;
    }
    
    area = abs(max_lat-min_lat)*abs(max_lon- min_lon);
    init_area = area;
    return area;
}

string lower_case_string_m1(string input){
    unsigned text_len = input.size();
    for(unsigned i=0;i<text_len;i++){
        input[i] = tolower(input[i]);
    }
    return input;
}

//load_map does the following
// 1- loads StreetDataBaseBin, given the map_name (a .bin file)
// 2- loads intersections unordered_map
// 3- loads streets unordered_map
// 4- loads POI unordered_map

bool load_map(std::string map_name) {
    bool load_success = loadStreetDatabaseBIN(map_name);    
    if(load_success){
        
        intersections_count = getNumberOfIntersections();
        streets_count = getNumberOfStreets();
        poi_count = getNumberOfPointsOfInterest();
        street_segment_count = getNumberOfStreetSegments();

        area_of_screen();
        lon_range = abs(min_lon - max_lon)*1000+1;
        lat_range = abs(min_lat - max_lat)*1000+1;
       // two_vectors inter_and_poi_vec;
//        cout<<min_lon<<" "<<min_lat<< " "<<endl;
//        cout<<lon_range<<" lon that was "<<lat_range<<" lat that was "<<endl;
        best_seg_id.clear();
        positions.clear();
        positions.resize(lon_range);
        for(unsigned i=0;i<lon_range;i++)
        {
            positions[i].clear();
            positions[i].resize(lat_range);
        }   
        
        //Loading street data from DatabaseBIN to our data structure of type unordered map
        //unsigned numOfStreets = getNumberOfStreets();
        string streetName;
        unsigned streetID;
        double speedLimit;
        for(unsigned i=0;i<streets_count;i++) //i is the street ID
        {
            streetName = getStreetName(i);
            streets.insert(make_pair(streetName,i));
        }
        
        //mapping street segments to the street they belong to
        //unsigned numOfStreetSegs = getNumberOfStreetSegments();
        for(unsigned i=0;i<street_segment_count;i++){ // i is the streetSeg ID
            streetID = getStreetSegmentStreetID(i);
            speedLimit = getStreetSegmentSpeedLimit(i);
            streetToStreetSegments[streetID].push_back(i);
            streetSegToSpeedLimit.insert(make_pair(i,speedLimit));
            
            double travel_time = find_street_segment_length(i)*UNITS_CONVERSION/getStreetSegmentSpeedLimit(i);
            seg_travel_time.insert(make_pair(i,travel_time));
            street_seg_street_id.insert(make_pair(i,getStreetSegmentStreetID(i)));
        }
        //cout << "after street segments\n";
        //Loading intersection data from DatabaseBIN to our data structure of type unordered map
        //unsigned numOfInters = getNumberOfIntersections();
        string interName;
        unsigned lon_range, lat_range;
        string intersection_name_string;
        unsigned long long joined_id, adj_id;
        for(unsigned i=0;i<intersections_count;i++) // i is the intersection ID
        {
            interName = getIntersectionName(i);
            intersections.insert(make_pair(interName,i));
            //intersection_names.push_back(interName.c_str());
           
            adjacent_intersections[i] = find_adjacent_intersections(i);
            
            for(unsigned j=0;j<adjacent_intersections[i].size();j++){
                joined_id = i*PUSH_LEFT + adjacent_intersections[i][j];
                adj_id = find_street_segment(i,adjacent_intersections[i][j]);
                //best_seg_id[joined_id] = adj_id;
                best_seg_id.insert(make_pair(joined_id,adj_id));
                //best_seg_id.insert(make_pair<unsigned,unsigned>(i,adjacent_intersections[i][j]),adj_id);
            }
            
            //finds at which index the intersection is (within which range in the map position)
            //cout << "";//<< endl;
            LatLon inter_position = getIntersectionPosition(i);
            lon_range = abs(inter_position.lon - min_lon)*1000;
            lat_range = abs(inter_position.lat - min_lat)*1000;
            //populating that vector at the right index with that id of the intersection
            positions[lon_range][lat_range].inters.push_back(i);
            
            intersection_names.push_back(interName);
            
            
            intersections_lowercase.insert(make_pair(lower_case_string_m1(interName),i));
            //intersections_lowercase(lower_case_string(interName));
            
           // //cout << getIntersectionPosition(i).lat << " " << getIntersectionPosition(i).lon << endl;
            //intersectionIDToName.insert(make_pair(i, interName));
            
        }
        //cout << "after intersections\n";
        
        //Loading POI data from DatabaseBIN to our data structure of type unordered map
        //unsigned numOfPOIs = getNumberOfPointsOfInterest();
        string POIName;
        for(unsigned i=0;i<poi_count;i++) // i is the POI ID
        {
            POIName = getPointOfInterestName(i);
            POI.insert(make_pair(POIName,i));
            POI_list[POIName].push_back(i);
            poi_lower_to_upper_names.insert(make_pair(lower_case_string_m1(POIName), POIName));
            poi_names.push_back(POIName);
            //finds at which index the poi is (within which range in the map position)
            LatLon poi_position = getPointOfInterestPosition(i);
            lon_range = abs(poi_position.lon - min_lon)*1000;
            lat_range = abs(poi_position.lat - min_lat)*1000;
            //populating that vector at the right index with that id of the poi
            positions[lon_range][lat_range].poi.push_back(i);            
            
            //POI_inter_id.insert(make_pair(i,find_closest_intersection(getPointOfInterestPosition(i))));
            
            ////cout << i << " " << poi_count << endl;
        }
        for(unsigned i = 0; i<poi_count; i++){
            POI_inter_id.insert(make_pair(i,find_closest_intersection(getPointOfInterestPosition(i))));    
        }
        //cout << "pois\n";
    }
    ////cout << intersections_count << endl;
    return load_success;
}

//close the map
void close_map() {
    closeStreetDatabase();
    
    // destroy any data structures you created in load_map
    // ...
}

//function to return intersection id for an intersection name
unsigned find_intersection_id_from_name(std::string intersection_name){
    //using the intersections unordered_map, find intersection id by name: O(1)
    unsigned interID = intersections[intersection_name];
    return interID;
}

//function to return street id for an intersection name
unsigned find_street_id_from_name(std::string street_name){
    //using the intersections unordered_map, find street id by name: O(1)
    unsigned streetID = streets[street_name];
    return streetID;
}

//function to return the street segments for a given intersection 
std::vector<unsigned> find_intersection_street_segments(std::string intersection_name){
    //find intersection ID and the number of segments at that intersection
    unsigned intersectionID = find_intersection_id_from_name(intersection_name);
    unsigned segmentCount = getIntersectionStreetSegmentCount(intersectionID);
    vector<unsigned> segmentIDs(segmentCount);
    
    for(unsigned i=0;i<segmentCount;i++)//for each segment at that intersection
        segmentIDs[i] = getIntersectionStreetSegment(intersectionID,i); //store the segment ID in a vector
    return segmentIDs;
}

//function to find street segments at a given intersection
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id){
    //variable initializations and declarations
    unsigned segmentCount = getIntersectionStreetSegmentCount(intersection_id);
    vector<unsigned> segmentIDs(segmentCount);
    
    //loop throught the street segments and store the segment IDs in a vector
    for(unsigned i=0;i<segmentCount;i++)
        segmentIDs[i] = getIntersectionStreetSegment(intersection_id,i);
    return segmentIDs;    
}

//function to find street segments at a given intersection which go out of the intersection
std::vector<unsigned> find_intersection_street_segments_one_way(unsigned intersection_id){
    //variable initializations and declarations
    unsigned segmentCount = getIntersectionStreetSegmentCount(intersection_id);
    //vector<unsigned> segmentIDs(segmentCount);
    unsigned segmentID;
    vector<unsigned> segmentIDs_final;
    StreetSegmentEnds sEnds;
    //loop throught the street segments and store the segment IDs in a vector
    for(unsigned i=0;i<segmentCount;i++){
        segmentID = getIntersectionStreetSegment(intersection_id,i);
        sEnds = getStreetSegmentEnds(segmentID);
        bool isOneWay = getStreetSegmentOneWay(segmentID);
        if( (sEnds.from==intersection_id) || (sEnds.to==intersection_id && !isOneWay)){
            segmentIDs_final.push_back(segmentID);
        }
    }
    return segmentIDs_final;    
}

/*
Given an intersection name, this function returns a vector of names of the streets
that meet at that intersection
*/
std::vector<std::string> find_intersection_street_names(std::string intersection_name){
    //variable initializations and declarations
    unsigned intersectionID = find_intersection_id_from_name(intersection_name);
    unsigned segmentCount = getIntersectionStreetSegmentCount(intersectionID);
    vector<string> streetNames;
    unsigned segmentID, streetID, j;
    string streetName;
    
    for(unsigned i=0;i<segmentCount;i++){ //for every segment at the intersection
        segmentID = getIntersectionStreetSegment(intersectionID,i);
        streetID = getStreetSegmentStreetID(segmentID);
        streetName = getStreetName(streetID);
        for(j=0; j<streetNames.size(); j++)//check that the street hasn't been pushed into the vector already
            if(streetNames[j]== streetName)
                break;
        if(j==streetNames.size())//if name was not found in the vector
            streetNames.push_back(streetName); //push it in
    }
    return streetNames;
}


/*
Given an intersection ID, this function returns a vector of names of the streets
that meet at that intersection
*/
std::vector<std::string> find_intersection_street_names(unsigned intersection_id){
    //variable initializations and declarations
    unsigned segmentCount = getIntersectionStreetSegmentCount(intersection_id);
    vector<string> streetNames;
    unsigned segmentID, streetID, j;
    string streetName;

    for(unsigned i=0;i<segmentCount;i++){ //for every segment at the intersection
        segmentID = getIntersectionStreetSegment(intersection_id,i);
        streetID = getStreetSegmentStreetID(segmentID);
        streetName = getStreetName(streetID);
        for(j=0; j<streetNames.size(); j++)//check that the street hasn't been pushed into the vector already
            if(streetNames[j]== streetName)
                break;
        if(j==streetNames.size())//if name was not found in the vector
            streetNames.push_back(streetName);//push it in
    }
    return streetNames;
}

/*
 Given two intersections, the function checks if there is a direct connection from
 * intersection 1 to intersection 2. The function takes into account the direction of
 * each street (one-way or two-way). returns 1 if there's a direct path, 0 otherwise
 */
bool are_directly_connected(std::string intersection_name1, std::string intersection_name2){
    //variable initializations and declarations
    unsigned intersectionID1 = find_intersection_id_from_name(intersection_name1);
    unsigned intersectionID2 = find_intersection_id_from_name(intersection_name2);
    unsigned segmentCount1 = getIntersectionStreetSegmentCount(intersectionID1);
    StreetSegmentEnds ends1;
    unsigned int segmentID1;
    
    for(unsigned i=0;i<segmentCount1;i++){ //for each segment at intersection 1
        segmentID1 = getIntersectionStreetSegment(intersectionID1,i);
        ends1 = getStreetSegmentEnds(segmentID1);
        //the one-way street check
        if(ends1.to ==  intersectionID2) // check if the end point of the segment is connected directly to intersection 2
            return true;
        //the two-way street check
        else if(getStreetSegmentOneWay(segmentID1)==false && (ends1.from==intersectionID2))
            return true;
    }
    return false;
}

/*
 given a intersection name, returns a list of intersection IDs that are directly
 * connected to the given intersection
 */
std::vector<unsigned> find_adjacent_intersections(std::string intersection_name){
    //variable initializations and declarations
    unsigned intersectionID = find_intersection_id_from_name(intersection_name);
    unsigned segmentCount = getIntersectionStreetSegmentCount(intersectionID);
    StreetSegmentEnds ends;
    unsigned int segmentID;
    vector<unsigned> adjInters;
    for(unsigned i=0;i<segmentCount;i++){ // for each segment at that intersection
        //find the intersection at the other end of that segment:
        //1) find the segment ID
        segmentID = getIntersectionStreetSegment(intersectionID,i);
        //2) find the segment ends
        ends = getStreetSegmentEnds(segmentID);
        //3) find the intersection at the of that intersection
        if(ends.from==intersectionID)  // one way street FROM -> TO
            adjInters.push_back(ends.to);
        else if(ends.to==intersectionID  && getStreetSegmentOneWay(segmentID)==false) 
            adjInters.push_back(ends.from);
    }
    return adjInters;
}

std::vector<unsigned> find_adjacent_intersections(unsigned intersectionID){
    //variable initializations and declarations
    //unsigned intersectionID = find_intersection_id_from_name(intersection_name);
    unsigned segmentCount = getIntersectionStreetSegmentCount(intersectionID);
    StreetSegmentEnds ends;
    unsigned int segmentID;
    vector<unsigned> adjInters;
    for(unsigned i=0;i<segmentCount;i++){ // for each segment at that intersection
        //find the intersection at the other end of that segment:
        //1) find the segment ID
        segmentID = getIntersectionStreetSegment(intersectionID,i);
        //2) find the segment ends
        ends = getStreetSegmentEnds(segmentID);
        //3) find the intersection at the of that intersection
        if(ends.from==intersectionID)  // one way street FROM -> TO
            adjInters.push_back(ends.to);
        else if(ends.to==intersectionID  && getStreetSegmentOneWay(segmentID)==false) 
            adjInters.push_back(ends.from);
    }
    return adjInters;
}

//for a given street, return all the street segments
std::vector<unsigned> find_street_street_segments(std::string street_name){
   //using our unordered_map that maps the street to its street segments
   unsigned mainStreetID = find_street_id_from_name(street_name);
   //return the vector of street segments that belong to that street ID
   return streetToStreetSegments[mainStreetID];
}

/*
 Given a street name, finds all street intersections that lie on that street
 */
std::vector<unsigned> find_all_street_intersections(std::string street_name){
    //variable declarations and initializations
    unsigned mainStreetID = find_street_id_from_name(street_name); //main street is the given street
    unsigned segmentCount = streetToStreetSegments[mainStreetID].size();
    StreetSegmentEnds sEnds;
    
    //vector to be returned: will contain the intersection IDs 
    vector<unsigned> streetIntersectionIDs;
    unsigned segmentID;
    
    //using a set to temporarily store the intersection IDs
    //a set has fast built-in sort and check for repeated elements
    set<unsigned> intersectionIDSet; 

    for(unsigned i=0;i<segmentCount;i++){//for each street segment that belongs to the main street
        segmentID = streetToStreetSegments[mainStreetID][i]; //get the segment ID
        sEnds = getStreetSegmentEnds(segmentID);
        intersectionIDSet.insert(sEnds.from); //insert end1 of the segment
        intersectionIDSet.insert(sEnds.to);   //insert end2 of the segment
    }
    
    //copy the content of the set into a vector in order to return it
    std::copy(intersectionIDSet.begin(), intersectionIDSet.end(), std::back_inserter(streetIntersectionIDs));
    return streetIntersectionIDs;
}

//find distance between two coordinates
//calculating the distance between two points given their (lat,long)
double find_distance_between_two_points(LatLon point1, LatLon point2){
    /*
    double latavg = (point1.lat + point2.lat)/2; // average latitude
    double radLat = latavg*DEG_TO_RAD;          //need to convert to radians because
    double x1, x2, y1, y2;                      //cmath functions like cos(x) use radians
    x1 = DEG_TO_RAD*point1.lon* cos(radLat);
    x2 = DEG_TO_RAD*point2.lon* cos(radLat);    
    y1 = DEG_TO_RAD*point1.lat;
    y2 = DEG_TO_RAD*point2.lat;

    return (EARTH_RADIUS_IN_METERS * sqrt((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1)));
    */
    double dlon = DEG_TO_RAD*(point2.lon - point1.lon);
    double dlat = DEG_TO_RAD*(point2.lat - point1.lat);
    double a = (sin(dlat/2))*(sin(dlat/2)) + cos(DEG_TO_RAD*point1.lat) * cos(DEG_TO_RAD*point2.lat) * (sin(dlon/2))*(sin(dlon/2));
    double c = 2 * atan2( sqrt(a), sqrt(1-a) );
    return (EARTH_RADIUS_IN_METERS * c ); 
}

//find the length of a given street segments
double find_street_segment_length(unsigned street_segment_id){
    StreetSegmentEnds sEnds = getStreetSegmentEnds(street_segment_id);
    unsigned int curvePoints = getStreetSegmentCurvePointCount(street_segment_id);
    double distance = 0;
    LatLon p1 = getIntersectionPosition(sEnds.from);
    LatLon p2;
    int i=0;
    
    //need to account for curved streets by dividing the segment into smaller segments
    //that approximate the curve then calculate the distance between the end points of the
    //curved segments
    while(curvePoints--){
        p2 = getStreetSegmentCurvePoint(street_segment_id,i);
        distance += find_distance_between_two_points(p1,p2); 
        p1 = p2;
        i++;
    }
    //adds the end point of the segment
    p2 = getIntersectionPosition(sEnds.to);
    distance +=find_distance_between_two_points(p1,p2);
    
    return distance;
}

/*
 * find the length of a whole street given a street name
 * */
double find_street_length(std::string street_name){
    //variable declaration and initialization
    vector<unsigned> streetSegsInStreet = find_street_street_segments(street_name);
    int streetSegCount = streetSegsInStreet.size();
    double distance = 0;
    for(int i=0;i<streetSegCount;i++){ //finds the length of each segment in the street
                                       // and adds it to find the length of the total street
        distance+= find_street_segment_length(streetSegsInStreet[i]);
    }
    return distance;
}

/*
 * find the travel time to drive a street segment (time(minutes) = distance(Km)/speed_limit(Km/h)*60)
*/
double find_segment_travel_time(unsigned street_segment_id){
    double speed = getStreetSegmentSpeedLimit(street_segment_id);
    double distance = find_street_segment_length(street_segment_id)/1000;
    return distance*60/speed; //multiply by 60 to convert to minutes
}

/*
 * find the nearest point of interest (by name) to a given position
 * All constants used in this function are declared in m1.h
 * a brief explanation for the constants is given
 */
std::string find_closest_point_of_interest(LatLon my_position){
    //unsigned POIcount = getNumberOfPointsOfInterest();
    LatLon POIposition;
    double distance, shortestDistance = EARTH_RADIUS_IN_METERS;
    //initializes the shortest distance to the largest distance possible on Earth
    //the assumption is that no city extends for EARTH_RADIUS meters
                                                               
    string POIname;
    unsigned searchRange = MIN_SEARCH_RANGE; 
    do{
        //if the point of interest is not found within the boundaries of the first range, 
        // add the minimum search range to the searching range and search again
        searchRange+=MIN_SEARCH_RANGE;
        for(unsigned i=0;i<poi_count;i++){
            POIposition = getPointOfInterestPosition(i);
            if((POIposition.lat <= my_position.lat+searchRange) &&
               (POIposition.lat >= my_position.lat-searchRange) &&
               (POIposition.lon <= my_position.lon+searchRange) &&
               (POIposition.lon >= my_position.lon-searchRange))//within a searchRange x searchRange degrees range (square range)
                {
                   distance = find_distance_between_two_points(my_position,POIposition);
                   //if found a closer POI, update shortest distance and save the POI ID
                   if(distance<shortestDistance){
                       shortestDistance = distance;
                       POIname = getPointOfInterestName(i);
                   }
                }
        }
    }while(shortestDistance==EARTH_RADIUS_IN_METERS && searchRange < MAX_SEARCH_RANGE);
    //keep looping while: no POIs were found   OR  the searchRange has reached the maximum search range

    return POIname;
}



unsigned number_of_street_segments_in_street_by_segmentID(unsigned segmentID){
    unsigned streetID = getStreetSegmentStreetID(segmentID);
    string streetName;
    streetName = getStreetName(streetID);
    double streetSize=0;
    if(streetName!= "(unknown)")
        streetSize = number_of_street_segments_in_street(streetID);
    return streetSize;
}

unsigned number_of_street_segments_in_street (unsigned streetID){
    return streetToStreetSegments[streetID].size();
}

double find_speed_limit_street_segmentID(unsigned segmentID){
    return streetSegToSpeedLimit[segmentID];
}

unsigned get_segmentID_by_streetID_segmentNum(unsigned streetID, unsigned segmentIndex){
    ////cout << streetID << " " << segmentIndex << endl;
    return streetToStreetSegments[streetID][segmentIndex];
}

StreetSegmentEnds find_segment_ends(unsigned streetID, unsigned segmentIndex){
    return getStreetSegmentEnds(streetToStreetSegments[streetID][segmentIndex]);
}


//This function returns a vector of locations of POIs with the target name
//ordered based on increasing distance from the start intersection location
vector<LatLon> find_POI_Locations_by_name(string POI_name,LatLon intersection_location){
   // unsigned POI_count=getNumberOfPointsOfInterest();
    LatLon cur_poi_location;    
    double distance; 
    vector<LatLon> POI_locations;
    vector<double> distances;

    for(unordered_map<string,unsigned>::iterator it=POI.begin();it!=POI.end();it++){
        if(POI_name==it->first){
//            //cout << "HERE IN THE IT LOOP" << it -> first << "\n";
            cur_poi_location = getPointOfInterestPosition(it->second);
            distance = find_distance_between_two_points(intersection_location,cur_poi_location);
            unsigned j;
            for(j=0;j<POI_locations.size();j++)
                if(distance < distances[j]){ // if cur position is closer to the intersection location
                    POI_locations.insert(POI_locations.begin()+j,cur_poi_location); // insert the current location at the jth position
                    distances.insert(distances.begin()+j, distance);                // insert its corresponding distance at the jth position
                    break;
                }
            if(j==0)
            {
                POI_locations.insert(POI_locations.begin(),cur_poi_location);
            }
        }
    }
    return POI_locations; 
}

bool compare_in_lower (string first, string second){
  for(unsigned i = 0; i < first.length() && i < second.length(); i++){
    if (tolower (first[i]) < tolower (second[i])) return true;
    else if (tolower (first[i]) > tolower (second[i])) return false;
  }
  if (first.length() < second.length()) return true;
  else return false;
}