#include <sc-memory/test/sc_test.hpp>

#include "agent/sc_agent_generate_task_for_student.hpp"
#include "keynodes/gen_task_keynodes.hpp"

namespace generateTaskForStudentTest
{

using GenerateTaskForStudentModuleTest = ScMemoryTest;

TEST_F(GenerateTaskForStudentModuleTest, CompareStructuresAgentTest)
{
  ScAgentContext & context = *m_ctx;
  context.SubscribeAgent<dm::ScAgentGenerateTaskForStudent>();
  ScAction testAction = context.GenerateAction(dm::GenerateTaskForStudentKeynodes::action_generate_task_for_student);
  EXPECT_TRUE(testAction.InitiateAndWait());
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  context.UnsubscribeAgent<dm::ScAgentGenerateTaskForStudent>();
}

}  // namespace exampleTest
