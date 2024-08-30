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
        return lhs->get_f() > rhs->get_f();  // Min-heap: smallest f value has the highest priority
    }
};
class Personaggio {
private:
    Griglia& griglia;
    Nodo* start;
    Nodo* goal;
    std::vector<Nodo*> percorso;
    std::unordered_set<Nodo*> closed_set;
    //reso metodo privato poiché chiamato soltanto da metodo a_star()
    void costruisci_percorso(Nodo* nodo) {
        while (nodo != nullptr) {
            percorso.push_back(nodo);
            nodo = nodo->get_genitore();
        }
    }
public:
    Personaggio(Griglia& griglia, Nodo* start, Nodo* goal)
            : griglia(griglia), start(start), goal(goal) {}

    void a_star() {
        std::priority_queue<Nodo*,std::vector<Nodo*>,CompareNodo> open_set; //Uso compare nodo per far sì che priority_queu ordini con f più piccolo
        start->set_g(0);
        start->set_h(griglia.calcola_h(*start, *goal));
        start->set_f(start->get_g() + start->get_h());
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

                if (!vicino->get_traversabile() || closed_set.count(vicino))
                    continue; //salta al loop successivo

                float nuovo_g = current_node->get_g() + 1;
                if (nuovo_g < vicino->get_g() || vicino->get_g() == 0) {
                    vicino->set_g(nuovo_g);
                    vicino->set_h(griglia.calcola_h(*vicino, *goal));
                    vicino->set_f(vicino->get_g() + vicino->get_h());
                    vicino->set_genitore(current_node);

                    open_set.push(vicino);
                }
            }
        }
    }

    void mostra_percorso(sf::RenderWindow& window) {
        for (auto nodo : percorso) {
            sf::CircleShape cerchio(10);
            cerchio.setPosition(nodo->get_x() * 40+10, nodo->get_y() * 40+10);
            cerchio.setFillColor(sf::Color::Yellow);
            window.draw(cerchio);
        }
    }
    Griglia& get_griglia() {return griglia;}
    void set_griglia(Griglia& g) {this->griglia=g;}
    Nodo* get_start() {return start;}
    void set_start(Nodo* s) {this->start=s;}
    Nodo* get_goal() {return goal;}
    void set_goal(Nodo* g) {this->goal=g;}
    std::vector<Nodo*>& get_percorso() {return percorso;}
    void set_percorso(const std::vector<Nodo*>& p) {this->percorso=p;}
    std::unordered_set<Nodo*>& get_closed_set() {return closed_set;}
    void set_closed_set(const std::unordered_set<Nodo*>& cs) {this->closed_set=cs;}
};
#endif //A_STAR_SFML_PERSONAGGIO_H
