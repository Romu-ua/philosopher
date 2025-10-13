#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

typedef struct s_node
{
	int				data;
	struct s_node	*next;
} t_node;

typedef struct s_queue
{
	t_node	*front;
	t_node	*rear;
}	t_queue;

typedef struct s_shared
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*mtx;
	pthread_mutex_t	is_waiting;
	pthread_mutex_t	adm_enter;
	int				capa;
	int				number_of_inside;
	pthread_mutex_t	mtx_die;
	int				is_die;
	int				*fin_flags;
	pthread_mutex_t	mtx_fin_flags;
	struct timeval	start;
	t_queue			*q;
}	t_shared;

typedef struct s_args
{
	int				tid;
	t_shared		*shared;
	struct timeval	*last_eat_ts;
}	t_args;

// pthread worker
void	*worker(void *args);
void	*die_worker(void *args);

// process
void	think(void *args);
void	waiting_in_line(void *args);
void	get_out_of_line(void *args);
void	enter(void *args);
void	eat(void *args);
void	leave(void *args);
void	philo_sleep(void *args);

// util
int		is_empty(t_queue *q);
int 	timestamp_ms(struct timeval start, struct timeval curr);
void	ft_msleep(int ms);
int		stop(void *args);
int		build_str(int ms, int tid, char *buf, int flag);
