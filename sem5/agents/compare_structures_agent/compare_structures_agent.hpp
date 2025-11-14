#pragma once

#include <sc-memory/sc_agent.hpp>

namespace dm
{
class CompareStructuresAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  bool AreVertexesEqual(ScAddr vertex1, ScAddr vertex2);

  bool AreArcsEqual(ScAddr arc1, ScAddr arc2);

  ScResult DoProgram(ScAction & action) override;
};
}  // namespace dm
