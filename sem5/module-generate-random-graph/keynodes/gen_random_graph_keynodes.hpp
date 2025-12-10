#pragma once
#include <sc-memory/sc_keynodes.hpp>

namespace dm
{
class GenerateRandomGraphKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_generate_random_graph{"action_generate_random_graph", ScType::ConstNodeClass};
};
}