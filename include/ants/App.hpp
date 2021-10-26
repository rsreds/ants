//
// Created by Jacopo Gasparetto on 25/10/21.
//

#ifndef ANTS_APP_H
#define ANTS_APP_H

#include <SFML/Graphics.hpp>
#include <ants/ant.hpp>

class App {
private:
    sf::Event m_event{};
    sf::RenderWindow m_window;
    ants::Ant test_ant{{400, 300}};

public:
    explicit App(int screenWidth = 800, int screenHeight = 600);

public:
    int run();

    bool init();

    void event();

    void loop();

    void render();

    void cleanup();
};


#endif //ANTS_APP_H
