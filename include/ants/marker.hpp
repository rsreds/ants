#if !defined(ANTS_MARKER_H)
#define ANTS_MARKER_H

#include <vector2/vector2.h>

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

namespace ants {

// Keep SIZE as last element to compute how big the enum is
enum MarkerType { toBase, toFood, SIZE };

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
};

template <size_t M, size_t N>
using Map_t = std::array<std::array<int, M>, N>;
template <size_t Size>
using AllMarkers_t = std::array<std::array<int, Size>, MarkerType::SIZE>;

template <size_t M, size_t N>
class Heatmap {
 private:
  Map_t<M, N> m_map{};  // rows x cols
  float m_dw;           // Box's width
  float m_dh;           // Box's height

 public:
  explicit Heatmap(sf::Vector2i const& windowSize)
      : m_dw{static_cast<float>(windowSize.x) / M},
        m_dh{static_cast<float>(windowSize.y) / N} {}

  void printHeatMap() const {
    for (auto x : m_map) {
      for (auto y : x) {
        std::cout << y << ' ';
      }
      std::cout << '\n';
    }
  }

  inline void incrementByOneAt(sf::Vector2f position) {
    assert(position.x >= 0 && position.y >= 0);
    int i = std::floor(position.x / m_dw);
    int j = std::floor(position.y / m_dh);
    m_map.at(i).at(j) += 1;
  }

  inline void decrementByOneAt(sf::Vector2f position) {
    assert(position.x >= 0 && position.y >= 0);
    int i = std::floor(position.x / m_dw);
    int j = std::floor(position.y / m_dh);
    if (m_map.at(i).at(j) > 0)
      m_map.at(i).at(j) -= 1;
  }

  [[nodiscard]] int getValueAtIndex(int i, int j) const { return m_map.at(i).at(j); }
};
}  // namespace ants

#endif  // ANTS_MARKER_H
