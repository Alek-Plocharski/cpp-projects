#include <cstdint>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <regex>
#include <cmath>
#include <iostream>

#include "wallet.h"

using namespace std;

constexpr uint64_t UNITS_IN_B = 1e8;
constexpr uint64_t MAX_UNITS_IN_CIRCULATION = 2.1e7 * UNITS_IN_B;

uint64_t Wallet::existing_units = 0;

string Wallet::Operation::units_to_B_representation(uint64_t units) {

    uint64_t whole_part = units / UNITS_IN_B;
    uint64_t after_dot = units - (whole_part * UNITS_IN_B);

    stringstream representation;
    representation << whole_part << "," << after_dot;

    return representation.str();
}

Wallet::Wallet() : units(0), operations(vector<Operation>()){
    this->operations.emplace_back(units);
}

Wallet::Wallet(int n) : operations(vector<Operation>()) {

    if (n < 0)
        throw invalid_argument("Wallet balance would be negative.");

    uint64_t number_of_B = n;
    if (((MAX_UNITS_IN_CIRCULATION - existing_units) / UNITS_IN_B) < number_of_B )
        throw invalid_argument("B in circulation limit exceeded");

    this->units = number_of_B * UNITS_IN_B;
    Wallet::existing_units += this->units;
    this->operations.emplace_back(this->units);
}

Wallet::Wallet(Wallet &&w) noexcept : units(w.units), operations(move(w.operations)) {

    w.units = 0;
    w.operations = vector<Operation>();
    this->operations.emplace_back(this->units);
}

Wallet::Wallet(Wallet &&w1, Wallet &&w2) : units(w1.units + w2.units) {

    this->operations = vector<Operation>(w1.operations.size() + w1.operations.size());

    merge(w1.operations.begin(), w1.operations.end(), w2.operations.begin(), w2.operations.end(), this->operations.begin());

    this->units = w1.units + w2.units;
    this->operations.emplace_back(this->units);
    w1.units = 0;
    w2.units = 0;
}

uint64_t Wallet::units_from_const_char(const char *str) {

    uint64_t units;
    cmatch groups;
    const regex floating_point_number(R"(^\s*([1-9]{1}\d{0,7}|0)((\.|\,)(\d{1,8}))?\s*$)");

    if (!regex_match(str, groups, floating_point_number))
        throw invalid_argument ("Invalid argument");

    units = stoi(groups[1]);
    units *= UNITS_IN_B;

    if (groups[4] != "") {
        uint64_t additional_units = stoi(groups[4]);
        additional_units *= UNITS_IN_B / pow(10, groups[4].length());
        units += additional_units;
    }

    return units;
}

uint64_t Wallet::units_from_string(const string &str) {

    uint64_t units;
    smatch groups;
    const regex floating_point_number(R"(^\s*([1-9]{1}\d{0,7}|0)((\.|\,)(\d{1,8}))?\s*$)");

    if (!regex_match(str, groups, floating_point_number))
        throw invalid_argument ("Invalid argument");

    units = stoi(groups[1]);
    units *= UNITS_IN_B;

    if (groups[4] != "") {
        uint64_t additional_units = stoi(groups[4]);
        additional_units *= UNITS_IN_B / pow(10, groups[4].length());
        units += additional_units;
    }

    return units;
}

Wallet::Wallet(const char *str) : operations(vector<Operation>()) {

    uint64_t units = units_from_const_char(str);
    if (MAX_UNITS_IN_CIRCULATION - Wallet::existing_units < units)
        throw invalid_argument("B in circulation limit exceeded");

    this->units = units;
    Wallet::existing_units += this->units;
    this->operations.emplace_back(this->units);
}

Wallet::Wallet(const string &str) : operations(vector<Operation>()) {

    uint64_t units = units_from_string(str);
    if (MAX_UNITS_IN_CIRCULATION - existing_units < units)
        throw invalid_argument("B in circulation limit exceeded");

    this->units = units;
    Wallet::existing_units += this->units;
    this->operations.emplace_back(this->units);
}

Wallet Wallet::fromBinary(std::string str) {

    int n = 0;
    size_t first_invalid;

    if (!str.empty() && isspace(str[0]))
        throw invalid_argument("Invalid argument");

    try {
        n = stoi(str, &first_invalid, 2);

    } catch (...) {
        throw invalid_argument("Invalid argument");
    }

    if (first_invalid < str.length())
        throw invalid_argument("Invalid argument");

    return Wallet(n);
}

Wallet::~Wallet() {
    Wallet::existing_units -= this->units;
}

Wallet& Wallet::operator=(Wallet&& rhs) noexcept {

    if(this != &rhs){
        this->operations = move(rhs.operations);
        this->units = rhs.units;
        this->operations.emplace_back(this->units);
        rhs.units = 0;
        rhs.operations = vector<Operation>();
    }
    return *this;
}

Wallet operator+(Wallet &&lhs, Wallet &&rhs) {

    Wallet result;
    result.units = lhs.units + rhs.units;
    lhs.units = 0;
    rhs.units = 0;
    result.operations.emplace_back(result.units);

    return result;
}

Wallet operator+(Wallet &&lhs, Wallet &rhs) {

    Wallet result;
    result.units = lhs.units + rhs.units;
    lhs.units = 0;
    rhs.units = 0;
    rhs.operations.emplace_back(rhs.units);
    result.operations.emplace_back(result.units);

    return result;
}

