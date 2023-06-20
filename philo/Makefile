NAME = philo

CC = CC

CFLAGS = -Wall -Wextra -Werror  
# -g -fsanitize=thread

SRCS = philo.c \
utils.c

OBJS = ${SRCS:.c=.o}

RM = rm -f

all: ${NAME}

${NAME}: ${OBJS}
	@${CC} ${CFLAGS} $^ -o $@

clean:
	@${RM} ${OBJS}

fclean: clean
	@${RM} ${NAME}

re: fclean all