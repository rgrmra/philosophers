/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:34:24 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/15 22:54:58 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include <sys/types.h>
# include <semaphore.h>

# define PHILOSOPHERS 200

# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define RFORK "has taken a fork"
# define LFORK "has taken a fork"
# define DIED "\033[0;91mdied\033[0m"

typedef struct s_ctx	t_ctx;
typedef struct s_philo	t_philo;

struct s_philo
{
	pid_t		pid;
	short		id;
	suseconds_t	start;
	suseconds_t	last_meal;
	char		*sem_meal;
	int			meals;
	sem_t		*meal_lock;
	_Bool		dead;
	char		*sem_fork;
	sem_t		*r_fork;
	sem_t		*l_fork;
	t_ctx		*ctx;
};

struct	s_ctx
{
	_Bool		dead;
	suseconds_t	die;
	suseconds_t	eat;
	suseconds_t	sleep;
	int			philos;
	int			meals;
	sem_t		*dead_lock;
	sem_t		*write_lock;
};

long		ft_atol(char *pntr);
void		ft_putendl_fd(char *str, int fd);
void		ft_usleep(t_philo *philo, suseconds_t milliseconds);
suseconds_t	current_time(void);
void		print_log(t_philo *philo, char *message);
void		eating(t_philo *philo);
void		sleeping(t_philo *philo);
void		thinking(t_philo *philo);
void		*monitoring(t_philo *philos);
void		*routine(t_philo *philos);
void		init_philos(t_ctx *ctx, t_philo *philos, sem_t *forks);
void		destroy_all(t_ctx *ctx, t_philo *philos);

#endif