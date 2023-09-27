# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/03 15:02:39 by iortega-          #+#    #+#              #
#    Updated: 2023/09/27 21:55:08 by orudek           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := gcc
STANDARD_FLAGS := -I/Users/$(USER)/.brew/opt/readline/include -lreadline -L /Users/$(USER)/.brew/opt/readline/lib -fsanitize=leak -Wall -Wextra -Werror
RM := rm -f

SRC_DIR := src
INCLUDE_DIR := include
OBJ_DIR := obj

LIBFT_DIR := libft
LIBFT_NAME := libft.a

LIB := readline

SRC_FILES := main.c\
	child.c\
	parse/split_args.c\
	parse/split_args2.c\
	parse/expand_variables.c\
	parse/parse.c\
	parse/split_pipe.c\
	variables/array_to_varlist.c\
	variables/free_var.c\
	variables/get_variable.c\
	variables/is_varname_ok.c\
	variables/set_variable.c\
	variables/unset_variable.c\
	variables/varlist_to_array.c\
	exec_cmd/cmd_cd.c\
	exec_cmd/cmd_echo.c\
	exec_cmd/cmd_env.c\
	exec_cmd/cmd_exit.c\
	exec_cmd/cmd_export.c\
	exec_cmd/cmd_pwd.c\
	exec_cmd/cmd_unset.c\
	exec_cmd/exec_cmd.c\
	exec_cmd/exec_builtin.c

SRC := $(addprefix $(SRC_DIR)/,$(SRC_FILES))
LIB_FLAG := $(addprefix -l,$(LIB))
INCLUDE_PATH := -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_FOLDER := $(dir $(OBJ))

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_DIR)/$(LIBFT_NAME)
	$(CC) $(STANDARD_FLAGS)  $(INCLUDE_PATH) $(OBJ) $(LIBFT_DIR)/$(LIBFT_NAME) -lreadline -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_FOLDER)
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
