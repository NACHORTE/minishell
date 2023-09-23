# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/03 15:02:39 by iortega-          #+#    #+#              #
#    Updated: 2023/09/23 20:29:43 by iortega-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := gcc
STANDARD_FLAGS := -I/Users/$(USER)/.brew/opt/readline/include -lreadline -L /Users/$(USER)/.brew/opt/readline/lib -fsanitize=leak #-Wall -Wextra -Werror
RM := rm -f

SRC_DIR := src
INCLUDE_DIR := include
OBJ_DIR := obj

LIBFT_DIR := libft
LIBFT_NAME := libft.a

LIB := readline

SRC_FILES := main.c\
	split_args.c\
	split_args2.c\
	set_variable.c\
	expand_variables.c\
	ft_parse.c\
	split_pipe.c\
	free_var.c\
	get_variable.c\
	varlist_to_array.c\
	array_to_varlist.c\
	child.c\
	unset_variable.c

SRC := $(addprefix $(SRC_DIR)/,$(SRC_FILES))
LIB_FLAG := $(addprefix -l,$(LIB))
INCLUDE_PATH := -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_DIR)/$(LIBFT_NAME)
	$(CC) $(STANDARD_FLAGS)  $(INCLUDE_PATH) $(OBJ) $(LIBFT_DIR)/$(LIBFT_NAME) -lreadline -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(STANDARD_FLAGS) $(INCLUDE_PATH) -c -o $@ $<

$(LIBFT_DIR)/$(LIBFT_NAME):
	@make -C $(LIBFT_DIR) all

clean:
	@$(RM) -r $(OBJ_DIR)
	@make -C $(LIBFT_DIR) fclean
	@echo "Deleted obj files:\n\t$(OBJ)"
	@echo "Deleted $(LIBFT_NAME)"

fclean: clean
	@$(RM) $(NAME)
	@echo "Deleted $(NAME) executable"
	
re: fclean all

.PHONY: clean fclean all re
