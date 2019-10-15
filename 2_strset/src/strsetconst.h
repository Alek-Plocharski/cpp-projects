#ifndef STRSETCONST_H
#define STRSETCONST_H

#ifdef __cplusplus
extern "C" {
    namespace jnp1 {
#endif

        /**
         * @brief Return the id of the 42 Set.
         * Returns the id of the 42 Set if it already exists,
         * if not the function creates it and then returns the id.
         * @return - id of the 42 Set.
         */
        unsigned long strset42();
        
#ifdef __cplusplus
    }
}
#endif //__cplusplus
#endif //STRSETCONST_H 
