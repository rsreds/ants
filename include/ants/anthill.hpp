#if !defined(ANTS_ANTHILL_H)
#define ANTS_ANTHILL_H

#include <SFML/Graphics.hpp>
namespace ants {
class Anthill : public sf::CircleShape {
 public:
  Anthill(sf::Vector2f pos) {
    setPosition(pos);
    setRadius(20);
  }
};
}  // namespace ants

#endif  // ANTS_ANTHILL_H
