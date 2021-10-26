#ifndef ANTS_ANT_H
#define ANTS_ANT_H

#include <SFML/Graphics.hpp>

class Ant
{
private:
  sf::Vector2f m_pos;
public:
  Ant(sf::Vector2f pos);
  sf::CircleShape makeShape();
  
  sf::Vector2f getPos() const;
  void setPos(sf::Vector2f const& pos);
};

#endif // MACRO

