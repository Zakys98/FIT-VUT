// Projekt 2 - Jiøí Žák 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define konst 100

double cfrac_log(double x, unsigned int n) {
    double z = (1 - x) / (-x - 1);
    double cfr = 1.0;
    double a = 0.0;
    double pomocna = (2 * n) - 1;
    for (unsigned int i = n; i > 0; i--) {
        a = i * i * z * z;
        cfr = pomocna - (a / cfr);
        pomocna -= 2;
    }
    return (2 * z) / cfr;
}

double taylor_log(double x, unsigned int n) {
    //prvni cast
    if (x <= 2) {
        double y = 1 - x;
        double tay = -y;
        double pomocna = 2;
        double mocnina = y*y;
        for (unsigned int i = 1; i < n; i++) {
            tay = tay - (mocnina / pomocna);
            pomocna++;
            mocnina *= y;
        }
        return tay;
    } else {
        //druha cast
        double tay = 0.0;
        double mocnina = ((x - 1) / x);
        for (unsigned int i = 1; i <= n; i++) {
            tay += (mocnina / i);
            mocnina *= ((x - 1) / x);
        }
        return tay;
    }
}

double taylor_pow(double x, double y, unsigned int n) {
    double tayPow = 1.0;
    double fak = 1.0;
    double yfun = y;
    double fun = taylor_log(x, konst);
    double nas = fun;
    for (unsigned int i = 1; i <= n; i++) {
        fak *= i;
        tayPow += ((yfun * fun) / fak);
        fun *= nas;
        yfun *= y;
    }
    if (tayPow > 0) {
        return tayPow;
    } else {
        return tayPow = 1;
    }
}

double taylorcf_pow(double x, double y, unsigned int n) {
    double taycfPow = 1.0;
    double fak = 1.0;
    double yfun = y;
    double fun = cfrac_log(x, konst);
    double nas = fun;
    for (unsigned int i = 1; i <= n; i++) {
        fak *= i;
        taycfPow += ((yfun * fun) / fak);
        fun *= nas;
        yfun *= y;
    }
    if (taycfPow > 0) {
        return taycfPow;
    } else {
        return taycfPow = 1;
    }
}

int main(int argc, char *argv[]) {

    if (argc == 4 && strcmp(argv[1], "--log") == 0) {
        //log
        char *ptr;
        double logA = strtod(argv[2], &ptr);
        int logB = strtol(argv[3], &ptr, 10);
        logB = fabs(logB);
        if(logB == 0){
            printf("Musis zde zadat nejkae cislo krome nuly!\n");
            return 1;
        }
        printf("log(%s) = %.12g\n", argv[2], log(logA));
        //cfrac_log
        printf("cfrac_log(%s) = %.12g\n", argv[2], cfrac_log(logA, logB));
        //taylor_log
        printf("taylor_log(%s) = %.12g\n", argv[2], taylor_log(logA, logB));
    } else if (argc == 5 && strcmp(argv[1], "--pow") == 0) {
        //pow
        char *ptr;
        double powA = strtod(argv[2], &ptr);
        double powB = strtod(argv[3], &ptr);
        int powC = strtol(argv[4], &ptr, 10);
        powC = fabs(powC);
        if(powC == 0){
            printf("Musis zde zadat nejkae cislo krome nuly!\n");
            return 1;
        }
        printf("pow(%s) = %.12g\n", argv[2], pow(powA, powB));
        //taylor_pow
        printf("taylor_pow(%s, %s) = %.12g\n", argv[2], argv[3], taylor_pow(powA, powB, powC));
        //taylorcf_pow
        printf("taylorcf_pow(%s, %s) = %.12g\n", argv[2], argv[3], taylorcf_pow(powA, powB, powC));
    } else {
        printf("Zadali jste spatny pocet argumentu\n");
    }

    return 0;
}