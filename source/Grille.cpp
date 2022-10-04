#include "Grille.h"
#include "images.h"

Grille::Grille(GrilleType type, C2D_SpriteSheet& spriteSheet): type(type), spriteSheet(spriteSheet) {}

void Grille::marquerRate(coord coordinate) {
    this->grille[coordinate.x][coordinate.y] = CaseRate;
}

void Grille::marquerTouche(coord coordinate) {
    this->grille[coordinate.x][coordinate.y] = CaseTouche;
}

/**
 * @brief Marque la case donnée en paramètre comme Coulé ainsi que tous ses voisins
 * 
 * @param coord Les coordonnées
 */
void Grille::marquerCoule(coord coordinate) {
    vector<coord> lst;
    lst.push_back(coordinate);

    while((int)lst.size() > 0) {
        coord& c = lst[0];
        int cx = c.x;
        int cy = c.y;
        lst.erase(lst.begin());

        this->grille[cx][cy] = CaseTouche;

        vector<coord> cases_voisines;
        cases_voisines.push_back((coord){cx-1, cy}); // à gauche
        cases_voisines.push_back((coord){cx, cy-1}); // en haut
        cases_voisines.push_back((coord){cx+1, cy}); // à droite
        cases_voisines.push_back((coord){cx, cy+1}); // en base

        for(coord& cv: cases_voisines) {
            if(Grille::estDansGrille(cv) && this->grille[cv.x][cv.y] == CaseTouche) {
                lst.push_back(cv);
            }
        }
    }
}

bool Grille::estDansGrille(coord coordinate) {
    return 0 <= coordinate.x && coordinate.x < Grille::DIM && 0 <= coordinate.y && coordinate.y < Grille::DIM;
}

void Grille::afficherGrille() {
    int grilleX;
    int grilleY = 0;

    if(this->type == GrilleTypePlayer) {
        // Écran du haut
        grilleX = 80;
    } else {
        // Écran du bas
        grilleX = 40;
    }

    // Affichage de la grille
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(this->spriteSheet, images_grille_idx), grilleX, grilleY, 0.0f);

    // Affichage des types des cases
    for (int y = 0; y < Grille::DIM; y++) {
        for (int x = 0; x < Grille::DIM; x++) {
            int imgX = grilleX + (24*x);
            int imgY = grilleY + (24*y);

            switch (this->grille[x][y])
            {
            case CaseRate:
                C2D_DrawImageAt(C2D_SpriteSheetGetImage(this->spriteSheet, images_rate_idx), imgX, imgY, 0.0f);
                break;
            case CaseTouche:
                C2D_DrawImageAt(C2D_SpriteSheetGetImage(this->spriteSheet, images_touche_idx), imgX, imgY, 0.0f);
                break;
            case CaseCoule:
                C2D_DrawImageAt(C2D_SpriteSheetGetImage(this->spriteSheet, images_coule_idx), imgX, imgY, 0.0f);
                break;
            default:
                break;
            }
        }
    }
    
}