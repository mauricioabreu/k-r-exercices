#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXOP 100 // max size of operand or operator
#define NUMBER '0' // signal that a number was found

int getop(char []);
void push(double);
double pop(void);
double get_last(void);
void print(void);
void duplicate(void);
int count(void);
int is_word(char []);

// commands
const char PRINT = 'p';
const char DUPLICATE = 'd';

// special operations
const int SIN = 256;
const int EXP = 257;
const int POW = 258;

char specials[3];

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
    if (!isdigit(c) && c != '.' && c != '-') {
        specials[0] = c;
        if (specials[0] == '\n') {
            return c;
        }

        specials[1] = getch();
        if (specials[1] == '\n') {
            ungetch(specials[1]);
            return specials[0];
        }

        specials[2] = getch();
        if (specials[2] == '\n') {
            ungetch(specials[1]);
            ungetch(specials[2]);
            return specials[0];
        }

        // a word may be a special expression like sin
        if (is_word(specials))
            if (specials[0] == 's' && specials[1] == 'i' && specials[2] == 'n')
                return SIN;
            if (specials[0] == 'e' && specials[1] == 'x' && specials[2] == 'p')
                return EXP;
            if (specials[0] == 'p' && specials[1] == 'o' && specials[2] == 'w')
                return POW;

        ungetch(specials[1]);
        ungetch(specials[2]);
        return c; // not a number
    }

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

/* print: print last element of stack */
void print(void)
{
    printf("print top element %g\n", get_last());
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

/* is_word: if all chars are alpha */
int is_word(char letters[])
{
    for (int i = 0; letters[i] != '\0'; i++) {
        if (!isalpha(letters[i]))
            return 0;
    }
    return 1;
}