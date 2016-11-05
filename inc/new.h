#pragma once

#define ___ADD_THIS_NEW(args...)                                        \
    ({                                                                  \
        NEW_TYPE() MACRO_GLUE(this__, __LINE__) =                       \
            NEW_TYPE(__shallow_new)();                                  \
        NEW_TOR((struct NEW_TYPE(__private) *)MACRO_GLUE(this__, __LINE__), ##args); \
        MACRO_GLUE(this__, __LINE__);                                   \
    })

#define new(type, name)                                         \
    UNDEF(NEW_TYPE)                                             \
    UNDEF(NEW_TOR)                                              \
    PROC(define NEW_TYPE(x) MACRO_GLUE_2_(type, x))             \
    PROC(define NEW_TOR MACRO_GLUE_2_(type, __ctor_ ## name))   \
    ___ADD_THIS_NEW
