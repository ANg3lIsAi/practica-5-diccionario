#ifndef __BINARYTHREESORT_H__
#define __BINATYTHREESORT_H__

#include "./../lib/TADListaDL.h"

//funcion para insertar en orden dentro un la lista l
void btsort(lista *l, elemento e);
//funcio para buscar usando la palabra como criterio, recibe lista l, char p que es la palabra a buscar e int i que es la direccion dentro de la tabla hash donde se buscara, si no hay coincidencias regresara null, si hay coincidencia regresara la posicion de la palabra dentro de la lista
posicion buscar(lista *l, char *p);
//funcion para buscar usando la definicion como criterio
posicion buscarPorDefinicion(lista *l, char *subcadena);
//funcion para buscar usando una letra como criterio
posicion buscarPorLetra(lista *l, char letra);
// Función para eliminar acentos y convertir a minúsculas
void eliminar_acentos(char *dest, const char *src);
#endif