/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_think.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:24:03 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:24:04 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	ph_think(void *args)
{
	t_args			*a;
	struct timeval	curr;
	int				ts;

	a = (t_args *)args;
	pthread_mutex_lock(&a->shared->mtx_printf);
	if (stop(args))
	{
		pthread_mutex_unlock(&a->shared->mtx_printf);
		return (1);
	}
	gettimeofday(&curr, NULL);
	ts = timestamp_ms(a->shared->start, curr);
	printf("%d %d is thinking\n", ts, a->tid + 1);
	pthread_mutex_unlock(&a->shared->mtx_printf);
	return (0);
}
