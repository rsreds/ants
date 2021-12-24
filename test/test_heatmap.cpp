#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <ants/marker.hpp>


TEST_CASE("Operations") {

  ants::Heatmap<6, 4> heatMap{{60, 40}};
  heatMap.incrementByOneAt({22, 35});
  heatMap.printHeatMap();
  CHECK_EQ(heatMap.getValueAtIndex({2, 3}), 1);

  heatMap.incrementByOneAt({22, 35});
  heatMap.printHeatMap();
  CHECK_EQ(heatMap.getValueAtIndex({2, 3}), 2);

  heatMap.incrementByOneAt({33, 12});
  heatMap.printHeatMap();
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
  heatmap3.printHeatMap();

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
  heatmap4.printHeatMap();
  CHECK_EQ(heatmap4.getValueAtIndex({31, 23}), 1);

}
