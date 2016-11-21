#define class(name, parent)                     \
    typedef struct name name;                   \
    _Pragma("")                                 \
    struct name __POSTFIX__(@ end_class @)

#define method                                  \
    virtual(__POSTFIX__(@ ) @, , @)             \
    __POSTFIX__(@BALENCED ( @, ) @, , @, , @)

char *tmp = "__POSTFIX__(@ YOLO @) {}";

class(String, Object)
{
    method func() void
    {
    }
}

'__POSTFIX__(@ @)\' __POSTFIX__(@ @)'

int main()
{
}

__POSTFIX__(@COUNTER VALUE @)
__POSTFIX__(@COUNTER NEXT @)
__POSTFIX__(@COUNTER PREV @)
__POSTFIX__(@COUNTER VALUE @)
__POSTFIX__(@COUNTER VALUE @, YOLO @)
__POSTFIX__(@STRLEN "YOLO\n" @)
__POSTFIX__(@STRLEN NULL @)
__POSTFIX__(@FORMAT "\n#define % %\n" @, U bitch @, 42 @)
