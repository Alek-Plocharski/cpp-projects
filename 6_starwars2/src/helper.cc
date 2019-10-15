#include <cassert>
#include "helper.h"

BasicShip::BasicShip(ShieldPoints shield) : shield(shield) {
    assert(shield >= 0);
}

ShieldPoints BasicShip::getShield() {
    return this->shield;
}

void BasicShip::takeDamage(AttackPower damage) {
    this->shield = damage > this->shield ? 0 : this->shield - damage;
}

size_t BasicShip::getAliveCount() {
    return this->shield == 0 ? 0 : 1;
}

ArmedShip::ArmedShip(AttackPower attack) : attack(attack) {
    assert(attack >= 0);
}

AttackPower ArmedShip::getAttackPower() {
    return this->attack;
}