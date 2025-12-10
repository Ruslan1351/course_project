#include "sc_agent_generate_task_for_student.hpp"
#include "keynodes/gen_task_keynodes.hpp"

#include <sc-memory/sc_memory_headers.hpp>

#include <cstdlib>
#include <string>
#include <thread> // Для std::this_thread::sleep_for
#include <chrono> // Для std::chrono::seconds, milliseconds, etc.


namespace dm
{
ScAddr ScAgentGenerateTaskForStudent::GetActionClass() const
{
  return GenerateTaskForStudentKeynodes::action_generate_task_for_student;
}

ScResult ScAgentGenerateTaskForStudent::DoProgram(ScAction & action)
{
  m_logger.Info("Agent Generate Task For Student started");

  auto const & [arg1Addr] = action.GetArguments<1>();

  if (!m_context.IsElement(arg1Addr))
  {
    m_logger.Error("Action does not have argument 1.");
    return action.FinishWithError();
  }

  ScAddr const & action_initiated = m_context.SearchElementBySystemIdentifier("action_initiated");
  ScAddr const & rrel_1 = m_context.SearchElementBySystemIdentifier("rrel_1");
  ScAddr const & rrel_2 = m_context.SearchElementBySystemIdentifier("rrel_2");
  ScAddr const & nrel_result = m_context.SearchElementBySystemIdentifier("nrel_result");
//  ScAddr const & nrel_expected_answer = m_context.SearchElementBySystemIdentifier("nrel_expected_answer");

ScAddr nrel_expected_answer;
if (!m_context.IsElement(m_context.SearchElementBySystemIdentifier("nrel_expected_answer")))
{
  nrel_expected_answer = m_context.GenerateNode(ScType::ConstNodeClass);
  m_context.SetElementSystemIdentifier("nrel_expected_answer", nrel_expected_answer);
} else {
  nrel_expected_answer = m_context.SearchElementBySystemIdentifier("nrel_expected_answer");
}

  ScAddr const & action_generate_random_graph = m_context.SearchElementBySystemIdentifier("action_generate_random_graph");
  ScAddr const & action_determine_number_of_graphs_for_task = m_context.SearchElementBySystemIdentifier("action_determine_number_of_graphs_for_task");
//  ScAddr const & action_check_student_answer = m_context.SearchElementBySystemIdentifier("action_check_student_answer");

ScAddr action_check_student_answer;
if (!m_context.IsElement(m_context.SearchElementBySystemIdentifier("action_check_student_answer")))
{
  action_check_student_answer = m_context.GenerateNode(ScType::ConstNodeClass);
  m_context.SetElementSystemIdentifier("action_check_student_answer", action_check_student_answer);
} else {
  action_check_student_answer = m_context.SearchElementBySystemIdentifier("action_check_student_answer");
}

  ScAddr agent_determine_number_of_graphs_for_task = m_context.GenerateNode(ScType::ConstNode);
  m_context.GenerateConnector(ScType::ConstPermPosArc, action_determine_number_of_graphs_for_task, agent_determine_number_of_graphs_for_task);
  m_context.GenerateConnector(ScType::ConstPermPosArc, rrel_1, m_context.GenerateConnector(ScType::ConstPermPosArc, agent_determine_number_of_graphs_for_task, arg1Addr));
  m_context.GenerateConnector(ScType::ConstPermPosArc, action_initiated, agent_determine_number_of_graphs_for_task);

  std::this_thread::sleep_for(std::chrono::milliseconds(200)); // ожидание обработки агента

  ScTemplate templ_determine_number_of_graphs_for_task;
  templ_determine_number_of_graphs_for_task.Quintuple(
    agent_determine_number_of_graphs_for_task,
    ScType::VarCommonArc,
    ScType::VarNodeStructure >> "_structure",
    ScType::VarPermPosArc,
    nrel_result
  );
  ScTemplateSearchResult templ_determine_number_of_graphs_for_task_result;
  m_context.SearchByTemplate(templ_determine_number_of_graphs_for_task, templ_determine_number_of_graphs_for_task_result);
  if (templ_determine_number_of_graphs_for_task_result.Size() != 1) {
    m_logger.Warning("Action finished incorectly 1.");
    return action.FinishWithError();
  }
  ScAddr const & determine_number_of_graphs_for_task_result = templ_determine_number_of_graphs_for_task_result[0]["_structure"];

  ScTemplate templ_determine_number_of_graphs_for_task_number;
  templ_determine_number_of_graphs_for_task_number.Triple(
    determine_number_of_graphs_for_task_result,
    ScType::VarPermPosArc,
    ScType::VarNodeLink >> "_link"
  );
  ScTemplateSearchResult templ_determine_number_of_graphs_for_task_number_result;
  m_context.SearchByTemplate(templ_determine_number_of_graphs_for_task_number, templ_determine_number_of_graphs_for_task_number_result);
  if (templ_determine_number_of_graphs_for_task_number_result.Size() != 1) {
    m_logger.Warning("Action finished incorectly 2.");
    return action.FinishWithError();
  }
  ScAddr const & determine_number_of_graphs_for_task_addr = templ_determine_number_of_graphs_for_task_number_result[0]["_link"];
  int determine_number_of_graphs_for_task_result_number;
  m_context.GetLinkContent(determine_number_of_graphs_for_task_addr, determine_number_of_graphs_for_task_result_number);
  m_logger.Info("The assignment generation agent for the student will create ", determine_number_of_graphs_for_task_result_number, " graphs");

  std::vector<ScAddr> graphs;

  for (int _ = 0; _ < determine_number_of_graphs_for_task_result_number; _ += 1 ) {
  	std::string nodeCount = std::to_string(1 + rand() % 20);
    ScAddr nodeCountAddr = m_context.GenerateLink(ScType::ConstNodeLink);
    m_context.SetLinkContent(nodeCountAddr, nodeCount);

    std::string edgeCount = std::to_string(1 + rand() % 100);
    ScAddr edgeCountAddr = m_context.GenerateLink(ScType::ConstNodeLink);
    m_context.SetLinkContent(edgeCountAddr, edgeCount);

	ScAddr const & agent_generate_random_graph = m_context.GenerateNode(ScType::ConstNode);
    m_context.GenerateConnector(ScType::ConstPermPosArc, action_generate_random_graph, agent_generate_random_graph);
	m_context.GenerateConnector(ScType::ConstPermPosArc, rrel_1, m_context.GenerateConnector(ScType::ConstPermPosArc, agent_generate_random_graph, nodeCountAddr));
    m_context.GenerateConnector(ScType::ConstPermPosArc, rrel_2, m_context.GenerateConnector(ScType::ConstPermPosArc, agent_generate_random_graph, edgeCountAddr));
    m_context.GenerateConnector(ScType::ConstPermPosArc, action_initiated, agent_generate_random_graph);

	std::this_thread::sleep_for(std::chrono::milliseconds(200)); // ожидание обработки агента

	ScTemplate templ_generate_random_graph_1;
    templ_generate_random_graph_1.Quintuple(
      agent_generate_random_graph,
      ScType::VarCommonArc,
      ScType::VarNodeStructure >> "_structure",
      ScType::VarPermPosArc,
      nrel_result
    );
    ScTemplateSearchResult templ_generate_random_graph_1_result;
    m_context.SearchByTemplate(templ_generate_random_graph_1, templ_generate_random_graph_1_result);
    if (templ_generate_random_graph_1_result.Size() != 1) {
      m_logger.Warning("Action finished incorectly 3.");
      return action.FinishWithError();
    }
    ScAddr const & generate_random_graph_1 = templ_generate_random_graph_1_result[0]["_structure"];

	ScTemplate templ_generate_random_graph_2;
    templ_generate_random_graph_2.Triple(
      generate_random_graph_1,
      ScType::VarPermPosArc,
      ScType::VarNodeStructure >> "_structure"
    );
    ScTemplateSearchResult templ_generate_random_graph_2_result;
    m_context.SearchByTemplate(templ_generate_random_graph_2, templ_generate_random_graph_2_result);
    if (templ_generate_random_graph_2_result.Size() != 1) {
      m_logger.Warning("Action finished incorectly 4.");
      return action.FinishWithError();
    }
    ScAddr const & generate_random_graph_2 = templ_generate_random_graph_2_result[0]["_structure"];

	graphs.push_back(generate_random_graph_2);
  }

  std::vector<ScAddr> result_graphs;

  for (auto const & graph : graphs) {
    ScAddr agent_arg_1 = m_context.GenerateNode(ScType::ConstNode);
    m_context.GenerateConnector(ScType::ConstPermPosArc, arg1Addr, agent_arg_1);
    m_context.GenerateConnector(ScType::ConstPermPosArc, rrel_1, m_context.GenerateConnector(ScType::ConstPermPosArc, agent_arg_1, graph));
    m_context.GenerateConnector(ScType::ConstPermPosArc, action_initiated, agent_arg_1);

    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // ожидание обработки агента

    ScTemplate templ_agent_arg_1_1;
    templ_agent_arg_1_1.Quintuple(
      agent_arg_1,
      ScType::VarCommonArc,
      ScType::VarNodeStructure >> "_structure",
      ScType::VarPermPosArc,
      nrel_result
    );



    ScTemplateSearchResult templ_agent_arg_1_1_result;
    m_context.SearchByTemplate(templ_agent_arg_1_1, templ_agent_arg_1_1_result);
    if (templ_agent_arg_1_1_result.Size() != 1) {
      m_logger.Warning("Action finished incorectly 5.");
      return action.FinishWithError();
    }
    ScAddr const & agent_arg_1_1_result = templ_agent_arg_1_1_result[0]["_structure"];



	ScTemplate templ_agent_arg_1_2;
    templ_agent_arg_1_2.Triple(
      agent_arg_1_1_result,
      ScType::VarPermPosArc,
      ScType::VarNodeStructure >> "_structure"
    );
    ScTemplateSearchResult templ_agent_arg_1_2_result;
    m_context.SearchByTemplate(templ_agent_arg_1_2, templ_agent_arg_1_2_result);
    if (templ_agent_arg_1_2_result.Size() != 1) {
      m_logger.Warning("Action finished incorectly 6.");
      return action.FinishWithError();
    }



    ScAddr const & agent_arg_1_2_result = templ_agent_arg_1_2_result[0]["_structure"];

    result_graphs.push_back(agent_arg_1_2_result);
  }

  ScAddr const & agent_check_student_answer = m_context.GenerateNode(ScType::ConstNode);
  ScStructure agent_check_student_answer_structure = m_context.GenerateStructure();

  agent_check_student_answer_structure << result_graphs[0];
  agent_check_student_answer_structure << arg1Addr;
  agent_check_student_answer_structure << m_context.GenerateConnector(ScType::ConstPermPosArc, arg1Addr, result_graphs[0]);

m_logger.Info("1");
  m_context.GenerateConnector(ScType::ConstPermPosArc, nrel_expected_answer, m_context.GenerateConnector(ScType::ConstPermPosArc, agent_check_student_answer, agent_check_student_answer_structure));
m_logger.Info("2");
  action.FormResult(graphs[0], arg1Addr, action_check_student_answer, agent_check_student_answer, m_context.GenerateConnector(ScType::ConstPermPosArc, action_check_student_answer, agent_check_student_answer));
m_logger.Info("3");
  return action.FinishSuccessfully();
}
}