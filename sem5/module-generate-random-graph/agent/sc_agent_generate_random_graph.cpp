#include "sc_agent_generate_random_graph.hpp"
#include "keynodes/gen_random_graph_keynodes.hpp"

#include <sc-memory/sc_memory_headers.hpp>

#include <vector>
#include <algorithm>

namespace dm
{
ScAddr ScAgentGenerateRandomGraph::GetActionClass() const
{
  return GenerateRandomGraphKeynodes::action_generate_random_graph;
}

ScResult ScAgentGenerateRandomGraph::DoProgram(ScAction & action)
{
  m_logger.Info("Agent Generate Random Graph started");
  
  auto const & [arg1Addr, arg2Addr] = action.GetArguments<2>();

  if (!m_context.IsElement(arg1Addr))
  {
    m_logger.Error("Action does not have argument 1.");
    return action.FinishWithError();
  }

  int arg1Content;
  bool const arg1ContentExist
          = m_context.GetLinkContent(arg1Addr, arg1Content);
  m_logger.Info("Element 1 value: ", arg1Content);

  if (!m_context.IsElement(arg2Addr))
  {
    m_logger.Error("Action does not have argument 1.");
    return action.FinishWithError();
  }

  int arg2Content;
  bool const arg2ContentExist
          = m_context.GetLinkContent(arg2Addr, arg2Content);
  m_logger.Info("Element 2 value: ", arg2Content);

  auto [nodes, edges] = generateRandomGraph(arg1Content, arg2Content);

  ScStructure result = m_context.GenerateStructure();

  std::vector<ScAddr> nodeAddrs;
  for (int i = 0; i < nodes.size(); ++i) {
      ScAddr nodeAddr = m_context.GenerateNode(ScType::ConstNode);
      nodeAddrs.push_back(nodeAddr);
      result << nodeAddr;
  }

  for (auto const & [from, to] : edges) {
      ScAddr edgeAddr = m_context.GenerateConnector(
//                   ScType::VarCommonArc, nodeAddrs[from], nodeAddrs[to]);
                   ScType::ConstCommonArc, nodeAddrs[from], nodeAddrs[to]);
      result << edgeAddr;
  }

  action.FormResult(result);

  m_logger.Info("Finish Agent Generate Random Graph");
  return action.FinishSuccessfully();
}

std::pair<std::vector<int>, std::vector<std::pair<int, int>>> ScAgentGenerateRandomGraph::generateRandomGraph(int nodeCount, int edgeCount) {
  std::vector<int> nodes;
  std::vector<std::pair<int, int>> edges;

  for (int i = 0; i < nodeCount; ++i) {
      nodes.push_back(i);
  }

  for (int i = 0; i < edgeCount; ++i) {
    int from = rand() % nodeCount;
    int to = rand() % nodeCount;
    edges.emplace_back(from, to);
  }

  std::sort(edges.begin(), edges.end());
  edges.erase(std::unique(edges.begin(), edges.end()), edges.end());

  return {nodes, edges};
}
}