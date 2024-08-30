#include <SFML/Graphics.hpp>
#include "gtest/gtest.h"
#include <unordered_set>
#include"../Nodo.h"
#include"../Griglia.h"
#include"../Personaggio.h"
TEST(NodoTest, Constructor) {
    Nodo nodo(1, 2, true);

    EXPECT_EQ(nodo.x, 1);   //EXPECT_EQ = valore aspettato nell'inizializzazione
    EXPECT_EQ(nodo.y, 2);
    EXPECT_TRUE(nodo.traversabile);
    EXPECT_FLOAT_EQ(nodo.g, 0.0);
    EXPECT_FLOAT_EQ(nodo.h, 0.0);
    EXPECT_FLOAT_EQ(nodo.f, 0.0);
    EXPECT_EQ(nodo.genitore, nullptr);
}
TEST(NodoTest, OperatorLessThan) {
    Nodo nodo1(0, 0);
    Nodo nodo2(1, 1);

    nodo1.f = 4.0;
    nodo2.f = 3.0;

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
            EXPECT_EQ(griglia.griglia[x][y].x, x);
            EXPECT_EQ(griglia.griglia[x][y].y, y);
            EXPECT_TRUE(griglia.griglia[x][y].traversabile);
        }
    }
}

TEST(GrigliaTest, GeneraOstacoliRandomici) {
    Griglia griglia(5, 5);
    griglia.genera_ostacoli_randomici(5);

    int non_traversabile_count = 0;
    for (int x = 0; x < griglia.larghezza; ++x) {
        for (int y = 0; y < griglia.altezza; ++y) {
            if (!griglia.griglia[x][y].traversabile) {
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

TEST(PersonaggioTest, AStarFindsPath) {
    //Test con tutte caselle libere
    Griglia griglia(5, 5);
    Nodo* start = &griglia.griglia[0][0];
    Nodo* goal = &griglia.griglia[4][4];
    Personaggio personaggio(griglia, start, goal);

    personaggio.a_star();

    EXPECT_FALSE(personaggio.percorso.empty());
    EXPECT_EQ(personaggio.percorso.front(), goal);
    EXPECT_EQ(personaggio.percorso.back(), start);
}

TEST(PersonaggioTest, AStarHandlesObstacles) {
    //Test con tutte caselle occupate
    Griglia griglia(5, 5);
    Nodo* start = &griglia.griglia[0][0];
    Nodo* goal = &griglia.griglia[4][4];

    griglia.griglia[1][0].traversabile = false;
    griglia.griglia[1][1].traversabile = false;
    griglia.griglia[1][2].traversabile = false;
    griglia.griglia[1][3].traversabile = false;
    griglia.griglia[1][4].traversabile = false;

    Personaggio personaggio(griglia, start, goal);
    personaggio.a_star();

    EXPECT_TRUE(personaggio.percorso.empty()); //Effettua sempre open_set.pop() perché non trova il percorso adatto
}

TEST(PersonaggioTest, CostruisciPercorso) {
    Griglia griglia(5, 5);
    Nodo* start = &griglia.griglia[0][0];
    Nodo* goal = &griglia.griglia[4][4];
    Nodo middle(2, 2);
    middle.genitore = start;
    goal->genitore = &middle;

    Personaggio personaggio(griglia, start, goal);
    personaggio.costruisci_percorso(goal);

    ASSERT_EQ(personaggio.percorso.size(), 3);
    EXPECT_EQ(personaggio.percorso[0], goal);
    EXPECT_EQ(personaggio.percorso[1], &middle);
    EXPECT_EQ(personaggio.percorso[2], start);
}