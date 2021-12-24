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
enum MarkerType { toBase, toFood };

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

// Rows x Columns
template <size_t COLS, size_t ROWS>
using Map_t = std::array<std::array<int, COLS>, ROWS>;

struct HeatmapIndex {
  size_t col;
  size_t row;
};

template <size_t COLS, size_t ROWS>
void printMap(const Map_t<COLS, ROWS>& map) {
  for (auto row : map) {
    for (auto el : row) {
      std::cout <<  std::setfill(' ') << std::setw(3) << el << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

/**
 * Static 2D Container (a.k.a 2D fixed array)
 * @tparam COLS Number of columns
 * @tparam ROWS Number of rows
 */
template <size_t COLS, size_t ROWS>
class Heatmap {
 private:
  Map_t<COLS, ROWS> m_map{};  // rows x cols
  sf::Vector2i m_windowSize;

 public:
  /**
   * Initialize Heatmap
   * @param windowSize Screen windows size to compute columns and rows sizes
   */
  explicit Heatmap(sf::Vector2i const& windowSize) : m_windowSize{windowSize} {}

  void print() const {
    printMap(m_map);
  }

  /**
   * Get the corresponding 2D index from screen position in pixels
   * @param position World position in pixel
   * @return HeatmapIndex
   */
  inline HeatmapIndex getIndexFromPosition(sf::Vector2f const& position) {
    assert(position.x >= 0 && position.x <= m_windowSize.x && position.y >= 0 &&
           position.y <= m_windowSize.y);
    size_t col = std::floor(position.x / m_windowSize.x * COLS);
    size_t row = std::floor(position.y / m_windowSize.y * ROWS);
    return {col, row};
  }

  /**
   * Increment by one the value at the corresponding element in world
   * coordinates
   * @param position World position in pixel
   */
  inline void incrementByOneAt(sf::Vector2f const& position) {
    auto index = getIndexFromPosition(position);
    m_map.at(index.row).at(index.col) += 1;
  }

  /**
   * Decrement by one the value at the corresponding element in world
   * coordinates
   * @param position World position in pixel
   */
  inline void decrementByOneAt(sf::Vector2f const& position) {
    auto index = getIndexFromPosition(position);
    if (m_map.at(index.row).at(index.col) > 0)
      m_map.at(index.row).at(index.col) -= 1;
  }

  /**
   * Ge element's value at HeatmapIndex
   * @param position World position in pixel
   */
  [[nodiscard]] int getValueAtIndex(HeatmapIndex index) const {
    assert(index.col >= 0 && index.col < COLS && index.row >= 0 &&
           index.row < COLS);
    return m_map.at(index.row).at(index.col);
  }

  /**
   * Given a point in World coordinates, returns the height adjacent cells
   * values
   * @param position World position in pixel
   * @param result
   *
   */
  void findNeighbours(sf::Vector2f const& position,
                      std::array<int, 8>& result) {
    auto index = getIndexFromPosition(position);

    // Add zero padding to all side of the map
    Map_t<COLS + 2, ROWS + 2> paddedMap{};

    // Copy the entire row data to the padded map
    for (size_t row = 1; row < ROWS + 1; ++row) {
       std::copy(m_map.at(row - 1).begin(), m_map.at(row - 1).end(), paddedMap.at(row).begin() + 1);
    }

    // Padded map is shifted by (1, 1)
    index.row += 1;
    index.col += 1;

    result[0] = paddedMap.at(index.row - 1).at(index.col - 1);  // NW
    result[1] = paddedMap.at(index.row - 1).at(index.col);      // N
    result[2] = paddedMap.at(index.row - 1).at(index.col + 1);  // NE
    result[3] = paddedMap.at(index.row).at(index.col - 1);      // W
    result[4] = paddedMap.at(index.row).at(index.col + 1);      // E
    result[5] = paddedMap.at(index.row + 1).at(index.col - 1);  // SW
    result[6] = paddedMap.at(index.row + 1).at(index.col);      // S
    result[7] = paddedMap.at(index.row + 1).at(index.col + 1);  // SE
  }
};
}  // namespace ants

#endif  // ANTS_MARKER_H
