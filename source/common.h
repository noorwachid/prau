#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

// bleed the standard library
// because this is final project

using std::cout;
using std::ifstream;
using std::ofstream;
using std::string;
using std::system;
using std::to_string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::make_unique;
using std::unique_ptr;

namespace fs = std::filesystem;
namespace chrono = std::chrono;
