CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread

TARGET = philo
SRCS = die_worker.c eat.c philo.c sleep.c stop.c think.c worker.c \
		ft_atoi.c destroy.c args_init.c shared_init.c util.c
OBJS = $(SRCS:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $< -c $(CFLAGS)

clean:
	rm -rf $(OBJS)

fclean: 
	rm -f $(OBJS) $(TARGET)

re: clean all

.PHONY: all clean fclean re