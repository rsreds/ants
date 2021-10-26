#include <ants/ant.hpp>

Ant::Ant(sf::Vector2f pos) : m_pos(pos) {}

sf::CircleShape Ant::makeShape() {
  float radius = 1;
  auto antShape = sf::CircleShape(radius);
  antShape.setOrigin({radius, radius});
  antShape.setScale({3, 1});
  antShape.setPosition(m_pos);
  return antShape;
}

sf::Vector2f Ant::getPos() const{
  return this->m_pos;
}

void Ant::setPos(sf::Vector2f const& pos){
  this->m_pos = pos;
}