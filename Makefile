tuby : FLAGS = -O3 -g3 -Wall -c -fmessage-length=0
d: FLAGS = -O0 -g -Wall -c -fmessage-length=0

OBJ += tuby/sourcefile.o
OBJ += tuby/redblack.o
OBJ += tuby/ast.o
OBJ += tuby/func_table.o
OBJ += tuby/interpretor.o
OBJ += tuby/list.o
OBJ += tuby/vector.o
OBJ += tuby/parser.o
OBJ += tuby/tokenizer.o
OBJ += tuby/utils.o
OBJ += tuby/main.o
OBJ += tuby/stack.o
OBJ += tuby/var_map.o
OBJ += tuby/allocator.o
OBJ += tuby/type_map.o
OBJ += tuby/string.o
OBJ += tuby/tuby_array.o
OBJ += tuby/tuby_string.o
OBJ += tuby/tuby_char.o
OBJ += tuby/int_type.o
OBJ += tuby/bool_type.o

%.o: %.c
	@echo Building: "$<"
	@gcc  $(INCLUDE) $(FLAGS) -o "$@" "$<"

tuby: $(OBJ)
	@gcc -o "tuby/tuby" $(OBJ)
	@echo Tuby Release Created.

d: $(OBJ)
	@gcc -o "tuby/tuby" $(OBJ)
	@echo Tuby Debug Created.

clean:
	@rm -rf $(OBJ)
	@rm -rf tuby/tuby 
	@echo Cleanup Finished.
 
all: tuby
