/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:34:24 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/19 19:51:07 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_BONUS_H
# define CONTEXT_BONUS_H

# include <sys/types.h>
# include <semaphore.h>

# define PHILOSOPHERS 200

# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define RFORK "has taken a fork"
# define LFORK "has taken a fork"
# define DIED "\033[0;91mdied\033[0m"

# define WRITE_LOCK "write_lock"
# define FORKS_LOCK "forks_lock"
# define FORKS "forks"

typedef struct s_ctx		t_ctx;
typedef struct s_philo		t_philo;
typedef struct s_garbage	t_garbage;

struct s_philo
{
	pid_t		pid;
	short		id;
	suseconds_t	last_meal;
	char		*sem_meal;
	int			meals;
	sem_t		*meal_lock;
	t_ctx		*ctx;
};

struct	s_ctx
{
	pthread_t	supervisor;
	suseconds_t	die;
	suseconds_t	eat;
	suseconds_t	sleep;
	suseconds_t	epoch;
	int			philos;
	int			meals;
	sem_t		*forks;
	sem_t		*forks_lock;
	sem_t		*write_lock;
};

struct	s_garbage
{
	t_philo	*philos;
	t_philo	*philo;
};

long		ft_atol(char *pntr);
void		ft_putendl_fd(char *str, int fd);
void		ft_usleep(suseconds_t milliseconds);
suseconds_t	current_time(void);
void		print_log(t_philo *philo, char *message);
void		eating(t_philo *philo);
void		sleeping(t_philo *philo);
void		thinking(t_philo *philo);
void		*monitoring(void *philos);
void		*routine(void *philos);
void		init_philos(t_ctx *ctx, t_philo *philos);
void		destroy_all(t_ctx *ctx, t_philo *philos);

#endif
