#include <SFML/Graphics.hpp>
#include "gtest/gtest.h"
#include <unordered_set>
#include"../Nodo.h"
#include"../Griglia.h"
#include"../Personaggio.h"
int main(){
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
TEST(NodoTest, Constructor) {
    Nodo nodo(1, 2, true);

    EXPECT_EQ(nodo.get_x(), 1);   //EXPECT_EQ = valore aspettato nell'inizializzazione
    EXPECT_EQ(nodo.get_y(), 2);
    EXPECT_TRUE(nodo.get_traversabile());
    EXPECT_FLOAT_EQ(nodo.get_g(), 0.0);
    EXPECT_FLOAT_EQ(nodo.get_h(), 0.0);
    EXPECT_FLOAT_EQ(nodo.get_f(), 0.0);
    EXPECT_EQ(nodo.get_genitore(), nullptr);
}
TEST(NodoTest, OperatorLessThan) {
    Nodo nodo1(0, 0);
    Nodo nodo2(1, 1);

    nodo1.set_f(4.0);
    nodo2.set_f(3.0);

    EXPECT_TRUE(nodo1 < nodo2);  // OPERATORE INVERTITO PER LA PRIORITY QUEUE!
}
TEST(NodoTest, OperatorEquality) {
    Nodo nodo1(0, 0);
    Nodo nodo2(0, 0);
    Nodo nodo3(1, 1);

    EXPECT_TRUE(nodo1 == nodo2);
    EXPECT_FALSE(nodo1 == nodo3);
}
TEST(GrigliaTest, Constructor) {
    Griglia griglia(5, 5);

    EXPECT_EQ(griglia.larghezza, 5);
    EXPECT_EQ(griglia.altezza, 5);
    EXPECT_EQ(griglia.griglia.size(), 5);
    EXPECT_EQ(griglia.griglia[0].size(), 5);

    for (int x = 0; x < griglia.larghezza; ++x) {
        for (int y = 0; y < griglia.altezza; ++y) {
            EXPECT_EQ(griglia.griglia[x][y].get_x(), x);
            EXPECT_EQ(griglia.griglia[x][y].get_y(), y);
            EXPECT_TRUE(griglia.griglia[x][y].get_traversabile());
        }
    }
}

TEST(GrigliaTest, GeneraOstacoliRandomici) {
    Griglia griglia(5, 5);
    griglia.genera_ostacoli_randomici(5);

    int non_traversabile_count = 0;
    for (int x = 0; x < griglia.larghezza; ++x) {
        for (int y = 0; y < griglia.altezza; ++y) {
            if (!griglia.griglia[x][y].get_traversabile()) {
                non_traversabile_count++;
            }
        }
    }

    EXPECT_EQ(non_traversabile_count, 5);
}

TEST(GrigliaTest, NodoVicini) {
    Griglia griglia(5, 5);
    Nodo& nodo = griglia.griglia[2][2];
    auto vicini = griglia.nodo_vicini(nodo);

    EXPECT_EQ(vicini.size(), 4);  // Il nodo al centro deve avere 4 vicini

    EXPECT_NE(std::find(vicini.begin(), vicini.end(), &griglia.griglia[1][2]), vicini.end());
    EXPECT_NE(std::find(vicini.begin(), vicini.end(), &griglia.griglia[3][2]), vicini.end());
    EXPECT_NE(std::find(vicini.begin(), vicini.end(), &griglia.griglia[2][1]), vicini.end());
    EXPECT_NE(std::find(vicini.begin(), vicini.end(), &griglia.griglia[2][3]), vicini.end());
}

TEST(GrigliaTest, CalcolaH) {
    Griglia griglia(5, 5);
    Nodo nodo1(0, 0);
    Nodo nodo2(3, 4);

    float h = griglia.calcola_h(nodo1, nodo2);
    EXPECT_EQ(h, 7);  // Distanza di Manhattan: |3-0| + |4-0| = 7
}

TEST(PersonaggioTestPercorsoMigliore, AStarFindsPath) {
    //Test percorso migliore
    Griglia griglia(5, 5);
    Nodo* start = &griglia.griglia[0][0];
    Nodo* goal = &griglia.griglia[4][0];
    //Blocco la parte centrale, mi aspetto che effettui il percorso di sopra
    for(int x=1;x<griglia.larghezza-1;x++){
        for(int y=1;y<griglia.altezza-1;y++)
            griglia.griglia[x][y].set_traversabile(false);
    }
    Personaggio personaggio(griglia, start, goal);

    personaggio.a_star();

    EXPECT_FALSE(personaggio.get_percorso().empty());
    EXPECT_EQ(personaggio.get_percorso().size(), 5); //Mi aspetto che il percorso sia composto da 5 nodi e non 5+4+4 overo l'altro unico percorso
    EXPECT_EQ(personaggio.get_percorso().front(), goal);
    EXPECT_EQ(personaggio.get_percorso().back(), start);
}
TEST(PersonaggioTest, AStarFindsPath) {
    //Test con tutte caselle libere
    Griglia griglia(5, 5);
    Nodo* start = &griglia.griglia[0][0];
    Nodo* goal = &griglia.griglia[4][4];
    Personaggio personaggio(griglia, start, goal);

    personaggio.a_star();

    EXPECT_FALSE(personaggio.get_percorso().empty());
    EXPECT_EQ(personaggio.get_percorso().front(), goal);
    EXPECT_EQ(personaggio.get_percorso().back(), start);
}

TEST(PersonaggioTest, AStarHandlesObstacles) {
    //Test con tutte caselle occupate
    Griglia griglia(5, 5);
    Nodo* start = &griglia.griglia[0][0];
    Nodo* goal = &griglia.griglia[4][4];

    griglia.griglia[1][0].set_traversabile(false);
    griglia.griglia[1][1].set_traversabile(false);
    griglia.griglia[1][2].set_traversabile(false);
    griglia.griglia[1][3].set_traversabile(false);
    griglia.griglia[1][4].set_traversabile(false);

    Personaggio personaggio(griglia, start, goal);
    personaggio.a_star();

    EXPECT_TRUE(personaggio.get_percorso().empty()); //Effettua sempre open_set.pop() perché non trova il percorso adatto
}