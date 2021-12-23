#include <ants/app.hpp>
#include <iostream>

App::App(int screenWidth, int screenHeight)
    : m_window(sf::VideoMode(screenWidth, screenHeight), "Ants"),
      m_world{{screenWidth, screenHeight}}{
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
    for (int i = 0; i < 20; ++i) colony.spawn();
  }

  for (auto& foodSource : m_world.getFoodSources()) {
    foodSource.setFillColor(m_themeManager.foodColor());
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
  for (auto& it : m_world.getMarkers()) {
    it.tickLife(elapsedTime);
  }

  // Workaround to fix segfault erasing last element
  m_world.getMarkers().erase(
      std::remove_if(
          m_world.getMarkers().begin(), m_world.getMarkers().end(),
          [](const ants::Marker& m) { return m.getRemainingLife() <= 0; }),
      m_world.getMarkers().end());

  for (auto& colony : m_world.getColonies())
    for (auto& ant : colony.m_ants) {
      m_world.updateAnt(colony, ant);
      ant.updatePosition(elapsedTime, m_window.getSize());
      ant.mark(m_world.getMarkers());
    }
}

void App::render() {
  m_window.clear(m_themeManager.backgroundColor());

  // Draw markers
  sf::Image markersMap;
  markersMap.create(m_window.getSize().x, m_window.getSize().y,
                    sf::Color::Transparent);

  for (auto& marker : m_world.getMarkers()) {
    assert(marker.getRemainingLife() >= 0);
    auto xPos = static_cast<int>(marker.getPosition().x);
    auto yPos = static_cast<int>(marker.getPosition().y);

    // If marker is outside visible window don't draw it
    if (xPos <= 0 || xPos >= (int)m_window.getSize().x || yPos <= 0 ||
        yPos >= (int)m_window.getSize().y) continue;

    sf::Color color = sf::Color::Red;
    color.a = static_cast<uint8_t>(marker.getRemainingLife());
    markersMap.setPixel(xPos, yPos, color);
  }

  sf::Texture t;
  t.loadFromImage(markersMap);
  sf::Sprite s(t);
  m_window.draw(s);

  // Draw Food
  for (auto& foodSource : m_world.getFoodSources()) {
    m_window.draw(foodSource);
  }

  // Draw ants
  for (auto& colony : m_world.getColonies()) {
    m_window.draw(colony.getAnthill());
    for (auto& ant : colony.m_ants) {
      m_window.draw(ant);
    }
  }
  m_window.display();
}

void App::cleanup() { m_world.getMarkers().clear(); }
