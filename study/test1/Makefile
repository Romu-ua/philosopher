CC = gcc
CFLAGS = -g -Wall -Werror -Wextra -pthread

TARGET = philo
SRCS = eat.c enter.c leave.c philo.c queue.c sleep.c think.c utils.c worker.c
OBJS = $(SRCS:%.c=%.o)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $< -c $(CFLAGS)

clean:
	rm -rf $(OBJS)

fclean: 
	rm -f $(OBJS) $(TARGET)

.PHONY: clean