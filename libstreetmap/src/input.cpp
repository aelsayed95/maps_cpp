#include "input.h"

//An example list of intersections
vector<const char*> city_names = {
    "Cairo","Toronto","Hamilton","New York", "Saint Helena", "St. Helena", "Moscow"
};

vector<const char*> command_names = {
    "directions","help", "find", "load"
};

char travel_direction[5] = "NESW";
//strcpy(travel_direction, );

typedef std::unordered_map<string,unsigned> Mymap;
extern Mymap intersections;    //used to intersections names to intersection IDs
extern Mymap intersections_lowercase;    //used to intersections names to intersection IDs
extern vector <string> intersection_names;
extern vector <string> poi_names;
extern std::unordered_map<string, string>poi_lower_to_upper_names;
vector<unsigned>path;
//extern vector<const char*>intersection_name_in_order;
//extern vector<const char*> intersection_names;
extern unsigned intersections_count;
extern unsigned streets_count;
extern unsigned poi_count;
extern unsigned street_segment_count;
extern unsigned lat_range, lon_range;
string map_path;
unsigned inter_id[2], inter_id_start;
unsigned inter_count;
string poi_name, input;

bool draw_on = false;

string convert_cstring_to_string(char* buf){
    unsigned text_len = strlen(buf);
    string ret = "";
    for(unsigned i=0;i<text_len;i++)
        ret+= buf[i];
    return ret;
        
}

char* lower_case_cstring(char* input){
    unsigned text_len = strlen(input);
    for(unsigned i=0;i<text_len;i++){
        input[i] = tolower(input[i]);
    }
    return input;
}

string lower_case_string(string input){
    unsigned text_len = input.size();
    for(unsigned i=0;i<text_len;i++){
        input[i] = tolower(input[i]);
    }
    return input;
}

char* extract_word_one_word(char* input){
    unsigned text_len = strlen(input);
    unsigned i, j;
    //char output[text_len];
    for(i=0, j=0;i<text_len;i++){
        if(input[i]!='\"'){
            input[j] = input[i];
            j++;
        }
    }
    if(j>1 && input[j-1]==' ')input[j-1] = '\0';
    input[j] = '\0';
    return input;
}

char* extract_word(char* input){
    unsigned text_len = strlen(input);
    unsigned i, j;
    //char output[text_len];
    for(i=0, j=0;i<text_len;i++){
        if(input[i]!='\"'){
            input[j] = input[i];
            j++;
        }
    }
    input[j] = '\0';
    if(j>1 && input[j-1]==' ')input[j-1] = '\0';
    return input;
}

string extract_word_string(string input){
    unsigned text_len = input.size();
    string output = "";
    unsigned i;
    //char input[text_len];
    for(i=0;i<text_len;i++){
        if(input[i]!='\"'){
            output+= input[i];
        }
    }
    if(output.size() > 1 && output[output.size()-1]==' ')
        return output.substr(0,output.size()-1);
    return output;
}


void help_input()
{
    cout << "Here's a list of all the commands:\nFor autocompletion, enter commands in quotes \" \".\n\n";
    cout << "Load: load in a new map of your choice.\nFormat:\"load\" [Enter]\n";
    cout << "\"cityName\" [Enter]" << endl;
    cout << "available city names are: Toronto, St. Helena, New York, Cairo, Moscow, and Hamilton\n"<<endl;
    
    cout << "Directions: finds directions between two intersections.\nFormat:\"directions\" [Enter]\n";
    cout << "\"startStreetName1 & startStreetName2\" [Enter]" << endl;
    cout << "\"endStreetName1 & endStreetName2\" [Enter]" << endl<<endl;
 
    cout << "Find: finds directions between an intersection and point of interest.\nFormat:\"find\" [Enter]\n";
    cout << "\"startStreetName1 & startStreetName2\" [Enter]" << endl;
    cout << "\"Point of Interest Name\" [Enter]" << endl<<endl;
    
    cout << "When you wish to close AHA!, simply type exit or quit.\n";
    return;
}

