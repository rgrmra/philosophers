/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleeping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 13:00:39 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/16 15:03:18 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"

void	sleeping(t_philo *philo)
{
	print_log(philo, SLEEP);
	ft_usleep(philo->ctx->sleep);
}
