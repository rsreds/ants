#include <vector2/vector2.h>

#include <ants/ant.hpp>

namespace ants {
Ant::Ant(sf::Vector2f pos) {
  float radius = 3;
  auto antShape = sf::CircleShape(radius);
  antShape.setOrigin({radius, radius});
  antShape.setScale({3, 1});
  m_shape = antShape;
  setPosition(pos);
}

void Ant::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(m_shape, states);
}

sf::Vector2f Ant::getDirection() const { return m_direction; }
float Ant::setSpeed() const { return m_speed; }
void Ant::setDirection(sf::Vector2f const& dir) {
  auto newAngle = sf::rad2deg(std::atan2(m_direction.y, m_direction.x));
  setRotation(newAngle);
  m_direction = dir;
}
void Ant::setSpeed(float const& spd) { m_speed = spd; }

}  // namespace ants