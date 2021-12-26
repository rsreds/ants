#include <ants/app.hpp>
#include <iostream>

App::App(uint screenWidth, uint screenHeight)
    : m_window(sf::VideoMode(screenWidth, screenHeight), "Ants"),
      elapsedTime{0},
      m_world{{screenWidth, screenHeight}} {
  m_window.setVerticalSyncEnabled(true);

  const unsigned int pixelSize{1u};
  const sf::Vector2u windowSize{screenWidth, screenHeight};
  const sf::View view(sf::Vector2f(windowSize / pixelSize),
                      sf::Vector2f(windowSize / pixelSize / 1u));
//  m_window.setView(view);
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
    for (int i = 0; i < 30; ++i) colony.spawn();
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
  m_world.updateMarkers(elapsedTime);
  m_world.updateColonies(elapsedTime);
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
        yPos >= (int)m_window.getSize().y)
      continue;

    sf::Color color;
    switch (marker.getType()) {
      case ants::toBase:
        color = sf::Color::Red;
        break;
      case ants::toFood:
        color = sf::Color::Blue;
        break;
      default:
        color = sf::Color::Magenta;
        break;
    }

    color.a = static_cast<uint8_t>(marker.getRemainingLife());
    markersMap.setPixel(xPos, yPos, color);
  }

  sf::Texture t;
  t.loadFromImage(markersMap);
  sf::Sprite s(t);
  m_window.draw(s);

  m_window.draw(m_world.getHeatmap(ants::toFood));
  m_window.draw(m_world.getHeatmap(ants::toBase));

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
