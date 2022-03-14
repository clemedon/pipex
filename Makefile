# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cvidon <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/24 15:04:55 by cvidon            #+#    #+#              #
#    Updated: 2022/03/05 10:14:13 by cvidon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ========== [ Project files ]

# TODO set name
PREFIX		= pipex
NAME		= $(PREFIX)

# TODO set libriaires
LIB_DIR		=
LIB			=
LIB			:= $(LIB:%=$(LIB_DIR)/%)

# TODO set frameworks
FRAMEWORK	=

INC_DIR		= include
INC			= $(INC_DIR) \
			  $(LIB:%=%/$(INC_DIR)) $(LIB)

# TODO set sources
SRC_DIR		= src
SRCS		= main.c				\
			  parsing.c				\
			  pipe.c				\
			  security.c			\
			  bonus_here_doc.c		\
			  utils/ft_putstr_fd.c	\
			  utils/ft_strncmp.c	\
			  utils/ft_strlen.c		\
			  utils/ft_strjoin.c	\
			  utils/ft_strdup.c		\
			  utils/ft_itoa.c		\
			  utils/ft_split.c		\
			  utils/ft_substr.c		\
			  utils/get_next_line.c	\
			  utils/perror.c
SRCS		:= $(SRCS:%=$(SRC_DIR)/%)

OBJ_DIR 	= obj
OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# ========== [ Compiler flags ]

CC			= gcc

# TODO set extra compiler flags
CFLAGS		=
CFLAGS		:= $(CFLAGS) -Wall -Wextra -Werror -Wconversion -Wsign-conversion

# TODO set extra linker flags
CPPFLAGS	=
CPPFLAGS	:= $(CPPFLAGS) $(INC:%=-I%)
LDFLAGS		=
LDFLAGS 	:= $(LDFLAGS) $(LIB:%=-L%) $(FRAMEWORK)
LDLIBS		=
LDLIBS		:= $(LDLIBS) $(LIB:$(LIB_DIR)/lib%=-l%)

# ========== [ Misc ]

RM		= rm -rf
MAKE	= make -C

# ========== [ Recipe ]

.PHONY: all debug clean fclean norm update re leaks run

all: $(NAME)

debug: CPPFLAGS	:= $(CPPFLAGS) -D PINFO=1
debug: NAME := $(NAME)
debug: $(NAME)

sanitize: CFLAGS := $(CFLAGS) -fsanitize=address,undefined,signed-integer-overflow
sanitize: debug

$(NAME): $(OBJS)
	@for f in $(LIB); do $(MAKE) $$f --no-print-directory; done
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)
	@$(ECHO)"$(G)created $(END)$(NAME)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@[ ! -d $(@D) ] && mkdir -p  $(@D) || true
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@$(ECHO)"$(G)created $(END)$@"

clean:
	@for f in $(LIB); do $(MAKE) $$f clean --no-print-directory; done
	@[ -d $(OBJ_DIR) ] \
		&& $(RM) $(OBJ_DIR) && $(ECHO)"$(R)removed$(END) $(OBJ_DIR)/\n" || true

fclean: clean
	@for f in $(LIB); do $(MAKE) $$f fclean --no-print-directory; done
	@[ -f $(NAME) ] \
		&& $(RM) $(NAME) && $(ECHO)"$(R)removed$(END) $(NAME)/\n" || true

norm:
	@for f in $(LIB); do $(MAKE) $$f norm --no-print-directory; done
	@norminette -R CheckForbiddenSourceHeader $(SRCS) | grep -v "OK" || true
	@$(ECHO)"$(G)checked$(END) sources\n"
	@norminette -R CheckDefine $(INC_DIR) | grep -v "OK" || true
	@$(ECHO)"$(G)checked$(END) headers\n"

update:
	@git pull
	@git submodule update
	@$(ECHO)"$(G)updated$(END)\n"

re : fclean all

# ========== [ Tests ]

test: fclean sanitize
	@sh ./test/comp_test.sh

leak: fclean debug
	@sh ./test/leak_test.sh

t: fclean sanitize
	@echo "---[here_doc]"
	./pipex test/file1_perm "cat -e" "wc -l" test/file2_perm; ret1=$(echo $?); out1=$(cat -e test/file2);

l: fclean debug
	@echo "---[multi pipe]"
	@valgrind -q --leak-check=yes --show-leak-kinds=all ./pipex test/in "cat" "grep salut" "wc -l" test/out; echo -n "return: $?\noutput: "; cat -e test/out; echo "\n"


# ========== [ Stdout ]

R		= $(shell tput setaf 1)
G		= $(shell tput setaf 2)
Y		= $(shell tput setaf 3)
B		= $(shell tput setaf 4)
M		= $(shell tput setaf 5)
C		= $(shell tput setaf 6)
W		= $(shell tput setaf 7)
K		= $(shell tput setaf 8)
END		= $(shell tput sgr0)
ECHO	= echo -n "\r\033[K$(PREFIX): "
