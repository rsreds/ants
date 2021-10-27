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

void Ant::setFillColor(const sf::Color& c) { m_shape.setFillColor(c); }

void Ant::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(m_shape, states);
}

}  // namespace ants