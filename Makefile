# ################################## #
#               COMMAND              #
# ################################## #
CC			= cc
MKDIR		= mkdir -p
RM			= rm -rf

# ################################## #
#              EXEC NAME             #
# ################################## #
NAME		= woody_woodpacker

# ################################## #
#               SOURCES              #
# ################################## #
C_DIR		= src
C_FILES		=	main.c\
				encryption.c\
				injection.c\
				segment.c\
				file_io.c\
				debug_utils.c\
				payload.c\
				relocations.c
				

SRCS		= $(patsubst %, $(C_DIR)/%, $(C_FILES))

# ################################## #
#               OBJECTS              #
# ################################## #
O_DIR		= objs
O_FILES		= $(C_FILES:.c=.o)
OBJS		= $(patsubst %, $(O_DIR)/%, $(O_FILES))

# ################################## #
#                FLAGS               #
# ################################## #
CFLAGS		= -Wall -Wextra -Werror -g -MMD

# ################################## #
#                INCLUDES            #
# ################################## #
CINCLUDES	=	-I ./inc

# ################################## #
#                RULES               #
# ################################## #

all:	$(NAME)

$(O_DIR):
		$(MKDIR) $(O_DIR)

$(O_DIR)/%.o: $(C_DIR)/%.c
		$(CC)  $(CFLAGS) $(CINCLUDES) -c $< -o $@

$(NAME): $(O_DIR) $(OBJS)
		$(CC)  $(OBJS) $(CFLAGS) -o $@

# ################################## #
#                CLEAN               #
# ################################## #

clean:
		$(RM) $(O_DIR)

fclean:
		$(RM) $(O_DIR)
		$(RM) $(NAME)

re:			fclean all
.PHONY: all clean fclean re

-include ./objs/*.d

.NOTPARALLEL:fclean