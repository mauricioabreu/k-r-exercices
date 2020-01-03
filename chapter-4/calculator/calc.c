#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXOP 100 // max size of operand or operator
#define NUMBER '0' // signal that a number was found

int getop(char []);
void push(double);
double pop(void);
double get_last(void);
void print(void);
void duplicate(void);
int count(void);

const char PRINT = 'p';
const char DUPLICATE = 'd';

/* reverse polish calculator */
int main()
{
    int type;
    int is_command = 0;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                is_command = 0;
                push(atof(s));
                break;
            case '+':
                is_command = 0;
                push(pop() + pop());
                break;
            case '*':
                is_command = 0;
                push(pop() * pop());
                break;
            case '-':
                is_command = 0;
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                is_command = 0;
                op2 = pop();
                if (op2 != 0.0)
                    push(pop() / op2);
                else
                    printf("error: zero divisor\n");
                break;
            case '%':
                is_command = 0;
                op2 = pop();
                push(fmod(pop(), op2));
                break;
            case PRINT:
                is_command = 1;
                print();
                break;
            case DUPLICATE:
                is_command = 1;
                duplicate();
                break;
            case '\n':
                if (!is_command && count() > 0)
                    printf("\t%.8g\n", get_last());
                break;
            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }
    return 0;
}

#define MAXVAL 100  // maximum depth of val stack

int sp = 0; // next free stack position
double val[MAXVAL]; // value stack

/* push: push f onto value stack */
void push(double f)
{
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

#define BUFSIZE 100

char buf[BUFSIZE];  // buffer for ungetch
int bufp = 0;       // next free position in buf

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

#include <ctype.h>

int getch(void);
void ungetch(int);

/* getop: get next operator or numeric operand */
int getop(char s[])
{
    int i, c;

    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;
    s[1] = '\0';
    if (!isdigit(c) && c != '.' && c != '-')
        return c; // not a number

    i = 0;

    if (c == ' ') {
        if (isdigit(c=getch()) || c == '.')
            s[++i] = c;
        else 
        {
            if (c != EOF)
                ungetch(c);
            return '-';
        }
    }
    
    if (isdigit(c)) // collect integer part
        while (isdigit(s[++i] = c = getch()))
            ;
    if (c == '.') // collect fraction part
        while (isdigit(s[++i] = c = getch()))
            ;
    s[i] = '\0';
    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

/* print last element of stack */
void print(void)
{
    printf("print top element %g\n", get_last());
}

/* duplicate the last element of stack */
void duplicate(void)
{
    if (sp > 0) 
        push(get_last());
    else
        printf("error: stack empty\n");
}

/* return last element of stack */
double get_last(void)
{
    if (count() > 0)
        return val[sp - 1];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

/* count how many items in the stack */
int count(void)
{
    return sp;
}

/* clear the stack */
void clear(void)
{
    sp = 0;
}