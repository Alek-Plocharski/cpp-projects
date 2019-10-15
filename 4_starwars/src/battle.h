#ifndef BATTLE_H
#define BATTLE_H

#include <iostream>
#include <tuple>
#include <algorithm>
#include "imperialfleet.h"

template<typename T, T t0, T t1, typename ... S>
class SpaceBattle {

    /*
     * Checks if t0 and t1 are valid.
     */
    static_assert(t0 >= 0, "t0 is negative");
    static_assert(t0 < t1, "t0 is not smaller than t1");

private:

    size_t numberOfRebelShips; //number of rebel ships that are currently not destroyed
    size_t numberOfImperialShips; //number of imperial ships that are currently not destroyed
    std::tuple<S ...> ships; //tuple of all ship taking part in the battle
    T currentTime; //represents current time on the battlefield

    /*
     * Counts the number of not destroyed ships of each kind at the start of the battle.
     */
    template<size_t n = 0, typename ... Sh>
    constexpr void initializeCounters(std::tuple<Sh ...> &ships) {

        if constexpr (n < sizeof...(Sh)) {
            if constexpr (std::tuple_element_t<n, std::tuple<Sh ...>>::isRebelShip()) {
                if (std::get<n>(ships).getShield() != 0) {
                    this->numberOfRebelShips++;
                }
            } else if constexpr (!std::tuple_element_t<n, std::tuple<Sh ...>>::isRebelShip()) {
                if (std::get<n>(ships).getShield() != 0) {
                    this->numberOfImperialShips++;
                }
            }

            initializeCounters<n + 1, Sh ...>(ships);
        }
    }

    /*
     * Iterates over all ships, if a ship is not destroyed and is a rebel ship an attack by a fixed attacker (passed in the argument) takes place.
     */
    template<size_t n = 0, typename I, typename ... Sh>
    constexpr void rebelsGetAttacked(std::tuple<Sh ...> &ships, I &imperiumShip) {

        if constexpr (n < sizeof... (Sh)) {

            if constexpr (std::tuple_element_t<n, std::tuple<Sh ...>>::isRebelShip()) {
                if (std::get<n>(ships).getShield() != 0) {

                    attack(imperiumShip, std::get<n>(ships));

                    if (std::get<n>(ships).getShield() == 0)
                        this->numberOfRebelShips--;

                    if (imperiumShip.getShield() == 0)
                        this->numberOfImperialShips--;
                }
            }

            rebelsGetAttacked<n + 1, I, Sh ...>(ships, imperiumShip);
        }
    }

    /*
     * Iterates over all ships, if a ship is not destroyed and is an imperial ship that ship attacks all not already destroyed rebels.
     */
    template<size_t n = 0, typename ... Sh>
    constexpr void imperiumAttacks(std::tuple<Sh ...> &ships) {

        if constexpr (n < sizeof... (Sh)) {
            if constexpr (!std::tuple_element_t<n, std::tuple<Sh ...>>::isRebelShip()) {
                if (std::get<n>(ships).getShield() != 0) {
                    rebelsGetAttacked(ships, std::get<n>(ships));
                }
            }

            imperiumAttacks<n + 1, Sh ...>(ships);
        }
    }

    /*
     * Counts how much natural numbers that when squared are <= t1 there are.
     */
    template<size_t n = 0>
    static constexpr size_t howMuchSquares() {

        if constexpr (n * n >= t1) {
            return n;
        } else {
            return howMuchSquares<n + 1>();
        }
    }

    /*
     * Creates a table of numbers that are squares of natural numbers and are <= t1.
     */
    static constexpr auto squaresTable() {

        std::array<T, howMuchSquares()> squares{};
        for (T i = 0; i < howMuchSquares(); i++)
            squares[i] = i * i;

        return squares;
    }

public:

    SpaceBattle(S ... ships)
            : numberOfRebelShips(0),
              numberOfImperialShips(0),
              ships(ships ...),
              currentTime(t0) {
        initializeCounters(this->ships);
    }

    /*
     * Returns how much imperial ships are not destroyed.
     */
    size_t countImperialFleet() const {
        return this->numberOfImperialShips;
    }

    /*
     * Returns how much rebel ships are not destroyed.
     */
    size_t countRebelFleet() const {
        return this->numberOfRebelShips;
    }

    /*
     * Checks current time, if it is time for attack an imperial attack comences.
     * At the end current time is increased by timeStep.
     */
    void tick(T timeStep) {

        if (this->numberOfImperialShips == 0 && this->numberOfRebelShips == 0) {
            std::cout << "DRAW" << std::endl;
        } else if (this->numberOfRebelShips == 0) {
            std::cout << "IMPERIUM WON" << std::endl;
        } else if (this->numberOfImperialShips == 0) {
            std::cout << "REBELS WON" << std::endl;
        }

        static constexpr std::array<T, howMuchSquares()> attackTimes = squaresTable();
        if (std::binary_search(attackTimes.begin(), attackTimes.end(), this->currentTime))
            imperiumAttacks(this->ships);

        this->currentTime = this->currentTime + timeStep;
        this->currentTime %= (t1 + 1);
    }
};

#endif //BATTLE_H
