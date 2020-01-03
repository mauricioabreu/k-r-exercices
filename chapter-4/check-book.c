#include <stdio.h>
#include <stdlib.h>

/* rudimentary calculator */
int main()
{
    char *buffer;
    size_t buffsize = 100;

    buffer = (char *)malloc(buffsize * sizeof(char));
    if (!buffer) {
        perror("Error allocating memory\n");
        exit(1);
    }

    double sum = 0;

    while (getline(&buffer, &buffsize, stdin) > 0) {
        printf("\tSum is %g\n", sum += atof(buffer));
    }

    return 0;
}