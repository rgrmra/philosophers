/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:34:24 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/06 23:56:30 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>

# define PHILOSOPHERS 200

# define RED "\033[0;91m"
# define GREEN "\033[0;92m"
# define YELLOW "\033[0;93m"
# define BLUE "\033[0;94m"
# define MAGENTA "\033[0;95m"
# define CYAN "\033[0;96m"
# define WHITE "\033[0;97m"
# define RESET "\033[0m"


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
	size_t		start;
	size_t		last_meal;
	int			meals;
	t_mtx		r_fork;
	t_mtx		*l_fork;
	t_ctx		*ctx;
};

struct	s_ctx
{
	t_philo	*philo;
	_Bool	dead;
	size_t	die_time;
	size_t	eat_time;
	size_t	sleep_time;
	int		philos;
	int		meals;
	t_mtx	meal_lock;
	t_mtx	dead_lock;
	t_mtx	write_lock;
};

long	ft_atol(char *pntr);
void	ft_putendl_fd(char *str, int fd);

#endif
