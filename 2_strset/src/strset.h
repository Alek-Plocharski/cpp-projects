#ifndef STRSET_H
#define STRSET_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
    namespace jnp1 {
#endif
        /**
         * @brief Creates new set and return its id.
         * @return - id of the created set.
         */
        extern unsigned long strset_new();

        /**
         * @brief Deletes a set of given id.
         * If set of given id exist the function deletes it, otherwise it does nothing.
         * @param id[in] - id of set that should be deleted.
         */
        extern void strset_delete(unsigned long id);
        
        /**
         * @brief Returns the number of elements in set.
         * If the set of given id exists the function returns the number of element in the set,
         * if not it returns 0.
         * @param[in] id - id of set.
         * @return - size of set or 0 if it does not exist.
         */
        extern size_t strset_size(unsigned long id);
        
        /**
         * @brief Inserts a value to the set of given id.
         * If the set of given id exists and the value is not already in the set it inserts the value,
         * otherwise the function does nothing.
         * @param id[in] - id of set we want to insert to,
         * @param value[in] - element that should be inserted to the set.
         */
        extern void strset_insert(unsigned long id, const char* value);
        
        /**
         * @brief Removes a value from the set of given id.
         * If the set of given id exists and the value is in the set the function removes it,
         * otherwise it does nothing.
         * @param id[in] - id of set we want to remove from,
         * @param value[in] - element that should be removed.
         */
        extern void strset_remove(unsigned long id, const char* value);
        
        /**
         * @brief Checks if a value is in the set of given id.serieses
         * If the set of given id exists and the value is in the set the function returns 1,
         * otherwise it returns 0.
         * @param id[in] - id of set we want to check,
         * @param value[in] - value to be checked.
         * @return - 1 if the value is in the set, 0 if not.
         */
        extern int strset_test(unsigned long id, const char* value);
        
        /**
         * @brief Deletes all elements from the set.
         * If the set of given id exists the function deletes all its elements,
         * otherwise it does nothing.
         * @param id[in] - id of set we want to clear
         */
        extern void strset_clear(unsigned long id);
        
        /**
         * @brief Compares two sets.
         * Compares sets of ids equal to id1 and id2. Let sorted(id) mean a lexicographically sorted set of given id.
         * We compare the serieses naturally: first position where they differ decides which one is bigger.
         * If one series is a prefix of the other, then the one being a prefix is smaller. Function strset_comp(id1, id2) should return
         * -1, when sorted(id1) < sorted(id2),
         * 0, when sorted(id1) = sorted(id2),
         * 1, when sorted(id1) > sorted(id2).
         * If one of the set does not exist it is treated as an empty set.
         * @param id1[in] - id of first set to compare,
         * @param id2[in] - id of second set to compare.
         * @return - 1 if first set si bigger, -1 if second set is bigger and 0 when they are equal.
         */
        extern int strset_comp(unsigned long id1, unsigned long id2);
#ifdef __cplusplus
    }
}
#endif //__cplusplus
#endif //STRSET_H
