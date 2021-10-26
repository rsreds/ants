#ifndef ANTS_ANT_H
#define ANTS_ANT_H

#include <SFML/Graphics.hpp>

class Ant
{
private:
  int m_posX;
  int m_posY;
public:
  Ant(int posX, int posY);
  sf::CircleShape makeShape();
};

#endif // MACRO

