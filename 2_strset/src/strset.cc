#ifndef STRSETCC
#define STRSETCC

#include "strset.h"
#include "strsetconst.h"

#include <iostream>
#include <map>
#include <set>

using namespace std;
using namespace jnp1;

namespace {

    constexpr int NONEXISTENT_SET_SIZE = 0;

    enum class comp_result {
        FIRST_GREATER = 1,
        SECOND_GREATER = -1,
        EQUAL = 0
    };
    
    unsigned long next_id = 0;
    
    using set_map = map<unsigned long, set<string>>;
    auto &stored_sets() {
        static set_map sets;
        return sets;
    }

    auto &err() {
        static ios_base::Init init;
        static ostream &error = cerr;
        return error;
    }

#ifdef NDEBUG
    const bool debug = false;
#else
    const bool debug = true;
#endif

    auto get_iterator_to_set(unsigned long id) {
        return stored_sets().find(id);
    }

    bool is_iterator_to_existing_set(set_map::iterator iterator) {
        return iterator != stored_sets().end();
    }    

    auto get_iterator_to_string(const set<string> &set, const string &element) {
        return set.find(element);
    }

    bool is_iterator_to_existing_string(set<string>::iterator iterator, const set<string> &set) {
        return iterator != set.end();
    }
    
    /**
     * @brief Compares two sets.
     * Compares two sets. Let sorted(set) mean a lexicographically sorted set.
     * We compare the serieses naturally: first position where they differ decides which one is bigger.
     * If one series is a prefix of the other, then the one being a prefix is smaller.
     * Function compareTwoExistingSets(first_set, second_set) should return
     * -1, when sorted(first_set) < sorted(second_set),
     * 0, when sorted(first_set) = sorted(second_set),
     * 1, when sorted(first_set) > sorted(second_set).
     * @param first_set[in] - first set to compare,
     * @param second_set[in] - second set to compare.
     * @return - 1 if first set is bigger, -1 if second set is bigger and 0 when they are equal.
     */
    comp_result compare_two_existing_sets(const set<string> &first_set, const set<string> &second_set) {

        pair<set<string>::iterator, set<string>::iterator> first_mismatch = mismatch(first_set.begin(), first_set.end(), second_set.begin());

        if (first_mismatch.first != first_set.end() && first_mismatch.second == second_set.end())
            return comp_result::FIRST_GREATER;

        if (first_mismatch.first == first_set.end() && first_mismatch.second != second_set.end())
            return comp_result::SECOND_GREATER;

        if (first_mismatch.first == first_set.end() && first_mismatch.second == second_set.end())
            return comp_result::EQUAL;

        int result = (*first_mismatch.first).compare((*first_mismatch.second));

        
        if (result > 0)
            return comp_result::FIRST_GREATER;

        if (result < 0)
            return comp_result::SECOND_GREATER;
        
        return comp_result::EQUAL;
    }

    /*************************************** LOGS *********************************************************************/
    
    void log_set_created(const char *function_name, unsigned long id) {
        if (debug)
            err() << function_name << ": set " << id << " created" << endl;
    }

    void log_set_does_not_exist(const char *function_name, unsigned long id) {
        if (debug)
            err() << function_name << ": set " << id << " does not exist" << endl;
    }

    void log_compare_result(const char *function_name, unsigned long id1, unsigned long id2, comp_result result) {
        if (debug)
            err() << function_name << ": result of comparing set " << id1 << " to set " << id2 << " is " << static_cast<int>(result) << endl;
    }

    void log_set_deleted(const char *function_name, unsigned long id) {
        if (debug)
            err() << function_name << ": set " << id << " deleted" << endl;
    }

    void log_attempt_to_remove_set42(const char *function_name) {
        if (debug)
            err() << function_name << ": attempt to remove the 42 Set" << endl;
    }

    void log_set_size(const char *function_name, unsigned long id, size_t number_of_elements) {
        if (debug)
            err() << function_name << ": set " << id << " contains " << number_of_elements << " element(s)" << endl;
    }

    void log_invalid_value_null(const char *function_name) {
        if (debug)
            err() << function_name << ": invalid value (NULL)" << endl;
    }

    void log_attempt_to_insert_to_set42(const char *function_name) {
        if (debug)
            err() << function_name << ": attempt to insert into the 42 Set" << endl;
    }

    void log_element_present_in_set(const char *function_name, unsigned long id, string &element) {
        if (debug)
            err() << function_name << ": set " << id << ", element \"" << element << "\" was already present" << endl;
    }

    void log_element_inserted(const char *function_name, unsigned long id, string &element) {
        if (debug)
            err() << function_name << ": set " << id << ", element \"" << element << "\" inserted" << endl;
    }

    void log_attempt_to_remove_from_set42(const char *function_name) {
        if (debug)
            err() << function_name << ": attempt to remove from the 42 Set" << endl;
    }

    void log_element_not_present_in_set(const char *function_name, unsigned long id, string &element) {
        if (debug)
            err() << function_name << ": set " << id << " does not contain the element \"" << element << "\"" << endl;
    }

    void log_element_removed_from_set(const char *function_name, unsigned long id, string &element) {
        if (debug)
            err() << function_name << ": set " << id << ", element \""<< element << "\"" << " removed" << endl;
    }

    void log_test_result(const char *function_name, unsigned long id, string &element, bool is_in_set) {
        if (debug) {
            if (is_in_set)
                err() << function_name << ": set " << id << " contains the element \"" << element << "\"" << endl;

            if (!is_in_set)
                err() << function_name << ": set " << id << " does not contain the element \"" << element << "\"" << endl;
        }
    }

    void log_attempt_to_clear_set42(const char *function_name) {
        if (debug)
            err() << function_name << ": attempt to clear the 42 Set" << endl;
    }

