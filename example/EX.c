#define class(name, parent)                     \
    typedef struct name name;                   \
    [@MACRO CLASS @, name @]                    \
    [@MACRO SUPER @, parent @]                  \
    struct name [@ end_class @]

#define method                                  \
    [@ %CLOSE @]                                \
    [@MACRO METHOD @,

char *tmp = "[@ YOLO @] {}";

'[@ @]\' [@ @]'

[@COUNTER VALUE @]
[@COUNTER NEXT @]
[@COUNTER PREV @]
[@COUNTER VALUE @]
[@COUNTER VALUE @, YOLO @]
[@STRLEN "YOLO\n" @]
[@STRLEN NULL @]
[@FORMAT "\n#define % %\n" @, U bitch @, 42 @]

//Nested test
[@ %NL #define LOL %NL @]
{
}END

//Macro test

[@MACRO CLASS @]
[@MACRO CLASS @, String @]
[@MACRO CLASS @]
[@MACRO CLASS @, @]
[@MACRO CLASS @]
[@MACRO CLASS @, Hello @]
[@MACRO CLASS @]

//class tests
class(String, Object)
{
    method func() void
    {
    }
}

[@MACRO METHOD @]
[@MACRO CLASS @]

[@LIST PUSH_BACK @, METHODS @, main @]
[@LIST PUSH_BACK @, METHODS @, printf @]
[@LIST PRINT @, METHODS @, , @]
