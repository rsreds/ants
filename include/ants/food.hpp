#if !defined(ANTS_FOOD_H)
#define ANTS_FOOD_H

#include <SFML/Graphics.hpp>

namespace ants {
class FoodSource : public sf::CircleShape {
 private:
  float m_amount;

 public:
  FoodSource(float amount, sf::Vector2f pos) : m_amount(amount) {
    setPosition(pos);
    float amountRadiusRatio = 0.2f;
    float new_radius = m_amount * amountRadiusRatio;
    setRadius(new_radius);
    setOrigin({new_radius, new_radius});
  }
};

}  // namespace ants

#endif  // ANTS_FOOD_H
