#include "philo.h"

int	is_empty(t_queue *q)
{
	if (q->front == NULL)
		return (1);
	return (0);
}

int timestamp_ms(struct timeval start, struct timeval curr)
{
	int		diff;
	long	curr_ms;
	long	start_ms;

	start_ms = start.tv_sec * 1000000 + start.tv_usec;
	curr_ms = curr.tv_sec * 1000000 + curr.tv_usec;
	diff = curr_ms - start_ms;
	return ((int)(diff / 1000));
}

void	msleep(int ms)
{
	if (ms > 0)
		usleep(ms * 1000);
}

int	stop(void *args)
{
	t_args	*a;
	int		flag;

	flag = 0;
	a = (t_args *)args;
	pthread_mutex_lock(&a->shared->mtx_die);
	if (a->shared->is_die)
		flag = 1;
	pthread_mutex_unlock(&a->shared->mtx_die);
	return (flag);
}
