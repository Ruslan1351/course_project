#pragma once

#include <sc-memory/sc_agent.hpp>

namespace dm
{
class ScAgentGenerateRandomGraph : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScAction & action) override;

  std::pair<std::vector<int>, std::vector<std::pair<int, int>>> generateRandomGraph(int nodeCount, int edgeCount);

};
}