char** intersection_completion(const char* stem_text, int start, int end) {
    char ** matches = NULL;

    if(start != 0) {
        //Only generate completions if stem_text' 
        //is not the first thing in the buffer
        int text_len = strlen(stem_text);
        char stem_text_modified[text_len];
        int i, j;
        for(i=0,j=0;i<text_len;i++){
            //if(isalpha(stem_text[i]) || isdigit(stem_text[i]) || stem_text[i] == '&' || stem_text[i] == '.'  || stem_text[i]==' '){
            if(stem_text[i]!='\"'){    
                cout << "" ;            // DOES MAGIC!!
                stem_text_modified[j]=tolower(stem_text[i]);
                j++;
            }
        }
        stem_text_modified[j] = '\0';
        //cout << "stem text mod = " << stem_text_modified << endl;
        matches = rl_completion_matches(stem_text_modified, intersection_name_generator);
    }
    return matches;
}

char* intersection_name_generator(const char* stem_text, int state) {
    //Static here means a variable's value persists across function invocations
    static int count;

    if(state == 0) { 
        //We initialize the count the first time we are called
        //with this stem_text
        count = -1;
    }

    int text_len = strlen(stem_text);
    //Search through city name until we find a match
    while(count < (int) intersection_names.size()-1) {
        count++;
        ///cout << "herer-------------------\n";
        unsigned intersection_name_len = intersection_names[count].size();
        char lower_intersection_name[intersection_name_len+1];
        strcpy(lower_intersection_name, lower_case_string(extract_word_string(intersection_names[count])).c_str());
        //cout << "after convering to lower------------------\n";        
        /*
        for(unsigned i=0;i<intersection_name_len;i++){
            lower_intersection_name[i] = tolower(intersection_names[count][i]);
        }*/
       
        //cout << "herer-------------------\n";
        //cout << lower_intersection_name << " " << stem_text << endl;
        if(strncmp(lower_intersection_name, stem_text, text_len) == 0) {
            //Must return a duplicate, Readline will handle
            //freeing this string itself.
            //cout << "saving " << intersection_names[count] << " at inter_id[" << inter_count << "]\n";
            //inter_id[inter_count] = intersections[intersection_names[count]];
            //inter_id_start = intersections[intersection_names[count]];
            return strdup(intersection_names[count].c_str());
        }
    }

    //No more matches
    return NULL;
}

char** poi_completion(const char* stem_text, int start, int end) {
    char ** matches = NULL;

    if(start != 0) {
        //Only generate completions if stem_text' 
        //is not the first thing in the buffer
        int text_len = strlen(stem_text);
        char stem_text_modified[text_len];
        int i, j;
        for(i=0,j=0;i<text_len;i++){
            //if(isalpha(stem_text[i]) || isdigit(stem_text[i]) || stem_text[i] == '&' || stem_text[i] == '.'  || stem_text[i]==' '){
            if(stem_text[i]!='\"'){    
                cout << "" ;            // DOES MAGIC!!
                stem_text_modified[j]=tolower(stem_text[i]);
                j++;
            }
        }
        stem_text_modified[j] = '\0';
        //cout << "stem text mod = " << stem_text_modified << endl;
        matches = rl_completion_matches(stem_text_modified, poi_name_generator);
    }
    return matches;
}

char* poi_name_generator(const char* stem_text, int state) {
    //Static here means a variable's value persists across function invocations
    static int count;

    if(state == 0) { 
        //We initialize the count the first time we are called
        //with this stem_text
        count = -1;
    }

    int text_len = strlen(stem_text);
    //Search through city name until we find a match
    while(count < (int) poi_names.size()-1) {
        count++;
        ///cout << "herer-------------------\n";
        unsigned intersection_name_len = poi_names[count].size();
        char lower_intersection_name[intersection_name_len+1];
        strcpy(lower_intersection_name, lower_case_string(extract_word_string(poi_names[count])).c_str());
        //cout << "after convering to lower------------------\n";        
        /*
        for(unsigned i=0;i<intersection_name_len;i++){
            lower_intersection_name[i] = tolower(intersection_names[count][i]);
        }*/
       
        //cout << "herer-------------------\n";
        //cout << lower_intersection_name << " " << stem_text << endl;
        if(strncmp(lower_intersection_name, stem_text, text_len) == 0) {
            //Must return a duplicate, Readline will handle
            //freeing this string itself.
            //inter_id[inter_count] = intersections[poi_names[count]];
            //poi_name = poi_names[count];
            return strdup(poi_names[count].c_str());
        }
    }

    //No more matches
    return NULL;
}

