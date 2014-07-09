#ifndef _PARSER_H_
#define _PARSER_H_

#include "ast.h"

////////////////////////////////////////////////////////////////////////////////
// Parser
////////////////////////////////////////////////////////////////////////////////
extern AstNode g_root;

/* Do the parsing job. Parse the THING. */
void parse();

/*
 * Parse the next expression.
 */
AstNode *parse_expr();

/* Add a new node to a AST Node list.*/
void add_node(struct AstNodeList *list, AstNode *node);

#endif // _PARSER_H_
