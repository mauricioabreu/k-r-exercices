#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXOP 100 // max size of operand or operator
#define NUMBER '0' // signal that a number was found
#define VAR 'v' // signal a var was found
#define MAXVARS 26 // max number of variables (alphabet)

int getop(char []);
void push(double);
double pop(void);
double get_last(void);
void print(void);
void duplicate(void);
void clear(void);
int count(void);
void set(unsigned char, double);
double get(unsigned char);

// commands
const char PRINT = 'P';
const char DUPLICATE = 'D';
const char CLEAR = 'C';

// special operations
const int SIN = 256;
const int EXP = 257;
const int POW = 258;
const int SET = 259;
const int GET = 260;

char specials[3];

double vars[MAXVARS];

/* reverse polish calculator */
int main()
{
    int type;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                push(atof(s));
                break;
            case VAR:
                continue;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                op2 = pop();
                if (op2 != 0.0)
                    push(pop() / op2);
                else
                    printf("error: zero divisor\n");
                break;
            case '%':
                op2 = pop();
                push(fmod(pop(), op2));
                break;
            case SIN:
                push(sin(pop()));
                break;
            case EXP:
                push(exp(pop()));
                break;
            case POW:
                op2 = pop();
                push(pow(pop(), op2));
                break;
            case PRINT:
                print();
                break;
            case DUPLICATE:
                duplicate();
                break;
            case CLEAR:
                clear();
                printf("stack cleared\n");
                break;
            case SET:
                set(s[0], get_last());
                break;
            case GET:
                get(s[0]);
                break;
            case '\n':
                if (count() > 0)
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
    i = 0;

    while ((s[0] = c = getch()) == ' ' || c == '\t') // skip blanks and tabs
        ;
    s[1] = '\0';

    if (c == '\n' || c == EOF) // return return carriage or end of line right away
        return c;

    if (isalpha(c)) {
        while (isalpha(s[++i] = c = getch()))
            ;
        ungetch(c);
        s[i] = '\0';

        if (i > 1) {    // special function like SIN, POW, etc
            if (strcmp(s, "sin") == 0)
                return SIN;
            if (strcmp(s, "exp") == 0)
                return EXP;
            if (strcmp(s, "pow") == 0)
                return POW;
        }
        if (i == 1 && islower(s[0])) {
            return VAR;
        } else {    // handle vars and functions
            if (strcmp(s, "SET") == 0)
                return SET; 
            if (strcmp(s, "GET") == 0)
                return GET;
            getch();
            return s[0];
        }
    } else if (isdigit(c)) {
        while (isdigit(s[++i] = c = getch()))
            ;
        if (c == '.') // collect fraction part
            while (isdigit(s[++i] = c = getch()))
                ;

        ungetch(c);
        s[i] = '\0';
        return NUMBER;
    }

    return c;
}

/* print: print last element of stack */
void print(void)
{
    printf("print top element: %g\n", get_last());
}

/* duplicate: duplicate the last element of stack */
void duplicate(void)
{
    if (sp > 0) 
        push(get_last());
    else
        printf("error: stack empty\n");
}

/* get_last: return last element of stack */
double get_last(void)
{
    if (count() > 0)
        return val[sp - 1];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

/* count: count how many items in the stack */
int count(void)
{
    return sp;
}

/* clear: clear the stack */
void clear(void)
{
    sp = 0;
}

/* set: create a var with the given n value */
void set(unsigned char var, double n)
{
    vars[var] = n;
}

/* get: return the variable value */
double get(unsigned char var)
{
    return vars[var];
}