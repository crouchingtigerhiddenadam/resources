#include <stdio.h>
#include <stdlib.h>

struct Context {
    char *cp, *error;
    int result;
};

typedef struct Context Context_t;

Context_t *init_context(char*);
void expression(Context_t*);
void term(Context_t*);
void factor(Context_t*);
void whitespace(Context_t*);
void error(Context_t*);

int main()
{
    Context_t *context = init_context("(1+2)*3-4");     // Create a parsing context.
    expression(context);                                // Evalute in RDP from expression.

    if (*context->error) {
        printf("error: %s\n", context->error);          // Output error.
        free(context);                                  // Free memory.

        return EXIT_FAILURE;
    }

    printf("success: result %i\n", context->result);    // Output result.
    free(context);                                      // Free memory.

    return EXIT_SUCCESS;
}

Context_t *init_context(char *expr)
{
    Context_t *context = (Context_t*)malloc(sizeof(Context_t));    // Create context on heap.
    context->cp = expr;                                            // Expression to solve.
    context->error = "";                                           // Set error to empty string (\0).
    context->result = 0;                                           // Set result to zero.
    return context;
}

void expression(Context_t *context)
{
    term(context);                                         // Get lvalue.
    while (!*context->error) {                             // Loop while there are no errors.
        whitespace(context);                               // Skip whitespace.
        int lvalue = context->result;                      // Store lvalue on stack.
        switch (*context->cp) {
        case '+':                                          // Check for add symbol.
            ++context->cp;                                 // Move to next character.
            term(context);                                 // Get rvalue.
            context->result = lvalue + context->result;    // Store lvalue plus rvalue.
            continue;
        case '-':                                          // Check for minus symbol.
            ++context->cp;                                 // Move to next character.
            term(context);                                 // Get rvalue.
            context->result = lvalue - context->result;    // Record lvalue minus rvalue.
            continue;
        default:
            return;
        }
    }
}

void term(Context_t *context)
{
    factor(context);                                       // Get lvalue.
    while (!*context->error) {                             // Loop while there are no errors.
        whitespace(context);                               // Skip whitespace.
        int lvalue = context->result;                      // Store lvalue on stack.
        switch (*context->cp) {
        case '*':                                          // Check for multiply symbol.
            ++context->cp;                                 // Move to next character.
            factor(context);                               // Get rvalue.
            context->result = lvalue * context->result;    // Store lvalue multiplied by rvalue.
            continue;
        case '/':                                          // Check for divide symbol.
            ++context->cp;                                 // Move to next character.
            factor(context);                               // Get rvalue.
            context->result = lvalue / context->result;    // Record lvalue divided by rvalue.
            continue;
        default:
            return;
        }
    }
}

void factor(Context_t *context)
{
    whitespace(context);                                        // Skip whitespace.
    if (*context->cp >= '0' && *context->cp <= '9') {           // Check if character is digit.
        context->result = *context->cp - '0';                   // Convert character to integer and assign to value.
        ++context->cp;                                          // Move to next character.
        while (*context->cp >= '0' && *context->cp <= '9') {    // Check if remaining characters are digits.
            context->result *= 10;                              // Multiply value by 10.
            context->result += *context->cp - '0';              // Convert character to integer and add to value.
            ++context->cp;                                      // Move to next character.
        }
    }
    else if (*context->cp == '(') {                             // Check for open paranthesis.
        ++context->cp;                                          // Move to next character.
        expression(context);                                    // Get expression.
        whitespace(context);                                    // Skip whitespace.
        if (*context->cp != ')') {                              // Check non-existence of close paranthesis
            context->error = "SyntaxError: closing paranthesis was expected.";
        }
        ++context->cp;                                          // Move to next character.
    }
    else {
        context->error = "SyntaxError: number was expected.";
    }
}

void whitespace(Context_t *context)
{
    while (*context->cp == ' ') {    // Check for spaces.
        ++context->cp;               // Move to next character.
    }
}