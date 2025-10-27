/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:23:23 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:23:24 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	make_thread(t_args *args, pthread_t *th, t_shared *shared)
{
	int			i;
	int			n;
	pthread_t	th_die;

	n = shared->number_of_philosophers;
	i = 0;
	while (i < n)
	{
		args[i].tid = i;
		args[i].shared = shared;
		*(args[i].last_eat_ts) = shared->start;
		pthread_create(&th[i], NULL, worker, &args[i]);
		i++;
	}
	pthread_create(&th_die, NULL, monitor, args);
	i = 0;
	while (i < n)
		pthread_join(th[i++], NULL);
	pthread_join(th_die, NULL);
}
