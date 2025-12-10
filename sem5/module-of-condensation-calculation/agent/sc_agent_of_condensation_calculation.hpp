#pragma once

#include <sc-memory/sc_agent.hpp>

namespace dm
{
class ScAgentOfCondensationCalculation : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScAction & action) override;

  void ScAddrToVector(const ScAddr &addr,
             std::vector<std::pair<ScAddr,
             std::vector<ScAddr>>> &graph);

  void dfs1(int u, const std::vector<std::vector<int>>& adj, std::vector<bool>& visited, std::stack<int>& st);
  void dfs2(int u, const std::vector<std::vector<int>>& rev_adj, std::vector<bool>& visited, std::vector<int>& component);
  int find_condensation_graph(
      const std::vector<std::pair<ScAddr, std::vector<ScAddr>>>& graph,
      std::vector<std::pair<ScAddr, int>>& scc_map,
      std::vector<std::vector<int>>& condensation_adj
  );
};
}
