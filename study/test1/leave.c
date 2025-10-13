#include "philo.h"

void	leave(void *args)
{
	t_args	*a;

	a = (t_args *)args;

	pthread_mutex_lock(&a->shared->adm_enter);
	a->shared->number_of_inside--;
	pthread_mutex_unlock(&a->shared->adm_enter);
}
