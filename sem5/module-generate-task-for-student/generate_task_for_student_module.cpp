#include "generate_task_for_student_module.hpp"

#include "agent/sc_agent_generate_task_for_student.hpp"

namespace dm
{
SC_MODULE_REGISTER(GenerateTaskForStudentModule)->Agent<ScAgentGenerateTaskForStudent>();
}
