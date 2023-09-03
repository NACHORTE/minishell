# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/03 15:02:39 by iortega-          #+#    #+#              #
#    Updated: 2023/09/03 19:20:37 by iortega-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= minishell

CC		:= gcc

LIB_DIR := libft

LIBFT	:= libft.a

INCLUDE_DIR := $(LIB_DIR)/include

STANDARD_FLAGS	:= -lreadline #-Wall -Wextra -Werror #-fsanitize=leak

SRCS			:= main.c \

OBJS			:= $(SRCS:.c=.o)

INCLUDE_PATH := $(addprefix -I, $(INCLUDE_DIR))

all:			$(NAME)

$(NAME): $(OBJS) $(LIB_DIR)/$(LIBFT)
				$(CC) $(OBJS) $(LIB_DIR)/$(LIBFT) $(STANDARD_FLAGS) -o $(NAME)

$(OBJS): $(SRCS)
				$(CC) $(STANDARD_FLAGS) $(INCLUDE_PATH) -c $(SRCS)

$(LIB_DIR)/$(LIBFT):
	@make -C $(LIB_DIR) all

clean:
				rm -rf $(OBJS)

fclean:
				make clean
				rm -rf $(NAME)
re:
				make fclean
				make