/*

char** intersection_completion(const char* stem_text, int start, int end) {
    char ** matches = NULL;

    if(start != 0) {
        //cout << "start!=0\n";
        //Only generate completions if stem_text' 
        //is not the first thing in the buffer
        int text_len = strlen(stem_text);
        char stem_text_modified[text_len];
        int i, j; 
        string stem_text_in_char_star;
        //strcpy(stem_text_modified,);
        for(i=0,j=0;i<text_len;i++){
            if(isalpha(stem_text[i]) || stem_text[i]==' ' || stem_text[i] == '.'){
                cout << "" ;            // DOES MAGIC!!
                stem_text_modified[j]=tolower(stem_text[i]);
                j++;
            }
        }
        stem_text_modified[j] = '\0';
        //puts the stem text that user entered, in order
        
        stringstream ss(stem_text_modified);
        vector<string> stem_text_sorted{istream_iterator<string>{ss}, istream_iterator<string>{}};
        sort(stem_text_sorted.begin(), stem_text_sorted.end(),compare_in_lower);
        for(unsigned i = 0; i<stem_text_sorted.size();i++){
            stem_text_in_char_star +=  stem_text_sorted[i];
        }
        
        //cout << "finding matches...\n";
        matches = rl_completion_matches(stem_text_in_char_star.c_str(), intersection_name_generator);
    }

    return matches;
}
*/


/*
char* intersection_name_generator(const char* stem_text, int state) {
    //Static here means a variable's value persists across function invocations
   static int count;

    if(state == 0) { 
        //We initialize the count the first time we are called
        //with this stem_text
        count = -1;
    }
   //cout << stem_text << endl;
    int text_len = strlen(stem_text);
    //Search through city name until we find a match
    while(count <(int) intersection_names.size()-1) {
         count++;
        //cout << intersection_names.size() << endl;

       // unsigned intersection_name_len = intersection_names[count].size();//
        //strcpy(lower_city_name, lower_case_cstring(city_names[count]));
        
        //char lower_intersection_name[intersection_names[count].size()];
        //string lower_intersection_name;
        //string cc = lower_case_string(intersection_names[count]);
        ////lower_intersection_name = cc;
        
        //strcpy(lower_intersection_name, cc.c_str()));
        
        if(strncmp(intersection_names[count].c_str(), stem_text, text_len) == 0) {
            //Must return a duplicate, Readline will handle
            //freeing this string itself.
            //returns the correct intersection name in the right order
           // cout << "the right order\n";
            //cout << intersection_name_in_order[count] << endl;
            return strdup(intersection_name_in_order[count]);
        }        
    }

    //No more matches
    return NULL;
}*/


//Called repeatedly for a given 'stem_text'. Each time it returns a potential
//match.  When there are no more matches it returns NULL.
//
//The 'state' variable is zero the first time it is called with a given
//'stem_text', and positive afterwards.
char* city_name_generator(const char* stem_text, int state) {
    //Static here means a variable's value persists across function invocations
    static int count;

    if(state == 0) { 
        //We initialize the count the first time we are called
        //with this stem_text
        count = -1;
    }

    int text_len = strlen(stem_text);
    //Search through city name until we find a match
    while(count < (int) city_names.size()-1) {
        count++;
        unsigned city_name_len = strlen(city_names[count]);
        char lower_city_name[city_name_len];
        //strcpy(lower_city_name, lower_case_cstring(city_names[count]));
        for(unsigned i=0;i<city_name_len;i++){
            lower_city_name[i] = tolower(city_names[count][i]);
        }
        if(strncmp(lower_city_name, stem_text, text_len) == 0) {
            //Must return a duplicate, Readline will handle
            //freeing this string itself.
            return strdup(city_names[count]);
        }
    }

    //No more matches
    return NULL;
}

