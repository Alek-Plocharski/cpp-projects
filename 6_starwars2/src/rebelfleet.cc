#include <cassert>
#include "rebelfleet.h"

namespace {
    const Speed LO_SLOW = 99999;
    const Speed HI_SLOW = 299795;
    const Speed LO_FAST = 299796;
    const Speed HI_FAST = 2997960;
}

RebelStarship::RebelStarship(ShieldPoints shield, Speed speed) : BasicShip(shield), speed(speed) {}

Speed RebelStarship::getSpeed() {
    return this->speed;
}

ArmedRebelStarship::ArmedRebelStarship(ShieldPoints shield, Speed speed, AttackPower attack) : RebelStarship(shield, speed),
                                                                                               ArmedShip(attack) {}

void ArmedRebelStarship::causeDamage(const std::shared_ptr<ShipUnit> &imperial) {
    imperial->takeDamage(this->getAttackPower());
}

Explorer::Explorer(ShieldPoints shield, Speed speed) : RebelStarship(shield, speed) {
    assert(LO_FAST <= speed && speed <= HI_FAST);
}

void Explorer::causeDamage(const std::shared_ptr<ShipUnit> &imperial) {
    imperial->takeDamage(0);
}

StarCruiser::StarCruiser(ShieldPoints shield, Speed speed, AttackPower attack) : ArmedRebelStarship(shield, speed,
                                                                                                    attack) {
    assert(LO_SLOW <= speed && speed <= HI_SLOW);
}

XWing::XWing(ShieldPoints shield, Speed speed, AttackPower attack) : ArmedRebelStarship(shield, speed, attack) {
    assert(LO_FAST <= speed && speed <= HI_FAST);
}

std::shared_ptr<RebelStarship> createExplorer(ShieldPoints shield, Speed speed) {
    return std::make_shared<Explorer>(shield, speed);
}

std::shared_ptr<RebelStarship> createStarCruiser(ShieldPoints shield, Speed speed, AttackPower attack) {
    return std::make_shared<StarCruiser>(shield, speed, attack);
}

std::shared_ptr<RebelStarship> createXWing(ShieldPoints shield, Speed speed, AttackPower attack) {
    return std::make_shared<XWing>(shield, speed, attack);
}
