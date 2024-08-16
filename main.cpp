#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <cstdlib>
#include <ctime>

class Nodo {
public:
    int x, y;
    bool traversabile;
    float g, h, f;
    Nodo* genitore;

    Nodo(int x, int y, bool traversabile = true)
            : x(x), y(y), traversabile(traversabile), g(0), h(0), f(0), genitore(nullptr) {}

    bool operator<(const Nodo& other) const {
        return f > other.f; // Inverted for min-heap behavior in priority queue
    }

    bool operator==(const Nodo& other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
    template <>
    struct hash<Nodo> {
        std::size_t operator()(const Nodo& nodo) const {
            return std::hash<int>()(nodo.x) ^ std::hash<int>()(nodo.y);
        }
    };
}

class Griglia {
public:
    int larghezza, altezza;
    std::vector<std::vector<Nodo>> griglia;

    Griglia(int larghezza, int altezza)
            : larghezza(larghezza), altezza(altezza), griglia(larghezza, std::vector<Nodo>(altezza, Nodo(0, 0))) {
        for (int x = 0; x < larghezza; ++x)
            for (int y = 0; y < altezza; ++y)
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

class Personaggio {
public:
    Griglia& griglia;
    Nodo* start;
    Nodo* goal;
    std::vector<Nodo*> percorso;
    std::unordered_set<Nodo*> closed_set;

    Personaggio(Griglia& griglia, Nodo* start, Nodo* goal)
            : griglia(griglia), start(start), goal(goal) {}

    void a_star() {
        std::priority_queue<Nodo*> open_set;
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
                    continue;

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

    void costruisci_percorso(Nodo* nodo) {
        while (nodo != nullptr) {
            percorso.push_back(nodo);
            nodo = nodo->genitore;
        }
    }

    void mostra_percorso(sf::RenderWindow& window) {
        for (auto nodo : percorso) {
            sf::RectangleShape rect(sf::Vector2f(20, 20));
            rect.setPosition(nodo->x * 20, nodo->y * 20);
            rect.setFillColor(sf::Color::Green);
            window.draw(rect);
        }
    }
};

int main() {
    const int larghezza = 20, altezza = 20;
    Griglia griglia(larghezza, altezza);

    // Generare ostacoli randomici
    griglia.genera_ostacoli_randomici(50); // Genera 50 ostacoli randomici

    Nodo* start = &griglia.griglia[0][0];
    Nodo* goal = &griglia.griglia[19][19];

    Personaggio personaggio(griglia, start, goal);
    personaggio.a_star();

    sf::RenderWindow window(sf::VideoMode(400, 400), "Algoritmo A* con SFML");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int x = 0; x < larghezza; ++x) {
            for (int y = 0; y < altezza; ++y) {
                sf::RectangleShape rect(sf::Vector2f(20, 20));
                rect.setPosition(x * 20, y * 20);
                rect.setFillColor(griglia.griglia[x][y].traversabile ? sf::Color::White : sf::Color::Black);
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(1);
                window.draw(rect);
            }
        }

        personaggio.mostra_percorso(window);

        window.display();
    }

    return 0;
}
