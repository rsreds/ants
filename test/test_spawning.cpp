#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <ants/colony.hpp>

#include "doctest.h"

TEST_CASE("Test single spawn") {
  size_t maxPop = 2;
  ants::Colony newColony{{0.f, 0.f}, maxPop};
  CHECK(newColony.getMaxPopulation() == maxPop);

  newColony.spawn();

  CHECK(newColony.getPopulation() == 1);

  newColony.spawn();
  CHECK(newColony.getPopulation() == 2);

  newColony.spawn();
  CHECK(newColony.getPopulation() == 2);
}
