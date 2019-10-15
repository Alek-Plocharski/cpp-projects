#ifndef HELPER_H
#define HELPER_H

#include <memory>

typedef int32_t ShieldPoints;
typedef int32_t Speed;
typedef int32_t AttackPower;

class ShipUnit {

public:

    /*
     * Returns the current value of ship's shield.
     */
    virtual ShieldPoints getShield() = 0;

    /*
     * Lowers ship's shield by the damage received or sets it to 0 if damage is greater that shield.
     */
    virtual void takeDamage(AttackPower damage) = 0;

    /*
     * Returns number of ships alive in this unit.
     */
    virtual size_t getAliveCount() = 0;

    virtual ~ShipUnit() = default;
};

class BasicShip : public virtual ShipUnit {

public:

    ShieldPoints getShield() override;

    void takeDamage(AttackPower damage) override;

    size_t getAliveCount() override;

protected:

    ShieldPoints shield; //represents value of ship's shield

    explicit BasicShip(ShieldPoints shield);
};


class ArmedShipUnit : public virtual ShipUnit {

public:

    /*
     * Returns the value of ship's attack power.
     */
    virtual AttackPower getAttackPower() = 0;

};

class ArmedShip : public virtual ArmedShipUnit {

public:

    AttackPower getAttackPower() override;

protected:

    AttackPower attack; //represents value of ship's attack power

    explicit ArmedShip(AttackPower attack);
};

class ImperialUnit : public virtual ArmedShipUnit {
};

#endif
