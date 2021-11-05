#ifndef ANTS_ANT_H
#define ANTS_ANT_H

#include <SFML/Graphics.hpp>
#include <gui/theme.hpp>

namespace ants {
class Ant : public sf::Drawable, public sf::Transformable {
 private:
  float m_speed = 100;
  sf::Vector2f m_direction{0.f, 0.f};
  sf::Color m_color;

 public:
  explicit Ant(sf::Vector2f pos);
  void move(float const& elapsedTime);
  sf::Vector2f getDirection() const;
  float setSpeed() const;
  void setDirection(sf::Vector2f const& dir);
  void setSpeed(float const& spd);
  void setFillColor(const sf::Color& c);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
}  // namespace ants

#endif  // MACRO
