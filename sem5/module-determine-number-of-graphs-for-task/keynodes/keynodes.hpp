#pragma once
#include <sc-memory/sc_keynodes.hpp>

namespace dm
{
class DetermineNumberOfGraphsForTaskKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_determine_number_of_graphs_for_task{"action_determine_number_of_graphs_for_task", ScType::ConstNodeClass};
};
}
