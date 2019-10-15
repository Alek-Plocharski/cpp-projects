#ifndef BATTLE_H
#define BATTLE_H

#include <vector>
#include "rebelfleet.h"
#include "imperialfleet.h"

typedef int Time;

class TimeStrategy {

public:

    /*
     * Moves the current time by timeStep.
     */
    virtual void changeTime(Time timeStep) = 0;

    /*
     * Checks if it's time to attack.
     */
    virtual bool checkTime() = 0;

    virtual ~TimeStrategy() = default;
};

class DefaultTimeStrategy : public virtual TimeStrategy {

private:

    Time time; // represents current time
    Time maxTime; //represents the maximum value of time(after surpassing this value time goes back to 0)

public:

    DefaultTimeStrategy(Time startTime, Time maxTime);

    void changeTime(Time timeStep) override;

    bool checkTime() override;
};

class AttackStrategy {

public:

    /*
     * Performs a single attack of imperial fleet on the rebel fleet.
     */
    virtual void imperialAttack(const std::vector<std::shared_ptr<ImperialUnit>> &imperials,
                                const std::vector<std::shared_ptr<RebelStarship>> &rebels) = 0;

    virtual ~AttackStrategy() = default;

private:

    virtual void attack(const std::shared_ptr<ImperialUnit> &imperial, const std::shared_ptr<RebelStarship> &rebel) = 0;
};

class DefaultAttackStrategy : public virtual AttackStrategy {

public:
    DefaultAttackStrategy() = default;

    void imperialAttack(const std::vector<std::shared_ptr<ImperialUnit>> &imperials,
                        const std::vector<std::shared_ptr<RebelStarship>> &rebels) override;

private:

    /*
     * Performs an attack of an imperial ship on a rebel ship.
    */
    void attack(const std::shared_ptr<ImperialUnit> &imperial, const std::shared_ptr<RebelStarship> &rebel) override;

};

class SpaceBattle {

public:

    SpaceBattle(const std::vector<std::shared_ptr<ImperialUnit>> &imperials,
                const std::vector<std::shared_ptr<RebelStarship>> &rebels,
                Time t0, Time t1);

    /*
     * Returns the number of imperial ships that are currently not destroyed.
     */
    size_t countImperialFleet();

    /*
     * Returns the number of rebel ships that are currently not destroyed.
     */
    size_t countRebelFleet();

    /*
     * Checks current time, if it's time for attacking performs the attack.
     * After that moves time by timeStep.
     */
    void tick(Time timeStep);

    class Builder {

    public:

        Builder() = default;

        Builder &ship(const std::shared_ptr<ImperialUnit> &imperial);

        Builder &ship(const std::shared_ptr<RebelStarship> &rebel);

        Builder &startTime(Time t);

        Builder &maxTime(Time t);

        SpaceBattle build();

    private:

        std::vector<std::shared_ptr<ImperialUnit>> imperials;
        std::vector<std::shared_ptr<RebelStarship>> rebels;
        Time t0;
        Time t1;
    };

private:

    std::vector<std::shared_ptr<ImperialUnit>> imperials; // imperial ships involved in the battle
    std::vector<std::shared_ptr<RebelStarship>> rebels; // rebel ships involved in the battle
    std::shared_ptr<TimeStrategy> timeStrategy; // strategy used to manage time
    std::shared_ptr<AttackStrategy> attackStrategy; // strategy used to manage attacks

};

#endif
