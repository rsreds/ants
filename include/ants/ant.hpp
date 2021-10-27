#ifndef ANTS_ANT_H
#define ANTS_ANT_H

#include <SFML/Graphics.hpp>
#include <gui/theme.hpp>

namespace ants {
class Ant : public sf::Drawable, public sf::Transformable {
 private:
  sf::CircleShape m_shape;
  sf::Color m_color;

 public:
  explicit Ant(sf::Vector2f pos);
  void setFillColor(const sf::Color& c);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
}  // namespace ants

#endif  // MACRO
