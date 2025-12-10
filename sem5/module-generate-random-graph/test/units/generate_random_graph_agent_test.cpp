#include <sc-memory/test/sc_test.hpp>

#include "agent/sc_agent_generate_random_graph.hpp"
#include "keynodes/gen_random_graph_keynodes.hpp"

namespace GenerateRandomGraphTest
{

using GenerateRandomGraphModuleTest = ScMemoryTest;

TEST_F(GenerateRandomGraphModuleTest , ExampleAgentTest)
{
  ScAgentContext & context = *m_ctx;
  context.SubscribeAgent<dm::ScAgentGenerateRandomGraph>();
  ScAction testAction = context.GenerateAction(dm::GenerateRandomGraphKeynodes::action_generate_random_graph);
  EXPECT_TRUE(testAction.InitiateAndWait());
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  context.UnsubscribeAgent<dm::ScAgentGenerateRandomGraph>();
}

}  // namespace exampleTest
