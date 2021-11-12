#ifndef ANTS_APP_H
#define ANTS_APP_H

#include <SFML/Graphics.hpp>
#include <ants/ant.hpp>
#include <ants/colony.hpp>
#include <ants/food.hpp>
#include <ants/marker.hpp>
#include <gui/theme.hpp>

class App {
 private:
  sf::Event m_event{};
  sf::RenderWindow m_window;
  sf::Clock clock;
  float elapsedTime;
  std::vector<ants::Marker> m_markers;
  ants::Colony test_colony{{200, 150}, 10};
  ants::FoodSource test_foodSource{100, {300, 300}};

  GUI::ThemeManager m_themeManager;

 public:
  explicit App(int screenWidth = 800, int screenHeight = 600);

 public:
  int run();

  bool init();

  void event();

  void loop();

  void render();

  void cleanup();
};

#endif  // ANTS_APP_H
