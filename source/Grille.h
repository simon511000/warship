#pragma once
#include <vector>
#include "citro2d.h"

using std::vector;

enum GrilleType {
    GrilleTypePlayer,
    GrilleTypeEnnemy
};

struct Coord {
    unsigned int x;
    unsigned int y;
};

class Grille {
    private:
        static const unsigned int DIM = 10;
        GrilleType type;
        C2D_SpriteSheet& spriteSheet;

    public:
        Grille(GrilleType type, C2D_SpriteSheet& spriteSheet);
        void marquerRate(Coord coordinate);
        void marquerTouche(Coord coordinate);
        void marquerCoule(Coord coordinate);
        void afficherGrille();
        static bool estDansGrille(Coord coordinate);
};