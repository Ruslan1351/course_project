#include <sc-memory/test/sc_test.hpp>

#include "agent/sc_agent_determine_number_of_graphs_for_task.hpp"
#include "keynodes/keynodes.hpp"

namespace determineNumberOfGraphsForTaskTest
{

using DetermineNumberOfGraphsModuleTest = ScMemoryTest;

TEST_F(DetermineNumberOfGraphsModuleTest, ScAgentDetermineNumberOfGraphsForTaskTest)
{
  ScAgentContext & context = *m_ctx;
  context.SubscribeAgent<dm::ScAgentDetermineNumberOfGraphsForTask>();
  ScAction testAction = context.GenerateAction(dm::DetermineNumberOfGraphsForTaskKeynodes::action_determine_number_of_graphs_for_task);
  EXPECT_TRUE(testAction.InitiateAndWait());
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  context.UnsubscribeAgent<dm::ScAgentDetermineNumberOfGraphsForTask>();
}

}  // namespace exampleTest
