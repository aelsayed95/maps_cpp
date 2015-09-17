#pragma once

#include <cfloat> // for FLT_MAX
#include <cstdlib>
#include "code/easygl_constants.h"
#include "code/graphics.h"
#include "m1.h"
#include <fstream>


#define LEFT_MOUSE_BUTTON 1
#define RIGHT_MOUSE_BUTTON 3

#define BIG_STREET_SIZE 500
#define MEDIUM_STREET_SIZE 200
#define SMALL_STREET_SIZE 50
#define HIGHWAY_SPEED_LIMIT 90

#define ZOOM_LEVEL1  0.45
#define ZOOM_LEVEL2  0.20
#define ZOOM_LEVEL3  0.09
#define ZOOM_LEVEL4  0.04
#define ZOOM_LEVEL5  0.018
#define ZOOM_LEVEL6  0.008

#define TRIANGLE_SIDE 0.005
#define POI_RADIUS 0.0001

#define POI_ZOOM_LEVEL 4
#define STREET_NAMES_ZOOM_LEVEL 10
#define WATERWAYS_ZOOM_LEVEL 2

#define GRID_WIDTH 12
#define GRID_HEIGHT 8
#define MAX_POI_PER_BOX 4

#define TEXT_OFFSET 0.0005

#define ARROW_LEN 0.01

void draw_map(std::string);
void draw_screen();
unsigned load_pins();
void act_on_button_press (float x, float y, t_event_buttonPressed event);
unsigned clicked_on_POI(double x, double y);
void draw_path(unsigned inter1, unsigned inter2, string map_path, bool );
void draw_poi_path(unsigned inter_id_start, string poi_name, string map_path, bool find_path_once);
void draw_tour(vector<unsigned>tour, vector<unsigned>inters, vector<unsigned> depots, string map_path);