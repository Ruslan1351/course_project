#include <sc-memory/test/sc_test.hpp>

#include "agent/sc_agent_of_condensation_calculation.hpp"
#include "keynodes/condensation_calc_keynodes.hpp"

namespace condensationCalculationTest
{

using CondensationCalculationModuleTest = ScMemoryTest;

TEST_F(CondensationCalculationModuleTest, ExampleAgentTest)
{
  ScAgentContext & context = *m_ctx;
  context.SubscribeAgent<dm::ScAgentOfCondensationCalculation>();
  ScAction testAction = context.GenerateAction(dm::CondensationCalculationKeynodes::action_of_condensation_calculation);
  EXPECT_TRUE(testAction.InitiateAndWait());
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  context.UnsubscribeAgent<dm::ScAgentOfCondensationCalculation>();
}

}  // namespace exampleTest
