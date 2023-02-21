#include <stdio.h>
#include <stdlib.h>

struct Context {
    char *cp, *error;
    int result;
};

typedef struct Context Context_t;

Context_t *init_context(char*);
void free_context(Context_t*);

void expression(Context_t*);
void term(Context_t*);
void factor(Context_t*);
void end(Context_t*);
void whitespace(Context_t*);

int main()
{
    Context_t *context = init_context("(1+2*3-4+1)/2");         /* Create a parsing context. */
    expression(context);                                        /* Evalute in RDP from expression. */
    end(context);
    if (*context->error) {
        printf("error: %s\n", context->error);                  /* Output error. */
        free_context(context);                                  /* Destroy context. */
        return EXIT_FAILURE;
    }
    printf("success: %i\n", context->result);                   /* Output result. */
    free_context(context);                                      /* Destroy context. */
    return EXIT_SUCCESS;
}

Context_t *init_context(char *expr)
{
    Context_t *context = (Context_t*)malloc(sizeof(Context_t)); /* Create context on heap. */
    context->cp = expr;                                         /* Expression to solve. */
    context->error = "";                                        /* Set error to empty string (\0). */
    context->result = 0;                                        /* Set result to zero. */
    return context;
}

void free_context(Context_t *context)
{
    free(context);                                              /* Free memory. */
}

void expression(Context_t *context)
{
    term(context);                                              /* Get lvalue. */
    while (!*context->error) {                                  /* Loop while there are no errors. */
        whitespace(context);                                    /* Skip whitespace. */
        int lvalue = context->result;                           /* Store lvalue on stack. */
        switch (*context->cp) {
        case '+':                                               /* Check for add symbol. */
            ++context->cp;                                      /* Move to next character. */
            term(context);                                      /* Get rvalue. */
            context->result = lvalue + context->result;         /* Store lvalue plus rvalue. */
            continue;
        case '-':                                               /* Check for minus symbol. */
            ++context->cp;                                      /* Move to next character. */
            term(context);                                      /* Get rvalue. */
            context->result = lvalue - context->result;         /* Record lvalue minus rvalue. */
            continue;
        default:
            return;
        }
    }
}

void term(Context_t *context)
{
    factor(context);                                            /* Get lvalue. */
    while (!*context->error) {                                  /* Loop while there are no errors. */
        whitespace(context);                                    /* Skip whitespace. */
        int lvalue = context->result;                           /* Store lvalue on stack. */
        switch (*context->cp) {
        case '*':                                               /* Check for multiply symbol. */
            ++context->cp;                                      /* Move to next character. */
            factor(context);                                    /* Get rvalue. */
            context->result = lvalue * context->result;         /* Store lvalue multiplied by rvalue. */
            continue;
        case '/':                                               /* Check for divide symbol. */
            ++context->cp;                                      /* Move to next character. */
            factor(context);                                    /* Get rvalue. */
            context->result = lvalue / context->result;         /* Record lvalue divided by rvalue. */
            continue;
        default:
            return;
        }
    }
}

void factor(Context_t *context)
{
    whitespace(context);                                        /* Skip whitespace. */
    switch (*context->cp) {
    case '0':                                                   /* Check for zero. */
        ++context->cp;                                          /* Move to next character. */
        while (*context->cp == '0') {                           /* Check for zero. */
            ++context->cp;                                      /* Skip zeros. */
        }
    case '1':                                                   /* Fallthrough to check for digits 1 to 9. */
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        context->result = *context->cp - '0';                   /* Convert character to integer and assign to value. */
        ++context->cp;                                          /* Move to next character. */
        while (*context->cp >= '0' && *context->cp <= '9') {    /* Check if remaining characters are digits. */
            context->result *= 10;                              /* Multiply value by 10. */
            context->result += *context->cp - '0';              /* Convert character to integer and add to value. */
            ++context->cp;                                      /* Move to next character. */
        }
        return;
    case '(':                                                   /* Check for open paranthesis. */
        ++context->cp;                                          /* Move to next character. */
        expression(context);                                    /* Get expression. */
        whitespace(context);                                    /* Skip whitespace. */
        if (*context->cp != ')') {                              /* Check non-existence of close paranthesis. */
            context->error = "Closing paranthesis was expected.";
            return;
        }
        ++context->cp;                                          /* Move to next character. */
        return;
    default:
        context->error = "Number was expected.";
        return;
    }
}

void end(Context_t *context)
{
    whitespace(context);                                        /* Skip whitespace. */
    if (*context->cp) {                                         /* Check for end of character. */
        context->error = "End of expression expected.";
    }
}

void whitespace(Context_t *context)
{
    while (*context->cp == ' ') {                               /* Check for spaces. */
        ++context->cp;                                          /* Move to next character. */
    }
}