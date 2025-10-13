// スタベーションの問題が解決できていない？
// sleepを効率的にする。

#include "philo.h"

t_shared	*shared_init(int argc, char **argv)
{
	t_shared	*shared;

	shared = (t_shared *)malloc(sizeof(t_shared));
	if (!shared)
		return (NULL);
	shared->number_of_philosophers = atoi(argv[1]);
	shared->time_to_die = atoi(argv[2]);
	shared->time_to_eat = atoi(argv[3]);
	shared->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		shared->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	else
		shared->number_of_times_each_philosopher_must_eat = -1;

	shared->mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * shared->number_of_philosophers);
	if (!shared->mtx)
	{
		free(shared);
		return (NULL);
	}
	if(pthread_mutex_init(&shared->is_waiting, NULL) != 0)
	{
		free(shared->mtx);
		return (NULL);
	}

	if (pthread_mutex_init(&shared->adm_enter, NULL) != 0)
	{
		pthread_mutex_destroy(&shared->is_waiting);
		free(shared->mtx);
		return (NULL);
	}
	
	shared->capa = shared->number_of_philosophers - 1;
	shared->q = (t_queue *)malloc(sizeof(t_queue));
	if (!shared->q)
	{
		free(shared->mtx);
		free(shared);
		pthread_mutex_destroy(&shared->is_waiting);
		pthread_mutex_destroy(&shared->adm_enter);
		return (NULL);
	}
	shared->q->front = NULL;
	shared->q->rear = NULL;
	shared->fin_flags = (int *)malloc(sizeof(int) * shared->number_of_philosophers);
	if (!shared->fin_flags)
	{
		free(shared->q);
		free(shared->mtx);
		free(shared);
		pthread_mutex_destroy(&shared->is_waiting);
		pthread_mutex_destroy(&shared->adm_enter);
		return (NULL);
	}
	memset(shared->fin_flags, 0, sizeof (int) * shared->number_of_philosophers);
	if (pthread_mutex_init(&shared->mtx_fin_flags, NULL))
	{
		free(shared->q);
		free(shared->mtx);
		free(shared);
		pthread_mutex_destroy(&shared->is_waiting);
		pthread_mutex_destroy(&shared->adm_enter);
		free(shared->fin_flags);
		return (NULL);
	}
	for (int i = 0; i < shared->number_of_philosophers; i++) {
		if (pthread_mutex_init(&(shared->mtx[i]), NULL))
		{
			for (int j = i; j > 0; j--) {
				pthread_mutex_destroy(&(shared->mtx[j - 1]));
			}
			free(shared->q);
			free(shared->mtx);
			free(shared);
			pthread_mutex_destroy(&shared->is_waiting);
			pthread_mutex_destroy(&shared->adm_enter);
			free(shared->fin_flags);
			pthread_mutex_destroy(&shared->mtx_fin_flags);
			return (NULL);
		}
	}
	if (gettimeofday(&shared->start, NULL) == -1)
	{
		for (int i = 0; i < shared->number_of_philosophers; i++) {
			pthread_mutex_destroy(&(shared->mtx[i]));
		}
		free(shared->q);
		free(shared->mtx);
		free(shared);
		pthread_mutex_destroy(&shared->is_waiting);
		pthread_mutex_destroy(&shared->adm_enter);
		free(shared->fin_flags);
		return (NULL);
	}
	return (shared);
}

int	make_thread(t_args *args, pthread_t *th, t_shared *shared)
{
	int			i;
	pthread_t	th_die;

	i = 0;
	pthread_mutex_init(&shared->mtx_die, NULL);
	while (i < shared->number_of_philosophers)
	{
		args[i].tid = i;
		args[i].shared = shared;
		*(args[i].last_eat_ts) = shared->start;
		if (pthread_create(&th[i], NULL, worker, &args[i]))
			return (1);
		i++;
	}
	if (pthread_create(&th_die, NULL, die_worker, args))
		return (1);
	for (int i = 0; i < shared->number_of_philosophers; i++) {
		pthread_join(th[i], NULL);
	}
	pthread_join(th_die, NULL);
	for (int i = 0; i < shared->number_of_philosophers; i++) {
		pthread_mutex_destroy(&shared->mtx[i]);
	}
	pthread_mutex_destroy(&shared->mtx_die);
	return (0);
}

t_args	*args_init(t_shared *shared)
{
	t_args	*args;
	int		i;

	args = (t_args *)malloc(sizeof(t_args) * shared->number_of_philosophers);
	i = 0;
	while (i < shared->number_of_philosophers)
	{
		args[i].last_eat_ts = (struct timeval *)malloc(sizeof(struct timeval));
		i++;
	}
	return (args);
}

int	main(int argc, char **argv)
{
	t_shared	*shared;
	pthread_t	*th;
	t_args		*args;

	if (argc > 6 || argc < 5)
		return (1);
	shared = shared_init(argc, argv);
	if (!shared)
		return (1);
	if (shared->number_of_philosophers == 1)
	{
		printf("%d 0 died\n", shared->time_to_die);
		return (0);
	}
	th = (pthread_t *)malloc(sizeof(pthread_t) * shared->number_of_philosophers);
	if (!th)
		return (1);
	args = args_init(shared);
	if (!args)
		return (1);
	if (make_thread(args, th, shared))
		return (1);
	free(shared->mtx);
	free(shared);
	free(th);
	free(args);
}
