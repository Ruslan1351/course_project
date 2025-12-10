#pragma once
#include <sc-memory/sc_keynodes.hpp>

namespace dm
{
class GenerateTaskForStudentKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_generate_task_for_student{"action_generate_task_for_student", ScType::ConstNodeClass};

};
}