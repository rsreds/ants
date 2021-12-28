#ifndef ANTS_APP_H
#define ANTS_APP_H

#include <SFML/Graphics.hpp>
#include <ants/world.hpp>
#include <cassert>
#include <gui/theme.hpp>

class App {
 private:
  sf::Event m_event{};
  sf::RenderWindow m_window;
  sf::Clock clock;
  float elapsedTime{};
  ants::World<64, 48> m_world;
  const int m_nAnts = 100;
  bool m_showHeatmap = false;
  GUI::ThemeManager m_themeManager;
 public:
  static constexpr uint WIDTH = 800 * 2;
  static constexpr uint HEIGHT = 600 * 2;

 public:
  explicit App(uint screenWidth = 1600, uint screenHeight = 1200);

 public:
  int run();

  bool init();

  void event();

  void loop();

  void render();

  void cleanup();
};

#endif  // ANTS_APP_H
