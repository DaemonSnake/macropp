#define end_class ;

#define class(name, parent)                     \
    typedef struct name name;                   \
    _Pragma("")                                 \
    struct name __POSTFIX__(@ end_class @)

#define method                                  \
    virtual(__POSTFIX__(@ ) @, , @)

char *tmp = "__POSTFIX__(@ YOLO @) {}";

class(String, Object)
{
    method func() void
    {
    }
}

int main()
{
}
