#ifndef ANTS_ANT_H
#define ANTS_ANT_H

#include <SFML/Graphics.hpp>

namespace ants {
class Ant : public sf::Drawable, public sf::Transformable {
 private:
  sf::CircleShape m_shape;
  float m_speed;
  sf::Vector2f m_direction;

 public:
  Ant(sf::Vector2f pos);
  sf::Vector2f getDirection() const;
  float setSpeed() const;
  void setDirection(sf::Vector2f const& dir);
  void setSpeed(float const& spd);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
}  // namespace ants

#endif  // MACRO
