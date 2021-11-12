#if !defined(ANTS_MARKER_H)
#define ANTS_MARKER_H

#include <vector2/vector2.h>

#include <SFML/Graphics.hpp>

namespace ants {

enum MarkerType { toBase, toFood };

class Marker : public sf::Transformable {
 private:
  MarkerType m_type;
  float m_remainingLife;

 public:
  Marker() : m_remainingLife(0){};
  Marker(sf::Vector2f pos, MarkerType type, float remainingLife = 100.f)
      : m_type(type), m_remainingLife(remainingLife) {
    setPosition(pos);
  }
  void tickLife(float const& elapsedTime);
  float getRemainingLife() const;
  MarkerType getType() const;
};

inline void Marker::tickLife(float const& elapsedTime) {
  m_remainingLife -= elapsedTime * 10;
}

inline float Marker::getRemainingLife() const { return m_remainingLife; }
inline MarkerType Marker::getType() const { return m_type; }
inline Marker const& findStrogestAdjacent(sf::Vector2f pos, MarkerType type,
                                          std::vector<ants::Marker>& markers) {
  auto bestMarker = markers.begin();
  for (auto it = markers.begin(); it < markers.end(); ++it) {
    if (sf::distanceBetween(it->getPosition(), pos) <= 1.f &&
        it->getType() == type)
      bestMarker = bestMarker->getRemainingLife() < it->getRemainingLife()
                       ? it
                       : bestMarker;
  }
  return *bestMarker;
};
}  // namespace ants

#endif  // ANTS_MARKER_H
