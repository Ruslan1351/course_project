#include "determine_number_of_graphs_for_task_module.hpp"

#include "agent/sc_agent_determine_number_of_graphs_for_task.hpp"

namespace dm
{
SC_MODULE_REGISTER(DetermineNumberOfGraphsForTaskModule)->Agent<ScAgentDetermineNumberOfGraphsForTask>();
}
