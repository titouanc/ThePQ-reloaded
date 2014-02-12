#include "json.hpp"

using namespace JSON;

/* ===== Integer ===== */
Type Integer::type(void) const 
{
    return Integer_t;
}

/* ===== Float ===== */
Type Float::type(void) const 
{
    return Float_t;
}
