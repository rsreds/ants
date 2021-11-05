#if !defined(ANTS_COLONY_H)
#define ANTS_COLONY_H

#include <ants/ant.hpp>
#include <ants/anthill.hpp>

namespace ants {
class Colony {
 private:
  Anthill m_anthill;
  size_t m_maxPopulation;

 public:
  std::vector<Ant> m_ants;
  Colony(sf::Vector2f pos, size_t maxPopulation)
      : m_anthill(pos), m_maxPopulation(maxPopulation) {
    m_ants.reserve(m_maxPopulation);
  }
  Anthill getAnthill() const { return m_anthill; };
  void spawn() {
    if (m_ants.size() < m_maxPopulation) {
      m_ants.emplace_back(m_anthill.getPosition());
    }
  }
  size_t getPopulation() const { return m_ants.size(); }
  size_t getMaxPopulation() const { return m_maxPopulation; }
};

}  // namespace ants

#endif  // ANTS_COLONY_H
