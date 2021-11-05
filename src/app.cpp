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
  test_ant.setFillColor(m_themeManager.antColor());
  return true;
}

void App::event() {
  // Handle events
  switch (m_event.type) {
    case sf::Event::Closed:
      m_window.close();
      break;
    case sf::Event::MouseMoved:
      test_ant.setDirection(
          static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window)) -
          test_ant.getPosition());
    default:;
  }
}

void App::loop() {
  // Update game logic
  test_ant.move(elapsedTime);
  std::cout << '\r' << "Markers " << m_markers.size() << std::flush;
  for (auto it = m_markers.begin(); it != m_markers.end(); ++it) {
    it->tickLife(elapsedTime);
    if (it->getRemainingLife() <= 0) {
      m_markers.erase(it);
    }
  }
  test_ant.mark(m_markers);
}

void App::render() {
  m_window.clear(m_themeManager.backgroundColor());

  sf::Image markersMap;
  markersMap.create(m_window.getSize().x, m_window.getSize().y,
                    sf::Color::Transparent);
  for (auto& marker : m_markers) {
    sf::Color color = sf::Color::Red;
    color.a = marker.getRemainingLife();
    markersMap.setPixel(marker.getPosition().x, marker.getPosition().y, color);
  }
  sf::Texture t;
  t.loadFromImage(markersMap);
  sf::Sprite s(t);
  m_window.draw(s);
  m_window.draw(test_ant);

  m_window.display();
}

void App::cleanup() {}
