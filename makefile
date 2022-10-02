CC=gcc
CFLAGS= -std=c99 -Wall -Wextra 

SCANNER=src/scanner/scanner
PARSER=src/parser/parser

OBJFILES = $(SCANNER).o $(PARSER).o main.o
NAME=IFJ22


all: $(NAME)
	
$(NAME) : $(OBJFILES)	
	$(CC) $(CFLAGS) -o $(NAME) $(OBJFILES) 

clean:
	rm -f $(OBJFILES) $(NAME) *~