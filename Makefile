# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/03 15:02:39 by iortega-          #+#    #+#              #
#    Updated: 2023/10/06 15:45:53 by iortega-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := gcc
STANDARD_FLAGS := -I/Users/$(USER)/.brew/opt/readline/include -L /Users/$(USER)/.brew/opt/readline/lib -fsanitize=leak -Wall -Wextra -Werror
RM := rm -f

SRC_DIR := src
INCLUDE_DIR := include
OBJ_DIR := obj

LIBFT_DIR := libft
LIBFT_NAME := libft.a

LIB := readline

SRC_FILES := variables/is_varname_ok.c\
	variables/varlist_to_array.c\
	variables/unset_variable.c\
	variables/set_variable.c\
	variables/get_variable.c\
	variables/dup_var.c\
	variables/array_to_varlist.c\
	variables/is_in_varlist.c\
	variables/new_var.c\
	variables/is_end_of_var.c\
	variables/free_var.c\
	variables/set_variable2.c\
	local_declare.c\
	error/return_msg.c\
	error/return_perror.c\
	exec_cmd/builtin/cmd_unset.c\
	exec_cmd/builtin/cmd_exit.c\
	exec_cmd/builtin/cmd_cd.c\
	exec_cmd/builtin/exec_builtin.c\
	exec_cmd/builtin/cmd_pwd.c\
	exec_cmd/builtin/cmd_echo.c\
	exec_cmd/builtin/cmd_env.c\
	exec_cmd/builtin/cmd_export.c\
	exec_cmd/builtin/is_builtin.c\
	exec_cmd/child.c\
	exec_cmd/exec_cmd.c\
	exec_cmd/heredoc.c\
	exec_cmd/exec_multicmd.c\
	exec_cmd/parse_cmd.c\
	exec_cmd/exec_firstpipe.c\
	exec_cmd/rm_assignation.c\
	parse/expand_variables/str_to_redir.c\
	parse/expand_variables/expand_variables.c\
	parse/expand_variables/str_to_arg.c\
	parse/expand_variables/get_expanded_str.c\
	parse/redir/redir.c\
	parse/redir/free_cmd.c\
	parse/redir/redir_streams.c\
	parse/redir/redir_heredoc.c\
	parse/parse.c\
	parse/split_pipe/split_pipe.c\
	parse/split_arg_redir/split_arg_redir.c\
	parse/split_arg_redir/new_arg_redir.c\
	parse/split_arg_redir/free_arg_redir.c\
	parse/format_variables/format_arg_len.c\
	parse/format_variables/format_variables.c\
	parse/format_variables/format_redir.c\
	parse/format_variables/format_args.c\
	main.c\
	is_command.c\
	signals.c

SRC := $(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_FOLDER := $(sort $(dir $(OBJ)))
LIB_FLAG := $(addprefix -l,$(LIB))
INCLUDE_PATH := -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_DIR)/$(LIBFT_NAME)
	$(CC) $(STANDARD_FLAGS)  $(INCLUDE_PATH) $(OBJ) $(LIBFT_DIR)/$(LIBFT_NAME) $(LIB_FLAG) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_FOLDER)
	$(CC) $(STANDARD_FLAGS) $(INCLUDE_PATH) -c -o $@ $<

$(LIBFT_DIR)/$(LIBFT_NAME):
	@make -C $(LIBFT_DIR) all

clean:
	@$(RM) -r $(OBJ_DIR)
	@make -C $(LIBFT_DIR) fclean
	@echo "Deleted obj files"
	@echo "Deleted $(LIBFT_NAME)"

fclean: clean
	@$(RM) $(NAME)
	@echo "Deleted $(NAME) executable"
	
re: fclean all

.PHONY: clean fclean all re
