#include "collisable.h"
#include "shoot.h"
#include "ship.h"
#include <iostream>

bool Collisable::isShip() const {
    return false;
}

bool Collisable::isShoot() const {
    return false;
}

bool Collisable::isVertice() const {
    return false;
}

bool Collisable::isBox() const {
    return false;
}

void Collisable::vertice_box_collision(const CollisablePtr first, const CollisablePtr second) {
    if(first->isShoot() && second->isShip()) {
        ShootPtr shoot = std::dynamic_pointer_cast<Shoot>(first);
        ShipPtr ship = std::dynamic_pointer_cast<Ship>(second);

        if(Rectangle::intersection(shoot->get_position(), ship->get_box())) {
            ship->take_damage(shoot->get_damage());        
        }
    }
}