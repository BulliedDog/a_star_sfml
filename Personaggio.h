#ifndef A_STAR_SFML_PERSONAGGIO_H
#define A_STAR_SFML_PERSONAGGIO_H
#include"Griglia.h"
#include"Nodo.h"
#include<iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <cmath>
struct CompareNodo {
    bool operator()(const Nodo* lhs, const Nodo* rhs) const {
        return lhs->f > rhs->f;  // Min-heap: smallest f value has the highest priority
    }
};
class Personaggio {
private:

    //reso metodo privato poiché chiamato soltanto da metodo a_star()
    void costruisci_percorso(Nodo* nodo) {
        while (nodo != nullptr) {
            percorso.push_back(nodo);
            nodo = nodo->genitore;
        }
    }
public:
    Griglia& griglia;
    Nodo* start;
    Nodo* goal;
    std::vector<Nodo*> percorso;
    std::unordered_set<Nodo*> closed_set;

    Personaggio(Griglia& griglia, Nodo* start, Nodo* goal)
            : griglia(griglia), start(start), goal(goal) {}

    void a_star() {
        std::priority_queue<Nodo*,std::vector<Nodo*>,CompareNodo> open_set; //Uso compare nodo per far sì che priority_queu ordini con f più piccolo
        start->g = 0;
        start->h = griglia.calcola_h(*start, *goal);
        start->f = start->g + start->h;
        open_set.push(start);

        while (!open_set.empty()) {
            Nodo* current_node = open_set.top();
            open_set.pop();

            if (current_node == goal) {
                costruisci_percorso(current_node);
                return;
            }

            closed_set.insert(current_node);

            auto vicini = griglia.nodo_vicini(*current_node);
            for (auto vicino : vicini) {

                if (!vicino->traversabile || closed_set.count(vicino))
                    continue; //salta al loop successivo

                float nuovo_g = current_node->g + 1;
                if (nuovo_g < vicino->g || vicino->g == 0) {
                    vicino->g = nuovo_g;
                    vicino->h = griglia.calcola_h(*vicino, *goal);
                    vicino->f = vicino->g + vicino->h;
                    vicino->genitore = current_node;

                    open_set.push(vicino);
                }
            }
        }
    }

    void mostra_percorso(sf::RenderWindow& window) {
        for (auto nodo : percorso) {
            sf::CircleShape cerchio(10);
            cerchio.setPosition(nodo->x * 40+10, nodo->y * 40+10);
            cerchio.setFillColor(sf::Color::Yellow);
            window.draw(cerchio);
        }
    }
};
#endif //A_STAR_SFML_PERSONAGGIO_H
