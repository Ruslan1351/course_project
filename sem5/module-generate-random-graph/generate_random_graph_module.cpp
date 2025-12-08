#include "generate_random_graph_module.hpp"

#include "agent/sc_agent_generate_random_graph.hpp"

SC_MODULE_REGISTER(GenerateRandomGraphModule)->Agent<ScAgentGenerateRandomGraph>();

