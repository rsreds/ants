
#include <ants/app.hpp>

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
}

void App::render() {
  m_window.clear(m_themeManager.backgroundColor());

  // test_ant.setRotation(test_ant.getRotation() + 1);
  m_window.draw(test_ant);
  m_window.draw(test_colony.getAnthill());
  m_window.display();
}

void App::cleanup() {}
