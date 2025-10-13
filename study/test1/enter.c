#include "philo.h"

void	enter(void *args)
{
	t_args	*a;

	a = (t_args *)args;
	while (1)
	{
		if (a->shared->capa > a->shared->number_of_inside)
		{
			pthread_mutex_lock(&a->shared->adm_enter);
			a->shared->number_of_inside++;
			pthread_mutex_lock(&a->shared->is_waiting);
			get_out_of_line(args);

			pthread_mutex_unlock(&a->shared->is_waiting);
			pthread_mutex_unlock(&a->shared->adm_enter);
			break ;
		}
	}
	return ;
}
