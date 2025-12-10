#include "condensation_calculation_module.hpp"

#include "agent/sc_agent_of_condensation_calculation.hpp"

namespace dm
{
SC_MODULE_REGISTER(CondensationCalculationModule)->Agent<ScAgentOfCondensationCalculation>();
}
