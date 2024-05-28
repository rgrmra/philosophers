/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:03:55 by rde-mour          #+#    #+#             */
/*   Updated: 2024/05/28 18:10:57 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

long	ft_atol(const char *nptr)
{
	short	sign;
	long	nb;

	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	sign = 0;
	if (*nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			sign++;
	nb = 0;
	while (*nptr >= '0' && *nptr <= '9')
	{
		nb *= 10;
		nb += *nptr - 48;
		nptr++;
	}
	if (sign)
		nb = -nb;
	return (nb);
}
