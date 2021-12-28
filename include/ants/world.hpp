#if !defined(ANTS_WORLD_H)
#define ANTS_WORLD_H

#include <ants/colony.hpp>
#include <ants/food.hpp>
#include <ants/heatmap.hpp>
#include <ants/marker.hpp>
#include <cmath>
#include <random>
#include <vector>

namespace ants {

template <size_t COLS, size_t ROWS>
class World {
 private:
  std::vector<Colony> m_colonies;
  std::vector<FoodSource> m_foodSources;
  std::vector<Marker> m_markers;

  sf::Vector2u m_worldSize;
  GUI::ThemeManager &m_themeManager;
  float m_markersLifetime = 100.0f;
  float m_huntingTimeout = 100.0f;
  float m_randomness = 0.1f;

  std::array<Heatmap<COLS, ROWS>, 2> m_heatMaps;

 public:
  explicit World(sf::Vector2u const& worldSize, GUI::ThemeManager &themeManager)
      : m_worldSize{worldSize},
        m_themeManager{themeManager},
        m_heatMaps{Heatmap<COLS, ROWS>{worldSize},
                   Heatmap<COLS, ROWS>{worldSize}} {
    m_colonies.emplace_back(sf::Vector2f{150.f, 300.f}, 200);

    int amount = 10'000;
    for (auto& colony : m_colonies)
      addSourceToHeatmap(colony.getAnthill(), m_heatMaps.at(toBase), amount);
    for (auto& food : m_foodSources)
      addSourceToHeatmap(food, m_heatMaps.at(toFood), amount);

    m_heatMaps.at(toFood).setColor(sf::Color::Blue);
    m_heatMaps.at(toBase).setColor(sf::Color::Red);
  }
  std::vector<Colony>& getColonies() { return m_colonies; }
  std::vector<FoodSource>& getFoodSources() { return m_foodSources; }
  std::vector<Marker>& getMarkers() { return m_markers; }
  Heatmap<COLS, ROWS>& getHeatmap(const MarkerType type) {
    return m_heatMaps.at(type);
  }
  [[nodiscard]] float getMarkersLifetime() const { return m_markersLifetime; }
  void setMarkersLifetime(float lifeTime) { m_markersLifetime = lifeTime; }
  [[nodiscard]] float getHuntingTimeout() const { return m_huntingTimeout; }
  void setHuntingTimeout(float timeout) { m_huntingTimeout = timeout; }
  void setRandomness(float rnd);
  [[nodiscard]] float getRandomness() const;
  void updateColonies(float elapsedTime);
  void updateMarkers(float elapsedTime);
  void addFoodSource(const FoodSource& foodSource);
  void reset();

