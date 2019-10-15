#include "imperialfleet.h"

ImperialStarship::ImperialStarship(ShieldPoints shield, AttackPower attack) : BasicShip(shield), ArmedShip(attack) {}

DeathStar::DeathStar(ShieldPoints shield, AttackPower attack) : ImperialStarship(shield, attack) {}

ImperialDestroyer::ImperialDestroyer(ShieldPoints shield, AttackPower attack) : ImperialStarship(shield, attack) {}

TIEFighter::TIEFighter(ShieldPoints shield, AttackPower attack) : ImperialStarship(shield, attack) {}

Squadron::Squadron(const std::initializer_list<std::shared_ptr<ImperialUnit>> ships) : ships(ships) {}

Squadron::Squadron(const std::vector<std::shared_ptr<ImperialUnit>> &ships) : ships(ships) {}

ShieldPoints Squadron::getShield() {
    ShieldPoints cumulativeShield = 0;
    for (auto &ship : this->ships) {
        cumulativeShield += ship->getShield();
    }
    return cumulativeShield;
}

AttackPower Squadron::getAttackPower() {
    AttackPower cumulativeAttack = 0;
    for (auto &ship : this->ships) {
        if (ship->getAliveCount() != 0)
            cumulativeAttack += ship->getAttackPower();
    }
    return cumulativeAttack;
}

void Squadron::takeDamage(AttackPower damage) {
    for (auto &s : this->ships) {
        s->takeDamage(damage);
    }
}

size_t Squadron::getAliveCount() {
    size_t aliveCount = 0;
    for (auto &ship : this->ships) {
        aliveCount += ship->getAliveCount();
    }
    return aliveCount;
}

std::shared_ptr<ImperialUnit> createDeathStar(ShieldPoints shield, AttackPower attack) {
    return std::make_shared<DeathStar>(shield, attack);
}

std::shared_ptr<ImperialUnit> createImperialDestroyer(ShieldPoints shield, AttackPower attack) {
    return std::make_shared<ImperialDestroyer>(shield, attack);
}

std::shared_ptr<ImperialUnit> createTIEFighter(ShieldPoints shield, AttackPower attack) {
    return std::make_shared<TIEFighter>(shield, attack);
}

std::shared_ptr<Squadron> createSquadron(const std::initializer_list<std::shared_ptr<ImperialUnit>> ships) {
    return std::make_shared<Squadron>(ships);
}

std::shared_ptr<Squadron> createSquadron(const std::vector<std::shared_ptr<ImperialUnit>> &ships) {
    return std::make_shared<Squadron>(ships);
}