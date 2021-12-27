MKDIR = mkdir -p
CC = gcc
CFLAGS	= -Wall -g
COFLAGS	= -Wall -c -g
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, main.o io.o time.o stringmatching.o bitvector.o)
LDFLAGS = -lm -lpthread
MAIN_TARGET = pattern-matching
vpath %.c  src
vpath %.h  src
.PHONY: directories
all: directories $(MAIN_TARGET)
$(MAIN_TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(OBJDIR)/%.o: %.c %.h
	gcc $(COFLAGS) $< -o $@
$(OBJDIR)/%.o: %.c
	gcc $(COFLAGS) $< -o $@
directories: $(OBJDIR)
$(OBJDIR):
	$(MKDIR) $@
clean:
	rm -rf $(OBJDIR) 
	rm -f $(MAIN_TARGET) $(GENERATOR_TARGET) $(PERFECT_TARGET) *~
