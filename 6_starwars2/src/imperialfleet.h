#ifndef IMPERIALFLEET_H
#define IMPERIALFLEET_H

#include <initializer_list>
#include <memory>
#include <vector>
#include "helper.h"
#include "rebelfleet.h"

class ImperialStarship : public BasicShip, public ArmedShip, public ImperialUnit {

protected:

    ImperialStarship(ShieldPoints shield, AttackPower attack);
};

class DeathStar : public ImperialStarship {

public:

    DeathStar(ShieldPoints shield, AttackPower attack);
};

class ImperialDestroyer : public ImperialStarship {

public:

    ImperialDestroyer(ShieldPoints shield, AttackPower attack);
};

class TIEFighter : public ImperialStarship {

public:

    TIEFighter(ShieldPoints shield, AttackPower attack);
};


class Squadron : public ImperialUnit {

public:

    Squadron(std::initializer_list<std::shared_ptr<ImperialUnit>> ships);

    explicit Squadron(const std::vector<std::shared_ptr<ImperialUnit>> &ships);

    ShieldPoints getShield() override;

    AttackPower getAttackPower() override;

    void takeDamage(AttackPower damage) override;

    size_t getAliveCount() override;

private:

    std::vector<std::shared_ptr<ImperialUnit>> ships;
};

/*
 * Funkcje fabrykujące
 */
std::shared_ptr<ImperialUnit> createDeathStar(ShieldPoints shield, AttackPower attack);

std::shared_ptr<ImperialUnit> createImperialDestroyer(ShieldPoints shield, AttackPower attack);

std::shared_ptr<ImperialUnit> createTIEFighter(ShieldPoints shield, AttackPower attack);

std::shared_ptr<Squadron> createSquadron(std::initializer_list<std::shared_ptr<ImperialUnit>> ships);

std::shared_ptr<Squadron> createSquadron(std::vector<std::shared_ptr<ImperialUnit>> ships);

#endif
