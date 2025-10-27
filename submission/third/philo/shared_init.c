/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:24:25 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:24:26 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	input_init(t_shared *shared, int argc, char **argv)
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

void	pthread_init(t_shared *shared, int *is_miss_init)
{
	int	i;
	int	n;

	n = shared->number_of_philosophers;
	shared->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * n);
	if (!shared->fork)
		return ;
	i = 0;
	while (i < n)
	{
		if (pthread_mutex_init(&shared->fork[i++], NULL) != 0)
			*is_miss_init = 1;
	}
	if (pthread_mutex_init(&shared->mtx_fin_flags, NULL) != 0)
		*is_miss_init = 1;
	if (pthread_mutex_init(&shared->mtx_printf, NULL) != 0)
		*is_miss_init = 1;
	if (pthread_mutex_init(&shared->mtx_blocked, NULL) != 0)
		*is_miss_init = 1;
	if (pthread_mutex_init(&shared->mtx_urgent, NULL) != 0)
		*is_miss_init = 1;
}

void	block_init(t_shared *shared)
{
	int	n;

	n = shared->number_of_philosophers;
	if (n % 2 == 1)
		shared->blocked_tid = 0;
	else
		shared->blocked_tid = -1;
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
	input_init(shared, argc, argv);
	pthread_init(shared, is_miss_init);
	if (*is_miss_init)
		return (shared);
	n = shared->number_of_philosophers;
	shared->is_die = 0;
	shared->fin_flags = (int *)malloc(sizeof(int) * n);
	if (!shared->fin_flags)
		*is_miss_init = 1;
	memset(shared->fin_flags, 0, n * sizeof(int));
	if (gettimeofday(&shared->start, NULL))
		*is_miss_init = 1;
	block_init(shared);
	shared->urgent_tid = -1;
	return (shared);
}
