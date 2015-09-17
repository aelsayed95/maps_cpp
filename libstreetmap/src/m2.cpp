#include "m3.h"
#include "input.h"

/*
 * loads pins from a file into a vector of t_points representing pin locations
 * The function updates the content of the vector every time it is called. Right
 * now the function updates the content based on a comparison between the content
 * of the file and the number of vectors in a pin_locations vector.
 */

/* May need revision: may need to clear vector content and reload it from scratch
 * every time the function is called in order to avoid problems with deleting and
 * inserting new pin locations.
*/

vector<t_point> pin_locations;
unsigned num_of_pins;
vector<bool> occupied;
vector<unsigned> occupied_poi;
vector<bool> visible_poi;
vector<bool> name_visible_poi;
vector<bool> visible_street;
vector<unsigned>final_path;
vector<unsigned>inters_visited;
vector<unsigned>depots_visited;
extern bool draw_on;

double grid_size_x, grid_size_y;
extern double min_lat , min_lon , max_lat, max_lon, init_area;

unsigned num_box_x, num_box_y;
double zoom_level;
unsigned line_width, highway_line_width, big_street_line_width;
t_bound_box updated_coords, current_visible_world;
t_point last_press_pos;
bool screen_grab;
unsigned numberOfPOIs;

unsigned load_pins(){
    
    ifstream fin;
    fin.open("pins.txt");
    if(!fin){
        //cout << "WARNING! file not found!\n";
        return 0;
    }
    
    t_point pin;
    unsigned i = 0;
    unsigned pins_number = pin_locations.size();
    
    // Emptying the vector ???
    for(i = 0; i < pins_number; i++)
        pin_locations.pop_back();
    ////cout << "emptying vector...\ndeleted " << pins_number << " locations\n";
    i = 0;
    fin >> pin.x >> pin.y;
    
    // Storing the locations of all points in the vector.
    while(!fin.eof()){
        pin_locations.push_back(pin);
        i++;
        fin >> pin.x >> pin.y;
    }
    
    fin.close();
    //i represents the most up-to-date number of pins
    return i; 
}

    
/* 
 * If user has clicked on space bar
 * get mouse location
 * draw a teal triangle where mouse is.
 * On mouse click
 * get mouse location
 * place triangle at mouse location
 * save location at on a text file
 */

void save_pins(){
    //update file
    ofstream fout;
    fout.open("pins.txt");
    unsigned i=0, pins_number = pin_locations.size();
    
    // Saving the latitude and longitude of each pin.
    while(pins_number--){
        fout << pin_locations[i].x << " " << pin_locations[i].y << endl;
        i++;
    }
    return;
}

bool clicked_on_pin(unsigned index, float x, float y){
    return((pin_locations[index].y-TRIANGLE_SIDE/zoom_level) <= y &&
           (pin_locations[index].y+TRIANGLE_SIDE/zoom_level) >= y &&
           (pin_locations[index].x-TRIANGLE_SIDE/zoom_level) <= x &&
           (pin_locations[index].x+TRIANGLE_SIDE/zoom_level) >= x);
}

unsigned clicked_on_POI(double x, double y){
    LatLon mouse_pos;
    mouse_pos.lon = x;
    mouse_pos.lat = y;
    unsigned POIcount = getNumberOfPointsOfInterest();
    LatLon POIposition;
    double distance, shortestDistance = EARTH_RADIUS_IN_METERS;
    //initializes the shortest distance to the largest distance possible on Earth
    //the assumption is that no city extends for EARTH_RADIUS meters
                                                               
    unsigned POI_ID = POIcount;
    double searchRange = POI_RADIUS; 
    for(unsigned i=0;i<POIcount;i++){
        if(!visible_poi[i])continue;
        POIposition = getPointOfInterestPosition(i);
        ////cout << "POI position: ";
        ////cout << POIposition.lon << "," << POIposition.lat << endl;
        ////cout << "mouse position: ";
        ////cout << mouse_pos.lon << "," << mouse_pos.lat << endl;
        if((POIposition.lat <= mouse_pos.lat+searchRange) &&
           (POIposition.lat >= mouse_pos.lat-searchRange) &&
           (POIposition.lon <= mouse_pos.lon+searchRange) &&
           (POIposition.lon >= mouse_pos.lon-searchRange))//within a searchRange x searchRange degrees range (square range)
            {
                ////cout << "found a POI!\n";
               distance = find_distance_between_two_points(mouse_pos,POIposition);
               //if found a closer POI, update shortest distance and save the POI ID
               if(distance<shortestDistance){
                   shortestDistance = distance;
                   POI_ID = i;
               }
            }
    }
    return POI_ID;
}

