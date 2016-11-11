# Copyright (C) 2016  Bastien Penavayre

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

SRC =		src/main.c		\
		src/look.c		\
		src/tools.c		\
		src/string_tools.c	\
		src/commands.c		\
		src/thread.c

OBJ =		$(SRC:.c=.o)

CFLAGS =	-W -Wall -Wextra -Iinc -g3 -pthread

NAME =		postfix

$(NAME):	$(OBJ)
		gcc $(OBJ) -o $@ $(CFLAGS) $(LDFLAGS)

test:		$(NAME)
		cpp example/EX.c -P | ./$(NAME)

cpp_test:	$(NAME)
		cpp example/foreach.cpp -P | ./$(NAME) | g++ -x c++ /dev/stdin -o example/$@

clean:
		rm -f $(OBJ)

fclean:		clean
		rm -f $(NAME)

re: 		fclean $(NAME)
