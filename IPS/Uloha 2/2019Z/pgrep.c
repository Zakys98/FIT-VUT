/*
 * Jiří Žák
 * xzakji02
 */
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <iostream>
#include <string.h>
#include <regex>

using namespace
std;

int minScore;               // minimalni score
int aktScore = 0;           // aktualni score
int *argScore;              // body za kazdy regex
string *argRegex;           // regex
bool onlyOnce = true;       // pro vytvoreni threadu jen jednou
string aktLine;             // aktualni radek
bool finished = false;

vector < std::thread * > threads; /* pole threadu promenne velikosti */
int line_wait = 0, main_wait = 0;
mutex *mtx_crit_line;
mutex *mtx_wait_line;
mutex *mtx_crit_main;
mutex *mtx_wait_main;
mutex *mtx_write_score;

void argument(int argcA, char **argvA) {
    if (argcA < 4) {
        fprintf(stderr, "Malo argumentu\n");
        exit(1);
    }
    if (argcA % 2 == 1) {
        fprintf(stderr, "Spatny pocet argumentu\n");
        exit(1);
    }
    for (int i = 1; i < argcA; ++i) {
        char *tmp;
        if (i == 1) {
            minScore = strtol(argvA[i], &tmp, 10);
            if (*tmp != '\0') {
                fprintf(stderr, "Prvni argument neni cislo\n");
                exit(1);
            }
        } else if (i % 2 == 1) {
            argScore[(i - 3) / 2] = strtol(argvA[i], &tmp, 10);
            if (*tmp != '\0') {
                fprintf(stderr, "Argument neni cislo\n");
                exit(1);
            }
        } else {
            argRegex[(i - 2) / 2] = string(argvA[i]);
        }
    }
}

void f(int ID) {
    while(finished == false) {
        (*mtx_crit_line).lock();
        line_wait++;
        (*mtx_crit_line).unlock();
        (*mtx_wait_line).lock();
        (*mtx_wait_line).unlock();
        if (regex_match(aktLine, regex(argRegex[ID]))) {
            (*mtx_write_score).lock();
            aktScore += argScore[ID];
            (*mtx_write_score).unlock();
        }
        (*mtx_crit_main).lock();
        main_wait++;
        (*mtx_crit_main).unlock();
        (*mtx_wait_main).lock();
        (*mtx_wait_main).unlock();
    }
}

/* vytvorime thready */
void createThreads(int numOfThread){
    if(onlyOnce == true) {
        threads.resize(numOfThread); /* nastavime si velikost pole threads */
        for (int i = 0; i < numOfThread; i++) {
            thread *new_thread = new
            thread(f, i);
            threads[i] = new_thread;
        }
    }
    onlyOnce = false;
}

int main(int argc, char **argv) {

    int numOfThread = (argc - 2) / 2;

    argScore = (int *) malloc(sizeof(int) * numOfThread);
    argRegex = new
    string[numOfThread];
    if (argScore == NULL || argRegex == NULL) {
        fprintf(stderr, "Chybna alokace\n");
        exit(1);
    }

    argument(argc, argv);

    /*******************************
     * Inicializace threadu a zamku
     * *****************************/

    /* vytvorime zamky */
    mtx_crit_line = new
    mutex();
    mtx_crit_main = new
    mutex();
    mtx_wait_line = new
    mutex();
    mtx_wait_main = new
    mutex();
    mtx_write_score = new
    mutex();
    if (mtx_crit_line == NULL ||
        mtx_crit_main == NULL ||
        mtx_wait_line == NULL ||
        mtx_wait_main == NULL ||
        mtx_write_score == NULL) {
        fprintf(stderr, "Nepodarilo se inicializovat zamky\n");
        exit(1);
    }

    (*mtx_wait_main).lock();
    for (string line; getline(cin, line);) {
        (*mtx_wait_line).lock();
        (*mtx_wait_main).unlock();
        createThreads(numOfThread);
        while (line_wait < numOfThread)
            continue;
        aktScore = 0;
        aktLine = line;
        (*mtx_wait_main).lock();
        (*mtx_wait_line).unlock();
        while (main_wait < numOfThread)
            continue;
        if(aktScore >= minScore){
            cout << line << "\n";
        }
        line_wait = 0;
        main_wait = 0;
    }

    finished = true;
    (*mtx_wait_main).unlock();

    /**********************************
     * Uvolneni pameti
     * ********************************/

    /* provedeme join a uvolnime pamet threads */
    for (int i = 0; i < numOfThread; i++) {
        (*(threads[i])).join();
        delete threads[i];
    }
    /* uvolnime pamet zamku */
    delete mtx_crit_line;
    delete mtx_crit_main;
    delete mtx_wait_line;
    delete mtx_wait_main;
    delete mtx_write_score;

    printf("everything finished\n");
    return 0;
}
