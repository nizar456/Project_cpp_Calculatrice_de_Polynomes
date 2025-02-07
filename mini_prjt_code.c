#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Structure pour représenter une fraction
typedef struct Fraction {
    int numerator;
    int denominator;
} Fraction;
// Fonction pour calculer le PGCD (Plus Grand Commun Dénominateur)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Fonction pour simplifier une fraction
void simplifyFraction(Fraction *frac) {
    int commonDivisor = gcd(frac->numerator, frac->denominator);
    frac->numerator /= commonDivisor;
    frac->denominator /= commonDivisor;
}

// Surcharge de l'opérateur +
Fraction addFractions(Fraction frac1, Fraction frac2) {
    Fraction result;
    result.numerator = frac1.numerator * frac2.denominator + frac2.numerator * frac1.denominator;
    result.denominator = frac1.denominator * frac2.denominator;
    simplifyFraction(&result);
    return result;
}

// Surcharge de l'opérateur -
Fraction subtractFractions(Fraction frac1, Fraction frac2) {
    Fraction result;
    result.numerator = frac1.numerator * frac2.denominator - frac2.numerator * frac1.denominator;
    result.denominator = frac1.denominator * frac2.denominator;
    simplifyFraction(&result);
    return result;
}

// Surcharge de l'opérateur *
Fraction multiplyFractions(Fraction frac1, Fraction frac2) {
    Fraction result;
    result.numerator = frac1.numerator * frac2.numerator;
    result.denominator = frac1.denominator * frac2.denominator;
    simplifyFraction(&result);
    return result;
}

// Surcharge de l'opérateur /
Fraction divideFractions(Fraction frac1, Fraction frac2) {
    Fraction result;
    result.numerator = frac1.numerator * frac2.denominator;
    result.denominator = frac1.denominator * frac2.numerator;
    simplifyFraction(&result);
    return result;
}
// Surcharge de l'opérateur ==
int areFractionsEqual(Fraction frac1, Fraction frac2) {
    // Simplifier les fractions avant de les comparer
    simplifyFraction(&frac1);
    simplifyFraction(&frac2);

    return (frac1.numerator == frac2.numerator) && (frac1.denominator == frac2.denominator);
}
// Fonction pour afficher une fraction
void displayFraction(Fraction frac) {
    simplifyFraction(&frac);
    if (frac.denominator == 1) {
        printf("%d", frac.numerator);
    } else {
        printf("%d/%d", frac.numerator, frac.denominator);
    }
}


// Structure pour représenter un terme du polynôme
typedef struct Term {
    Fraction coefficient;
    int exponent;
    struct Term *next;
} Term;

// Structure pour représenter un polynôme
typedef struct Polynomial {
    Term *head;
} Polynomial;

// Fonction pour créer un terme
/*Term* createTerm(int coefficient, int exponent) {
    Term *term = (Term*)malloc(sizeof(Term));
    term->coefficient = coefficient;
    term->exponent = exponent;
    term->next = NULL;
    return term;
}*/
// Fonction pour créer un terme avec un coefficient de type fraction
Term* createTerm(Fraction coefficient, int exponent) {
    Term *term = (Term*)malloc(sizeof(Term));
    term->coefficient = coefficient;
    term->exponent = exponent;
    term->next = NULL;
    return term;
}

// Fonction pour ajouter un terme à un polynôme
void addTerm(Polynomial *poly, Fraction coefficient, int exponent) {
    Term *term = createTerm(coefficient, exponent);

    if (poly->head == NULL || exponent > poly->head->exponent) {
        term->next = poly->head;
        poly->head = term;
    } else {
        Term *current = poly->head;
        while (current->next != NULL && current->next->exponent > exponent) {
            current = current->next;
        }
        term->next = current->next;
        current->next = term;
    }
}


// Fonction pour afficher un polynôme
void displayPolynomial(Polynomial *poly) {
    Term *current = poly->head;

    while (current != NULL) {
        if (current->exponent == 0) {
            displayFraction(current->coefficient);
        } else {
            printf("(");
            displayFraction(current->coefficient);
            printf(")x");

            if (current->exponent > 1) {
                printf("^%d", current->exponent);
            }
        }

        if (current->next != NULL) {
            printf(" + ");
        }

        current = current->next;
    }

    printf("\n");
}

