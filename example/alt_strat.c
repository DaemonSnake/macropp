thread main_loop
{
    b1 = new transfer buffer(in_buff);
    b2 = new transfer buffer(b1);
    c1 = create command_1(b1);
    c2 = create command_2(b2);
}

thread command_1
{
    while (read from <= input)
    {
        if (found)
        {
            write until_found => command_2;
            close connection => command_2;
            wait new_output <= command_2;
            if (new_output != -1) {
                kill this;
                return ;
            }
        }
        else
        {
            write all => command_2;
        }
    }
}

thread command_2
{
    while (read from <= comand_1)
    {
        if (found)
        {
            if (command_1.input_index < input_index)
            {
                close connection => main_loop;
                kill this;
                send new_output => command_1;
                return ;
            }
            send lose => command_1;
        }
        else
        {
            write all => main_loop;
        }
    }
}
