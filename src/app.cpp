#include <ants/app.hpp>
#include <iostream>

App::App(uint screenWidth, uint screenHeight)
    : m_window(sf::VideoMode(screenWidth, screenHeight), "Ants"),
      elapsedTime{0},
      m_world{{WIDTH, HEIGHT}, m_themeManager} {
  m_window.setVerticalSyncEnabled(true);
  sf::FloatRect area(0, 0, WIDTH, HEIGHT);
  m_window.setView(sf::View(area));
  m_themeManager.applyTheme(GUI::Theme::Type::polarDark);
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
    for (int i = 0; i < m_nAnts; ++i) colony.spawn();
  }

  return true;
}

void App::event() {
  // Handle events
  switch (m_event.type) {
    case sf::Event::Closed:
      m_window.close();
      break;
    case sf::Event::MouseButtonPressed:
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f mousePos =
            static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
        mousePos.x *= static_cast<float>(WIDTH) /
                      static_cast<float>(m_window.getSize().x);
        mousePos.y *= static_cast<float>(HEIGHT) /
                      static_cast<float>(m_window.getSize().y);
        m_world.addFoodSource({100, mousePos});
        std::cout << "Add source food\n";
      }
      break;
    case sf::Event::KeyPressed:
      switch (m_event.key.code) {
          // Show/Hide heatmap
        case sf::Keyboard::Key::H: {
          m_showHeatmap = !m_showHeatmap;
          if (m_showHeatmap)
            std::cout << "Hide heatmap\n";
          else
            std::cout << "Show heatmap\n";
        } break;
          // Reset scene
        case sf::Keyboard::Key::R: {
          m_world.reset();
          for (auto& colony : m_world.getColonies()) {
            for (int i = 0; i < m_nAnts; ++i) colony.spawn();
          }
          std::cout << "Reset scene\n";
        } break;
          // Up arrow key
        case sf::Keyboard::Key::Up: {
          // Increase markers lifetime
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
            m_world.setMarkersLifetime(m_world.getMarkersLifetime() + 10.0f);
            std::cout << "Markers life time: " << m_world.getMarkersLifetime()
                      << '\n';
          }
          // Increase ants hunting timeout
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
            m_world.setHuntingTimeout(m_world.getHuntingTimeout() + 5.0f);
            std::cout << "Ants hunting timeout: " << m_world.getHuntingTimeout()
                      << '\n';
          }
        } break;
          // Down key
        case sf::Keyboard::Key::Down: {
          // Decrease markers lifetime
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
            m_world.setMarkersLifetime(
                std::max(0.0f, m_world.getMarkersLifetime() - 10));
            std::cout << "Markers life time: " << m_world.getMarkersLifetime()
                      << '\n';
          }
          // Decrease ants hunting timeout
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
            m_world.setHuntingTimeout(
                std::max(0.0f, m_world.getHuntingTimeout() - 5.0f));
            std::cout << "Ants hunting timeout: " << m_world.getHuntingTimeout()
                     << '\n';
          }
        } break;
        default:;
      }
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

  if (m_showHeatmap) {
    m_window.draw(m_world.getHeatmap(ants::toFood));
    m_window.draw(m_world.getHeatmap(ants::toBase));
  }
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
