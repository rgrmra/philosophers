/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:03:55 by rde-mour          #+#    #+#             */
/*   Updated: 2024/05/30 15:20:51 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

long	ft_atol(const char *nptr)
{
	char	sign;
	long	nb;

	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	sign = 0;
	if (*nptr == '+' || *nptr == '-')
		sign = *nptr++;
	nb = 0;
	while (*nptr >= '0' && *nptr <= '9')
		nb = (nb * 10) + *nptr++ - 48;
	if (sign == '-')
		nb = -nb;
	if (*nptr)
		return (-1);
	return (nb);
}
