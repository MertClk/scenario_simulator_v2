// Copyright 2015 TIER IV, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <openscenario_interpreter/reader/element.hpp>
#include <openscenario_interpreter/syntax/open_scenario.hpp>
#include <openscenario_interpreter/syntax/open_scenario_category.hpp>
#include <openscenario_interpreter/syntax/scenario_definition.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
OpenScenario::OpenScenario(const boost::filesystem::path & pathname)
: Scope(this),
  pathname(pathname),
  file_header(readElement<FileHeader>("FileHeader", load(pathname).child("OpenSCENARIO"), local())),
  category(readElement<OpenScenarioCategory>("OpenSCENARIO", script, local()))
{
}

auto OpenScenario::evaluate() -> Object
{
  ++frame;
  return category.evaluate();
}

auto OpenScenario::load(const boost::filesystem::path & filepath) -> const pugi::xml_node &
{
  if (const auto result = script.load_file(filepath.string().c_str()); not result) {
    throw SyntaxError(result.description(), ": ", filepath);
  } else {
    return script;
  }
}

auto operator<<(rapidjson::Value & json, const OpenScenario & datum) -> rapidjson::Value &
{
  json.AddMember("version", "1.0", get_json_allocator());

  json.AddMember("frame", datum.frame, get_json_allocator());

  auto states = rapidjson::Value(rapidjson::kObjectType);
  states.AddMember(
    "completeState", openscenario_interpreter::complete_state.use_count() - 1,
    get_json_allocator());
  states.AddMember(
    "runningState", openscenario_interpreter::running_state.use_count() - 1, get_json_allocator());
  states.AddMember(
    "standbyState", openscenario_interpreter::standby_state.use_count() - 1, get_json_allocator());
  states.AddMember(
    "startTransition", openscenario_interpreter::start_transition.use_count() - 1,
    get_json_allocator());
  states.AddMember(
    "stopTransition", openscenario_interpreter::stop_transition.use_count() - 1,
    get_json_allocator());

  json.AddMember("currentStates", states, get_json_allocator());

  if (datum.category.is<ScenarioDefinition>()) {
    json.AddMember("OpenSCENARIO", rapidjson::Value(rapidjson::kObjectType), get_json_allocator());
    json["OpenSCENARIO"] << datum.category.as<ScenarioDefinition>();
  }

  return json;
}
}  // namespace syntax
}  // namespace openscenario_interpreter
