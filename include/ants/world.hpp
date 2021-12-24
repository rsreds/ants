#if !defined(ANTS_WORLD_H)
#define ANTS_WORLD_H

#include <ants/colony.hpp>
#include <ants/food.hpp>
#include <ants/marker.hpp>
#include <random>
#include <vector>

namespace ants {

class World {
 private:
  std::vector<Colony> m_colonies;
  std::vector<FoodSource> m_foodSources;
  std::vector<Marker> m_markers;
  sf::Vector2i m_worldSize;
  Heatmap<32, 24> m_heatMap;

 public:
  explicit World(sf::Vector2i const& worldSize)
      : m_worldSize{worldSize}, m_heatMap{worldSize} {
    m_colonies.emplace_back(sf::Vector2f{150.f, 300.f}, 100);
    m_foodSources.emplace_back(100, sf::Vector2f{200.f, 350.f});
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
  std::uniform_real_distribution<float> uniform_dist(-0.2, 0.2);
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
  MarkerType targetMarker = MarkerType::toFood;

  switch (currentState) {
    case AntState::returningAnthill: {

      // Ant is within the Anthill
      if (colony.getAnthill().getGlobalBounds().contains(ant.getPosition())) {
        ant.setState(leavingAnthill);
        return;
      }
      // Here chase food Marker
      break;
    }
    case AntState::leavingAnthill: {
      for (auto& foodSource : m_foodSources) {
        // Ant is within the food source
        if (foodSource.getGlobalBounds().contains(ant.getPosition())) {
          ant.setState(returningAnthill);
          return;
        }
      }
      break;
    }
  }

  // This function returns if the ant is out of screen
  checkBounds(ant);

  // Add ant marker
  auto dropped = ant.dropMarker();
  if (dropped.getPosition().x > 1 &&
      dropped.getPosition().x < (static_cast<float>(m_worldSize.x) - 5.f) &&
      dropped.getPosition().y > 1 &&
      dropped.getPosition().y < (static_cast<float>(m_worldSize.y) - 5.f)) {
    m_markers.push_back(dropped);
    m_heatMap.incrementByOneAt(dropped.getPosition());
  }

  ant.setDirection(ant.getDirection() + randomDirection());
}

inline void World::updateMarkers(float elapsedTime) {
  for (auto& m : m_markers) {
    m.tickLife(elapsedTime);
    if (m.getRemainingLife() <= 0) m_heatMap.decrementByOneAt(m.getPosition());
  }

  m_markers.erase(std::remove_if(m_markers.begin(), m_markers.end(),
                                 [](const ants::Marker& m) {
                                   return m.getRemainingLife() <= 0;
                                 }),
                  m_markers.end());

  m_heatMap.printHeatMap();
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
