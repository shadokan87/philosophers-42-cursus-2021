/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoure <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 02:06:32 by motoure           #+#    #+#             */
/*   Updated: 2021/10/19 02:06:37 by motoure          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libphilo.h"

static void	take_fork(t_philo *philo, pthread_mutex_t *_fork)
{
	pthread_mutex_lock(_fork);
	echo(philo, TOOK_FORK);
}

static void	solo_eat(t_philo *philo)
{
	take_fork(philo, &philo->kernel_ptr->_fork[philo->id - 1]);
	kernel_sleep(philo->kernel_ptr, philo->ttd);
	final_log(philo, "died");
	pthread_mutex_unlock(&philo->kernel_ptr->_fork[philo->id - 1]);
	philo->kernel_ptr->sig = SIG_TERMINATE;
}

static void	eat(t_philo *philo)
{
	if (philo->id == philo->n->id)
	{
		solo_eat(philo);
		return ;
	}
	if (philo->id < philo->n->id)
	{
		take_fork(philo, &philo->kernel_ptr->_fork[philo->id - 1]);
		take_fork(philo, &philo->kernel_ptr->_fork[philo->n->id - 1]);
	}
	else
	{
		take_fork(philo, &philo->kernel_ptr->_fork[philo->n->id - 1]);
		take_fork(philo, &philo->kernel_ptr->_fork[philo->id - 1]);
	}
	echo(philo, EATING);
	send_stats_to_kernel_and_unlock_fork(philo);
	echo(philo, "is sleeping");
	kernel_sleep(philo->kernel_ptr, philo->tts);
	echo(philo, THINKING);
}

void	*loop(t_philo *philo)
{
	int	sig;

	sig = -1;
	while (sig != SIG_TERMINATE && sig != SIG_BLOCK_EAT)
	{
		philo_signal(philo->kernel_ptr, &sig);
		if ((philo->id % 2 == 0 && sig == SIG_EVEN)
			|| ((philo->id % 2 != 0 && sig == SIG_ODD)))
			eat(philo);
	}
	end_prog(philo);
	return ((void *)0);
}
