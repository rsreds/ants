#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <ants/marker.hpp>
#include <iostream>


TEST_CASE("Operations") {

  ants::Heatmap<6, 4> heatMap{{60, 40}};
  heatMap.incrementByOneAt({22, 35});
  heatMap.print();
  CHECK_EQ(heatMap.getValueAtIndex({2, 3}), 1);

  heatMap.incrementByOneAt({22, 35});
  heatMap.print();
  CHECK_EQ(heatMap.getValueAtIndex({2, 3}), 2);

  heatMap.incrementByOneAt({33, 12});
  heatMap.print();
  CHECK_EQ(heatMap.getValueAtIndex({3, 1}), 1);

  ants::Heatmap<80, 60> heatmap2 {{800, 600}};
  heatmap2.incrementByOneAt({22, 45});
  CHECK_EQ(heatmap2.getValueAtIndex({2, 4}), 1);

  heatmap2.incrementByOneAt({22, 45});
  CHECK_EQ(heatmap2.getValueAtIndex({2, 4}), 2);

  heatmap2.incrementByOneAt({33, 12});
  CHECK_EQ(heatmap2.getValueAtIndex({3, 1}), 1);

  heatmap2.incrementByOneAt({50, 50});
  heatmap2.incrementByOneAt({50, 50});
  heatmap2.incrementByOneAt({50, 50});
  heatmap2.incrementByOneAt({49.99, 49.99});
  heatmap2.incrementByOneAt({50, 50});
  heatmap2.decrementByOneAt({50, 50});
  CHECK_EQ(heatmap2.getValueAtIndex({5, 5}), 3);

  ants::Heatmap<10, 10> heatmap3 {{800, 600}};
  heatmap3.incrementByOneAt({222, 450});
  CHECK_EQ(heatmap3.getValueAtIndex({2, 7}), 1);
  heatmap3.print();

  heatmap3.incrementByOneAt({222, 450});
  CHECK_EQ(heatmap3.getValueAtIndex({2, 7}), 2);

  heatmap3.incrementByOneAt({333, 120});
  CHECK_EQ(heatmap3.getValueAtIndex({4, 2}), 1);

  heatmap3.incrementByOneAt({500, 500});
  heatmap3.incrementByOneAt({500, 500});
  heatmap3.incrementByOneAt({500, 500});
  heatmap3.incrementByOneAt({450, 499});
  heatmap3.incrementByOneAt({500, 500});
  heatmap3.decrementByOneAt({500, 500});
  CHECK_EQ(heatmap3.getValueAtIndex({6, 8}), 3);


  ants::Heatmap<32, 24> heatmap4 {{800, 600}};
  heatmap4.incrementByOneAt({798, 598});
  CHECK_EQ(heatmap4.getValueAtIndex({31, 23}), 1);

}

template <size_t size>
bool operator==(std::array<int, size> &a1, std::array<int, size> &a2) {
  for (size_t i = 0; i < size; ++i)
    if (a1.at(i) != a2.at(i)) return false;

  return true;
}

TEST_CASE("Neighbours") {
  std::array<int, 8> result{};
  std::array<int, 8> expected{};
  bool eq;

  ants::Heatmap<32, 24> heatmap {{800, 600}};
  heatmap.incrementByOneAt({10, 20});
  heatmap.incrementByOneAt({55, 5});
  heatmap.findNeighbours({26, 27}, result);

  expected = {0, 1, 0, 0, 0, 0, 0, 1};
  eq = (result == expected);
  CHECK(eq);

  heatmap.incrementByOneAt({797, 580});
  heatmap.findNeighbours({754, 566}, result);
  expected = {0, 0, 0, 1, 0, 0, 0, 0};
  eq = (result == expected);
  CHECK(eq);

  ants::Heatmap<8, 6> heatmap1 {{64, 36}};

  heatmap1.incrementByOneAt({32.5, 12.3});  // NW
  heatmap1.incrementByOneAt({40.7, 12.4});  // N
  heatmap1.incrementByOneAt({48.1, 13.2});  // NE
  heatmap1.incrementByOneAt({32.1, 20.4});  // W
  heatmap1.incrementByOneAt({48.1, 21.4});  // E
  heatmap1.incrementByOneAt({38.1, 26.4});  // SW
  heatmap1.incrementByOneAt({40.5, 27.4});  // S
  heatmap1.incrementByOneAt({48.2, 26.4});  // SE

  heatmap1.incrementByOneAt({49.4, 14.3});  // NE
  heatmap1.incrementByOneAt({36.8, 27.9});  // SW

  heatmap1.print();
  heatmap1.findNeighbours({40, 20}, result);
  expected = {1, 2, 1, 1, 1, 2, 1, 1};
  eq = (result == expected);
  CHECK(eq);
}