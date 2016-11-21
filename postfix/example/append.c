#define NL __POSTFIX__(@FORMAT "\n" @)
#define HS #
#define PROC(args...) NL HS args NL

#define APPEND(name, sep_str, args...)                          \
    PROC(ifndef name)                                           \
    PROC(define __POSTFIX__(@COUNTER NEXT @, name @)            \
         args)                                                  \
    PROC(else)                                                  \
    PROC(undef name)                                            \
    PROC(define __POSTFIX__(@COUNTER VALUE @, name @)           \
         __POSTFIX__(@COUNTER PREV @, name @)                   \
         __POSTFIX__(@UNSTRING sep_str @)                       \
         args)                                                  \
    PROC(endif)                                                 \
    PROC(define name __POSTFIX__(@COUNTER VALUE @, name @))

APPEND(alphabet, ",", A)

alphabet

APPEND(alphabet, ",", B)

alphabet

APPEND(alphabet, ",", C)

alphabet

PROC(define YOLO 42)
