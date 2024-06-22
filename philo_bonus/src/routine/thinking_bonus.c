/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thinking_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 13:01:55 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/22 18:42:50 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context_bonus.h"
#include <semaphore.h>

void	thinking(t_philo *philo)
{
	print_log(philo, THINK);
	ft_usleep((philo->ctx->die - philo->ctx->eat - philo->ctx->sleep) - 100);
}
