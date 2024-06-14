/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:34:24 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/14 14:11:00 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

# define PHILOSOPHERS 200

# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define RFORK "has taken a fork"
# define LFORK "has taken a fork"
# define DIED "\033[0;91mdied\033[0m"

typedef struct s_ctx	t_ctx;
typedef struct s_philo	t_philo;
typedef pthread_mutex_t	t_mtx;

struct s_philo
{
	pthread_t	thread;
	short		id;
	suseconds_t	start;
	suseconds_t	last_meal;
	int			meals;
	t_mtx		meal_lock;
	_Bool		dead;
	t_mtx		*r_fork;
	t_mtx		*l_fork;
	t_ctx		*ctx;
};

struct	s_ctx
{
	pthread_t	supervisor;
	_Bool		dead;
	suseconds_t	die_time;
	suseconds_t	eat_time;
	suseconds_t	sleep_time;
	int			philos;
	int			meals;
	t_mtx		dead_lock;
	t_mtx		write_lock;
};

long	ft_atol(char *pntr);
void	ft_putendl_fd(char *str, int fd);

#endif
