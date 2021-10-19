
NAME	=	philo

CC		=	clang

RM		=	rm -f

CFLAGS	=	-Wall -Wextra -Werror

LFLAGS	=	-lpthread


SRCS	=	./kernel/kernel.c \
			./kernel/kernel2.c \
			./philo_threads/routine.c \
			./philo_threads/threads[philo].c \
			./utils/string_utils1.c \
			./utils/string_utils2.c \
			./main.c

OBJS	=	$(SRCS:.c=.o)


all:		$(NAME)
	if ls | grep -w Makesplash;then echo "\t\t\n\t\t------\n\tPhilo: generated\n\t\t------\n" && cat Makesplash;fi

$(NAME):	$(OBJS)
			${CC} ${CFLAGS} ${LFLAGS} -o ${NAME} ${OBJS} ${LIBFT}

clean:
			$(RM) $(OBJS)
	if ls | grep -w Makesplash;then echo "\t\t\n\t\t------\n\tPhilo: cleaned object files\n\t\t------\n" && cat Makesplash;fi

fclean:		clean
			$(RM) $(NAME)

re:			fclean $(NAME)
	if ls | grep -w Makesplash;then echo "\t\t\n\t\t------\n\tPhilo: Re make\n\t\t------\n" && cat Makesplash;fi

.PHONY:		all clean fclean re f
