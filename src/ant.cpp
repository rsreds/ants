#include <ants/ant.hpp>

Ant::Ant(int posX, int posY) : m_posX(posX), m_posY(posY) {}

sf::CircleShape Ant::makeShape() {
  float radius = 3;
  auto antShape = sf::CircleShape(radius);
  antShape.setOrigin({radius, radius});
  antShape.setScale({3, 1});
  antShape.setPosition({m_posX, m_posY});
  return antShape;
}