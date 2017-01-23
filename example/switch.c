#define case_b(value) case value [@BALANCED '{' @, '}' @, break; @, : @]

int main()
{
    int i = 0;

    switch(i)
    {
        case_b(0) {
        }
    }
}
