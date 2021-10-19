/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoure <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 02:06:19 by motoure           #+#    #+#             */
/*   Updated: 2021/10/19 14:57:26 by motoure          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libphilo.h"

void	philo_signal(t_kernel *kernel, int *signal)
{
	int	ret;

	pthread_mutex_lock(&kernel->read_data);
	pthread_mutex_lock(&kernel->write_data);
	if (kernel->sig == SIG_EVEN && !kernel->even_max_eat)
	{
		kernel->sig = SIG_ODD;
		kernel->even_max_eat = kernel->even_max_eat_tmp;
	}
	else if (kernel->sig == SIG_ODD && !kernel->odd_max_eat)
	{
		kernel->sig = SIG_EVEN;
		kernel->odd_max_eat = kernel->odd_max_eat_tmp;
	}
	ret = check_death(kernel);
	if (ret == 1)
		kernel->sig = SIG_TERMINATE;
	else if (ret == 2)
		kernel->sig = SIG_BLOCK_EAT;
	*signal = kernel->sig;
	pthread_mutex_unlock(&kernel->write_data);
	pthread_mutex_unlock(&kernel->read_data);
}

void	send_stats_to_kernel_and_unlock_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->kernel_ptr->write_data);
	philo->le = ft_time() - philo->kernel_ptr->time;
	if (philo->kernel_ptr->sig == SIG_EVEN)
		philo->kernel_ptr->even_max_eat--;
	else
		philo->kernel_ptr->odd_max_eat--;
	philo->ate++;
	pthread_mutex_unlock(&philo->kernel_ptr->write_data);
	kernel_sleep(philo->kernel_ptr, philo->tte);
	pthread_mutex_unlock(&philo->kernel_ptr->_fork[philo->n->id - 1]);
	pthread_mutex_unlock(&philo->kernel_ptr->_fork[philo->id - 1]);
}

void	kernel_sleep(t_kernel *kernel, long t)
{
	long	target;
	int		sig;

	target = ft_time() + t;
	sig = 0x0;
	while (ft_time() < target && sig != SIG_TERMINATE
		&& sig != SIG_BLOCK_EAT)
	{
		philo_signal(kernel, &sig);
		usleep(10);
	}
}

static t_kernel	*init_kernel2(t_kernel *ret, int argc, char **argv)
{
	int	i;

	i = -1;
	if (argc == 6)
		ret->eat_limit = ft_atoi(argv[EAT_MAX]);
	else
		ret->eat_limit = 0x0;
	if (ret->n_philo % 2 == 0)
		ret->odd_max_eat = ret->even_max_eat;
	else
		ret->odd_max_eat = ret->even_max_eat + 1;
	ret->even_max_eat_tmp = ret->even_max_eat;
	ret->odd_max_eat_tmp = ret->odd_max_eat;
	pthread_mutex_init(&ret->write_data, NULL);
	pthread_mutex_init(&ret->read_data, NULL);
	pthread_mutex_init(&ret->echoing, NULL);
	pthread_mutex_init(&ret->check_death, NULL);
	pthread_mutex_init(&ret->terminate, NULL);
	while (++i < ret->n_philo)
		pthread_mutex_init(&ret->_fork[i], NULL);
	return (ret);
}

t_kernel	*init_kernel(int argc, char **argv)
{
	t_kernel	*ret;

	(void)argc;
	ret = malloc(sizeof(t_kernel));
	if (!ret)
		return (NULL);
	ret->sig = SIG_EVEN;
	ret->n_philo = ft_atoi(argv[N]);
	ret->_fork = malloc(sizeof(pthread_mutex_t) * ret->n_philo);
	if (!ret->_fork)
		return (NULL);
	ret->time = ft_time();
	ret->even_max_eat = ret->n_philo / 2;
	return (init_kernel2(ret, argc, argv));
}
