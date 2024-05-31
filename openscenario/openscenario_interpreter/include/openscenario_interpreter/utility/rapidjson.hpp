#pragma once

#include <rapidjson/document.h>

rapidjson::MemoryPoolAllocator<> & get_json_allocator();