#include <ctype.h>
#include <math.h>
#include <stdio.h>

/* atof: convert string s to double */
double atof(char s[])
{
    double val, power, number;
    int i, sign, nsign, npower;

    for (i = 0; isspace(s[i]); i++) // skip white spaces
        ;

    sign = (s[i] == '-') ? -1 : 1;  
    if (s[i] == '+' || s[i] == '-')
        i++;
    for (val = 0.0; isdigit(s[i]); i++)
        val = 10.0 * val + (s[i] - '0');
    if (s[i] == '.')
        i++;
    for (power = 1.0; isdigit(s[i]); i++) {
        val = 10.0 * val + (s[i] - '0');
        power *= 10.0;
    }

    number =  sign * val / power;

    // check if is scientific notation
    if (s[i] == 'e' || s[i] == 'E') {
        i++;
        nsign = (s[i] == '-') ? -1 : 1;  
        if (s[i] == '+' || s[i] == '-')
            i++;
        npower = s[i] - '0';
        number = number * pow(10, nsign * npower);
    }

    return number;
}

int main()
{
    printf("Converting '2.0' to float %g\n", atof("2.0"));
    printf("Converting '-2.0' to float %g\n", atof("-2.0"));
    printf("Converting '2.5' to float %g\n", atof("2.5"));
    printf("Converting '2.9999' to float %g\n", atof("2.9999"));
    printf("Converting '123.45e-6' to float %g\n", atof("123.45e-6"));
    printf("Converting '1.247e2' to float: %g\n", atof("1.247e2"));
}