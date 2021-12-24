#if !defined(ANTS_MARKER_H)
#define ANTS_MARKER_H

#include <vector2/vector2.h>

#include <SFML/Graphics.hpp>
#include <array>
#include <cassert>
#include <iostream>
#include <iomanip>

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

// Rows x Columns
template <size_t COLS, size_t ROWS>
using Map_t = std::array<std::array<int, COLS>, ROWS>;
template <size_t Size>
using AllMarkers_t = std::array<std::array<int, Size>, MarkerType::SIZE>;
struct HeatMapIndex {
  int row;
  int col;
};

template <size_t COLS, size_t ROWS>
class Heatmap {
 private:
  Map_t<COLS, ROWS> m_map{};  // rows x cols
  sf::Vector2i m_windowSize;
  float m_dw;  // Box's width
  float m_dh;  // Box's height

 public:
  explicit Heatmap(sf::Vector2i const& windowSize)
      : m_windowSize{windowSize},
        m_dw{static_cast<float>(windowSize.x) / COLS},
        m_dh{static_cast<float>(windowSize.y) / ROWS} {}

  void printHeatMap() const {
    for (auto row : m_map) {
      for (auto el : row) {
        std::cout <<  std::setfill(' ') << std::setw(3) << el << ' ';
      }
      std::cout << '\n';
    }
    std::cout << '\n';
  }

  [[nodiscard]] inline sf::Vector2f getCellSize() const { return {m_dw, m_dh}; }

  inline HeatMapIndex getIndexFromPosition(sf::Vector2f const& position) {
    assert(position.x >= 0 && position.x <= m_windowSize.x &&
           position.y >= 0 && position.y <= m_windowSize.y);
    int col = std::floor(position.x / m_windowSize.x * COLS);
    int row = std::floor(position.y / m_windowSize.y * ROWS);
    return {row, col};
  }

  inline void incrementByOneAt(sf::Vector2f const& position) {
    auto index = getIndexFromPosition(position);
    m_map.at(index.row).at(index.col) += 1;
  }

  inline void decrementByOneAt(sf::Vector2f position) {
    auto index = getIndexFromPosition(position);
    if (m_map.at(index.row).at(index.col) > 0)
      m_map.at(index.row).at(index.col) -= 1;
  }

  [[nodiscard]] int getValueAtIndex(int col, int row) const {
    assert(col >= 0 && col < COLS && row >= 0 && row < COLS);
    return m_map.at(row).at(col);
  }
};
}  // namespace ants

#endif  // ANTS_MARKER_H
