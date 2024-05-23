#include <ArduinoJson.hpp>
#include <foonathan_memory/foonathan/memory/container.hpp>
#include <foonathan_memory/foonathan/memory/memory_pool.hpp>
#include <foonathan_memory/foonathan/memory/std_allocator.hpp>
#include <map>
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "ArduinoJson/Document/JsonDocument.hpp"

namespace openscenario_interpreter
{
inline namespace utility
{

using Json = ArduinoJson::JsonObject;
using JsonArray = ArduinoJson::JsonArray;
}  // namespace utility
}  // namespace openscenario_interpreter