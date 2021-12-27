#ifndef ANTS_ANT_H
#define ANTS_ANT_H

#include <SFML/Graphics.hpp>
#include <ants/marker.hpp>
#include <ants/heatmap.hpp>
#include <gui/theme.hpp>

namespace ants {

enum AntState { leavingAnthill, returningAnthill, noSuccess};

class Ant : public sf::Drawable, public sf::Transformable {
  static constexpr float huntingTimeout = 100.0f;
 private:
  float m_speed = 150;
  sf::Vector2f m_direction;
  sf::Color m_color;
  AntState m_state = AntState::leavingAnthill;
  HeatmapIndex m_currentMapIndex{0, 0};
  float m_remainingTimeToHunt = huntingTimeout;

 public:
  explicit Ant(sf::Vector2f pos);
  void updatePosition(float const& elapsedTime);
  sf::Vector2f getDirection() const;
  float getHeading() const;
  AntState getState() const;
  HeatmapIndex getCurrentHeatmapIndex() const;
  float getSpeed() const;
  float getRemainingTimeToHunt() const;
  void setDirection(sf::Vector2f const& dir);
  void setDirection(float angle);
  void setDirection(Marker const& marker);
  void setSpeed(float const& spd);
  void setState(AntState const& state);
  void setFillColor(const sf::Color& c);
  void setCurrentMapIndex(const HeatmapIndex &index);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  ants::Marker dropMarker();
  void resetHuntingTimer();
  void tickHuntingTimer(float elapsedTime);
};
}  // namespace ants

#endif  // ANTS_ANT_H
