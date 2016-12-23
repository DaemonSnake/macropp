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

SRC =		src/main.c
MALLOC_TRACE =	.leaks
export MALLOC_TRACE
OBJ =		$(SRC:.c=.o)
CFLAGS =	-W -Wall -Wextra -Iinc -g3 -pthread
LDFLAGS =	-Wl,-rpath=$(CURDIR) -L$(CURDIR) -lmacro++
NAME =		macro++

all:		$(NAME)

libmacro++.so:
		$(MAKE) -f lib.mk

$(NAME):	libmacro++.so $(OBJ)
		gcc $(OBJ) -o $@ $(CFLAGS) $(LDFLAGS)

test:		$(NAME)
		cpp example/EX.c -P | ./$(NAME)

mem_check:	$(NAME)
		rm -f $(MALLOC_TRACE);
		cpp example/EX.c -P | ./$(NAME);
		mtrace $(NAME) $(MALLOC_TRACE);

valgrind:	$(NAME)
		cpp example/EX.c -P | valgrind --leak-check=full ./$(NAME);

cpp_test:	$(NAME)
		cpp example/foreach.cpp -P | ./$(NAME) | g++ -x c++ /dev/stdin -o example/$@

clean:
		rm -f $(OBJ)

fclean:		clean
		$(MAKE) -f lib.mk fclean
		rm -f $(NAME)

re: 		fclean $(NAME)
