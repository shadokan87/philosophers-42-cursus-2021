/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libphilo.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoure <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 02:07:31 by motoure           #+#    #+#             */
/*   Updated: 2021/10/19 02:07:33 by motoure          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBPHILO_H
# define LIBPHILO_H

/* ========================================================================== */
/*                                 INCLUDES                                   */
/* ========================================================================== */

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

/* ========================================================================== */
/*                                  KERNEL SIGNALS                            */
/* ========================================================================== */

# define SIG_EVEN 0x00
# define SIG_ODD 0x01
# define SIG_TERMINATE 0x02
# define SIG_BLOCK_EAT 0x03

/* ========================================================================== */
/*                                  ARGV INDEXES / ECHO MESSAGES              */
/* ========================================================================== */

# define N 1
# define TTD 2
# define TTE 3
# define TTS 4
# define EAT_MAX 5

# define TOOK_FORK "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"

/* ========================================================================== */
/*                                  STRUCTS                                   */
/* ========================================================================== */

typedef struct s_kernel
{
	int				sig;
	int				odd_max_eat;
	int				even_max_eat;
	int				even_max_eat_tmp;
	int				odd_max_eat_tmp;
	int				eat_limit;
	int				n_philo;
	long			time;
	void			*philo_ptr;
	pthread_mutex_t	write_data;
	pthread_mutex_t	read_data;
	pthread_mutex_t	check_death;
	pthread_mutex_t	terminate;
	pthread_mutex_t	echoing;
	pthread_mutex_t	*_fork;
}	t_kernel;

typedef struct s_philo
{
	int					id;
	int					died;
	int					tte;
	int					ttd;
	int					tts;
	int					ate;
	long				le;
	t_kernel			*kernel_ptr;
	pthread_mutex_t		_fork;
	pthread_t			tid;
	struct s_philo		*n;
}	t_philo;

/* ========================================================================== */
/*                                  STRING_UTILITIES                          */
/* ========================================================================== */

int			ft_strlen(char *str);
char		*ft_strdup(const char *s1);
int			ft_atoi(const char *str);

char		*ft_itoa(int n);

/* ========================================================================== */
/*                                  PHILO_THREADS                             */
/* ========================================================================== */

void		join_threads(t_philo *philo);
void		create_threads(t_philo *philo);
void		*loop(t_philo *philo);
t_philo		*create_one_philo(t_kernel *kernel, int id, int argc, char **argv);
t_philo		*create_n_philo(t_kernel *kernel, int argc, char **argv);

/* ========================================================================== */
/*                                  KERNEL                                    */
/* ========================================================================== */

t_kernel	*init_kernel(int argc, char **argv);
void		end_prog(t_philo *philo);
void		kernel_sleep(t_kernel *kernel, long t);
void		philo_signal(t_kernel *kernel, int *signal);
void		send_stats_to_kernel_and_unlock_fork(t_philo *philo);

long		ft_time(void);
int			check_death(t_kernel *kernel);
void		final_log(t_philo *philo, char *message);
void		echo(t_philo *philo, char *message);
#endif
