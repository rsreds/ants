#if !defined(ANTS_WORLD_H)
#define ANTS_WORLD_H

#include <ants/colony.hpp>
#include <ants/food.hpp>
#include <ants/marker.hpp>
#include <ants/heatmap.h>
#include <random>
#include <vector>
#include <cmath>

namespace ants {

class World {
 private:
  std::vector<Colony> m_colonies;
  std::vector<FoodSource> m_foodSources;
  std::vector<Marker> m_markers;
  sf::Vector2u m_worldSize;
  static constexpr int COLS = 32 * 3;
  static constexpr int ROWS = 24 * 3;
  std::array<Heatmap<World::COLS, World::ROWS>, MarkerType::allMarkers>
      m_heatMaps;

 public:
  explicit World(sf::Vector2u const& worldSize)
      : m_worldSize{worldSize},
        m_heatMaps{Heatmap<World::COLS, World::ROWS>{worldSize},
                   Heatmap<World::COLS, World::ROWS>{worldSize}} {
    m_colonies.emplace_back(sf::Vector2f{150.f, 300.f}, 100);
    m_foodSources.emplace_back(100, sf::Vector2f{350.f, 450.f});
  }
  std::vector<Colony>& getColonies() { return m_colonies; }
  std::vector<FoodSource>& getFoodSources() { return m_foodSources; }
  std::vector<Marker>& getMarkers() { return m_markers; }
  void updateColonies(float elapsedTime);
  void updateMarkers(float elapsedTime);

 private:
  void checkBounds(Ant& ant) const;
  void updateAnt(Colony& colony, Ant& ant);
};

inline sf::Vector2f randomDirection() {
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_real_distribution<float> uniform_dist(-0.1, 0.1);
  float r1 = uniform_dist(e1);
  float r2 = uniform_dist(e1);
  return {r1, r2};
}

inline void World::updateColonies(float elapsedTime) {
  for (auto& colony : m_colonies)
    for (auto& ant : colony.m_ants) {
      updateAnt(colony, ant);
      ant.updatePosition(elapsedTime);
    }
}

inline void World::updateAnt(Colony& colony, Ant& ant) {
  AntState currentState = ant.getState();

  switch (currentState) {
    case AntState::returningAnthill: {
      // Ant is within the Anthill
      if (colony.getAnthill().getGlobalBounds().contains(ant.getPosition())) {
        ant.setState(leavingAnthill);
        ant.setDirection(ant.getDirection() * -1.f);
        return;
      }
      break;
    }
    case AntState::leavingAnthill: {
      for (auto& foodSource : m_foodSources) {
        // Ant is within the food source
        if (foodSource.getGlobalBounds().contains(ant.getPosition())) {
          ant.setState(returningAnthill);
          ant.setDirection(ant.getDirection() * -1.f);
          std::cout << "Rotate!\n";
          return;
        }
      }
      break;
    }
  }

  // This function returns if the ant is out of screen
  checkBounds(ant);

  if (ant.getPosition().x < 1 ||
      ant.getPosition().x > static_cast<float>(m_worldSize.x) - 1 ||
      ant.getPosition().y < 1 ||
      ant.getPosition().y > static_cast<float>(m_worldSize.y) - 1) {
    return;
  }

  // Add ant marker
  auto dropped = ant.dropMarker();
  m_markers.push_back(dropped);
  m_heatMaps.at(dropped.getType()).incrementByOneAt(dropped.getPosition());

  // Find the strongest neighbour
  // Cardinal points order: [N, NE, W, SE, S, SW, W, NW]
  //                                                          | kernel|
  // Padded Cardinal neighbours: [N, NE, W, SE, S, SW, W, NW, N, NE, E]
  //
  // N = 0°, NE = 45°, E = 90°, SE = 135°, S = 180°, SW = 225°, NW = 315°

  constexpr int kernelSize = 5;
  std::array<int, 8> neighbours{};
  std::array<int, 8 + kernelSize> paddedNeighbours{};
  std::array<HeatmapIndex, 8> neighboursIndexes{};
  MarkerType targetMarker = ant.getState() == leavingAnthill ? toFood : toBase;

  m_heatMaps.at(targetMarker)
      .findNeighbours(ant.getPosition(), neighbours, neighboursIndexes);

  for (size_t i = 0; i < paddedNeighbours.size(); ++i) {
    paddedNeighbours.at(i) = neighbours.at(i % neighbours.size());
  }

  // Map heading in [0, 360)
  const int currentHeading =
      static_cast<int>(ant.getHeading() + 90 + 360) % 360;

  // Sample heading with steps of 45°. Now heading is the index of
  // cardinal directions: i = 0 -> N; ...; i = 4 -> S; ... i = 7 -> NW;
  int heading = currentHeading / 45;
  assert(heading >= 0 && heading < 8);

  // Account for heading looking at N: min = NW, max = NE
  // Looking at North
  if (heading == 0) heading += 8;

  int minHeading = heading - 2;  // Min view angle
  int maxHeading = heading + 2;  // Max view angle
//  assert(minHeading >= 0 && minHeading < (8 + kernelSize - 1));
//  assert(maxHeading >= (kernelSize - 1) && maxHeading < (8 + kernelSize));

  // Restrict search only between min and max view angle
  auto highestMarkerConcentration =
      std::max(paddedNeighbours.begin() + minHeading,
               paddedNeighbours.begin() + maxHeading);

  if (*highestMarkerConcentration != 0) {
    // Find the cardinal direction index with the highest concentration
    int direction = static_cast<int>(highestMarkerConcentration - paddedNeighbours.begin());

    // Remove padding
    if (direction > 7) direction -= 8;
    assert(direction < 8);

    auto antIndexInHeatmap =
        m_heatMaps.at(targetMarker).getIndexFromPosition(ant.getPosition());
    auto targetIndex = Heatmap<COLS, ROWS>::adjacentFromCardinalDirection(
        antIndexInHeatmap, direction);

    auto targetPosition = m_heatMaps.at(targetMarker).getPositionFromIndex(targetIndex);
    auto newDirection = targetPosition - ant.getPosition();
    ant.setDirection(newDirection);
  }
  ant.setDirection(ant.getDirection() + randomDirection());
  //  ant.setDirection(90);
}

inline void World::updateMarkers(float elapsedTime) {
  for (auto& m : m_markers) {
    m.tickLife(elapsedTime);
    if (m.getRemainingLife() <= 0)
      m_heatMaps.at(m.getType()).decrementByOneAt(m.getPosition());
  }

  m_markers.erase(std::remove_if(m_markers.begin(), m_markers.end(),
                                 [](const ants::Marker& m) {
                                   return m.getRemainingLife() <= 0;
                                 }),
                  m_markers.end());
}

inline void World::checkBounds(Ant& ant) const {
  auto x = ant.getPosition().x;
  auto y = ant.getPosition().y;

  float turnFactor = 0.05;
  float margin = 25.f;
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
