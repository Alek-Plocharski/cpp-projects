#ifndef IMPERIALFLEET_H
#define IMPERIALFLEET_H

#include "rebelfleet.h"

template <typename U>
class ImperialStarship {

public:
    typedef U valueType;

protected:
    U shield; //represents value of ship's shield
    U attackPower; //represents value of ship's attack power

public:

    ImperialStarship(U shield, const U attackPower) : shield(shield), attackPower(attackPower) {}

    /*
     * Checks if ship is a rebel ship.
     */
    static constexpr bool isRebelShip() {
        return false;
    }

    U getShield() const {
        return this->shield;
    }

    U getAttackPower() const {
        return this->attackPower;
    }

    /*
     * Ship gets its shield value lowered by damage (if damage is greater than shield value of shield becomes 0)
     */
    void takeDamage(U damage) {
        this->shield = (shield > damage ? shield - damage : 0);
    }
};

/*
 * Simulates an attack of an imperial ship on a rebel ship.
 * Defender's shield is lowerd by the attackPower of the attacker.
 * If the rebel ship is a StarCruiser or XWing it attacks the imperial ship back in the same manner.
 */
template <typename I, typename R>
void attack(I &imperialShip, R &rebelShip) {

    rebelShip.takeDamage(imperialShip.getAttackPower());

    if constexpr (R::canCounterAttack())
        imperialShip.takeDamage(rebelShip.getAttackPower());
}

template <typename U>
using DeathStar = ImperialStarship<U>;

template <typename U>
using ImperialDestroyer = ImperialStarship<U>;

template <typename U>
using TIEFighter = ImperialStarship<U>;

#endif //IMPERIALFLEET_H
