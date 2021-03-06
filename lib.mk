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

SRC =		src/tools.c			\
		src/buffer.c			\
		src/string_tools.c		\
		src/arguments.c			\
		src/thread.c			\
		src/command/commands.c		\
						\
		src/command/commands/macro.c	\
		src/command/commands/look.c	\
		src/command/commands/list.c	\
		src/command/commands/counter.c

OBJ =		$(SRC:.c=.o)
CFLAGS +=	-W -Wall -Wextra -Iinc -g3 -pthread -fPIC
LDFLAGS +=	-shared
NAME =		libmacro++.so

$(NAME):	$(OBJ)
		gcc $(OBJ) -o $@ $(CFLAGS) $(LDFLAGS)

clean:
		rm -f $(OBJ)

fclean:		clean
		rm -f $(NAME)

re: 		fclean $(NAME)
