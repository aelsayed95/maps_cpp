#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip>

#include <readline/readline.h>
#include <readline/history.h>

#include "m3.h"

using namespace std;

void start_AHA();

char** city_completion(const char* stem_text, int start, int end);
char* city_name_generator(const char* stem_text, int state);

char** intersection_completion(const char* stem_text, int start, int end);
char* intersection_name_generator(const char* stem_text, int state);

char** poi_completion(const char* stem_text, int start, int end);
char* poi_name_generator(const char* stem_text, int state);

char* lower_case_cstring(char* input);

string lower_case_string(string input);

void directions_input(stringstream & input_stream);

void help_input(stringstream & input_stream);

string extract_word_string(string input);
char* extract_word(char* input);

void print_directions(vector<unsigned>& path, string input);