 private:
  void checkBounds(Ant& ant) const;
  void updateAnt(Colony& colony, Ant& ant, float elapsedTime);
  void addSourceToHeatmap(sf::Shape const& source, Heatmap<COLS, ROWS>& map,
                          int value);
};

// Add food/base marker at sources locations
template <size_t COLS, size_t ROWS>
void World<COLS, ROWS>::addSourceToHeatmap(sf::Shape const& source,
                                           Heatmap<COLS, ROWS>& map,
                                           int value) {
  auto minX = source.getGlobalBounds().left;
  auto minY = source.getGlobalBounds().top;
  auto maxX = minX + source.getGlobalBounds().width;
  auto maxY = minY + source.getGlobalBounds().height;

  HeatmapIndex topLeft = map.getIndexFromPosition({minX, minY});
  HeatmapIndex bottomRight = map.getIndexFromPosition({maxX, maxY});

  for (size_t row = topLeft.row; row <= bottomRight.row; ++row) {
    for (size_t col = topLeft.col; col <= bottomRight.col; ++col) {
      map.setAmountAtIndex({col, row}, value);
    }
  }
};

template <size_t COLS, size_t ROWS>
void World<COLS, ROWS>::addFoodSource(const FoodSource& foodSource) {
  m_foodSources.emplace_back(foodSource);
  m_foodSources.back().setFillColor(m_themeManager.foodColor());
  addSourceToHeatmap(foodSource, m_heatMaps.at(toFood), foodSource.getAmount() * 100);
}

template <size_t COLS, size_t ROWS>
void World<COLS, ROWS>::reset() {
  m_foodSources.clear();
  for (auto &colony : m_colonies)
    colony.m_ants.clear();
  m_markers.clear();
  m_heatMaps.at(toFood).clear();
  m_heatMaps.at(toBase).clear();
}

template <size_t COLS, size_t ROWS>
void World<COLS, ROWS>::setRandomness(float rnd) {
  m_randomness = rnd;
  for (auto &colony : m_colonies) {
    for (auto &ant : colony.m_ants)
      ant.setRandomness(rnd);
  }
}

template <size_t COLS, size_t ROWS>
float World<COLS, ROWS>::getRandomness() const {
  return m_randomness;
}

template <size_t COLS, size_t ROWS>
inline void World<COLS, ROWS>::updateColonies(float elapsedTime) {
  for (auto& colony : m_colonies)
    for (auto& ant : colony.m_ants) {
      updateAnt(colony, ant, elapsedTime);
      ant.updatePosition(elapsedTime);
    }
}
template <size_t COLS, size_t ROWS>
inline void World<COLS, ROWS>::updateAnt(Colony& colony, Ant& ant,
                                         float elapsedTime) {
  ant.tickHuntingTimer(elapsedTime);
  // This function returns if the ant is out of screen
  checkBounds(ant);

  AntState currentState = ant.getState();
  switch (currentState) {
    default: {
      // Ant is within the Anthill
      if (colony.getAnthill().getGlobalBounds().contains(ant.getPosition())) {
        // If we are coming from a food source, then turn back
        if (currentState != noSuccess)
          ant.setDirection(ant.getDirection() * -1.f);
        ant.setState(leavingAnthill);
        ant.resetHuntingTimer();
        return;
      }
      break;
    }
    case AntState::leavingAnthill: {
      for (auto& foodSource : m_foodSources) {
        // Ant is within the food source
        if (foodSource.getGlobalBounds().contains(ant.getPosition())) {
          ant.setState(returningAnthill);
          // Come back to where we arrived from
          ant.setDirection(ant.getDirection() * -1.f);
          ant.resetHuntingTimer();
          return;
        }
      }
      break;
    }
  }

  // Ops, got lost.
  if (ant.getRemainingTimeToHunt() <= 0) {
    ant.setState(noSuccess);
    ant.setHuntingTimeout(m_huntingTimeout);
    return;
  }

  if (m_huntingTimeout <= 0) return;

  // Still check we are within screen view
  if (ant.getPosition().x < 1 ||
      ant.getPosition().x > static_cast<float>(m_worldSize.x) - 1 ||
      ant.getPosition().y < 1 ||
      ant.getPosition().y > static_cast<float>(m_worldSize.y) - 1) {
    return;
  }

  // We don't care which map use to get ant's index
  auto antIndexInHeatmap =
      m_heatMaps.at(toFood).getIndexFromPosition(ant.getPosition());

  // Add ant marker only if we did not get lost
  if (ant.getState() != noSuccess) {
    auto dropped = ant.dropMarker(m_markersLifetime);
    m_markers.push_back(dropped);
    auto cellValue = m_heatMaps.at(dropped.getType()).getValueAtIndex(antIndexInHeatmap);
    if (cellValue < 2550) {
      m_heatMaps.at(dropped.getType())
          .incrementByOneAtPosition(dropped.getPosition());
    }
  }

  // Choose which mark to follow: if we are leaving the anthill search for food,
  // in any other case, search for home
  MarkerType targetMarker = currentState == leavingAnthill ? toFood : toBase;

  // Find 8 closest neighbours
  // Cardinal points order: [N, NE, W, SE, S, SW, W, NW]
  std::array<int, 8> neighbours{};
  std::array<HeatmapIndex, 8> neighboursIndexes{};
  m_heatMaps.at(targetMarker)
      .findNeighbours(ant.getPosition(), neighbours, neighboursIndexes);

  // Neighbours padding
  // We copy the first viewWindow elements at the end to handle window at north
  // E.g. padded: [N, NE, W, SE, S, SW, W, NW, N, NE, E]
  constexpr int viewWindow = 3;
  std::array<int, 8 + viewWindow> paddedNeighbours{};
  for (size_t i = 0; i < paddedNeighbours.size(); ++i) {
    paddedNeighbours.at(i) = neighbours.at(i % neighbours.size());
  }

  // Get ant current heading and map it to [0, 360)
  const int currentHeading =
      static_cast<int>(ant.getHeading() + 90 + 360) % 360;

  // Sample heading with steps of 45°. Now heading is the index of
  // N = 0°, NE = 45°, E = 90°, SE = 135°, S = 180°, SW = 225°, NW = 315°
  // cardinal directions: i = 0 -> N; ...; i = 4 -> S; ... i = 7 -> NW;
  int heading = currentHeading / 45;
  assert(heading >= 0 && heading < 8);

  // Account for heading looking at N: min = NW, max = NE
  // Looking at North
  if (heading == 0) heading += 8;

  int minHeading = heading - 1;  // Min view angle
  int maxHeading = heading + 1;  // Max view angle
  assert(minHeading >= 0 && minHeading < (8 + viewWindow - 1));
  assert(maxHeading >= (viewWindow - 1) && maxHeading < (8 + viewWindow));

  // From neighbour sample the three at Front Left, Front and Front Right
  auto FL = paddedNeighbours.at(minHeading);
  auto F = paddedNeighbours.at(heading);
  auto FR = paddedNeighbours.at(maxHeading);

  // Set direction looking forward
  auto direction = heading;

  if (F < FL && F < FR) {
    // Rotate by towards larger of FL and FR
    direction = FL > FR ? minHeading : maxHeading;
  } else if (FL > FR) {
    // Rotate left
    direction = minHeading;
  } else if (FL < FR) {
    // Rotate right
    direction = maxHeading;
  }

  // Remove padding
  if (direction > 7) direction -= 8;
  if (heading > 7) heading -= 8;

  assert(direction >= 0 && direction < 8);

  if (direction != heading) {
    // Account for heatmap edges
    if (antIndexInHeatmap.col == 0)
      antIndexInHeatmap.col += 1;
    else if (antIndexInHeatmap.col == COLS - 1)
      antIndexInHeatmap.col -= 1;
    if (antIndexInHeatmap.row == 0)
      antIndexInHeatmap.row += 1;
    else if (antIndexInHeatmap.row == ROWS - 1)
      antIndexInHeatmap.row -= 1;

    // Now that we know in which cardinal direction to go [N, NE, ..., NE]
    // we ask for the adjacent cell in that direction
    auto targetIndex = Heatmap<COLS, ROWS>::adjacentFromCardinalDirection(
        antIndexInHeatmap, direction);

    // Get the position of the cell centre
    auto targetPosition =
        m_heatMaps.at(targetMarker).getPositionFromIndex(targetIndex);

    // Compute new direction
    auto newDirection = targetPosition - ant.getPosition();
    sf::normalize(newDirection);

    auto currentDirection = ant.getDirection();
    // Smooth down the steering velocity
    ant.setDirection(sf::lerp(currentDirection, newDirection, 0.3f));
  }
}

template <size_t COLS, size_t ROWS>
inline void World<COLS, ROWS>::updateMarkers(float elapsedTime) {
  for (auto& m : m_markers) {
    if (m.getType() == isFood || m.getType() == isBase) continue;
    m.tickLife(elapsedTime);
    if (m.getRemainingLife() <= 0 && (m.getType()))
      m_heatMaps.at(m.getType()).decrementByOneAtPosition(m.getPosition());
  }

  m_markers.erase(std::remove_if(m_markers.begin(), m_markers.end(),
                                 [](const ants::Marker& m) {
                                   return m.getRemainingLife() <= 0;
                                 }),
                  m_markers.end());
}

template <size_t COLS, size_t ROWS>
inline void World<COLS, ROWS>::checkBounds(Ant& ant) const {
  auto x = ant.getPosition().x;
  auto y = ant.getPosition().y;

  float turnFactor = 0.2;
  float margin = 0.f;
  auto currentDirection = ant.getDirection();

  if (x <= margin) {
    currentDirection.x += turnFactor;
    ant.setDirection(currentDirection);
    return;
  } else if (x >= static_cast<float>(m_worldSize.x) - margin) {
    currentDirection.x -= turnFactor;
    ant.setDirection(currentDirection);
    return;
  }
  if (y <= margin) {
    currentDirection.y += turnFactor;
    ant.setDirection(currentDirection);
    return;
  } else if (y >= static_cast<float>(m_worldSize.y) - margin) {
    currentDirection.y -= turnFactor;
    ant.setDirection(currentDirection);
    return;
  }
}
}  // namespace ants

#endif  // ANTS_WORLD_H
