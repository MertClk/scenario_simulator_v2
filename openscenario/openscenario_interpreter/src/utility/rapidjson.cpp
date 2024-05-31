#include <openscenario_interpreter/utility/rapidjson.hpp>

rapidjson::MemoryPoolAllocator<> json_allocator;

rapidjson::MemoryPoolAllocator<> & get_json_allocator() { return json_allocator; }
