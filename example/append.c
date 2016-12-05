#define NL [@FORMAT "\n" @]
#define HS #
#define PROC(args...) NL HS args NL

#define APPEND(name, sep_str, args...)                  \
    PROC(ifndef name)                                   \
    PROC(define [@COUNTER NEXT @, name @]               \
         args)                                          \
    PROC(else)                                          \
    PROC(undef name)                                    \
    PROC(define [@COUNTER VALUE @, name @]              \
         [@COUNTER PREV @, name @]                      \
         [@UNSTRING sep_str @]                          \
         args)                                          \
    PROC(endif)                                         \
    PROC(define name [@COUNTER VALUE @, name @])

APPEND(alphabet, ",", A)

alphabet

APPEND(alphabet, ",", B)

alphabet

APPEND(alphabet, ",", C)

alphabet

PROC(define YOLO 42)
