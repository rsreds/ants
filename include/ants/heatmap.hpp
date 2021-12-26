#ifndef ANTS_HEATMAP_HPP
#define ANTS_HEATMAP_HPP

#include <array>
#include <iostream>

#include "ants/marker.hpp"

namespace ants {
// Rows x Columns
template <size_t COLS, size_t ROWS>
using Map_t = std::array<std::array<int, COLS>, ROWS>;

struct HeatmapIndex {
  size_t col;
  size_t row;
};

template <size_t COLS, size_t ROWS>
inline void printMap(const Map_t<COLS, ROWS>& map) {
  for (auto &row : map) {
    for (auto el : row) {
      std::cout << std::setfill(' ') << std::setw(3) << el << ' ';
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
  sf::Vector2u m_windowSize;

 public:
  /**
   * Initialize Heatmap
   * @param windowSize Screen windows size to compute columns and rows sizes
   */
  explicit Heatmap(sf::Vector2u const& windowSize) : m_windowSize{windowSize} {}

  void print() const { printMap(m_map); }

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

  inline sf::Vector2f getPositionFromIndex(HeatmapIndex const& index) {
    float w = m_windowSize.x / static_cast<float>(COLS);
    float h = m_windowSize.y / static_cast<float>(ROWS);
    return {static_cast<float>(index.col) * w + w / 2.f,
            static_cast<float>(index.row) * h + h / 2.f};
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
  void findNeighbours(sf::Vector2f const& position, std::array<int, 8>& result,
                      std::array<HeatmapIndex, 8>& indexes) {
    auto index = getIndexFromPosition(position);

    // Add zero padding to all side of the map
    Map_t<COLS + 2, ROWS + 2> paddedMap{};

    // Copy the entire row data to the padded map
    // Fixme we could directly use the padded map instead of two separated maps
    for (size_t row = 1; row < ROWS + 1; ++row) {
      std::copy(m_map.at(row - 1).begin(), m_map.at(row - 1).end(),
                paddedMap.at(row).begin() + 1);
    }

    // Compute the neighbours
    for (size_t t = 0; t < indexes.size(); ++t) {
      auto idx = adjacentFromCardinalDirection(index, t);
      // Padded map is shifted by (1, 1)
      result[t] = paddedMap.at(idx.row + 1).at(idx.col + 1);
    }
  }

 public:
  /**
   * Get from a given index its neighbour which points to a given cardinal
   * direction
   * @param index The cell we are asking for its neighbour
   * @param direction The direction of the neighbour, starting from N clockwise.
   * E.g. direction = 3 means SE
   * @return Index of the neighbour in the given direction
   */
  static inline constexpr HeatmapIndex adjacentFromCardinalDirection(
      HeatmapIndex const& index, int direction) {
    std::array<HeatmapIndex, 8> cardinalDirections{{
        {index.col, index.row - 1},      // N
        {index.col + 1, index.row - 1},  // NE
        {index.col + 1, index.row},      // E
        {index.col + 1, index.row + 1},  // SE
        {index.col, index.row + 1},      // S
        {index.col - 1, index.row + 1},  // SW
        {index.col - 1, index.row},      // W
        {index.col - 1, index.row - 1}   // NW
    }};
    return cardinalDirections[direction];
  }
};
}  // namespace ants
#endif  // ANTS_HEATMAP_HPP
