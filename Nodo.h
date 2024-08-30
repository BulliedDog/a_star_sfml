#ifndef A_STAR_SFML_NODO_H
#define A_STAR_SFML_NODO_H
class Nodo {
private:
    int x, y;
    bool traversabile;
    float g, h, f; //g costo da start a nodo corrente, h costo da nodo corrente a goal, f=g+h costo totale passando da nodo corrente
    Nodo* genitore;
public:
    Nodo(int x, int y, bool traversabile = true)
            : x(x), y(y), traversabile(traversabile), g(0), h(0), f(0), genitore(nullptr) {}

    bool operator<(const Nodo& other) const {
        return f > other.get_f(); // Inverted per far scegliere il nodo con f più piccolo nella priority queue
    }

    bool operator==(const Nodo& other) const {
        return x == other.x && y == other.y;
    }
    int get_x() const{return x;}
    void set_x(int x){this->x=x;}
    int get_y() const{return y;}
    void set_y(int y){this->y=y;}
    bool get_traversabile() const{return traversabile;}
    void set_traversabile(bool traversabile){this->traversabile=traversabile;}
    float get_g() const{return g;}
    void set_g(float g){this->g=g;}
    float get_h() const{return h;}
    void set_h(float h){this->h=h;}
    float get_f() const{return f;}
    void set_f(float f){this->f=f;}
    Nodo* get_genitore(){return genitore;}
    void set_genitore(Nodo* genitore){this->genitore=genitore;}
};
#endif //A_STAR_SFML_NODO_H
