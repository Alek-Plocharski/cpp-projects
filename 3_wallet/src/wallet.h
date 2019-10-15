#ifndef WALLET_H
#define WALLET_H

#include <vector>
#include <cstdint>
#include <ctime>
#include <chrono>
#include <ostream>
#include <string>
#include <boost/operators.hpp>

class Wallet : boost::ordered_field_operators<Wallet> {

public:

    /*
     * Creates empty wallet. Wallet history has one entry.
     */
    Wallet();

    /*
     * Creates wallet with n BajtekCoins. Wallet history has one entry.
     */
    Wallet(int n);

    /*
     * Move constructor. Wallet operations history is 'w' operations history
     * with one new entry.
     */
    Wallet(Wallet &&w) noexcept;

    /*
     * Creates wallet with w1.getUnits() + w2.getUnits() units.
     * Wallet history is sum of 'w1' and 'w2' operations history plus one new entry.
     * Wallet history is ordered by time of entry.
     * Wallets 'w1' and 'w2' are empty.
     */
    Wallet(Wallet &&w1, Wallet &&w2);

    /*
     * Creates wallet based on 'str'.
     * 'str' is inscription of value of BajtekCoins.
     */
    explicit Wallet(const char *str);
    explicit Wallet(const std::string &str);

    /*
     * Class method that creates wallet based on 'str'.
     * 'str' is inscription of BajtekCoins in binary number system.
     * 'str' is represented in big-endian.
     */
    static Wallet fromBinary(std::string str);

    /*
     * Deleted constructors incompatible with task.
     */
    Wallet(const Wallet &w) = delete;
    Wallet(bool) = delete;
    Wallet(double) = delete;
    Wallet(char) = delete;

    /*
     * Destructor.
     */
    ~Wallet();

    /*
     * Overloaded assignment operators.
     * If both objects are the same one, does nothing.
     * In other case: history of wallet is 'rhs' history and one new entry.
     * There is only move assignment (no copy assignment).
     */
    Wallet& operator=(Wallet &&rhs) noexcept;
    Wallet& operator=(Wallet const &rhs) = delete;

    /*
     * Overloaded addition operators.
     * 'rhs' has 0 units and one new entry.
     * Returned object units are 'lhs' units plus 'rhs' units.
     */
    friend Wallet operator+(Wallet &&lhs, Wallet &rhs);
    friend Wallet operator+(Wallet &&lhs, Wallet &&rhs);

    /*
     * Overloaded substraction operators.
     * 'rhs' has twice as many units than before.
     * Returned object units are 'lhs' units minus 'rhs' units.
     */
    friend Wallet operator-(Wallet &&lhs, Wallet &rhs);
    friend Wallet operator-(Wallet &&lhs, Wallet &&rhs);

    /*
     * Overloaded '+=' operators.
     * After operation 'rhs' has 0 units and one new entry.
     * 'lhs' has 'rhs' units plus 'lhs' units and one new entry.
     */
    friend Wallet& operator+=(Wallet &lhs, Wallet &rhs);
    friend Wallet& operator+=(Wallet &lhs, Wallet &&rhs);

    /*
     * Overloaded '-=' operators.
     * After operation 'rhs' 'rhs' has twice as many units than before and one new entry.
     * 'lhs' has 'lhs' units minus 'rhs' units and one new entry.
     */
    friend Wallet& operator-=(Wallet &lhs, Wallet &rhs);
    friend Wallet& operator-=(Wallet &lhs, Wallet &&rhs);

    /*
     * Overloaded multiplication operators.
     * Multiplies units by a natural number.
     * One new entry added.
     * Returned object units are 'w' units multiplied by n.
     */
    friend Wallet operator*(Wallet &w, uint64_t n);
    friend Wallet operator*(Wallet &&w, uint64_t n);
    friend Wallet operator*(uint64_t n, Wallet &w);
    friend Wallet operator*(uint64_t n, Wallet &&w);

    /*
     * Overloaded '*=' operators.
     * Multiplies units by a natural number.
     * One new entry added.
     */
    friend Wallet& operator*=(Wallet &w, int n);

    /*
     * Overloaded comparision operators.
     * Compares number of units in 'lhs' and 'rhs'
     */
    friend bool operator==(const Wallet &lhs, const Wallet &rhs);
    friend bool operator<(const Wallet &lhs, const Wallet &rhs);

    /*
     * Prints "Wallet[b B]" to os stream, where b is number of units.
     */
    friend std::ostream &operator<<(std::ostream &os, const Wallet &w);

    class Operation : boost::ordered_field_operators<Operation> {

    private:
        /*
         * Units after operation.
         */
        uint64_t units;

        /*
         * Time of operation.
         */
        std::chrono::milliseconds time_of_operation;
        struct tm *time_info;

    public:

        /*
         * Operation contsructor, where 'units' is number of units after operation.
         */
        Operation(uint64_t units);
        Operation() = default;

        /*
         * Returns number of units in wallet after operation.
         */
        uint64_t getUnits() const;

        /*
         * Prints "Wallet[b B]" to os stream, where b is number of units.
         */
        friend std::ostream &operator<<(std::ostream &os, const Operation &operation);

        /*
         * Overloaded comparision operators.
         * Compares values of time of operations.
         */
        bool operator==(const Operation &rhs) const;
        bool operator<(const Operation &rhs) const;

        /*
         * Returns string representation of units as amount of B.
         */
        static std::string units_to_B_representation(uint64_t units);
    };

    /*
     * Operator []. w[k] returns kth operation.
     */
    const Operation& operator[](size_t i) const;

    /*
     * Returns number of units in wallet.
     */
    size_t getUnits() const;

    /*
     * Returns number of operations in wallet history.
     */
    size_t opSize() const ;

private:

    /*
     * Wallet balance.
     */
    uint64_t units;

    /*
     * Operations history.
     */
    std::vector<Operation> operations;

    /*
     * Number of currently existing units. It cannot exceed limit
     * of 21 milions BajtekCoins.
     */
    static uint64_t existing_units;

    /*
     * Returns the number of units that are represented by str.
     * str represents number of B.
     */
    static uint64_t units_from_const_char(const char *str);
    static uint64_t units_from_string(const std::string &str);
};

/*
 * Returns object which represents empty wallet.
 */
const Wallet &Empty();

#endif //WALLET_H