// Fonction pour additionner deux polynômes
Polynomial addPolynomials(Polynomial *poly1, Polynomial *poly2) {
    Polynomial result;
    result.head = NULL;

    Term *term1 = poly1->head;
    Term *term2 = poly2->head;

    while (term1 != NULL || term2 != NULL) {
        Fraction frac1 = (term1 != NULL) ? term1->coefficient : (Fraction){0, 1};
        int exp1 = (term1 != NULL) ? term1->exponent : 0;
        Fraction frac2 = (term2 != NULL) ? term2->coefficient : (Fraction){0, 1};
        int exp2 = (term2 != NULL) ? term2->exponent : 0;

        if (exp1 > exp2) {
            addTerm(&result, frac1, exp1);
            term1 = term1->next;
        } else if (exp1 < exp2) {
            addTerm(&result, frac2, exp2);
            term2 = term2->next;
        } else {
            addTerm(&result, addFractions(frac1,frac2), exp1);
            term1 = term1->next;
            term2 = term2->next;
        }
    }

    return result;
}


// Fonction pour soustraire deux polynômes
Polynomial subtractPolynomials(Polynomial *poly1, Polynomial *poly2) {
    Polynomial result;
    result.head = NULL;

    Term *term1 = poly1->head;
    Term *term2 = poly2->head;

    while (term1 != NULL || term2 != NULL) {
        Fraction frac1 = (term1 != NULL) ? term1->coefficient : (Fraction){0, 1};
        int exp1 = (term1 != NULL) ? term1->exponent : 0;
        Fraction frac2 = (term2 != NULL) ? term2->coefficient : (Fraction){0, 1};
        int exp2 = (term2 != NULL) ? term2->exponent : 0;

        if (exp1 > exp2) {
            addTerm(&result, frac1, exp1);
            term1 = term1->next;
        } else if (exp1 < exp2) {
            Fraction negativeFrac2 = {0 - frac2.numerator, frac2.denominator};
            addTerm(&result, negativeFrac2, exp2);
            term2 = term2->next;
        } else {
            Fraction diff = subtractFractions(frac1, frac2);
            addTerm(&result, diff, exp1);
            term1 = term1->next;
            term2 = term2->next;
        }
    }

    return result;
}
// Fonction pour multiplier deux polynômes
Polynomial multiplyPolynomials(Polynomial *poly1, Polynomial *poly2) {
    Polynomial result;
    result.head = NULL;

    Term *term1 = poly1->head;

    while (term1 != NULL) {
        Term *term2 = poly2->head;

        while (term2 != NULL) {
            Fraction product = multiplyFractions(term1->coefficient, term2->coefficient);
            int expSum = term1->exponent + term2->exponent;

            // Recherche du terme dans le résultat avec la même puissance
            Term *existingTerm = result.head;
            while (existingTerm != NULL && existingTerm->exponent > expSum) {
                existingTerm = existingTerm->next;
            }

            if (existingTerm != NULL && existingTerm->exponent == expSum) {
                // Terme existant avec la même puissance, additionner les coefficients
                existingTerm->coefficient = addFractions(existingTerm->coefficient, product);
            } else {
                // Aucun terme existant avec la même puissance, ajouter un nouveau terme
                addTerm(&result, product, expSum);
            }

            term2 = term2->next;
        }

        term1 = term1->next;
    }

    return result;
}

// Fonction pour élever un polynôme à une puissance donnée
Polynomial powerPolynomial(Polynomial *poly, int power) {
    Polynomial result;
    result.head = NULL;

    if (power == 0) {
        // Cas spécial : puissance 0, le résultat est 1
        addTerm(&result, (Fraction){1, 1}, 0);
    } else {
        // Copie du polynôme d'origine
        Polynomial tempPoly = *poly;

        result = tempPoly;  // Utiliser le polynôme d'origine pour la première multiplication
        power--;

        while (power > 0) {
            result = multiplyPolynomials(&result, &tempPoly);
            power--;
        }
    }

    return result;
}
// Fonction pour affecter un polynôme à un autre
void affectPolynomial(Polynomial *dest, Polynomial *src) {
    // Libérer la mémoire occupée par le polynôme de destination
    clearPolynomial(dest);

    Term *srcTerm = src->head;

    // Copiez chaque terme du polynôme source dans le polynôme de destination
    while (srcTerm != NULL) {
        addTerm(dest, srcTerm->coefficient, srcTerm->exponent);
        srcTerm = srcTerm->next;
    }
}

// Fonction pour libérer la mémoire occupée par un polynôme
void clearPolynomial(Polynomial *poly) {
    Term *current = poly->head;
    Term *next;

    // Parcourez chaque terme et libérez la mémoire
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // Définir la tête du polynôme à NULL après la libération
    poly->head = NULL;
}

