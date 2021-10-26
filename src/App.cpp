//
// Created by Jacopo Gasparetto on 25/10/21.
//

#include <ants/App.hpp>

App::App(int screenWidth, int screenHeight)
    : m_window(sf::VideoMode(screenWidth, screenHeight), "Ants") {}

int App::run() {
  if (!init()) return EXIT_FAILURE;

  while (m_window.isOpen()) {
    while (m_window.pollEvent(m_event)) {
      event();
    }

    loop();
    render();
  }

  cleanup();
  return EXIT_SUCCESS;
}

bool App::init() {
  // Perform initial setup
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
}

void App::render() {
  m_window.clear();
  // Draw stuff here

  m_window.draw(test_ant.makeShape());

  m_window.display();
}

void App::cleanup() {}
