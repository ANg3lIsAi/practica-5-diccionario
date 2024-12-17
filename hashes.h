#ifndef HASHES_H
#define HASHES_H
#include "hashes.c"

//primer hasheo, recibe la cadena del termino y aplica modulo del numero primo 73, regresa un numero entero
int hash1(char *term);
//segundo hasheo, recibe nuevamente la cadena del termino y aplica modulo del numero primo 37, regresa un numero entero
int hash2(char *term);

#endif