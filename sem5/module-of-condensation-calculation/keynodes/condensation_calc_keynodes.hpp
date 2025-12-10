#pragma once
#include <sc-memory/sc_keynodes.hpp>

namespace dm
{
class CondensationCalculationKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_of_condensation_calculation{"action_of_condensation_calculation", ScType::ConstNodeClass};
};
}
