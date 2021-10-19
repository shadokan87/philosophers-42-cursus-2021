/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoure <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 02:05:38 by motoure           #+#    #+#             */
/*   Updated: 2021/10/19 15:08:32 by motoure          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libphilo.h"

static void	safe_exit(t_philo *philo)
{
	int			i;
	int			n_philo;
	t_kernel	*kernel;
	t_philo		*next;

	i = -1;
	kernel = philo->kernel_ptr;
	n_philo = kernel->n_philo;
	if (kernel->sig == SIG_BLOCK_EAT)
		usleep(philo->ttd * 1000);
	while (++i < n_philo)
		pthread_mutex_destroy(&kernel->_fork[i]);
	free(kernel->_fork);
	free(kernel);
	i = -1;
	while (++i < n_philo)
	{
		next = philo->n;
		free(philo);
		philo = next;
	}
}	

void	end_prog(t_philo *philo)
{
	t_philo	*iterator;
	int		i;

	pthread_mutex_lock(&philo->kernel_ptr->check_death);
	iterator = philo->kernel_ptr->philo_ptr;
	i = philo->kernel_ptr->n_philo;
	while (iterator && i)
	{
		if (iterator->died)
		{
			final_log(philo, "died");
			iterator->died = 0;
			break ;
		}
		iterator = iterator->n;
		i--;
	}
	pthread_mutex_unlock(&philo->kernel_ptr->check_death);
}

static int	params_are_invalid(int argc, char **argv)
{
	int	i;
	int	y;

	i = 0;
	if (!(argc == 5 || argc == 6))
	{
		printf("philo: usage:\n-> number_of_philosophers\n");
		printf("-> time_to_die\n-> time_to_eat\n-> time_to_sleep\n");
		printf("-> [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	while (++i < argc)
	{
		y = 0;
		while (argv[i][y] && argv[i][y] >= '0' && argv[i][y] <= '9')
			y++;
		if (y != ft_strlen(argv[i]) || !y)
			return (params_are_invalid(0, NULL));
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo		*philo;
	t_kernel	*kernel;

	if (params_are_invalid(argc, argv))
		return (0);
	kernel = init_kernel(argc, argv);
	philo = create_n_philo(kernel, argc, argv);
	kernel->philo_ptr = philo;
	create_threads(philo);
	join_threads(philo);
	safe_exit(philo->kernel_ptr->philo_ptr);
	return (0);
}
