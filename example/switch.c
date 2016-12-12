#define case_b(value) case value [@BALENCED_SW '{' @, '}' @, break; @, : @]

int main()
{
    int i = 0;

    switch(i)
    {
        case_b(0) {
        }
    }
}
