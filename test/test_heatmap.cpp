#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include "doctest.h"
#include "ants/marker.hpp"
#include "ants/heatmap.hpp"

constexpr float epsilon = 1e-4;

TEST_CASE("Operations") {

  ants::Heatmap<6, 4> heatMap{{60, 40}};
  heatMap.incrementByOneAtPosition({22, 35});
  heatMap.print();
  CHECK_EQ(heatMap.getValueAtIndex({2, 3}), 1);

  heatMap.incrementByOneAtPosition({22, 35});
  heatMap.print();
  CHECK_EQ(heatMap.getValueAtIndex({2, 3}), 2);

  heatMap.incrementByOneAtPosition({33, 12});
  heatMap.print();
  CHECK_EQ(heatMap.getValueAtIndex({3, 1}), 1);

  ants::Heatmap<80, 60> heatmap2 {{800, 600}};
  heatmap2.incrementByOneAtPosition({22, 45});
  CHECK_EQ(heatmap2.getValueAtIndex({2, 4}), 1);

  heatmap2.incrementByOneAtPosition({22, 45});
  CHECK_EQ(heatmap2.getValueAtIndex({2, 4}), 2);

  heatmap2.incrementByOneAtPosition({33, 12});
  CHECK_EQ(heatmap2.getValueAtIndex({3, 1}), 1);

  heatmap2.incrementByOneAtPosition({50, 50});
  heatmap2.incrementByOneAtPosition({50, 50});
  heatmap2.incrementByOneAtPosition({50, 50});
  heatmap2.incrementByOneAtPosition({49.99, 49.99});
  heatmap2.incrementByOneAtPosition({50, 50});
  heatmap2.decrementByOneAtPosition({50, 50});
  CHECK_EQ(heatmap2.getValueAtIndex({5, 5}), 3);

  ants::Heatmap<10, 10> heatmap3 {{800, 600}};
  heatmap3.incrementByOneAtPosition({222, 450});
  CHECK_EQ(heatmap3.getValueAtIndex({2, 7}), 1);
  heatmap3.print();

  heatmap3.incrementByOneAtPosition({222, 450});
  CHECK_EQ(heatmap3.getValueAtIndex({2, 7}), 2);

  heatmap3.incrementByOneAtPosition({333, 120});
  CHECK_EQ(heatmap3.getValueAtIndex({4, 2}), 1);

  heatmap3.incrementByOneAtPosition({500, 500});
  heatmap3.incrementByOneAtPosition({500, 500});
  heatmap3.incrementByOneAtPosition({500, 500});
  heatmap3.incrementByOneAtPosition({450, 499});
  heatmap3.incrementByOneAtPosition({500, 500});
  heatmap3.decrementByOneAtPosition({500, 500});
  CHECK_EQ(heatmap3.getValueAtIndex({6, 8}), 3);


  ants::Heatmap<32, 24> heatmap4 {{800, 600}};
  heatmap4.incrementByOneAtPosition({798, 598});
  CHECK_EQ(heatmap4.getValueAtIndex({31, 23}), 1);

  auto position = heatmap4.getPositionFromIndex({30, 20});
  CHECK(abs(position.x - 762.5) < epsilon);
  CHECK(abs(position.y - 512.5) < epsilon);
}

template <size_t size>
bool operator==(std::array<int, size> &a1, std::array<int, size> &a2) {
  for (size_t i = 0; i < size; ++i)
    if (a1.at(i) != a2.at(i)) return false;

  return true;
}

TEST_CASE("Neighbours") {
  std::array<int, 8> result{};
  std::array<ants::HeatmapIndex, 8> indices{};
  std::array<int, 8> expected{};
  bool eq;

  ants::Heatmap<32, 24> heatmap {{800, 600}};
  heatmap.incrementByOneAtPosition({10, 20});
  heatmap.incrementByOneAtPosition({55, 5});
  heatmap.findNeighbours({26, 27}, result, indices);

  expected = {0, 1, 0, 0, 0, 0, 0, 1};
  eq = (result == expected);
  CHECK(eq);

  heatmap.incrementByOneAtPosition({797, 580});
  heatmap.findNeighbours({754, 566}, result, indices);
  expected = {0, 0, 0, 1, 0, 0, 0, 0};
  eq = (result == expected);
  CHECK(eq);

  ants::Heatmap<8, 6> heatmap1 {{64, 36}};

  heatmap1.incrementByOneAtPosition({32.5, 12.3});  // NW
  heatmap1.incrementByOneAtPosition({40.7, 12.4});  // N
  heatmap1.incrementByOneAtPosition({48.1, 13.2});  // NE
  heatmap1.incrementByOneAtPosition({32.1, 20.4});  // W
  heatmap1.incrementByOneAtPosition({48.1, 21.4});  // E
  heatmap1.incrementByOneAtPosition({38.1, 26.4});  // SW
  heatmap1.incrementByOneAtPosition({40.5, 27.4});  // S
  heatmap1.incrementByOneAtPosition({48.2, 26.4});  // SE

  heatmap1.incrementByOneAtPosition({49.4, 14.3});  // NE
  heatmap1.incrementByOneAtPosition({36.8, 27.9});  // SW

  heatmap1.print();
  heatmap1.findNeighbours({40, 20}, result, indices);
  expected = {1, 2, 1, 1, 1, 2, 1, 1};
  eq = (result == expected);
  CHECK(eq);
}