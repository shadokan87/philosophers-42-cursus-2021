/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoure <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 02:06:12 by motoure           #+#    #+#             */
/*   Updated: 2021/10/19 14:59:11 by motoure          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libphilo.h"

long	ft_time(void)
{
	struct timeval	tv;
	long			ret;

	gettimeofday(&tv, (void *)0);
	ret = tv.tv_sec * 1000;
	ret += tv.tv_usec / 1000;
	return (ret);
}

void	echo(t_philo *philo, char *message)
{
	char	*elapsed_time;
	char	*id;
	int		terminate;

	pthread_mutex_lock(&philo->kernel_ptr->echoing);
	id = ft_itoa(philo->id);
	elapsed_time = ft_itoa(ft_time() - philo->kernel_ptr->time);
	pthread_mutex_lock(&philo->kernel_ptr->write_data);
	terminate = (philo->kernel_ptr->sig == SIG_TERMINATE
			|| philo->kernel_ptr->sig == SIG_BLOCK_EAT);
	pthread_mutex_unlock(&philo->kernel_ptr->write_data);
	if (!terminate)
	{
		write(1, elapsed_time, ft_strlen(elapsed_time));
		write(1, " ", 1);
		write(1, id, ft_strlen(id));
		write(1, " ", 1);
		write(1, message, ft_strlen(message));
		write(1, "\n", 1);
	}
	free(id);
	free(elapsed_time);
	pthread_mutex_unlock(&philo->kernel_ptr->echoing);
}

void	final_log(t_philo *philo, char *message)
{
	static int	log;
	char		*elapsed_time;
	char		*id;

	id = ft_itoa(philo->id);
	elapsed_time = ft_itoa(ft_time() - philo->kernel_ptr->time);
	if (!log)
	{
		write(1, elapsed_time, ft_strlen(elapsed_time));
		write(1, " ", 1);
		write(1, id, ft_strlen(id));
		write(1, " ", 1);
		write(1, message, ft_strlen(message));
		write(1, "\n", 1);
		log = 1;
	}
	free(id);
	free(elapsed_time);
}

static	int	set_ret(int statement, int ret)
{
	if (statement)
		return (2);
	return (ret);
}

int	check_death(t_kernel *kernel)
{
	t_philo	*iterator;
	int		i;
	int		ret;
	int		count_eat;

	pthread_mutex_lock(&kernel->check_death);
	iterator = kernel->philo_ptr;
	i = kernel->n_philo + 1;
	ret = 0;
	count_eat = 0;
	while (iterator && --i)
	{
		if (iterator->le != 0x0 && ((ft_time() - kernel->time)
				- iterator->le) >= iterator->ttd)
		{
			ret = 1;
			iterator->died = 1;
		}
		if (kernel->eat_limit > 0 && iterator->ate == kernel->eat_limit)
			count_eat++;
		iterator = iterator->n;
	}
	ret = set_ret((count_eat == kernel->n_philo), ret);
	pthread_mutex_unlock(&kernel->check_death);
	return (ret);
}
