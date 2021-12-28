#if !defined(ANTS_ANTHILL_H)
#define ANTS_ANTHILL_H

#include <SFML/Graphics.hpp>
namespace ants {
class Anthill : public sf::CircleShape {
 public:
  explicit Anthill(sf::Vector2f pos, float radius = 20.0f) {
    setPosition(pos);
    setRadius(radius);
    setOrigin({radius, radius});
  }
};
}  // namespace ants

#endif  // ANTS_ANTHILL_H
