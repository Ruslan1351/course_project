#include "sc_agent_determine_number_of_graphs_for_task.hpp"
#include "keynodes/keynodes.hpp"

#include <sc-memory/sc_memory_headers.hpp>

namespace dm
{
ScAddr ScAgentDetermineNumberOfGraphsForTask::GetActionClass() const
{
  return DetermineNumberOfGraphsForTaskKeynodes::action_determine_number_of_graphs_for_task;
}

ScResult ScAgentDetermineNumberOfGraphsForTask::DoProgram(ScAction & action)
{
  m_logger.Info("Agent Determine Number Of Graphs For Task started");

  auto const & [arg1Addr] = action.GetArguments<1>();

  if (!m_context.IsElement(arg1Addr))
  {
    m_logger.Error("Action does not have argument 1.");
    return action.FinishWithError();
  }

  ScStructure result = m_context.GenerateStructure();
  ScAddr result_link = m_context.GenerateLink(ScType::ConstNodeLink);

  if (m_context.GetElementType(arg1Addr) == ScType::ConstNodeClass) {

    m_context.SetLinkContent(result_link, "1");
   // result << result_link;
  } else {
    ScTemplate templ_determine_number_of_graphs_for_task;
    templ_determine_number_of_graphs_for_task.Quintuple(
      ScType::Node,
      ScType::VarCommonArc,
      ScType::Node,
      ScType::VarPermPosArc,
      arg1Addr
    );
    ScTemplateSearchResult templ_determine_number_of_graphs_for_task_result;
    m_context.SearchByTemplate(templ_determine_number_of_graphs_for_task, templ_determine_number_of_graphs_for_task_result);
    if (templ_determine_number_of_graphs_for_task_result.Size() <= 1) {
      m_logger.Warning("Action finished incorectly.");
      return action.FinishWithError();
    }
    m_context.SetLinkContent(result_link, std::to_string(templ_determine_number_of_graphs_for_task_result.Size() - 1));
  }

  action.FormResult(result_link);
  return action.FinishSuccessfully();
}
}