void act_on_button_press (float x, float y, t_event_buttonPressed event) {
   // std:://cout << "User clicked a mouse button at coordinates (" 
//             << x << "," << y << ")";
   // std:://cout << std::endl;
    if (event.shift_pressed) {

        unsigned num_pins = pin_locations.size(), i;
        for(i=0; i<num_pins; i++)
            if(clicked_on_pin(i,x,y)){
                ////cout << pin_locations.size();
                pin_locations.erase(pin_locations.begin()+i);
                ////cout << " and new size is " << pin_locations.size() << endl;
                break; //found the pin to be deleted
            }
        if(i<num_pins)
          //  //cout << "successfully deleted pin at location (" << pin_locations[i].x << ", " << pin_locations[i].y << endl;      
        save_pins();
    }
    else if(event.ctrl_pressed){
        t_point new_pin;
        new_pin.x = x;
        new_pin.y = y;
        pin_locations.push_back(new_pin);
       // //cout << "successfully added new pin at location (" << x << ", " << y << endl;   
        save_pins();
    }
    else{
        if(event.button==LEFT_MOUSE_BUTTON){
            if(screen_grab==true){
                current_visible_world = get_visible_world();
                last_press_pos.x = x;
                last_press_pos.y = y;
              //  //cout << "screen grabbed\n";
            }
            else{
                updated_coords = get_visible_world();
                updated_coords.offset(last_press_pos.x-x,last_press_pos.y-y);
                set_visible_world(updated_coords);
              //  //cout << "screen released\n";
            }
            screen_grab = !screen_grab;
        }else if(event.button==RIGHT_MOUSE_BUTTON){
            unsigned POI_ID = clicked_on_POI(x,y);
            if(POI_ID!=numberOfPOIs){ // user clicked on a POI
                name_visible_poi[POI_ID] = !name_visible_poi[POI_ID];
            }
        }
    }
    draw_screen(); 
}


void draw_map(std::string map){
    StreetSegmentEnds ends;
    LatLon st_p,  en_p;
    screen_grab = true;
    numberOfPOIs = getNumberOfPointsOfInterest();
    unsigned numberOfStreetSegments = getNumberOfStreetSegments();
    
    name_visible_poi.clear();
    name_visible_poi.resize(numberOfPOIs);
    visible_poi.clear();
    visible_poi.resize(numberOfPOIs);
    for(unsigned i=0;i<numberOfPOIs;i++){
        name_visible_poi[i] = false;
        visible_poi[i] = false;
    }
    
    visible_street.clear();
    visible_street.resize(getNumberOfStreets());
    //raw_on = false;
    
    
    for(unsigned i=0;i<numberOfStreetSegments;i++){
        ends = getStreetSegmentEnds(i);
        st_p = getIntersectionPosition(ends.from);
        en_p = getIntersectionPosition(ends.to);
        min_lat = (st_p.lat < min_lat)? st_p.lat : min_lat;
        min_lon = (st_p.lon < min_lon)? st_p.lon : min_lon;
        max_lat = (st_p.lat > max_lat)? st_p.lat : max_lat;
        max_lon = (st_p.lon > max_lon)? st_p.lon : max_lon;
    }

    const t_bound_box initial_coords = t_bound_box(min_lon,min_lat,max_lon,max_lat); 
    t_color baseColor, streetsColor;
//    baseColor.red = 0xEB;
//    baseColor.green = 0xE4;
//    baseColor.blue = 0xD8;
    
    baseColor.red = 0xED;
    baseColor.green = 0xE6;
    baseColor.blue = 0xDB;

    streetsColor.red = 0xD8;
    streetsColor.green = 0xDF;
    streetsColor.blue = 0xEB;
    
    init_graphics(map, baseColor); // you could pass a t_color RGB triplet instead
    set_visible_world(initial_coords);
    //update_message("");
    event_loop(act_on_button_press, NULL, NULL, draw_screen);
    return;
}

