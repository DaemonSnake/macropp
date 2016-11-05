#define end_class )

#define class(name, parent)                     \
    typedef struct name name;                   \
    _Pragma("")                                 \
    struct name __POSTFIX__(@end_class@)

#define virtual                                 \
    __POSTFIX__(@end_virtual@)

class(String, Object)
{
    virtual void func()
    {
    }
}

int main()
{
}