//Called repeatedly for a given 'stem_text'. Each time it returns a potential
//match.  When there are no more matches it returns NULL.
//
//The 'state' variable is zero the first time it is called with a given
//'stem_text', and positive afterwards.
char* command_name_generator(const char* stem_text, int state) {
    //Static here means a variable's value persists across function invocations
    static int count;

    if(state == 0) { 
        //We initialize the count the first time we are called
        //with this stem_text
        count = -1;
    }

    int text_len = strlen(stem_text);
    //Search through city name until we find a match
    while(count < (int) command_names.size()-1) {
        count++;
        unsigned command_len = strlen(command_names[count]);
        char lower_command[command_len];
        //strcpy(lower_command,lower_case_cstring(command_names[count]));
        for(unsigned i=0;i<command_len;i++){
            lower_command[i] = tolower(command_names[count][i]);
        }
        if(strncmp(lower_command, stem_text, text_len) == 0) {
            //Must return a duplicate, Readline will handle
            //freeing this string itself.
            return strdup(command_names[count]);
        }
    }

    //No more matches
    return NULL;
}

//Given the stem 'stem_text' perform auto completion. 
//It returns an array of strings that are potential completions
//
//Note:
// 'start' and 'end' denote the location of 'stem_text' in the global
// 'rl_line_buffer' variable which contains the users current input line.
// If you need more context information to determine how to complete 'stem_text'
// you could look at 'rl_line_buffer'.
char** city_completion(const char* stem_text, int start, int end) {
    char ** matches = NULL;

    if(start != 0) {
        //Only generate completions if stem_text' 
        //is not the first thing in the buffer
        int text_len = strlen(stem_text);
        char stem_text_modified[text_len];
        int i,j;
        for(i=0,j=0;i<text_len;i++){
            if(isalpha(stem_text[i]) || stem_text[i]==' '){
                cout << "" ;            // DOES MAGIC!!
                stem_text_modified[j]=tolower(stem_text[i]);
                j++;
            }
        }
        stem_text_modified[j] = '\0';
        matches = rl_completion_matches(stem_text_modified, city_name_generator);
    }
    return matches;
}

char** command_completion(const char* stem_text, int start, int end) {
    char ** matches = NULL;

    if(start != 0) {
        //Only generate completions if stem_text' 
        //is not the first thing in the buffer
        int text_len = strlen(stem_text);
        char stem_text_modified[text_len];
        int i, j;
        for(i=0,j=0;i<text_len;i++){
            if(isalpha(stem_text[i]) || stem_text[i]==' '){
                cout << "" ;            // DOES MAGIC!!
                stem_text_modified[j]=tolower(stem_text[i]);
                j++;
            }
        }
        stem_text_modified[j] = '\0';
        matches = rl_completion_matches(stem_text_modified, command_name_generator);
    }
    return matches;
}

bool loadmap(char* input_city_name){
    //"Cairo","Toronto","Hamilton","New York", "Saint Helena", "St. Helena"dg
    bool valid = true;
    draw_on = false;
    input_city_name = extract_word_one_word(input_city_name);
    if(strcmp(input_city_name,"Toronto")==0){
        map_path = "/cad2/ece297s/public/maps/toronto.bin";

    }
    else if(strcmp(input_city_name,"Cairo")==0){
        map_path = "/cad2/ece297s/public/maps/cairo_egypt.bin";
    }
    else if(strcmp(input_city_name,"Hamilton")==0){
        map_path = "/cad2/ece297s/public/maps/hamilton_canada.bin";
    }
    else if (strcmp(input_city_name,"New York")==0){
        map_path = "/cad2/ece297s/public/maps/newyork.bin";
    }
    else if (strcmp(input_city_name,"Moscow")==0){
        map_path = "/cad2/ece297s/public/maps/moscow.bin";
    }
    else if ((strcmp(input_city_name,"St. Helena")==0)||strcmp(input_city_name,"Saint Helena")==0){
        map_path = "/cad2/ece297s/public/maps/saint_helena.bin";
    }
    else 
        valid = false;
    if(valid){
        load_map(map_path);
        draw_map(map_path);
    }
    return valid;
}

//bool check_command(char* command){
//    command = lower_case_cstring(command);
//    command = extract_word_one_word(command);   
//    return ((strcmp(command,"directions ")==0) ||
//            (strcmp(command,"help ")==0) ||
//            (strcmp(command,"find ")==0)||
//            (strcmp(command,"\"load\" ")==0)||
//            (strcmp(command,"load " )==0));
//}

