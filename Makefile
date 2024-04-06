# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mpihur <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/06 16:36:18 by mpihur            #+#    #+#              #
#    Updated: 2024/04/06 17:51:54 by mpihur           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= pipex
CCFL	= cc -Wall -Wextra -Werror -g
LIB		= -I. -I./Libft -lft -L./Libft
LIBFT	= ./Libft/libft.a
SRC		= main.c run_processes.c
OBJ		= $(SRC:.c=.o)

%.o:	%.c
		$(CCFL) -c $^ -o $@

$(NAME):	$(LIBFT) $(OBJ)
			$(CCFL) $(OBJ) -o $@ $(LIB)

$(LIBFT): 
		make -C ./Libft

clean:	
		rm -f $(OBJ)
		make -C ./Libft clean
	
fclean:	
		clean
		rm -f $(NAME)
		make -C ./Libft fclean

all:	$(LIBFT) $(NAME) 

re:		fclean all

.PHONY: all clean fclean re
