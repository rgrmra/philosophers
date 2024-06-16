/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thinking.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 13:01:55 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/15 22:55:43 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"

void	thinking(t_philo *philo)
{
	print_log(philo, THINK);
	ft_usleep(philo, (philo->ctx->die - philo->ctx->eat - philo->ctx->sleep) / 5);
}