void update_grid_size(){
    grid_size_x = (max_lon - min_lon)/(zoom_level*GRID_WIDTH);

    num_box_x = zoom_level*GRID_WIDTH;
    grid_size_y = (max_lat - min_lat)/(zoom_level*GRID_HEIGHT);
    num_box_y = zoom_level*GRID_HEIGHT;
    occupied.clear();
    occupied.resize(num_box_x*num_box_y);
    
    occupied_poi.clear();
    occupied_poi.resize(num_box_x*num_box_y);
}


unsigned update_zoom_level(){
    
    double initial_area;
    unsigned street_size_threshold;
    initial_area = init_area;//area_of_screen();
    
    if(LOD_area_test(ZOOM_LEVEL1*initial_area)==false){//at very zoomed out
        street_size_threshold = 200;
        zoom_level = 1;
        update_grid_size();
        line_width = 1;
        highway_line_width=3;
        big_street_line_width=1.25;
    }
    
    else if(LOD_area_test(ZOOM_LEVEL2*initial_area)==false){//at very zoomed out
        street_size_threshold = 150;
        zoom_level = 1;
        update_grid_size();
        line_width = 1.5;
        highway_line_width=3;
        big_street_line_width=1.75;
    }
    
    else if(LOD_area_test(ZOOM_LEVEL3*initial_area)==false){//at very zoomed out
        street_size_threshold = 100;
        zoom_level = 3;
        update_grid_size();
        line_width = 2;
        highway_line_width=3;
        big_street_line_width=2.25;
    }
    
    else if(LOD_area_test(ZOOM_LEVEL4*initial_area)==false){//at very zoomed out
        street_size_threshold = 90;
        zoom_level = 4;
        update_grid_size();
        line_width = 2.25;
        highway_line_width=3;
        big_street_line_width=2.5;
    }
    
    else if(LOD_area_test(ZOOM_LEVEL5*initial_area)==false){//at very zoomed out
        street_size_threshold = 70;
        zoom_level = 5;
        update_grid_size();
        line_width = 2.5 ;
        highway_line_width=3;
        big_street_line_width=2.75;
    }
    
    else if(LOD_area_test(ZOOM_LEVEL6*initial_area)==false){//at very zoomed out
        street_size_threshold = 50;
        zoom_level = 6;
        update_grid_size();
        line_width = 2.75;
        highway_line_width=5;
        big_street_line_width=3;
    }
    
    else{
        street_size_threshold = 0;
        zoom_level = 7;
        update_grid_size();
        line_width = 3;
        highway_line_width=6;
        big_street_line_width=3.25;
    }
    
    return street_size_threshold;
}

void draw_tour(vector<unsigned>tour, vector<unsigned>inters, vector<unsigned> depots, string map_path){
    draw_on = true;
    final_path.clear();
    final_path = tour;
    
    inters_visited.clear();
    inters_visited = inters;
    
    depots_visited.clear();
    depots_visited = depots;
    draw_map(map_path);
    flushinput();
}

void draw_path(unsigned inter1, unsigned inter2, string map_path, bool find_path_once){
    //cout<<inter1<<" "<<inter2<<endl;
    final_path.clear();
    draw_on = true;
    if (find_path_once == true)
    {
        final_path =  find_path_between_intersections(inter1, inter2);
        print_directions(final_path, "directions");
        find_path_once = false;
    }
    if (final_path.size()!=0){
        //draw_screen();
        draw_map(map_path);
        flushinput();
    }
}

