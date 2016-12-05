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

%.o: %.c
	cpp $< $(CFLAGS) | ../postfix | cpp $(CFLAGS) | cpp $(CFLAGS) | \
	gcc -c -x c $(CFLAGS) -o $@ /dev/stdin;

SRC = 	src/object.c	\
	src/Object.c	\
	String.c	\
	main.c		\

OBJ= 	$(SRC:.c=.o)

CFLAGS += -fplan9-extensions -W -Wall -Wextra

main:	$(OBJ)
	gcc $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@;

out:
	@cpp test.c $(CFLAGS) | cpp $(CFLAGS) | cpp $(CFLAGS) | grep -v '#'

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f main

re: fclean main