bool check_POI(char* poi){
    //return true;
    bool valid = false;
    poi = lower_case_cstring(poi);
    poi = extract_word(poi);
    for(unsigned i = 0; i < poi_count; i++)
    {
        valid = (strcmp(poi,lower_case_string(extract_word_string(poi_names[i])).c_str())==0);
        if(valid) break;
    }//---------------------------------------------needs to be completed----------------------------#
    return valid;
}

bool check_intersection(char* intersection){
    //return true;
    bool valid = false;
    intersection = lower_case_cstring(intersection);
    intersection = extract_word(intersection);
    for(unsigned i = 0; i < intersections_count; i++)
    {
        valid = (strcmp(intersection,lower_case_string(extract_word_string(intersection_names[i])).c_str())==0);
        if(valid) break;
    }//---------------------------------------------needs to be completed----------------------------#
    return valid;
}



char find_direction(StreetSegmentEnds sEnds, bool dir){
    LatLon start, end;
    double angle;
    
    //dir = true if the .from of the next segment = the .to of the current segment
    if(dir==true){
        start = getIntersectionPosition(sEnds.from);
        end = getIntersectionPosition(sEnds.to);
    }
    else{
        start = getIntersectionPosition(sEnds.to);
        end = getIntersectionPosition(sEnds.from);        
    }
    
    angle = atan((end.lat-start.lat)/(end.lon-start.lon))/DEG_TO_RAD;
    //cout << "angle in degrees = " << angle << endl;
    if(dir){
        if(angle <= -45){
            return 'S';
        }
        else if(angle <= 45)
            return 'E';
        else// if(angle <=135)
            return 'N';
    }
    else{
        if(angle <= -45){
            return 'N';
        }
        else if(angle <= 45)
            return 'W';
        else// if(angle <=135)
            return 'S';        
    }
    
}

string find_turn_word (char prev_direction, char cur_direction)
{
	int i = 0;
    string turn_word = "";
    for (i=0; i<4; i++)
    {
        if (prev_direction == travel_direction[i])
            break;
    }

    if (cur_direction == travel_direction[(i+1)%4])
        turn_word = "right";
    else if (cur_direction == travel_direction[(i-1+4)%4])
        turn_word = "left";

    return turn_word;
}

