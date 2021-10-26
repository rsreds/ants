#include <ants/ant.hpp>

Ant::Ant(sf::Vector2f pos) : m_pos(pos) {}

sf::CircleShape Ant::makeShape() {
  float radius = 3;
  auto antShape = sf::CircleShape(radius);
  antShape.setOrigin({radius, radius});
  antShape.setScale({3, 1});
  antShape.setPosition(m_pos);
  return antShape;
}