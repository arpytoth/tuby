# Make Tranlsator-App.

# Protect against environment variables
OBJ =
FLAGS =

# Initialize variables
tuby : FLAGS = -O3 -g3 -Wall -c -fmessage-length=0
d: FLAGS = -O0 -g -Wall -c -fmessage-length=0

# All Object Files
OBJ += ast.o
OBJ += func_table.o
OBJ += interpretor.o
OBJ += list.o
OBJ += vector.o
OBJ += parser.o
OBJ += tokenizer.o
OBJ += utils.o
OBJ += main.o
OBJ += stack.o
OBJ += var_map.o
OBJ += allocator.o

# Build binaries
%.o: ~/Dropbox/programming/tuby/src/%.c
	@echo Building: "$<"
	@gcc  $(INCLUDE) $(FLAGS) "$<"

tuby: $(OBJ)
	@echo Linking Tuby Release
	@gcc -o "tuby" $(OBJ)

d: $(OBJ)
	@echo Linking Tuby Debug
	@gcc -o "tuby" $(OBJ)

clean:
	@rm -rf $(OBJ)
	@rm -rf tuby 
	@echo Cleanup Finished.
 
# By default build tutorial project
all: tuby