void print_directions(vector<unsigned>& path, string input)
{
    
    ofstream fout;
    fout.open("directions.txt");
    
    if (!fout.is_open())
        cout << "Error! The file hasn't opened!!\n";

    string start_inter_name = getIntersectionName(inter_id[0]);
    string end_inter_name;// = getIntersectionName(inter_id[1]);

    fout << "Driving directions from:\n";
    fout << "Start Intersection Name: " << start_inter_name << endl;

    if (input == "directions")
    {
        start_inter_name = getIntersectionName(inter_id[0]);
        end_inter_name = getIntersectionName(inter_id[1]);
        fout << "End Intersection Name: " << end_inter_name << endl << endl << endl;
    }

    else if (input == "find"){
        fout << "To the Point of Intersection: " << poi_name << endl;//" at  the nearest intersection " << findIntersectionName(best_inter_id) << endl << endl << endl;
        start_inter_name = getIntersectionName(inter_id_start);
        //end_inter_name = find_closest_intersection()
    }

    double path_time = compute_path_travel_time(path);
    fout << "The total time to reach the destination is " << ceil(path_time) << " minutes. "<< endl;

    unsigned cur_street_seg, prev_street_seg, cur_street_id, prev_street_id ;
    string prev_street_name, cur_street_name;
    double prev_seg_time, cur_seg_time, prev_seg_length, street_time, cur_seg_length;
    double street_distance;
    char prev_direction, cur_direction;
    string direction_word = "";  

    prev_street_seg = path[0];
    prev_street_id = getStreetSegmentStreetID(prev_street_seg);
    prev_street_name = getStreetName(prev_street_id);
    prev_seg_time = find_segment_travel_time(prev_street_seg);
    prev_seg_length = find_street_segment_length(prev_street_seg);
    street_time = prev_seg_time;
    street_distance = prev_seg_length;
    
    StreetSegmentEnds sEnds_prev, sEnds_cur;

    char direction;// = find_direction(sEnds_prev,true);
    bool direc;

    fout << "Start from " << start_inter_name << endl;
    
    for (unsigned i=1; i< path.size(); i++)
    {
        cur_street_seg = path[i];
        cur_street_id = getStreetSegmentStreetID(cur_street_seg);
        cur_seg_time = find_segment_travel_time(cur_street_seg);
        cur_street_name = getStreetName(cur_street_id);
        cur_seg_length = find_street_segment_length(cur_street_seg);
        
        sEnds_prev = getStreetSegmentEnds(prev_street_seg);	
		
        if (i==1)
            fout << "Head on " << prev_street_name << endl;//" towards " << cur_street_name << ".\n";

        if (cur_street_id != prev_street_id)
        {            
            sEnds_cur = getStreetSegmentEnds(cur_street_seg);
            bool oneWay = getStreetSegmentOneWay(cur_street_seg);
            if((sEnds_prev.to==sEnds_cur.from) || 
               (!oneWay && (sEnds_prev.to==sEnds_cur.to)))
                direc = true;
            else direc = false;
            //cout << "direc = " << direc << endl;

            cur_direction = find_direction(sEnds_prev,direc);	

            if (i ==1){
                    prev_direction = cur_direction;
                    fout << " in the " << cur_direction << " direction." << endl<< endl;
            }

            direction_word = find_turn_word(prev_direction, cur_direction);

            fout << "Travel along " << prev_street_name<<  " for " << setprecision(2) << 
            street_distance/1000 << " km. It will take nearly " << ceil(street_time) << " minutes\n";
            fout << "Turn " << direction_word << " on " << cur_street_name << endl << endl; 
            prev_street_id = cur_street_id;
            prev_street_name = cur_street_name;
            street_time = cur_seg_time;
            street_distance = cur_seg_length;
			
            //cout << cur_direction << endl;
            
        }
        else if(i==path.size()-1){
            fout << "Travel on " << prev_street_name << " for " << setprecision(2) << 
                    street_distance/1000 << " km. It will take nearly " << ceil(street_time) << " minutes\n";
            prev_street_id = cur_street_id;
            prev_street_name = cur_street_name;
            street_time = cur_seg_time;
            street_distance = cur_seg_length;
            
            sEnds_cur = getStreetSegmentEnds(cur_street_seg);
            if(sEnds_prev.to==sEnds_cur.from)direc = true;
            else direc = false;
            direction = find_direction(sEnds_prev,direc);
            //cout << direction << endl;            
        }
        else
        {
            prev_street_seg = cur_street_seg;
            street_time += cur_seg_time;
            street_distance += cur_seg_length;
        }
    }
    
    fout << "Arrive at the destination " << end_inter_name << endl;
    fout.close();
}

