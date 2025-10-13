#include "philo.h"

void	think(void *args)
{
	t_args			*a;
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	a = (t_args *)args;
	printf("%d %d is thinking\n", timestamp_ms(a->shared->start, curr), a->tid);
}
