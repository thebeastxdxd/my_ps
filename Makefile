# CC is a standard variable in Makefiles for the compiler used
CC = gcc
# OBJ variable is used to store all of the intermidate OBJs files we want
OBJ = main.o linked_list.o
# add header files as dependencies so that when they are changed we recompile
DEPS = main.h linked_list.h

OUT_EX = my_ps

# PHONY allows me to create a make rule that the rule's target is not actually a file
.PHONY: clean 

# rule for compiling all c files to objs
# the $@ is an automatic variable which is replaced with the target name
# the $< is an automatic variable which is replaced with the first dependencies
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

# rule for our executable
# the $^ is an automatic variable which is replaced with the list of dependencies
run: $(OBJ)
	$(CC) -o $(OUT_EX) $^


clean:
	rm -f $(OBJ) $(OUT_EX)