void draw_poi_path(unsigned inter_id_start, string poi_name, string map_path, bool find_path_once){
    //cout<<inter_id_start<<" "<<poi_name<<endl;
    draw_on = true;
    final_path.clear();
    if (find_path_once == true)
    {
        final_path =  find_path_to_point_of_interest(inter_id_start, poi_name);
        print_directions(final_path, "find");
        find_path_once = false;
    }
    if (final_path.size()!=0){
        //draw_screen();
        draw_map(map_path);
        flushinput();
    }
}


void draw_screen(){
    set_draw_mode (DRAW_NORMAL);  // Should set this if your program does any XOR drawing in callbacks.
    clearscreen();  /* Should precede drawing for all drawscreens */
    
    setfontsize (10);
    setlinestyle (SOLID);
    setlinewidth (1);

    StreetSegmentEnds ends;
    t_point start_point, end_point;
    LatLon st_p, en_p;    
    
    unsigned size_of_street, street_id;
    double speed_of_street;
    
    unsigned street_size_threshold = update_zoom_level();

    t_color streetsColor;

    streetsColor.red = 0xff;
    streetsColor.green = 0xff;
    streetsColor.blue = 0xff;
    //std::fill(visible_street.begin(),visible_street.end(),false);
    for(unsigned i=0;i<visible_street.size();i++)
        visible_street[i] = false;
    
    for(unsigned street_segment_id=0;street_segment_id<getNumberOfStreetSegments();street_segment_id++){
        
        //--------------------------------------Drawing Street --------------------------------//
        street_id = getStreetSegmentStreetID(street_segment_id);
        size_of_street = number_of_street_segments_in_street_by_segmentID(street_segment_id);
        speed_of_street = find_speed_limit_street_segmentID(street_segment_id);
        if(size_of_street < street_size_threshold && speed_of_street < HIGHWAY_SPEED_LIMIT) continue;

        int curvePoints;
        t_point start_point, end_point;
        ends = getStreetSegmentEnds(street_segment_id);                      //cmath functions like cos(x) use radians
        curvePoints = getStreetSegmentCurvePointCount(street_segment_id);
        st_p = getIntersectionPosition(ends.from);       
        start_point.y = st_p.lat;
        start_point.x = st_p.lon;
        
        for(int j= 0; j<curvePoints; j++){          
            en_p = getStreetSegmentCurvePoint(street_segment_id,j);
            end_point.y = en_p.lat;
            end_point.x = en_p.lon;
        
            if(speed_of_street >= HIGHWAY_SPEED_LIMIT){
                setlinewidth (highway_line_width);
                setcolor(ORANGE);            
            }
            else if(size_of_street > BIG_STREET_SIZE){
                setlinewidth (big_street_line_width);
                setcolor(streetsColor);
            }
                
            else{
                setlinewidth (line_width);
                setcolor(streetsColor);
            }
           
            drawline (start_point, end_point);
            flushinput();
            start_point = end_point;
        }
        en_p = getIntersectionPosition(ends.to);
        end_point.y = en_p.lat;
        end_point.x = en_p.lon;
        if(speed_of_street >= HIGHWAY_SPEED_LIMIT){
            setlinewidth (highway_line_width);
            setcolor(ORANGE);            
        }
        else if(size_of_street > BIG_STREET_SIZE){
            setlinewidth (big_street_line_width);
            setcolor(streetsColor);
        }

        else{
            setlinewidth (line_width);
            setcolor(streetsColor);
        }

        drawline (start_point, end_point);
        
        
        visible_street[street_id] = true;
        //if (zoom_level > STREET_NAMES_ZOOM_LEVEL)
        ////cout << "street " << street_id << " is visible\n";
        flushinput();           
    }
    
//--------------------------------------Drawing POI---------------------------------//
    
    LatLon poi;
    unsigned box_x, box_y;
    setlinewidth(1);
    
    if(zoom_level>=POI_ZOOM_LEVEL){
        setcolor (RED);
        setfontsize (pow(zoom_level,0.1)*10);
        settextrotation(0); 
        for(unsigned i = 0; i<numberOfPOIs; i++){
            poi = getPointOfInterestPosition(i);
            box_x = (unsigned)abs((poi.lon-min_lon)/grid_size_x);
            box_y = (unsigned)abs((poi.lat-min_lat)/grid_size_y);
            if(occupied_poi[box_y*num_box_x+box_x]<MAX_POI_PER_BOX*zoom_level){
                fillarc (poi.lon,poi.lat,POI_RADIUS,0,360);
                occupied_poi[box_y*num_box_x+box_x]++;
                visible_poi[i] = true;
                if(name_visible_poi[i]){
                    drawtext (poi.lon,poi.lat+TEXT_OFFSET,getPointOfInterestName(i),FLT_MAX, FLT_MAX);
                }
            }
            else{
                visible_poi[i] = false;
            }
        }
    }
    
    
//--------------------------------------Drawing Features----------------------------//
    
    LatLon vertex, prev_vertex, cur_vertex;
    unsigned num_points;
    string attr;
    t_color water_color, waterway_color;
    
    water_color.red = 0xA2;
    water_color.green = 0xDA;
    water_color.blue = 0xF2;
    
    waterway_color.red = 0x5F;
    waterway_color.green = 0xBF;
    waterway_color.blue = 0xE9;
    
    t_point prev_point, cur_point;
    setlinewidth (1);
    for(unsigned i = 0; i<getFeatureCount(); i++){
        attr = getFeatureAttribute(i,"water");
        if(attr!=""){
            num_points = getFeaturePointCount(i);
            t_point points[num_points];
            for(unsigned j=0;j<num_points;j++){
                vertex = getFeaturePoint(i,j);
                points[j].x = vertex.lon;
                points[j].y = vertex.lat;
            }
            setcolor(water_color);
            fillpoly(points,num_points);
            continue;
        }
        
        attr = getFeatureAttribute(i,"waterway");
        if(attr!=""){ 
            setlinewidth(1);
            num_points = getFeaturePointCount(i);
            setcolor(waterway_color);
            prev_vertex = getFeaturePoint(i,0);
            prev_point.x = prev_vertex.lon;
            prev_point.y = prev_vertex.lat;
            for(unsigned j=1;j<num_points;j++){
                cur_vertex = getFeaturePoint(i,j);
                cur_point.x = cur_vertex.lon;
                cur_point.y = cur_vertex.lat;
                drawline(prev_point,cur_point);
                prev_vertex = cur_vertex;
                prev_point.x = cur_vertex.lon;
                prev_point.y = cur_vertex.lat;
            }
            continue;
        }
        
        t_color feature_color, sand_color;
        
//        feature_color.red = 0xD0;
//        feature_color.green = 0xE3;
//        feature_color.blue = 0xB4;
        
        feature_color.red = 0x32;
        feature_color.green = 0xCD;
        feature_color.blue = 0x32;
        
        sand_color.red = 0xF4;
        sand_color.green = 0xA4;
        sand_color.blue = 0x60;
        
        attr = getFeatureAttribute(i,"natural");
        if(attr!=""){
            if(attr == "water"){
                setcolor(water_color);
            }
            else if(attr == "sand" ||attr == "beach")        
                setcolor(sand_color);
            else 
                 setcolor(feature_color);
            
            num_points = getFeaturePointCount(i);
            t_point points[num_points];
            for(unsigned j=0;j<num_points;j++){
                vertex = getFeaturePoint(i,j);
                points[j].x = vertex.lon;
                points[j].y = vertex.lat;
            }          
            fillpoly(points,num_points);
            continue;
        }
        
        t_color land_color;
        land_color.red = 0xED;
        land_color.green = 0xE6;
        land_color.blue = 0xDB;        
        
        attr = getFeatureAttribute(i,"land");
        if(attr!=""){
            num_points = getFeaturePointCount(i);
            t_point points[num_points];
            for(unsigned j=0;j<num_points;j++){
                vertex = getFeaturePoint(i,j);
                points[j].x = vertex.lon;
                points[j].y = vertex.lat;
            }
            setcolor(land_color);
            fillpoly(points,num_points);
            continue;
        }        
    }
    
    //--------------------------------------Drawing Street Names----------------------------//
    

    StreetSegmentEnds sEnds;
    LatLon begin, end, avgPos;
    string streetName;
    double slope, slopeInDegrees;
    setcolor(BLACK);
    //t_bound_box segment_sides;
    if (zoom_level > STREET_NAMES_ZOOM_LEVEL){
        for(unsigned i = 0; i<getNumberOfStreets(); i++){
            streetName = getStreetName(i);
            if(streetName!= "(unknown)"){
                ////cout << "at street " << streetName << endl;
                ////cout << visible_street[i] << endl;
                //if(!visible_street[i])continue;
                ////cout << streetName << " is visible\n";
                unsigned numOfStreetSegs = number_of_street_segments_in_street(i);

                for(unsigned j=0;j<numOfStreetSegs;j++){
                    unsigned segmentID = get_segmentID_by_streetID_segmentNum(i,j);
                    if (zoom_level<6 && getStreetSegmentCurvePointCount(segmentID)==0)
                        continue;
                    
                    sEnds = find_segment_ends(i,j);

                    begin = getIntersectionPosition(sEnds.from);
                    end = getIntersectionPosition(sEnds.to);
                    avgPos.lat = (begin.lat + end.lat)/2.0;
                    avgPos.lon = (begin.lon + end.lon)/2.0;  

                    box_x = (unsigned)abs((avgPos.lon-min_lon)/grid_size_x);
                    box_y = (unsigned)abs((avgPos.lat-min_lat)/grid_size_y);

                    if(zoom_level>6 && !occupied[box_y*num_box_x+box_x]){
                        slope = (end.lat - begin.lat)/(end.lon- begin.lon);   
                        slopeInDegrees = atan(slope)/DEG_TO_RAD;

                        setfontsize (pow(zoom_level,0.2)*5);
                        settextrotation(slopeInDegrees); 

                        drawtext (avgPos.lon,avgPos.lat,streetName,FLT_MAX, FLT_MAX);

                        occupied[box_y*num_box_x+box_x] = true;
                        break;
                    }
                }

            }
        }
    }

    num_of_pins = load_pins();
    
    t_color pin_color;
    pin_color.red = 0x00;
    pin_color.green = 0x80;
    pin_color.blue = 0x80;
    //draw pins (represented by teal triangles)
    t_point tri_vertices[3] = {{0,0},{0,0},{0,0}};
    setcolor (pin_color);
    for(unsigned i=1;i<num_of_pins;i++){
        ////cout << "drawing triangle number " << i << endl;
        //top vertix
        tri_vertices[0].y = pin_locations[i].y - (TRIANGLE_SIDE/zoom_level); 
        tri_vertices[0].x = pin_locations[i].x;
        
        //right vertix
        tri_vertices[1].x = pin_locations[i].x - (TRIANGLE_SIDE/zoom_level);
        tri_vertices[1].y = pin_locations[i].y + (TRIANGLE_SIDE/zoom_level);

        //left vertix
        tri_vertices[2].x = pin_locations[i].x + (TRIANGLE_SIDE/zoom_level);
        tri_vertices[2].y = pin_locations[i].y + (TRIANGLE_SIDE/zoom_level);        
        
        fillpoly(tri_vertices,3);
    }
    
//---------------------------------------------------------------------    
   
    setlinewidth (highway_line_width);
    setcolor(BLUE);    
    int curvePoints;    
    //vector<unsigned> path =  find_path_between_intersections(1648, 23162);
    //vector<unsigned> path =  find_path_between_intersections(66103, 64524);
   
    /*
     this is to set the zoom
    StreetSegmentEnds last_segment_ends= getStreetSegmentEnds(path[path.size()-1]);    
    StreetSegmentEnds first_segment_ends = getStreetSegmentEnds(path[0]); 
    LatLon start_latlon = getIntersectionPosition(first_segment_ends.from);
    LatLon end_latlon = getIntersectionPosition(last_segment_ends.to);
    set_visible_world(start_latlon.lon, start_latlon.lat, end_latlon.lon, end_latlon.lat);
    */
    ////cout << final_path.size() << endl;
    
    // Drawing the input path------------------------------------------------------
    if(draw_on==false)return;
    t_point vertices[3], arrow_center;
    double length = zoom_level*ARROW_LEN*100;

    for(unsigned i = 0; i<final_path.size(); i++){
    //cout << "drawing the tour/path\n";        
        ends = getStreetSegmentEnds(final_path[i]);                      //cmath functions like cos(x) use radians
        curvePoints = getStreetSegmentCurvePointCount(final_path[i]);
        st_p = getIntersectionPosition(ends.from);       
        start_point.y = st_p.lat;
        start_point.x = st_p.lon;
        for(int j= 0; j<curvePoints; j++){
            en_p = getStreetSegmentCurvePoint(final_path[i],j);
            end_point.y = en_p.lat;
            end_point.x = en_p.lon;  
            drawline (start_point, end_point);
            flushinput();
            start_point = end_point;
        }
        en_p = getIntersectionPosition(ends.to);
        end_point.y = en_p.lat;
        end_point.x = en_p.lon;     
        drawline (start_point, end_point);

        //draw the arrow
        slope = (end_point.y - start_point.y) / (end_point.x - start_point.x);
        
        //arrow_center.x =  (end_point.x-start_point.x)/2;
        //arrow_center.y =  (end_point.y-start_point.y)/2;
        
/*        arrow_center.x =  (en_p.lon-st_p.lon)/2;
        arrow_center.y =  (en_p.lat-st_p.lat)/2;
        
        
        vertices[0].x = arrow_center.x + length*cos(atan(slope))/DEG_TO_RAD;
        vertices[0].y = arrow_center.y + length*sin(atan(slope))/DEG_TO_RAD;
        vertices[1].x = arrow_center.x;
        vertices[1].y = arrow_center.y - sqrt(2) * length;
        vertices[2].x = arrow_center.x - sqrt(2) * length;
        vertices[2].y = arrow_center.y;  
  */  
        ////cout << vertices[0].x << " " << vertices[0].y << endl;
        ////cout << vertices[1].x << " " << vertices[1].y << endl;
        ////cout << vertices[2].x << " " << vertices[2].y << endl;
        fillpoly(vertices, 3);
        setlinewidth(4);
        drawline(vertices[0],vertices[1]);
        flushinput();
    }
    if(final_path.size()>0){
            unsigned marker_radius;
            marker_radius = POI_RADIUS*sqrt(zoom_level);
            
            setfontsize(zoom_level);
            ends = getStreetSegmentEnds(final_path[0]);                      //cmath functions like cos(x) use radians
            st_p = getIntersectionPosition(ends.from); 
            drawtext (st_p.lon,st_p.lat+TEXT_OFFSET*2/(zoom_level),getIntersectionName(ends.from),FLT_MAX, FLT_MAX);

            ends = getStreetSegmentEnds(final_path[final_path.size()-1]);                      //cmath functions like cos(x) use radians
            en_p = getIntersectionPosition(ends.from); 

            drawtext (en_p.lon,en_p.lat+TEXT_OFFSET*2/(zoom_level),getIntersectionName(ends.from),FLT_MAX, FLT_MAX);

            // Labeling the start and end of the path----------------------------------------        
            fillarc (st_p.lon,st_p.lat ,marker_radius,0,360);
            fillarc (en_p.lon,en_p.lat ,marker_radius,0,360);
    }
    setfontsize(12);
    for(unsigned i=0;i<inters_visited.size();i++){
        LatLon location = getIntersectionPosition(inters_visited[i]);
        setcolor(CYAN);
        fillarc (location.lon,location.lat ,POI_RADIUS*zoom_level*2,0,360);
        drawtext (location.lon,location.lat+TEXT_OFFSET*4,to_string(i),FLT_MAX, FLT_MAX);
    }
    
    for(unsigned i=0;i<depots_visited.size();i++){
        LatLon location = getIntersectionPosition(depots_visited[i]);
        setcolor(BLACK);
        fillarc (location.lon,location.lat ,POI_RADIUS*zoom_level*2,0,360);
        drawtext (location.lon,location.lat+TEXT_OFFSET*4,to_string(i),FLT_MAX, FLT_MAX);
    }

    flushinput();           
}