Wallet operator-(Wallet &&lhs, Wallet &rhs) {

    if (lhs.units < rhs.units)
        throw invalid_argument("Wallet balance would be negative.");

    Wallet result;
    result.units = lhs.units - rhs.units;
    rhs.units = 2 * rhs.units;
    lhs.units = 0;
    rhs.operations.emplace_back(rhs.units);
    result.operations.emplace_back(result.units);

    return result;
}

Wallet operator-(Wallet &&lhs, Wallet &&rhs) {

    if (lhs.units < rhs.units)
        throw invalid_argument("Wallet balance would be negative.");

    Wallet result;
    result.units = lhs.units - rhs.units;
    rhs.units = 2 * rhs.units;
    lhs.units = 0;
    result.operations.emplace_back(result.units);

    return result;
}

Wallet& operator+=(Wallet &lhs, Wallet &rhs) {

    lhs.units = lhs.units + rhs.units;
    lhs.operations.emplace_back(lhs.units);
    rhs.units = 0;
    rhs.operations.emplace_back(rhs.units);

    return lhs;
}

Wallet& operator+=(Wallet &lhs, Wallet &&rhs) {

    lhs.units = lhs.units + rhs.units;
    lhs.operations.emplace_back(lhs.units);
    rhs.units = 0;
    rhs.operations.emplace_back(rhs.units);

    return lhs;
}

Wallet& operator-=(Wallet &lhs, Wallet &rhs) {

    if (lhs.units < rhs.units)
        throw invalid_argument("Wallet balance would be negative.");

    lhs.units = lhs.getUnits() - rhs.units;
    lhs.operations.emplace_back(lhs.units);
    rhs.units = (2 * rhs.getUnits());
    rhs.operations.emplace_back(rhs.units);

    return lhs;
}

Wallet& operator-=(Wallet &lhs, Wallet &&rhs) {

    if (lhs.units < rhs.units)
        throw invalid_argument("Wallet balance would be negative.");

    lhs.units = lhs.getUnits() - rhs.units;
    lhs.operations.emplace_back(lhs.units);
    rhs.units = (2 * rhs.getUnits());
    rhs.operations.emplace_back(rhs.units);

    return lhs;
}

Wallet operator*(Wallet &w, uint64_t n) {

    if (n != 0 && (MAX_UNITS_IN_CIRCULATION - Wallet::existing_units) / n < w.units)
        throw invalid_argument("B in circulation limit exceeded");

    Wallet result;
    result.units = w.units * n;
    Wallet::existing_units += result.units;
    result.operations.emplace_back(result.units);

    return result;
}

Wallet operator*(Wallet &&w, uint64_t n) {

    if (n != 0 && (MAX_UNITS_IN_CIRCULATION - Wallet::existing_units) / n < w.units)
        throw invalid_argument("B in circulation limit exceeded");

    Wallet result;
    result.units = w.units * n;
    Wallet::existing_units += result.units;
    result.operations.emplace_back(result.units);

    return result;
}

Wallet operator*(uint64_t n, Wallet &w) {

    return (w * n);
}

Wallet operator*(uint64_t n, Wallet &&w) {

    return (w * n);
}

Wallet& operator*=(Wallet &w, int n) {

    if (n != 0 && n != 1 && (MAX_UNITS_IN_CIRCULATION - Wallet::existing_units) / (n - 1) < w.units)
        throw invalid_argument("B in circulation limit exceeded");

    Wallet::existing_units -= w.units;
    w.units *= n;
    Wallet::existing_units += w.units;
    w.operations.emplace_back(w.units);
    return w;
}

bool operator==(const Wallet &lhs, const Wallet &rhs) {
    return lhs.units == rhs.units;
}

bool operator<(const Wallet &lhs, const Wallet &rhs) {
    return lhs.units < rhs.units;
}

ostream &operator<<(ostream &os, const Wallet &w) {
    os << "Wallet[" << Wallet::Operation::units_to_B_representation(w.units) << " B]";
    return os;
}

size_t Wallet::getUnits() const {
    return this->units;
}

size_t Wallet::opSize() const {
    return this->operations.size();
}

const Wallet::Operation &Wallet::operator[](size_t i) const {
    return this->operations[i];
}

Wallet::Operation::Operation(uint64_t units) {

    chrono::time_point<chrono::system_clock> curr_time = chrono::system_clock::now();
    time_t time = chrono::system_clock::to_time_t(curr_time);

    this->time_of_operation = chrono::duration_cast<chrono::milliseconds>(curr_time.time_since_epoch());
    this->time_info = localtime(&time);
    this->units = units;
}

ostream &operator<<(ostream &os, const Wallet::Operation &operation) {
    os << "Wallet balance is " << Wallet::Operation::units_to_B_representation(operation.units) << " B after operation made at day " << put_time(operation.time_info, "%F ");
    return os;
}

bool Wallet::Operation::operator==(const Wallet::Operation &rhs) const {
    return time_of_operation == rhs.time_of_operation;
}

bool Wallet::Operation::operator<(const Wallet::Operation &rhs) const {
    return time_of_operation < rhs.time_of_operation;
}

uint64_t Wallet::Operation::getUnits() const {
    return this->units;
}

const Wallet &Empty() {
    static Wallet w;
    return w;
}

