#include <vector2/vector2.h>

#include <ants/ant.hpp>
#include <random>

namespace ants {
Ant::Ant(sf::Vector2f pos) {
  setPosition(pos);
  setDirection(sf::randomVector(-1, 1));
}

void Ant::setFillColor(const sf::Color& c) { m_color = c; }

void Ant::setCurrentMapIndex(const HeatmapIndex& index) {
  m_currentMapIndex = index;
}
void Ant::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  float radius = 3;
  auto abdomenShape = sf::CircleShape(radius);
  abdomenShape.setFillColor(m_color);
  abdomenShape.setOrigin({radius, radius});
  abdomenShape.setPosition({-1.5f * radius, 0.f});

  auto thoraxShape = sf::CircleShape(radius * 0.7f);
  thoraxShape.setFillColor(m_color);
  thoraxShape.setOrigin({radius * 0.7f, radius * 0.7f});
  thoraxShape.setScale({3, 0.7});

  auto headShape = sf::CircleShape(radius * 0.8f);
  headShape.setFillColor(m_color);
  headShape.setOrigin({radius * 0.8f, radius * 0.8f});
  headShape.setPosition({1.5f * radius, 0.f});

  target.draw(abdomenShape, states);
  target.draw(thoraxShape, states);
  target.draw(headShape, states);
}

sf::Vector2f Ant::getDirection() const { return m_direction; }
float Ant::getHeading() const {
  return sf::rad2deg(std::atan2(m_direction.y, m_direction.x));
}
AntState Ant::getState() const { return m_state; }
float Ant::getSpeed() const { return m_speed; }
HeatmapIndex Ant::getCurrentHeatmapIndex() const { return m_currentMapIndex; }
float Ant::getRemainingTimeToHunt() const { return m_remainingTimeToHunt; }

// Setters
void Ant::setState(AntState const& state) { m_state = state; }

void Ant::setDirection(float angle) {
  setRotation(angle);
  auto rad = sf::deg2rad(angle);
  m_direction.x = cos(rad);
  m_direction.y = sin(rad);
}

void Ant::setDirection(sf::Vector2f const& dir) {
  auto newAngle = sf::rad2deg(std::atan2(m_direction.y, m_direction.x));
  setRotation(newAngle);
  m_direction = sf::normalizeCopy(dir);
}

void Ant::setDirection(Marker const& marker) {
  setDirection(marker.getPosition());
}

void Ant::setSpeed(float const& spd) { m_speed = spd; }

void Ant::updatePosition(float const& elapsedTime) {
  sf::Vector2f velocity{m_direction};
  sf::setMagnitude(velocity, m_speed);
  //  setPosition(getPosition() + velocity * elapsedTime);
  move(velocity * elapsedTime);
}

Marker Ant::dropMarker(float lifetime) {
  switch (m_state) {
    case AntState::returningAnthill:
      return {getPosition(), ants::MarkerType::toFood, lifetime};

    default:
      return {getPosition(), ants::MarkerType::toBase, lifetime};
  }
}

void Ant::resetHuntingTimer() {
  m_remainingTimeToHunt = huntingTimeout;
}

void Ant::tickHuntingTimer(const float elapsedTime) {
  m_remainingTimeToHunt -= elapsedTime * 10;
}
}  // namespace ants
