#include <ants/app.hpp>
#include <iostream>

App::App(int screenWidth, int screenHeight)
    : m_window(sf::VideoMode(screenWidth, screenHeight), "Ants") {
  m_window.setVerticalSyncEnabled(true);

  m_themeManager.applyTheme(GUI::Theme::Type::polar);
}

int App::run() {
  if (!init()) return EXIT_FAILURE;

  while (m_window.isOpen()) {
    while (m_window.pollEvent(m_event)) {
      event();
    }
    elapsedTime = clock.getElapsedTime().asSeconds();
    clock.restart();
    loop();
    render();
  }

  cleanup();
  return EXIT_SUCCESS;
}

bool App::init() {
  // Perform initial setup
  m_themeManager.applyTheme(GUI::Theme::Type::polar);
  // test_ant.setFillColor(m_themeManager.antColor());
  for (auto& colony : m_world.getColonies()) {
    colony.spawn();
  }
  return true;
}

void App::event() {
  // Handle events
  switch (m_event.type) {
    case sf::Event::Closed:
      m_window.close();
      break;
    default:;
  }
}

void App::loop() {
  // Update game logic
  for (auto it = m_world.getMarkers().begin(); it != m_world.getMarkers().end();
       ++it) {
    it->tickLife(elapsedTime);
    if (it->getRemainingLife() <= 0) {
      m_world.getMarkers().erase(it);
    }
  }
  for (auto& colony : m_world.getColonies())
    for (auto& ant : colony.m_ants) {
      m_world.updateAnt(colony, ant);
      ant.move(elapsedTime);
      ant.mark(m_world.getMarkers());
    }
}

void App::render() {
  m_window.clear(m_themeManager.backgroundColor());

  sf::Image markersMap;
  markersMap.create(m_window.getSize().x, m_window.getSize().y,
                    sf::Color::Transparent);
  for (auto& marker : m_world.getMarkers()) {
    sf::Color color = sf::Color::Red;
    color.a = marker.getRemainingLife();
    markersMap.setPixel(marker.getPosition().x, marker.getPosition().y, color);
  }
  sf::Texture t;
  t.loadFromImage(markersMap);
  sf::Sprite s(t);
  m_window.draw(s);
  for (auto& foodSource : m_world.getFoodSources()) {
    std::cout << "printed food source";
    foodSource.setFillColor(m_themeManager.foodColor());
    m_window.draw(foodSource);
  }
  for (auto& colony : m_world.getColonies()) {
    m_window.draw(colony.getAnthill());
    for (auto& ant : colony.m_ants) {
      m_window.draw(ant);
    }
  }
  m_window.display();
}

void App::cleanup() {}
