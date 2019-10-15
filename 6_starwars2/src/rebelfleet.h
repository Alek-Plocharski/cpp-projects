#ifndef REBELFLEET_H
#define REBELFLEET_H

#include <memory>
#include "helper.h"

class RebelStarship : public BasicShip {

public:

    Speed getSpeed();

    /*
     * If the ship is armed function inflicts damage (equal to the attack power of the ship) on the imperial ship.
     */
    virtual void causeDamage(const std::shared_ptr<ShipUnit> &imperial) = 0;

protected:

    Speed speed; //represents value of ship's speed

    RebelStarship(ShieldPoints shield, Speed speed);
};

class ArmedRebelStarship : public RebelStarship, public ArmedShip {

public:

    void causeDamage(const std::shared_ptr<ShipUnit> &imperial) override;

protected:

    ArmedRebelStarship(ShieldPoints shield, Speed speed, AttackPower attack);
};


class Explorer : public RebelStarship {

public:

    void causeDamage(const std::shared_ptr<ShipUnit> &imperial) override;

    Explorer(ShieldPoints shield, Speed speed);
};

class StarCruiser : public ArmedRebelStarship {

public:

    StarCruiser(ShieldPoints shield, Speed speed, AttackPower attack);
};

class XWing : public ArmedRebelStarship {

public:

    XWing(ShieldPoints shield, Speed speed, AttackPower attack);
};

/*
 * Funkcje fabrykujące
 */
std::shared_ptr<RebelStarship> createExplorer(ShieldPoints shield, Speed speed);

std::shared_ptr<RebelStarship> createStarCruiser(ShieldPoints shield, Speed speed, AttackPower attack);

std::shared_ptr<RebelStarship> createXWing(ShieldPoints shield, Speed speed, AttackPower attack);

#endif
