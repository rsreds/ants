#if !defined(ANTS_MARKER_H)
#define ANTS_MARKER_H

#include <SFML/Graphics.hpp>

namespace ants {

enum MarkerType { ToBase };

class Marker : public sf::Transformable {
 private:
  MarkerType m_type;
  float m_remainingLife;

 public:
  Marker(sf::Vector2f pos, MarkerType type, float remainingLife = 100)
      : m_type(type), m_remainingLife(remainingLife) {
    setPosition(pos);
  }
  void tickLife(float const& elapsedTime);
  float getRemainingLife() const;
};

inline void Marker::tickLife(float const& elapsedTime) {
  m_remainingLife -= elapsedTime * 10;
}

inline float Marker::getRemainingLife() const { return m_remainingLife; }

}  // namespace ants

#endif  // ANTS_MARKER_H
