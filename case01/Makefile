CC := gcc
CFLAGS := -Wall -Werror -Wextra -Wvla -Wfloat-equal -Wfloat-conversion -std=c99
OBJS := out/model.o out/receiver.o out/sender.o

a.out : $(OBJS) out/main.o
	$(CC) $(CFLAGS) $^

out/%.o : src/%.c inc/*.h
	$(CC) $(CFLAGS) -c $< -o $@

debug : CFLAGS += -DDEBUG --coverage -g3
debug : a.out

clean :
	rm -rf a.out out/*

verbose : CFLAGS += -DV
verbose : a.out

.PHONY : clean debug verbose
