#define class(name, parent)                     \
    typedef struct name name;                   \
    _Pragma("")                                 \
    struct name [@ end_class @]

#define method                                  \
    virtual( [@ ) @, , @]             \
    [@BALENCED ( @, ) @, , @, , @]

char *tmp = "[@ YOLO @] {}";

class(String, Object)
{
    method func() void
    {
    }
}

'[@ @]\' [@ @]'

int main()
{
}

[@COUNTER VALUE @]
[@COUNTER NEXT @]
[@COUNTER PREV @]
[@COUNTER VALUE @]
[@COUNTER VALUE @, YOLO @]
[@STRLEN "YOLO\n" @]
[@STRLEN NULL @]
[@FORMAT "\n#define % %\n" @, U bitch @, 42 @]

[@ [@FORMAT "\n" @] #define LOL [@FORMAT "\n" @] @]
{
}END
