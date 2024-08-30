#include <SFML/Graphics.hpp>
#include <unordered_set>
#include"Nodo.h"
#include"Griglia.h"
#include"Personaggio.h"
#include "gtest/gtest.h"

int main() {
    ::testing::InitGoogleTest();
    RUN_ALL_TESTS();

    const int larghezza = 20, altezza = 20;
    Griglia griglia(larghezza, altezza);
    ///////////////////
    //CASO IN CUI NON ESISTE PERCORSO!
    //griglia.griglia[0][1].traversabile=false;griglia.griglia[1][0].traversabile=false;
    ///////////////////
    griglia.genera_ostacoli_randomici(50); // Genera 50 ostacoli randomici

    Nodo* start = &griglia.griglia[0][0];
    Nodo* goal = &griglia.griglia[19][19];

    Personaggio personaggio(griglia, start, goal);
    personaggio.a_star();

    sf::RenderWindow window(sf::VideoMode(800, 800), "Algoritmo A* con SFML");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int x = 0; x < larghezza; ++x) {
            for (int y = 0; y < altezza; ++y) {
                sf::RectangleShape rect(sf::Vector2f(40, 40));
                rect.setPosition(x * 40, y * 40);
                rect.setFillColor(griglia.griglia[x][y].traversabile ? sf::Color::Black : sf::Color::Red);
                rect.setOutlineColor(sf::Color::Red);
                rect.setOutlineThickness(1);
                window.draw(rect);
            }
        }

        personaggio.mostra_percorso(window);

        window.display();
    }

    return 0;
}
