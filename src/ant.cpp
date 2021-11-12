#include <vector2/vector2.h>

#include <ants/ant.hpp>
#include <random>

namespace ants {
Ant::Ant(sf::Vector2f pos) { setPosition(pos); }

void Ant::setFillColor(const sf::Color& c) { m_color = c; }

void Ant::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  float radius = 3;
  auto abdomenShape = sf::CircleShape(radius);
  abdomenShape.setFillColor(m_color);
  abdomenShape.setOrigin({radius, radius});
  abdomenShape.setPosition({-1.5f * radius, 0.f});

  auto thoraxShape = sf::CircleShape(radius * 0.7f);
  thoraxShape.setFillColor(m_color);
  thoraxShape.setOrigin({radius * 0.7f, radius * 0.7f});
  thoraxShape.setScale({3, 0.7});

  auto headShape = sf::CircleShape(radius * 0.8f);
  headShape.setFillColor(m_color);
  headShape.setOrigin({radius * 0.8f, radius * 0.8f});
  headShape.setPosition({1.5f * radius, 0.f});

  target.draw(abdomenShape, states);
  target.draw(thoraxShape, states);
  target.draw(headShape, states);
}

sf::Vector2f Ant::getDirection() const { return m_direction; }
float Ant::setSpeed() const { return m_speed; }
void Ant::setDirection(sf::Vector2f const& dir) {
  auto newAngle = sf::rad2deg(std::atan2(m_direction.y, m_direction.x));
  setRotation(newAngle);
  m_direction = sf::normalizeCopy(dir);
}
void Ant::setDirection(Marker const& marker) {
  setDirection(marker.getPosition());
}

void Ant::setSpeed(float const& spd) { m_speed = spd; }
void Ant::move(float const& elapsedTime) {
  sf::Vector2f velocity{m_direction};
  sf::setMagnitude(velocity, m_speed);
  setPosition(getPosition() + velocity * elapsedTime);
}
void Ant::mark(std::vector<ants::Marker>& markers) {
  switch (m_state) {
    case AntState::leavingAnthill:
      markers.emplace_back(getPosition(), ants::MarkerType::toBase);
      break;

    default:
      break;
  }
}

sf::Vector2f randomDirection() {
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_real_distribution<float> uniform_dist(-0.5, 0.5);
  float r1 = uniform_dist(e1);
  float r2 = uniform_dist(e1);
  return {r1, r2};
}
void Ant::update(std::vector<ants::Marker>& markers) {
  switch (m_state) {
    case returningAnthill: {
      ants::Marker strongestMarker =
          findStrogestAdjacent(getPosition(), toBase, markers);
      setDirection(strongestMarker);
      break;
    }
    case leavingAnthill: {
      setDirection(getDirection() + randomDirection());
      std::cout << "\rPosition: " << getDirection().x << ' ' << getDirection().y
                << std::flush;
    } break;
  }
}

}  // namespace ants
