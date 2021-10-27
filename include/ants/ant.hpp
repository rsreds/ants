#ifndef ANTS_ANT_H
#define ANTS_ANT_H

#include <SFML/Graphics.hpp>

namespace ants {
class Ant : public sf::Drawable, public sf::Transformable {
 private:
  sf::CircleShape m_shape;

 public:
  Ant(sf::Vector2f pos);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
}  // namespace ants

#endif  // MACRO
