#include "philo.h"

long long now_ts(void)
{
	struct timeval	tv;
	long long		now;

	gettimeofday(&tv, NULL);
	now = (long long)tv.tv_sec * 1000000 + (long long)tv.tv_usec;
	return (now);
}

void ft_msleep(int ms) {
	const long long end = now_ts() + (long long)ms * 1000;
	while (now_ts() < end) {
		usleep(10);
	}
}

void	philo_sleep(void *args)
{
	t_args			*a;
	struct timeval	curr;

	a = (t_args *)args;
	gettimeofday(&curr, NULL);
	printf("%d %d is sleeping\n", timestamp_ms(a->shared->start, curr), a->tid);
	// msleep(a->shared->time_to_sleep);
	ft_msleep(a->shared->time_to_sleep);
}
