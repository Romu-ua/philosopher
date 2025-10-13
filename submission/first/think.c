/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   think.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:25:50 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/12 17:25:51 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	think(void *args)
{
	t_args			*a;
	struct timeval	curr;

	a = (t_args *)args;
	pthread_mutex_lock(&a->shared->mtx_printf);
	gettimeofday(&curr, NULL);
	printf("%d %d is thinking\n", timestamp_ms(a->shared->start, curr), a->tid);
	pthread_mutex_unlock(&a->shared->mtx_printf);
}
