/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:34:24 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/01 18:19:12 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_ctx	t_ctx;
typedef struct s_time	t_time;
typedef struct s_philo	t_philo;
typedef pthread_mutex_t	t_mtx;

struct s_time
{
	long	hour;
	long	min;
	long	sec;
	long	milli;
};

struct s_philo
{
	pthread_t	thread;
	int			id;
	int			eating;
	int			meals_eaten;
	size_t		last_meal;
	t_mtx		r_fork;
	t_mtx		*l_fork;
	t_ctx		*ctx;
};

struct	s_ctx
{
	t_philo	philo[200];
	int		dead_flag;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	int		num_of_philos;
	int		num_times_to_eat;
	t_time	time;
	t_mtx	dead_lock;
	t_mtx	meal_lock;
	t_mtx	write_lock;
};

long	ft_atol(char *pntr);
void	ft_putendl_fd(char *str, int fd);

#endif
