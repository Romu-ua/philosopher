/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:36:18 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/13 18:36:18 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	atois_init(t_shared *shared, int argc, char **argv)
{
	shared->number_of_philosophers = ft_atoi(argv[1]);
	shared->time_to_die = ft_atoi(argv[2]);
	shared->time_to_eat = ft_atoi(argv[3]);
	shared->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		shared->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		shared->number_of_times_each_philosopher_must_eat = -1;
}

void	pthreads_init(t_shared *shared, int *is_miss_init)
{
	int	i;
	int	n;

	n = shared->number_of_philosophers;
	i = 0;
	while (i < n)
	{
		if (pthread_mutex_init(&shared->mtx[i++], NULL) != 0)
			*is_miss_init = 1;
	}
	if (pthread_mutex_init(&shared->mtx_waiting, NULL) != 0)
		*is_miss_init = 1;
	if (pthread_mutex_init(&shared->mtx_fin_flags, NULL) != 0)
		*is_miss_init = 1;
	if (pthread_mutex_init(&shared->mtx_printf, NULL) != 0)
		*is_miss_init = 1;
}

void	shared_cleanup(t_shared *shared)
{
	if (!shared)
		return ;
	if (shared->mtx)
		free(shared->mtx);
	if (shared->fin_flags)
		free(shared->fin_flags);
	free(shared);
}

t_shared	*shared_init(int argc, char **argv, int *is_miss_init)
{
	t_shared	*shared;
	int			n;

	shared = (t_shared *)malloc(sizeof(t_shared));
	if (!shared)
	{
		*is_miss_init = 1;
		return (NULL);
	}
	atois_init(shared, argc, argv);
	n = shared->number_of_philosophers;
	shared->mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * n);
	if (!shared->mtx)
		util_return(is_miss_init, shared);
	pthreads_init(shared, is_miss_init);
	if (*is_miss_init)
		return (cleanup_return(shared));
	shared->is_die = 0;
	shared->fin_flags = (int *)malloc(sizeof(int) * n);
	if (!shared->fin_flags)
		return (util_return(is_miss_init, shared));
	if (gettimeofday(&shared->start, NULL))
		return (util_return(is_miss_init, shared));
	return (shared);
}
