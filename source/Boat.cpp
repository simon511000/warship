#include "Boat.h"

Boat::Boat(
    string name,
    unsigned int size,
    Coord coord,
    BoatOrientation orientation,
    int imageIndex
):  name(name),
    size(size),
    coord(coord),
    orientation(orientation),
    imageIndex(imageIndex)
{
    this->segments = vector<BoatSegment>(size, BoatSegmentIntact);
}

string Boat::getName() const {
    return this->name;
}

unsigned int Boat::getSize() const {
    return this->size;
}

Coord& Boat::getCoord() const {
    return this->coord;
}
BoatOrientation getOrientation const;
int getImageIndex() const;
vector<BoatSegment>& getSegments const;