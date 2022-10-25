#pragma once
#include <string>
#include <vector>
#include "Grille.h"

using std::string;
using std::vector;

enum BoatOrientation {
    BoatOrientationHorizontal,
    BoatOrientationVertical
};

enum BoatSegment {
    BoatSegmentIntact,
    BoatSegmentTouched
};

class Boat {
    private:
        string name;
        unsigned int size;
        Coord coord;
        BoatOrientation orientation;
        int imageIndex;
        vector<BoatSegment> segments;
    public:
        Boat(
            string name,
            unsigned int size,
            Coord coord,
            BoatOrientation orientation,
            int image_index
        );

        string getName() const;
        unsigned int getSize() const;
        Coord& getCoord() const;
        BoatOrientation getOrientation() const;
        int getImageIndex() const;
        vector<BoatSegment>& getSegments() const;
};