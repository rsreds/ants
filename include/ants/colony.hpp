#if !defined(ANTS_COLONY_H)
#define ANTS_COLONY_H

#include <ants/ant.hpp>
#include <ants/anthill.hpp>

namespace ants {
class Colony {
 private:
  Anthill m_anthill;
  size_t m_maxPopulation;
  std::vector<Ant> m_ants;

 public:
  Colony(sf::Vector2f pos, size_t maxPopulation)
      : m_anthill(pos), m_maxPopulation(maxPopulation) {}
  Anthill getAnthill() const { return m_anthill; };
};

}  // namespace ants

#endif  // ANTS_COLONY_H
