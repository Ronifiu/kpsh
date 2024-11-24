CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address -fanalyzer
TARGET = kpsh
SRCS = main.c shell_interactive.c prompt.c read_line.c parse.c handle_input.c execute_command.c redirect.c pipe.c stdout.c stdin.c stderr.c
HEADERS = shell.h

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)