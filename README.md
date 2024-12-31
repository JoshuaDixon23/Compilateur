# Projet : Compilateur

## Description
Ce projet est un **compilateur** écrit en C pour du langage ALGO vers du langage machine RAM.  
Il analyse le code source en 3 étapes principales : 
1. Analyse lexicale 
2. Analyse syntaxique 
3. Analyse sémantique  
Ensuite, il génère un code machine RAM que l'on peut executer sur le simulateur de machine RAM de Mr Zanotti ici: https://zanotti.univ-tln.fr/RAM/newram.php.


---

## Table des matières
1. [Description](#description)
2. [Fonctionnalités](#fonctionnalités)
3. [Langage ALGO](#langage-algo)
4. [Prérequis](#prérequis)
5. [Installation](#installation)
6. [Utilisation](#utilisation)
7. [Organisation du projet](#organisation-du-projet)
8. [Exemples](#exemples)
9. [Limitation](#limitation)
10. [Contributeurs](#contributeurs)

---

## Fonctionnalités
- **Analyse lexicale** : Détecte les tokens dans le code source.
- **Analyse syntaxique** : Définit la grammaire du code et génère l'arbre syntaxique abstrait (AST).
- **Analyse sémantique** : Détermine la taille du code final pour les sauts (jumps).
- **Génération de code machine** : Convertit le code source en instructions pour la machine RAM.

---
## Langage ALGO

Voici ce que vous pouvez faire avec ce langage :

### **Fonctionnalités principales**

1. **Gestion des variables** :
   - Déclaration de variables avec le mot-clé `VAR`.
   - Affectation de valeurs à des variables avec l'opérateur `:=`.
   - Support des types entiers (dans ce projet, d'autres types peuvent être ajoutés ultérieurement).

   Exemple :
   ```algo
   VAR x;
   x := 5;
   ```

2. **Expressions mathématiques** :
    - Support des opérateurs arithmétiques : +, -, *, /
    - Gestion de la priorité des opérateurs grâce aux parenthèses.

    Exemple:
    ```algo
    x := 5 + 3 * 2;  // Évalue à 11
    ```

3. **Structures de contrôle** :
    - Boucle tant que (TQ) :
    Permet d'exécuter un bloc d'instructions tant qu'une condition est vraie.

    Exemple : 
    ```algo
    TQ x > 0 FAIRE
    x := x - 1;
    FINTQ
    ```

    - Condition (SI) :
    Permet d'exécuter des blocs d'instructions conditionnels, avec ou sans `SINON`.

    Exemple 1:
    ```algo
    SI x > 0 ALORS
    y := 1;
    FINSI
    ```

    Exemple 2:
    ```algo
    SI x > 0 ALORS
    y := 1;
    SINON
    y := -1;
    FINSI
    ```

4. **Fonctions** :
    - Définition et appel de fonctions avec des paramètres.
    - Les fonctions sont déclarées avec le mot-clé `ALGO` suivi d'un `ID`.

    Exemple :
    ```algo
    ALGO somme(a, b)
    VAR res;
    DEBUT
        res := a + b;
    FIN

    x := somme(5, 3);  // Appelle la fonction avec les arguments 5 et 3.
    ```

5. **Conditions complexes** :
    - Comparaison avec les opérateurs >, <, =, et !.
    - Possibilité d'utiliser des expressions comme conditions.

    Exemple : 
    ```algo
    SI (x + y) > 10 ALORS
        z := 1;
    FINSI
    ```

6. **Expressions mathématiques** :
    - Les programmes finnissent par une fonction principale (MAIN) où le programme est exécuté.
    - Les fonctions et les variables peuvent être déclarées globalement ou localement.

    Exemple : 
    ```algo
    MAIN()
    VAR x, y;
    DEBUT
        x := 5;
        y := 10;
        z := somme(x, y);
    FIN
    ```


---
## Prérequis
- Langages/technologies requis : GCC, Flex, Bison.
- Outils spécifiques: Makefile

---

## Installation
1. Clonez le projet :
    ```bash
    git clone https://github.com/evilbutcool73/Compilo.git
    cd mon-compilateur
    ```
2. Installez les dépendances :
    ```bash
    sudo apt-get update
    sudo apt-get install flex
    sudo apt-get install bison
    sudo make install
    ```

---

## Utilisation
1. Pour compiler un fichier source, utilisez la commande suivante :
    ```bash
    ./bin/arc [emplacement du fichier en lanagage algo]
    ```
    Exemple :
    ```bash
    ./bin/arc ./test/exemple3.algo
    ```

---

## Organisation du projet
Voici une vue d'ensemble des fichiers et dossiers :
- **/bin** : Contient le fichier compiler C final.
- **/include** : Contient le code source .h du compilateur.
- **/obj** : Contient le code source .o du compilateur.
- **/src** : Contient le code source .c du compilateur.
- **/test** : Contient des fichiers de test pour valider les fonctionnalités.
- **Makefile** : Automatisation des tâches.
- **a.out** : le fichier de sortie du code compile 

---

## Exemples
Voici un exemple d'entrée et de sortie pour le compilateur :

### Entrée
exemple.algo : 
```algo
MAIN()
VAR x;
VAR y;
DEBUT
x <- 0;
y <- 10;
TQ x < y FAIRE
    x <- x + 2;
    y<-y+1;
FINTQ
FIN
```

### Sortie
a.out :
```RAM
LOAD #11
STORE 3
LOAD #0
STORE @3
INC 3
DEC 3
LOAD @3
STORE 10
LOAD #10
STORE @3
INC 3
DEC 3
LOAD @3
STORE 9
LOAD 10
STORE @3
INC 3
LOAD 9
STORE @3
INC 3
DEC 3
LOAD @3
DEC 3 
SUB @3
JUML 57
NOP
LOAD 10
STORE @3
INC 3
LOAD #2
STORE @3
INC 3
DEC 3
LOAD @3
DEC 3 
ADD @3 
STORE @3
INC 3
DEC 3
LOAD @3
STORE 10
LOAD 9
STORE @3
INC 3
LOAD #1
STORE @3
INC 3
DEC 3
LOAD @3
DEC 3 
ADD @3 
STORE @3
INC 3
DEC 3
LOAD @3
STORE 9
JUMP 14
NOP
```

## Limitation :
- Le compilateur ne prend en charge que les types entiers pour l'instant.
- Les chaînes de caractères et autres types de données complexes ne sont pas encore gérés.
- Le support pour la gestion des erreurs de syntaxe est limité aux cas les plus simples.

## Contributeurs

Ce projet a été développé par les créateurs suivants :

- **[Joshua DIXON]**
- **[Austin LAROQUE]**