    void log_set_cleared(const char *function_name, unsigned long id) {
        if (debug)
            err() << function_name << ": set " << id << " cleared" << endl;
    }
}

namespace jnp1 {

    unsigned long strset_new() {
        if(debug)
            err() << __func__ << "()" << endl;

        unsigned long id = next_id;
        next_id++;
        stored_sets()[id] = set<string>();

        log_set_created(__func__, id);

        return id;
    }

    void strset_delete(unsigned long id) {
        if (debug)
            err() << __func__ << "(" << id << ")" << endl;

        auto iterator_to_id = get_iterator_to_set(id);
        if (!is_iterator_to_existing_set(iterator_to_id)) {
            log_set_does_not_exist(__func__, id);
            return;
        }
        if (id == strset42()) {
            log_attempt_to_remove_set42(__func__);
            return;
        }

        stored_sets().erase(iterator_to_id);
        log_set_deleted(__func__, id);
    }

    size_t strset_size(unsigned long id) {
        if(debug)
            err() << __func__ << "("<< id << ")" << endl;

        auto iterator_to_id = get_iterator_to_set(id);
        if (!is_iterator_to_existing_set(iterator_to_id)) {
            log_set_does_not_exist(__func__, id);
            return NONEXISTENT_SET_SIZE;
        }

        size_t number_of_elements = (iterator_to_id->second).size();
        log_set_size(__func__, id, number_of_elements);
        return number_of_elements;
    }

    void strset_insert(unsigned long id, const char *value) {
        if(debug)
            err() << __func__ << "("<< id << ", " << (value == nullptr ? "NULL" : ("\"" + string(value)) + "\"") << ")" << endl;

        auto iterator_to_id = get_iterator_to_set(id);
        if (value == nullptr) {
            log_invalid_value_null(__func__);
            return;
        }

        if (!is_iterator_to_existing_set(iterator_to_id)) {
            log_set_does_not_exist(__func__, id);
            return;
        }

        if (id == strset42() && !(iterator_to_id->second).empty()) {
            log_attempt_to_insert_to_set42(__func__);
            return;
        }

        string element(value);
        auto &our_set = iterator_to_id->second;
        auto iterator_to_string = get_iterator_to_string(our_set, element);

        if (is_iterator_to_existing_string(iterator_to_string, our_set)) {
            log_element_present_in_set(__func__, id, element);
        }
        else {
            (iterator_to_id->second).insert(element);
            log_element_inserted(__func__, id, element);
        }
    }

    void strset_remove(unsigned long id, const char *value) {
        if(debug)
            err() << __func__ << "("<< id << ", " << (value == nullptr ? "NULL" : ("\"" + string(value)) + "\"") + "\"" << ")" << endl;

        if (value == nullptr) {
            log_invalid_value_null(__func__);
            return;
        }

        auto iterator_to_set = get_iterator_to_set(id);
        if (!is_iterator_to_existing_set(iterator_to_set)) {
            log_set_does_not_exist(__func__, id);
            return;
        }

        if (id == strset42()) {
            log_attempt_to_remove_from_set42(__func__);
            return;
        }

        string element(value);
        auto &our_set = iterator_to_set->second;
        auto iterator_to_string = get_iterator_to_string(our_set, element);

        if (!is_iterator_to_existing_string(iterator_to_string, our_set)) {
            log_element_not_present_in_set(__func__, id, element);
        }
        else {
            our_set.erase(element);
            log_element_removed_from_set(__func__, id, element);
        }
    }

    int strset_test(unsigned long id, const char* value) {
        if(debug)
            err() << __func__ << "("<< id << ", " << (value == nullptr ? "NULL" : ("\"" + string(value)) + "\"") << ")" << endl;

        bool is_in_set = false;
        auto iterator_to_set = get_iterator_to_set(id);
        if (value == nullptr) {
            log_invalid_value_null(__func__);
            return is_in_set;
        }

        if (!is_iterator_to_existing_set(iterator_to_set)) {
            log_set_does_not_exist(__func__, id);
            return is_in_set;
        }

        string element(value);
        auto &our_set = iterator_to_set->second;
        auto iterator_to_string = get_iterator_to_string(our_set, element);

        is_in_set = is_iterator_to_existing_string(iterator_to_string, our_set);
        log_test_result(__func__, id, element, is_in_set);

        return is_in_set;
    }

    void strset_clear(unsigned long id) {
        if(debug)
            err() << __func__ << "("<< id << ")" << endl;

        auto iterator_to_id = get_iterator_to_set(id);
        if (!is_iterator_to_existing_set(iterator_to_id)) {
            log_set_does_not_exist(__func__, id);
            return;
        }

        if(id == strset42()) {
            log_attempt_to_clear_set42(__func__);
            return;
        }

        (iterator_to_id->second).clear();
        log_set_cleared(__func__, id);

    }

    int strset_comp(unsigned long id1, unsigned long id2) {
        if(debug)
            err() << __func__ << "("<< id1 << ", " << id2 << ")" << endl;
        
        comp_result result;
        auto first_iterator = get_iterator_to_set(id1);
        auto second_iterator = get_iterator_to_set(id2);
        bool first_exists = is_iterator_to_existing_set(first_iterator);
        bool second_exists = is_iterator_to_existing_set(second_iterator);
        set<string> empty_set;

        result = (compare_two_existing_sets(first_exists ? first_iterator->second : empty_set,
                                            second_exists ? second_iterator->second : empty_set));

        log_compare_result(__func__, id1, id2, result);
        
        if (!first_exists)
            log_set_does_not_exist(__func__, id1);
        
        if (!second_exists)
            log_set_does_not_exist(__func__, id2);
        
        return static_cast<int>(result);
    }
}
#endif
