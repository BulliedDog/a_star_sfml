#ifndef A_STAR_SFML_GRIGLIA_H
#define A_STAR_SFML_GRIGLIA_H
#include<vector>
#include<iostream>
#include"Nodo.h"
class Griglia {
public:
    int larghezza, altezza;
    std::vector<std::vector<Nodo>> griglia;

    Griglia(int larghezza, int altezza)
            : larghezza(larghezza), altezza(altezza), griglia(larghezza, std::vector<Nodo>(altezza, Nodo(0, 0))) {
        for (int x = 0; x < larghezza; x++)
            for (int y = 0; y < altezza; y++)
                griglia[x][y] = Nodo(x, y);
    }

    void genera_ostacoli_randomici(int num_ostacoli) {
        srand(static_cast<unsigned int>(time(0)));
        int ostacoli_generati = 0;
        while (ostacoli_generati < num_ostacoli) {
            int x = rand() % larghezza;
            int y = rand() % altezza;

            // Evitare di generare ostacoli nella posizione di partenza e arrivo
            if ((x == 0 && y == 0) || (x == larghezza - 1 && y == altezza - 1)) {
                continue;
            }

            if (griglia[x][y].traversabile) {
                griglia[x][y].traversabile = false;
                ostacoli_generati++;
            }
        }
    }

    std::vector<Nodo*> nodo_vicini(Nodo& nodo) {
        std::vector<Nodo*> vicini;
        if (nodo.x > 0)
            vicini.push_back(&griglia[nodo.x - 1][nodo.y]);
        if (nodo.x < larghezza - 1)
            vicini.push_back(&griglia[nodo.x + 1][nodo.y]);
        if (nodo.y > 0)
            vicini.push_back(&griglia[nodo.x][nodo.y - 1]);
        if (nodo.y < altezza - 1)
            vicini.push_back(&griglia[nodo.x][nodo.y + 1]);
        return vicini;
    }

    float calcola_h(Nodo& nodo, Nodo& nodo_finale) {
        return std::abs(nodo.x - nodo_finale.x) + std::abs(nodo.y - nodo_finale.y);
    }
};
#endif //A_STAR_SFML_GRIGLIA_H
