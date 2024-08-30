#ifndef A_STAR_SFML_NODO_H
#define A_STAR_SFML_NODO_H
class Nodo {
    //TODO: rendere attributi privati
public:
    int x, y;
    bool traversabile;
    float g, h, f; //g costo da start a nodo corrente, h costo da nodo corrente a goal, f=g+h costo totale passando da nodo corrente
    Nodo* genitore;

    Nodo(int x, int y, bool traversabile = true)
            : x(x), y(y), traversabile(traversabile), g(0), h(0), f(0), genitore(nullptr) {}

    bool operator<(const Nodo& other) const {
        return f > other.f; // Inverted per far scegliere il nodo con f più piccolo nella priority queue
    }

    bool operator==(const Nodo& other) const {
        return x == other.x && y == other.y;
    }
};
#endif //A_STAR_SFML_NODO_H
