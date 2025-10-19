/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:25:34 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/12 17:25:35 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	make_thread(t_args *args, pthread_t *th, t_shared *shared)
{
	int			i;
	pthread_t	th_die;

	i = 0;
	while (i < shared->number_of_philosophers)
	{
		args[i].tid = i;
		args[i].shared = shared;
		*(args[i].last_eat_ts) = shared->start;
		pthread_create(&th[i], NULL, worker, &args[i]);
		i++;
	}
	pthread_create(&th_die, NULL, die_worker, args);
	i = 0;
	while (i < shared->number_of_philosophers)
		pthread_join(th[i++], NULL);
	pthread_join(th_die, NULL);
}

int	input_check(int argc, char **argv)
{
	int	i;
	int	nbr;

	if (argc > 6 || argc < 5)
		return (-1);
	i = 1;
	while (i < argc)
	{
		nbr = ft_atoi(argv[i]);
		if (nbr <= 0)
			return (-1);
		i++;
	}
	if (argc == 6)
	{
		nbr = ft_atoi(argv[5]);
		if (nbr < 0)
			return (-1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_shared	*shared;
	pthread_t	*th;
	t_args		*args;
	int			is_miss_init;

	if (input_check(argc, argv) == -1)
		return (1);
	is_miss_init = 0;
	shared = shared_init(argc, argv, &is_miss_init);
	if (!shared || is_miss_init)
	{
		destory(NULL, NULL);
		return (1);
	}
	th = NULL;
	th_init(shared, &th, &is_miss_init);
	args = args_init(shared, &is_miss_init);
	if (!args || is_miss_init)
	{
		destory(args, th);
		return (1);
	}
	make_thread(args, th, shared);
	destory(args, th);
	return (0);
}
