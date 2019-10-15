#ifndef REBELFLEET_H
#define REBELFLEET_H

#include <string>
#include <cassert>
#include <type_traits>

namespace rebels_settings {

    constexpr long long MAX_SPEED_CRUISER = 299795;
    constexpr long long MIN_SPEED_CRUISER = 99999;
    constexpr long long MAX_SPEED_REST = 2997960;
    constexpr long long MIN_SPEED_REST = 299796;

    /*
     * Enum representing different kinds of rebel ships.
     */
    enum class kindOfShip {
        EXPLORER,
        STARCRUISER,
        XWING
    };

    /*
     * Returns minimal speed required for a specific rebel ship.
     */
    static long long minSpeed(kindOfShip shipKind) {

        switch (shipKind) {
            case kindOfShip::STARCRUISER :
                return MIN_SPEED_CRUISER;

            case kindOfShip::EXPLORER :
            case kindOfShip::XWING:
                return MIN_SPEED_REST;
        }
    }

    /*
     * Returns maximum speed required for a specific rebel ship.
     */
    static long long maxSpeed(kindOfShip shipKind) {

        switch (shipKind) {
            case kindOfShip::STARCRUISER :
                return MAX_SPEED_CRUISER;

            case kindOfShip::EXPLORER :
            case kindOfShip::XWING:
                return MAX_SPEED_REST;
        }
    }
}

template <typename U, rebels_settings::kindOfShip ship>
class RebelStarship {

private:
    U shield; //represents value of ship's shield
    U speed; //represents value of ship's speed
    U attackPower; //represents value of ship's attack power

public:
    typedef U valueType;

    template<rebels_settings::kindOfShip s = ship, typename = typename std::enable_if<s != rebels_settings::kindOfShip::EXPLORER>::type>
    RebelStarship(U shield, const U speed, const U attackPower) : shield(shield), speed(speed), attackPower(attackPower) {
        assert(speed <= rebels_settings::maxSpeed(ship) && speed >= rebels_settings::minSpeed(ship));
    }

    template<rebels_settings::kindOfShip s = ship, typename = typename std::enable_if<s == rebels_settings::kindOfShip::EXPLORER>::type>
    RebelStarship(U shield, const U speed) : shield(shield), speed(speed) {
        assert(speed <= rebels_settings::maxSpeed(ship) && speed >= rebels_settings::minSpeed(ship));
    }

    U getShield() const {
        return this->shield;
    }

    U getSpeed() const {
        return this->speed;
    }

    U getAttackPower() const {
        return this->attackPower;
    }

    /*
     * Ship gets its shield value lowered by damage (if damage is greater than shield value of shield becomes 0)
     */
    void takeDamage(U damage) {
        this->shield = (this->shield > damage ? this->shield - damage : 0);
    }

    /*
     * Checks if the ship can counter attack if it is attacked itself.
     */
    static constexpr bool canCounterAttack() {
        return ship != rebels_settings::kindOfShip::EXPLORER;
    }

    /*
     * Checks if ship is a rebel ship.
     */
    static constexpr bool isRebelShip() {
        return true;
    }
};

template <typename U>
using XWing = RebelStarship<U, rebels_settings::kindOfShip::XWING>;

template <typename U>
using StarCruiser = RebelStarship<U, rebels_settings::kindOfShip::STARCRUISER>;

template <typename U>
using Explorer = RebelStarship<U, rebels_settings::kindOfShip::EXPLORER>;

#endif //REBELFLEET_H
