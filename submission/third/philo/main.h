/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:44:14 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/23 15:44:15 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_shared
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*fork;
	int				is_die;
	int				*fin_flags;
	pthread_mutex_t	mtx_fin_flags;
	pthread_mutex_t	mtx_printf;
	int				blocked_tid;
	pthread_mutex_t	mtx_blocked;
	int				urgent_tid;
	pthread_mutex_t	mtx_urgent;
	struct timeval	start;
}	t_shared;

typedef struct s_args
{
	int				tid;
	t_shared		*shared;
	pthread_mutex_t	mtx_last_eat_ts;
	struct timeval	*last_eat_ts;
}	t_args;

typedef struct s_urgent
{
	struct timeval	now;
	int				i;
	int				n;
	int				max_ms;
	int				min_ms;
	int				urgent;
	int				sated;
	int				ms;
}	t_urgent;

int			input_check(int argc, char **argv);
t_shared	*shared_init(int argc, char **argv, int *is_miss_init);
pthread_t	*th_init(t_shared *shared, int *is_miss_init);
t_args		*args_init(t_shared *shared, int *is_miss_init);
void		make_thread(t_args *args, pthread_t *th, t_shared *shared);
void		destory(t_args *args, pthread_t *th);
int			timestamp_ms(struct timeval start, struct timeval curr);
void		ft_msleep(int ms);
void		*worker(void *args);
void		*monitor(void *args);
void		update_priority(t_args *a);
int			stop(void *args);
int			printf_wrap(char *str, void *args);
char		*ft_itoa(int n);
int			ft_atoi(const char *nptr);
int			ph_think(void *args);
int			odd_case(void *args);
int			ph_eat(void *args);
int			ph_sleep(void *args);
char		*ft_strdup(const char *s);
size_t		ft_strlen(const char *s);
int			stop(void *args);
#endif