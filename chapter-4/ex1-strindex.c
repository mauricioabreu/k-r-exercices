#include <stdio.h>
#define MAXLINE 1000    // maximum line length

int getlinec(char line[], int max);
int strindex(char source[], char searchfor[]);

char pattern[] = "ould";    // pattern to search for

/* find all lines matching pattern */
int main()
{
    char line[MAXLINE];
    int found = 0;

    while (getlinec(line, MAXLINE) > 0) {
        int idx = strindex(line, pattern);
        if (idx > 0)
            printf("Index of righmost occurrence of pattern is %d\n", idx);
    }
    return found;
}

/* getlinec: get line into s, return length */
int getlinec(char s[], int lim)
{
    int c, i;

    i = 0;
    while (--lim > 0 && (c=getchar()) != EOF && c != '\n')
        s[i++] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}

/* strindex: return index of t in s, -1 if none */
int strindex(char s[], char t[])
{
    int i, j, k;
    int idx = -1;

    for (i = 0; s[i] != '\0'; i++) {
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
            ;
        if (k > 0 && t[k] == '\0')
            idx = i;
    }
    return idx;
}