#ifndef    _COMMON_DEFS_H
#define    _COMMON_DEFS_H

#include "iostream"
#include "string"
#include "vector"
#include "list"
#include "fstream"
#include "algorithm"
#include "queue"
#include "atomic"
#include "thread"
#include "mutex"
#include "shared_mutex"
#include "unordered_map"
#include "sstream"
#include "sys/stat.h"
#include "memory"

#define DISKSIZE 1000000 
typedef std::unordered_map<std::string, std::string> MemCache;
typedef std::list<std::string> CachedItemsList;
typedef unsigned int C_UINT;
typedef long int C_LINT;

#endif
