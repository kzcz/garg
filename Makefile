CC = clang
WFLAGS = -Wall -Wextra -Wpedantic -Werror
CFLAGS = -O3 -I.
SRC = garg.c
OBJ = garg.o
TST = test
all: $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(WFLAGS) $(CFLAGS) -c $< -o $@

$(TST).o: $(TST).c
	$(CC) $(WFLAGS) $(CFLAGS) -c $< -o $@

$(TST): $(TST).o $(OBJ)
	$(CC) $(WFLAGS) $(CFLAGS) $^ -o $@
clean:
	rm -f $(OBJ)
	if [ -f $(TST) ]; then rm -f $(TST) $(TST).o; fi