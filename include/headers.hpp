#ifndef HEADER_HEADERS_HPP
#define HEADER_HEADERS_HPP

#ifdef _WIN32
// windows version.
#include <io.h>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else // _WIN32
// linux version.
#include <unistd.h>
#include <sys/file.h>
#endif // _WIN32
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

// C++17
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <variant>
#include <optional>
#include <fstream>

#include "define.h"
#include "exception.hpp"
#include "format.hpp"
#include "logger.hpp"
#include "insert_order_map.hpp"
#include "line_reader.hpp"
#include "ini_parser.hpp"

#endif // HEADER_HEADERS_HPP
