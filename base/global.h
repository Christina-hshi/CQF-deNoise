/*
 * ============================================================================
 *  
 *        Authors: Christina SHI <hshi@cse.cuhk.edu.hk>
 *                 Kevin Yip <kevinyip@cse.cuhk.edu.hk>
 *
 * ============================================================================
 */

#pragma once

#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<map>
#include<unordered_map>
#include<set>
#include<list>
#include<stack>
#include<fstream>
#include<sstream>
#include<stdexcept>
#include<ctime>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<thread>
#include<chrono>
#include<mutex>
#include<condition_variable>
#include<algorithm>
#include<limits>
#include<numeric>
#include<iomanip>
//#include<malloc.h>
//#include<malloc/malloc.h>
//#include<stdlib.h>
#include<queue>
#include<atomic>
#include<assert.h>
#include<sys/resource.h>
#include<sys/time.h>

#include<boost/config.hpp>
#include<boost/tokenizer.hpp>
#include<boost/bimap.hpp>
#include<boost/program_options.hpp> 
//#include "Config.h"

//modules used as default in this project
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::istringstream;
using std::ostringstream;
using std::stringstream;
using std::vector;
using std::map;
using std::unordered_map;
using std::set;
using std::list;
using std::pair;
using std::bitset;
using std::array;
using std::stack;
using std::to_string;

using std::streampos;

typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

const std::string currentDateTime();