// Fonction pour dériver un polynôme par rapport à x
Polynomial derivativePolynomial(Polynomial *poly) {
    Polynomial result;
    result.head = NULL;

    Term *current = poly->head;

    while (current != NULL) {
        // Calculer le nouveau coefficient et exposant pour le terme dérivé
        Fraction newCoefficient = multiplyFractions(current->coefficient, (Fraction){current->exponent, 1});
        int newExponent = current->exponent - 1;

        // Ajouter le terme dérivé au polynôme résultant
        addTerm(&result, newCoefficient, newExponent);

        current = current->next;
    }

    return result;
}


// Fonction pour intégrer un polynôme par rapport à x
Polynomial integratePolynomial(Polynomial *poly) {
    Polynomial result;
    result.head = NULL;

    Term *current = poly->head;

    while (current != NULL) {
        // Calculer le nouveau coefficient et exposant pour le terme intégré
        Fraction newCoefficient = divideFractions(current->coefficient, (Fraction){current->exponent + 1, 1});
        int newExponent = current->exponent + 1;

        // Ajouter le terme intégré au polynôme résultant
        addTerm(&result, newCoefficient, newExponent);

        current = current->next;
    }

    return result;
}


// Fonction pour évaluer un polynôme pour une valeur donnée de x
Fraction evaluatePolynomial(Polynomial *poly, int x) {
    Fraction result = {0, 1};  // Initialiser le résultat à zéro

    Term *current = poly->head;

    while (current != NULL) {
        // Calculer le terme évalué en utilisant la valeur de x
        Fraction termValue = multiplyFractions(current->coefficient, (Fraction){pow(x, current->exponent), 1});

        // Ajouter le terme évalué au résultat
        result = addFractions(result, termValue);

        current = current->next;
    }

    return result;
}


// Fonction principale
int main() {
    // Exemples d'utilisation
    Polynomial T,P, Q, result;

    P.head = NULL;
    Q.head = NULL;

    // Création de polynômes avec des coefficients de type fraction
    Fraction frac1 = {1, 1};  // 1/1
    Fraction frac2 = {7, 2};  // 7/2
    Fraction frac3 = {11, 1}; // 11/1

    addTerm(&P, frac1, 3);
    addTerm(&P, frac2, 1);
    addTerm(&P, frac3, 0);

    Fraction frac4 = {5, 1};  // 1/1
    Fraction frac5 = {7, 1};  // 7/2
    Fraction frac6 = {-11, 1}; // 11/1

    addTerm(&Q, frac4, 2);
    addTerm(&Q, frac5, 1);
    addTerm(&Q, frac6, 0);


    // Affichage des polynômes
    printf("Polynomial P: ");
    displayPolynomial(&P);

    printf("Polynomial Q: ");
    displayPolynomial(&Q);

    // Addition de polynômes
    result = addPolynomials(&P, &Q);
    printf("P + Q: ");
    displayPolynomial(&result);

    // Soustraction de polynômes (à compléter)
    result = subtractPolynomials(&P, &Q);
    printf("P - Q: ");
    displayPolynomial(&result);

    // Multiplication de polynômes (à compléter)
    result = multiplyPolynomials(&P, &Q);
    printf("P * Q: ");
    displayPolynomial(&result);

    // Puissance d'un polynôme (à compléter)
    result = powerPolynomial(&P, 3);
    printf("P pow(3) : ");
    displayPolynomial(&result);

    // Affectation d'un polynôme à un autre (à compléter)
    affectPolynomial(&Q, &P);
    printf("Q : ");
    displayPolynomial(&Q);

    // Dérivation d'un polynôme (à compléter)
    Polynomial P_prime = derivativePolynomial(&P);

        // Afficher le polynôme dérivé
    printf("P' : ");
    displayPolynomial(&P_prime);

    // Intégration d'un polynôme (à compléter)
    Polynomial P_integral = integratePolynomial(&P);

        // Afficher le polynôme intégré
    printf("INT P : ");
    displayPolynomial(&P_integral);

    // Évaluation d'un polynôme pour x = 5 (à compléter)
        // Évaluer le polynôme P pour x = 5
    Fraction result_fraction = evaluatePolynomial(&P, 5);

        // Afficher le résultat de l'évaluation
    printf("P(5) : ");
    displayFraction(result_fraction);

    // Libération de la mémoire des polynômes
    clearPolynomial(&P);
    clearPolynomial(&Q);
    clearPolynomial(&result);
    clearPolynomial(&P_prime);
    clearPolynomial(&P_integral);
    printf("\n");
    printf("EXIT ");

    return 0;
}
