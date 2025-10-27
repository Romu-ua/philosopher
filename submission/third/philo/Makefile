CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread

TARGET = philo
SRCS = args_init.c destroy.c ft_atoi.c ft_itoa.c ft_strdup.c ft_strlen.c \
		input_check.c main.c make_thread.c monitor.c odd_case.c ph_eat.c \
		ph_sleep.c ph_think.c printf_wrap.c shared_init.c th_init.c \
		urgent.c utils.c worker.c stop.c
OBJS = $(SRCS:%.c=%.o) 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $< -c $(CFLAGS)

clean:
	rm -rf $(OBJS)

fclean: 
	rm -f $(OBJS) $(TARGET)

re: clean all

.PHONY: all clean fclean re