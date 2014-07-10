#include <stdio.h>

#include "tokenizer.h"
#include "func_table.h"
#include "interpretor.h"
#include "stack.h"
#include "vector.h"
#include "type_map.h"

int main()
{
    stack_init();
    type_map_init();
    init_func_table();
    load_from_file("program.txt");
    parse();
    interpret();
    return 1;
}
