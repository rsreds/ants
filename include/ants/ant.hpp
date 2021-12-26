#ifndef ANTS_ANT_H
#define ANTS_ANT_H

#include <SFML/Graphics.hpp>
#include <ants/marker.hpp>
#include <ants/heatmap.hpp>
#include <gui/theme.hpp>

namespace ants {

enum AntState { leavingAnthill, returningAnthill };

class Ant : public sf::Drawable, public sf::Transformable {
 private:
  float m_speed = 100;
  sf::Vector2f m_direction{1.f, 1.f};
  sf::Color m_color;
  AntState m_state = AntState::leavingAnthill;
  HeatmapIndex m_currentMapIndex{0, 0};

 public:
  explicit Ant(sf::Vector2f pos);
  void updatePosition(float const& elapsedTime);
  sf::Vector2f getDirection() const;
  float getHeading() const;
  AntState getState() const;
  HeatmapIndex getCurrentHeatmapIndex() const;
  float getSpeed() const;
  void setDirection(sf::Vector2f const& dir);
  void setDirection(float angle);
  void setDirection(Marker const& marker);
  void setSpeed(float const& spd);
  void setState(AntState const& state);
  void setFillColor(const sf::Color& c);
  void setCurrentMapIndex(const HeatmapIndex &index);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  ants::Marker dropMarker();
};
}  // namespace ants

#endif  // ANTS_ANT_H
