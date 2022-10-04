#pragma once
#include "Case.h"
#include "Coordonnees.h"
#include <vector>
#include "citro2d.h"

using std::vector;

enum GrilleType {
    GrilleTypePlayer,
    GrilleTypeEnnemy
};

class Grille {
    private:
        static const int DIM = 10;
        GrilleType type;
        vector<vector<Case>> grille = vector<vector<Case>>(DIM, vector<Case>(DIM, CaseVide));
        C2D_SpriteSheet& spriteSheet;

    public:
        Grille(GrilleType type, C2D_SpriteSheet& spriteSheet);
        void marquerRate(coord coordinate);
        void marquerTouche(coord coordinate);
        void marquerCoule(coord coordinate);
        void afficherGrille();
        static bool estDansGrille(coord coordinate);
};