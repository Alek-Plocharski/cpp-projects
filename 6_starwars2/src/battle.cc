#include <iostream>
#include <cassert>
#include "battle.h"

/*
It is a dark time for the
Rebellion. Although the DeathStar
has been destroyed, ImperialStarships
have driven the Rebel forces
from their hidden base and pursued
them across the galaxy.

Evading the dreaded Imperial
SoloStarships and Squadrons,
a group of freedom fighters
led by Luke Skywalker has
established a new secret
base on the remote ice world
of Hoth.

The evil lord Darth Vader,
obsessed with finding young
Skywalker, has dispatched
thousands of remote probes into
the far reaches of space....
*/

SpaceBattle::Builder &SpaceBattle::Builder::ship(const std::shared_ptr<ImperialUnit> &imperial) {
    imperials.emplace_back(imperial);
    return *this;
}

SpaceBattle::Builder &SpaceBattle::Builder::ship(const std::shared_ptr<RebelStarship> &rebel) {
    rebels.emplace_back(rebel);
    return *this;
}

SpaceBattle::Builder &SpaceBattle::Builder::startTime(Time t) {
    t0 = t;
    return *this;
}

SpaceBattle::Builder &SpaceBattle::Builder::maxTime(Time t) {
    t1 = t;
    return *this;
}

SpaceBattle::SpaceBattle(const std::vector<std::shared_ptr<ImperialUnit>> &imperials,
                         const std::vector<std::shared_ptr<RebelStarship>> &rebels,
                         Time t0, Time t1) : imperials(imperials),
                                             rebels(rebels),
                                             timeStrategy(std::make_shared<DefaultTimeStrategy>(t0, t1)),
                                             attackStrategy(std::make_shared<DefaultAttackStrategy>()) {}

SpaceBattle SpaceBattle::Builder::build() {
    return SpaceBattle(imperials, rebels, t0, t1);
}

size_t SpaceBattle::countImperialFleet() {
    size_t size = 0;
    for (auto &ship : imperials) {
        size += ship->getAliveCount();
    }
    return size;
}

size_t SpaceBattle::countRebelFleet() {
    size_t size = 0;
    for (auto &ship : rebels) {
        size += ship->getAliveCount();
    }
    return size;
}

void SpaceBattle::tick(Time timeStep) {
    if (countImperialFleet() == 0 && countRebelFleet() == 0) {
        std::cout << "DRAW\n";
    } else if (countRebelFleet() == 0) {
        std::cout << "IMPERIUM WON\n";
    } else if (countImperialFleet() == 0) {
        std::cout << "REBELLION WON\n";
    } else {
        if (timeStrategy->checkTime())
            attackStrategy->imperialAttack(this->imperials, this->rebels);
        timeStrategy->changeTime(timeStep);
    }
}

DefaultTimeStrategy::DefaultTimeStrategy(Time startTime, Time maxTime) : time(startTime), maxTime(maxTime) {
    assert(time >= 0);
    assert(time <= maxTime);
}

void DefaultTimeStrategy::changeTime(Time timeStep) {
    this->time = (this->time + timeStep) % (this->maxTime + 1);
}

bool DefaultTimeStrategy::checkTime() {
    return (this->time % 2 == 0 || this->time % 3 == 0) && this->time % 5 != 0;
}

void DefaultAttackStrategy::attack(const std::shared_ptr<ImperialUnit> &imperial,
                                   const std::shared_ptr<RebelStarship> &rebel) {
    rebel->takeDamage(imperial->getAttackPower());
    rebel->causeDamage(imperial);
}

void DefaultAttackStrategy::imperialAttack(const std::vector<std::shared_ptr<ImperialUnit>> &imperials,
                                           const std::vector<std::shared_ptr<RebelStarship>> &rebels) {
    for (auto &imperial : imperials) {
        for (auto &rebel : rebels) {
            if (imperial->getAliveCount() != 0 && rebel->getAliveCount() != 0) {
                attack(imperial, rebel);
            }
        }
    }
}