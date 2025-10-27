/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:24:50 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:24:50 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	timestamp_ms(struct timeval start, struct timeval curr)
{
	int		diff;
	long	curr_ms;
	long	start_ms;

	start_ms = start.tv_sec * 1000000 + start.tv_usec;
	curr_ms = curr.tv_sec * 1000000 + curr.tv_usec;
	diff = curr_ms - start_ms;
	return ((int)(diff / 1000));
}

long long	now_ts(void)
{
	struct timeval	tv;
	long long		now;

	gettimeofday(&tv, NULL);
	now = (long long)tv.tv_sec * 1000000 + (long long)tv.tv_usec;
	return (now);
}

void	ft_msleep(int ms)
{
	const long long	end = now_ts() + (long long)ms * 1000;

	while (now_ts() < end)
	{
		usleep(10);
	}
}
