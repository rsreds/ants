#ifndef ANTS_APP_H
#define ANTS_APP_H

#include <SFML/Graphics.hpp>
#include <ants/world.hpp>
#include <gui/theme.hpp>
#include <cassert>

class App {
 private:
  sf::Event m_event{};
  sf::RenderWindow m_window;
  sf::Clock clock;
  float elapsedTime;
  ants::World m_world;

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
