/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads[philo].c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoure <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 02:06:39 by motoure           #+#    #+#             */
/*   Updated: 2021/10/19 14:52:33 by motoure          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libphilo.h"

void	join_threads(t_philo *philo)
{
	int			n;
	t_philo		*iterator;

	n = philo->kernel_ptr->n_philo;
	iterator = philo;
	while (n)
	{
		pthread_join(iterator->tid, NULL);
		iterator = iterator->n;
		n--;
	}
}

void	create_threads(t_philo *philo)
{
	int			n;
	t_philo		*iterator;

	n = philo->kernel_ptr->n_philo;
	iterator = philo;
	while (n)
	{
		pthread_create(&iterator->tid, NULL, (void *)&loop, iterator);
		iterator = iterator->n;
		n--;
	}
}

t_philo	*create_one_philo(t_kernel *kernel, int id, int argc, char **argv)
{
	t_philo	*ret;

	(void)argc;
	ret = malloc(sizeof(t_philo));
	if (!ret)
		return (NULL);
	ret->id = id;
	ret->died = 0x0;
	ret->tte = ft_atoi(argv[TTE]);
	ret->ttd = ft_atoi(argv[TTD]);
	ret->tts = ft_atoi(argv[TTS]);
	ret->le = 0x0;
	ret->ate = 0;
	ret->kernel_ptr = kernel;
	ret->n = NULL;
	return (ret);
}

t_philo	*create_n_philo(t_kernel *kernel, int argc, char **argv)
{
	t_philo	*ret;
	t_philo	*current;
	t_philo	*last;
	int		i;

	i = 0;
	ret = NULL;
	while (i < ft_atoi(argv[N]))
	{
		if (!ret)
			ret = create_one_philo(kernel, i + 1, argc, argv);
		else
		{
			current = create_one_philo(kernel, i + 1, argc, argv);
			current->n = ret;
			ret = current;
		}
		i++;
	}
	last = ret;
	while (last && last->n)
		last = last->n;
	last->n = ret;
	return (ret);
}
