/*libreria dedicada a los 2 niveles de hasheo, cada uno en referencia al mejor numero segunSheldon Cooper
personaje principal de la serie The Big Bang Theory, el numero primo 73*/

#include <stdio.h>
#include<string.h>

//Modulo 73 de la suma de los valores ASCII de c/letra del termino
int hash1(char *term){
    int sum=0;
     for (int i=0; i < strlen(term); i++){
        sum +=term[i];
     }
    return sum % 73;
}

//Modulo 37 (espejo del 73) suma de los valores ASCII de c/letra del termino
int hash2(char *term){
    int sum=0;
    for(int i=0; i < strlen(term);i++){
        sum += term[i];
    }
    return sum % 37;
}