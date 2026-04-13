#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ETATS 32
#define MAX_SYMBOLES 2 

// ---------------- NFA ----------------
typedef struct
{
    int nbEtats;
    int nbSymboles;
    int etatInitial;
    int etatsFinaux[MAX_ETATS];
    int nbFinaux;
    int trans[MAX_ETATS][MAX_SYMBOLES][MAX_ETATS]; // liste d'états
} NFA;

// ---------------- DFA ----------------
typedef struct
{
    int nbEtats;
    int nbSymboles;
    int etatInitial;
    int etatsFinaux[MAX_ETATS];
    int nbFinaux;
    int trans[MAX_ETATS][MAX_SYMBOLES];
} DFA;

// ---------------- UTILS ----------------
int contient(int *tab, int taille, int val)
{
    for (int i = 0; i < taille; i++)
    {
        if (tab[i] == val)
        {
            return 1;
        }
    }
    return 0;
}

// ---------------- DETERMINISATION ----------------
void determinisation(NFA *nfa, DFA *dfa)
{

    int file[MAX_ETATS][MAX_ETATS];
    int tailles[MAX_ETATS];

    int front = 0;
    int rear = 0;

    // état initial = {q0}
    file[rear][0] = nfa->etatInitial;
    tailles[rear] = 1;
    rear++;

    dfa->nbEtats = 0;
    dfa->nbSymboles = nfa->nbSymboles;
    dfa->etatInitial = 0;

    while (front < rear)
    {

        int *courant = file[front];
        int taille = tailles[front];
        front++;

        int index = dfa->nbEtats;
        dfa->nbEtats++;

        for (int s = 0; s < nfa->nbSymboles; s++)
        {

            int nouvelEtat[MAX_ETATS];
            int tailleNew = 0;

            for (int i = 0; i < taille; i++)
            {
                int e = courant[i];

                for (int j = 0; j < nfa->nbEtats; j++)
                {
                    if (nfa->trans[e][s][j])
                    {
                        if (!contient(nouvelEtat, tailleNew, j))
                        {
                            nouvelEtat[tailleNew] = j;
                            tailleNew++;
                        }
                    }
                }
            }

            // vérifier si l'état existe déjà
            int existe = -1;

            for (int i = 0; i < rear; i++)
            {
                if (tailles[i] == tailleNew)
                {

                    int ok = 1;

                    for (int j = 0; j < tailleNew; j++)
                    {
                        if (!contient(file[i], tailles[i], nouvelEtat[j]))
                        {
                            ok = 0;
                        }
                    }

                    if (ok)
                    {
                        existe = i;
                    }
                }
            }

            // ajouter si nouveau
            if (existe == -1 && tailleNew > 0)
            {
                memcpy(file[rear], nouvelEtat, sizeof(int) * tailleNew);
                tailles[rear] = tailleNew;
                existe = rear;
                rear++;
            }

            dfa->trans[index][s] = existe;
        }
    }

    // états finaux
    dfa->nbFinaux = 0;

    for (int i = 0; i < dfa->nbEtats; i++)
    {
        for (int j = 0; j < tailles[i]; j++)
        {

            if (contient(nfa->etatsFinaux, nfa->nbFinaux, file[i][j]))
            {
                dfa->etatsFinaux[dfa->nbFinaux] = i;
                dfa->nbFinaux++;
                break;
            }
        }
    }
}

// ---------------- ACCEPTATION ----------------
int accepter(DFA *dfa, char *mot)
{

    int etat = dfa->etatInitial;

    for (int i = 0; i < strlen(mot); i++)
    {

        int s = mot[i] - '0';
        etat = dfa->trans[etat][s];

        if (etat == -1)
        {
            return 0;
        }
    }

    for (int i = 0; i < dfa->nbFinaux; i++)
    {
        if (etat == dfa->etatsFinaux[i])
        {
            return 1;
        }
    }

    return 0;
}

// ---------------- MINIMISATION ----------------
void minimisation(DFA *dfa)
{

    int classe[MAX_ETATS];

    // init (finaux / non finaux)
    for (int i = 0; i < dfa->nbEtats; i++)
    {

        classe[i] = 0;

        for (int j = 0; j < dfa->nbFinaux; j++)
        {
            if (i == dfa->etatsFinaux[j])
            {
                classe[i] = 1;
            }
        }
    }

    int change = 1;

    while (change)
    {

        change = 0;

        for (int i = 0; i < dfa->nbEtats; i++)
        {
            for (int j = i + 1; j < dfa->nbEtats; j++)
            {

                if (classe[i] == classe[j])
                {

                    for (int s = 0; s < dfa->nbSymboles; s++)
                    {

                        if (classe[dfa->trans[i][s]] != classe[dfa->trans[j][s]])
                        {
                            classe[j]++;
                            change = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    printf("\nClasses d'équivalence :\n");

    for (int i = 0; i < dfa->nbEtats; i++)
    {
        printf("Etat %d -> Classe %d\n", i, classe[i]);
    }
}

// ---------------- MAIN ----------------
int main()
{

    NFA nfa = {0};

    nfa.nbEtats = 3;
    nfa.nbSymboles = 2;
    nfa.etatInitial = 0;

    nfa.nbFinaux = 1;
    nfa.etatsFinaux[0] = 2;

    // transitions NFA
    nfa.trans[0][0][0] = 1;
    nfa.trans[0][1][0] = 1;
    nfa.trans[1][1][2] = 1;

    DFA dfa;

    determinisation(&nfa, &dfa);

    printf("Test mot 01 : %d\n", accepter(&dfa, "01"));
    printf("Test mot 11 : %d\n", accepter(&dfa, "11"));

    minimisation(&dfa);

    return 0;
}