void start_AHA(){
    
    //Use tab for auto completion
    rl_bind_key('\t', rl_complete);
    //Use our function for auto-complete
    rl_attempted_completion_function = command_completion;
    //Tell readline to handle double and single quotes for us
    rl_completer_quote_characters = strdup("\"\'"); 

    cout << "Welcome to AHA! :) \nPlease enter load to load the map you wish to see first\n"; //----------------------------------6723452378459723542
    //cout << (extract_word_string(lower_case_string("8974NaMEE,,vd oet 4 9t.... ??$#534"))).c_str() << endl;
    
    char* buf; //Buffer of line entered by user
    char* buf2; //
    bool valid_city = false, first = true, valid_intersection;
        
    while((buf = readline("> ")) != NULL) { //Prompt the user for input
        if(strcmp(buf, "") != 0)
            add_history(buf);
        
        if(strcmp(buf, "exit") == 0 || strcmp(buf, "quit") == 0)
            return;
        buf = lower_case_cstring(extract_word_one_word(buf));
        //cout << "--"  << buf << "--" << endl;
        
        if(strcmp(buf,"load")==0){
            cout<<"Enter the name of the city: " <<endl;
            close_map();
            rl_attempted_completion_function = city_completion;
             while((buf2 = readline("> ")) != NULL) { //Prompt the user for input
                if(strcmp(buf2, "") != 0)
                    add_history(buf2); 

                if(strcmp(buf2, "exit") == 0 || strcmp(buf2, "quit") == 0)
                    return;
                
                //buf = lower_case_cstring(extract_word_one_word(buf));
                
                valid_city = loadmap(buf2);
                
                if(!valid_city)
                    cout<<"City name is not found in our database\n";
                else{ // found the city. GET THE HELL OUT!!
                    first = false;
                    //populate_street_names();
                    break;
                }
            }

        }
        else if(first==true){
            cout << "Please load a map first. Type \"help\" for more intstructions.\n\n";
        }
        else if (strcmp(buf,"help")==0 || strcmp(buf,"help")==0)
        {
            help_input();
        }
        else if (!first && (strcmp(buf, "\"directions\" ")==0 || strcmp(buf, "directions")==0)) {
            input = "directions";
            rl_attempted_completion_function = intersection_completion;
            cout << "Please enter the first intersection\n";
            //inter_count = 0;
            //cout << "intercout = " << inter_count << endl;
            while((buf = readline("> ")) != NULL) { //Prompt the user for input
                //cout << "heree---------------------\n";
                if(strcmp(buf, "") != 0)
                    add_history(buf); 
                if(strcmp(buf, "exit") == 0 || strcmp(buf, "quit") == 0)
                    return;
                valid_intersection = check_intersection(buf);
                if(!valid_intersection){
                    cout << "The intersection is not found in our database. Enter the intersection again\n";
                }
                else{
                    cout << "Found the intersection\n";
                    //inter_count++;
                    break;
                }
            }
            //cout << "intercout = " << inter_count << endl;
            /*
            rl_attempted_completion_function = poi_completion;
            while((buf = readline("> ")) != NULL) { //Prompt the user for input
                valid_poi = check_poi(buf);
                if(!valid_poi) cout << "The intersection is not found in our database. Enter the intersection again\n";
                else break;
            }*/
            
            cout << "Please enter the second intersection\n";
            valid_intersection = false;
            while((buf2 = readline("> ")) != NULL) { //Prompt the user for input
                if(strcmp(buf2, "") != 0)
                    add_history(buf2); 
                if(strcmp(buf2, "exit") == 0 || strcmp(buf2, "quit") == 0)
                    return;
                valid_intersection = check_intersection(buf2);
                if(!valid_intersection) cout << "The intersection is not found in our database. Enter the intersection again\n";
                else{
                    cout << "Found the intersection\n";
                    break;
                }
            }
           // cout << "intercout = " << inter_count << endl;

                inter_id[0] = intersections_lowercase[string(buf)];
                inter_id[1] = intersections_lowercase[string(buf2)];
			//cout << inter_id[0] << endl;
            //cout << inter_id[1] << endl;
            draw_path(inter_id[0], inter_id[1], map_path, true);
            
        }
        else if (!first && (strcmp(buf, "\"find\" ")==0 || strcmp(buf, "find")==0)) {
            input = "find";
            rl_attempted_completion_function = intersection_completion;
            cout << "Please enter the starting intersection\n";
            while((buf = readline("> ")) != NULL) { //Prompt the user for input
                //cout << "heree---------------------\n";
                if(strcmp(buf, "") != 0)
                    add_history(buf); 
                if(strcmp(buf, "exit") == 0 || strcmp(buf, "quit") == 0)
                    return;
                valid_intersection = check_intersection(buf);
                if(!valid_intersection){
                    cout << "The intersection is not found in our database. Enter the intersection again\n";
                }
                else{
                    cout << "Found the intersection\n";
                    break;
                }
            }
            
            rl_attempted_completion_function = poi_completion;
            cout << "Please enter the name of the Point of Interest that you wish to go to\n";
            bool valid_poi = false;
            while((buf2 = readline("> ")) != NULL) { //Prompt the user for input
                if(strcmp(buf2, "") != 0)
                    add_history(buf2); 
                if(strcmp(buf2, "exit") == 0 || strcmp(buf2, "quit") == 0)
                    return;
                valid_poi = check_POI(buf2);
                if(!valid_poi) cout << "The Point of Interest is not found in our database. Enter the intersection again\n";
                else{
                    cout << "Found the POI\n";
                    break;
                }
            }           
            inter_id_start = intersections_lowercase[string(buf)];
            poi_name = poi_lower_to_upper_names[string(buf2)];
            draw_poi_path(inter_id_start, poi_name, map_path, true); 
        }
//        
        else{
            cout<<"This command doesn't exist in our database. Please enter help to get a list of all possible commands\n";
        }
        rl_attempted_completion_function = command_completion;
    }
    return; //end of start_AHA 
}

