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
#include <openscenario_interpreter/syntax/act.hpp>
#include <openscenario_interpreter/syntax/custom_command_action.hpp>
#include <openscenario_interpreter/syntax/maneuver_group.hpp>

#include "ArduinoJson/Variant/VariantTo.hpp"

namespace openscenario_interpreter
{
inline namespace syntax
{
Act::Act(const pugi::xml_node & node, Scope & scope)
: Scope(readAttribute<String>("name", node, scope), scope),
  StoryboardElement(
    readElement<Trigger>("StartTrigger", node, local()),
    readElement<Trigger>("StopTrigger", node, local()))  // NOTE: Optional element
{
  traverse<1, unbounded>(node, "ManeuverGroup", [&](auto && node) {
    return elements.push_back(readStoryboardElement<ManeuverGroup>(node, local()));
  });
}

auto Act::run() -> void
{
  std::size_t index{0};
  for (auto && maneuver_group : elements) {
    try {
      assert(maneuver_group.is_also<ManeuverGroup>());
      maneuver_group.evaluate();
      ++index;
    } catch (const SpecialAction<EXIT_FAILURE> & action) {
      throw SpecialAction<EXIT_FAILURE>(name, "ManeuverGroup", index, action);
    }
  }
}

auto operator<<(JsonObject json, const Act & datum) -> JsonObject
{
  json["name"] = datum.name.c_str();

  json["currentState"] = boost::lexical_cast<std::string>(datum.state());

  auto maneuverGroup = json["ManeuverGroup"].to<JsonArray>();
  for (auto && maneuver_group : datum.elements) {
    maneuverGroup.add<JsonObject>() << maneuver_group.as<ManeuverGroup>();
  }

  return json;
}
}  // namespace syntax
}  // namespace openscenario_interpreter
