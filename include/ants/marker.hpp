#if !defined(ANTS_MARKER_H)
#define ANTS_MARKER_H

#include <vector2/vector2.h>

#include <SFML/Graphics.hpp>
#include <array>
#include <cassert>
#include <iomanip>
#include <iostream>

namespace ants {

// Keep SIZE as last element to compute how big the enum is
enum MarkerType { toBase, toFood, allMarkers };

class Marker : public sf::Transformable {
 private:
  MarkerType m_type;
  float m_remainingLife;

 public:
  Marker() : m_type{}, m_remainingLife(0){};
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

inline std::vector<Marker>::iterator findStrongestAdjacent(
    sf::Vector2f pos, MarkerType targetMarker, std::vector<Marker>& markers) {
  std::vector<Marker> adjacentMarkers;

  auto condition = [&](Marker& m) {
    auto d = sf::distanceBetween(m.getPosition(), pos);
    return m.getType() == targetMarker && d < 1;
  };

  std::copy_if(markers.begin(), markers.end(),
               std::back_inserter(adjacentMarkers), condition);

  if (adjacentMarkers.empty()) return markers.end();

  return std::max_element(adjacentMarkers.begin(), adjacentMarkers.end(),
                          [](Marker& a, Marker& b) {
                            return a.getRemainingLife() < b.getRemainingLife();
                          });
}
}  // namespace ants

#endif  // ANTS_MARKER_H
