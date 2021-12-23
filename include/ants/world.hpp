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
  sf::Vector2i m_woldSize;

 public:
  explicit World(sf::Vector2i const & worldSize) : m_woldSize{worldSize} {
    m_colonies.emplace_back(sf::Vector2f{150.f, 300.f}, 100);
    m_foodSources.emplace_back(100, sf::Vector2f{200.f, 350.f});
  }
  std::vector<Colony>& getColonies() { return m_colonies; }
  std::vector<FoodSource>& getFoodSources() { return m_foodSources; }
  std::vector<Marker>& getMarkers() { return m_markers; }
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

inline void World::updateAnt(Colony& colony, Ant& ant) {
  AntState currentState = ant.getState();
  MarkerType targetMarker = MarkerType::toFood;

  switch (currentState) {
    case AntState::returningAnthill: {
      if (colony.getAnthill().getGlobalBounds().contains(ant.getPosition())) {
        ant.setState(leavingAnthill);
        return;
      }
      targetMarker = toBase;
      break;
    }
    case AntState::leavingAnthill: {
      for (auto& foodSource : m_foodSources) {
        if (foodSource.getGlobalBounds().contains(ant.getPosition())) {
          ant.setState(returningAnthill);
          return;
        }
      }
      targetMarker = toFood;
      break;
    }
  }
  auto strongestMarker =
      findStrongestAdjacent(ant.getPosition(), targetMarker, m_markers);

  auto x = ant.getPosition().x;
  auto y = ant.getPosition().y;

  float turnFactor = 0.05; // With value less than 1 the ant tunnels the walls
  float margin = 25.f;
  auto currentDirection = ant.getDirection();

  if (x <= margin) {
    currentDirection.x += turnFactor;
    ant.setDirection(currentDirection);
    return;
  } else if (x >= static_cast<float>(m_woldSize.x) - margin) {
    currentDirection.x -= turnFactor;
    ant.setDirection(currentDirection);
    return;
  }
  if (y <= margin) {
    currentDirection.y += turnFactor;
    ant.setDirection(currentDirection);
    return;
  } else if (y >= static_cast<float>(m_woldSize.y) - margin) {
    currentDirection.y -= turnFactor;
    ant.setDirection(currentDirection);
    return;
  }

  if (strongestMarker == m_markers.end())
    ant.setDirection(ant.getDirection() + randomDirection());
  else
    ant.setDirection(*strongestMarker);
}
}  // namespace ants

#endif  // ANTS_WORLD_H
