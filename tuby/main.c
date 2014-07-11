#include <stdio.h>

#include "tokenizer.h"
#include "func_table.h"
#include "interpretor.h"
#include "stack.h"
#include "vector.h"
#include "type_map.h"
#include "var_map.h"

int main()
{
    stack_init();
    type_map_init();
    init_func_table();
    load_from_file("program.txt");
    parse();
    varmap_release();
    interpret();
    return 1;
}
