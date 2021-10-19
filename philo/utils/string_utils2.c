/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoure <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 02:07:10 by motoure           #+#    #+#             */
/*   Updated: 2021/10/19 02:07:13 by motoure          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libphilo.h"

static char	*ft_strrev(char *str)
{
	int		i;
	int		y;
	char	swap;

	swap = '0';
	i = 0;
	y = ft_strlen(str) - 1;
	while (i < y)
	{
		swap = str[i];
		str[i] = str[y];
		str[y] = swap;
		i++;
		y--;
	}
	return (str);
}

static int	count(int n)
{
	int	count;

	count = 0;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		i;
	char	*ret;

	i = 0;
	if (n == 0)
		return (ft_strdup("0"));
	ret = malloc(sizeof(char) * count(n) + 1);
	if (!ret)
		return (0);
	while (n)
	{
		ret[i] = n % 10 + 48;
		n /= 10;
		i++;
	}
	ret[i] = '\0';
	return (ft_strrev(ret));
}
