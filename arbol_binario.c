#include "./arbol_binario.h"
#include <stdlib.h>
#include <string.h>

//funcion para insertar un elemento e en orden alfabetico dentro de un lista l
void btsort(lista *l, elemento e){
    posicion nuevo = (posicion)malloc(sizeof(nodo));
    if (nuevo != NULL){
        nuevo->e =e;
        nuevo->siguiente = NULL;
        nuevo->anterior = NULL;

        if (l->tam ==0){
            l->frente = nuevo;
            l->final = nuevo;
        }else{
            posicion actual = l->frente;
            while ( actual != NULL){
                //comparacion de palabras letra por letra entre las palabras
                int cmp = strcmp(e.p, actual->e.p);
                if(cmp<0){
                    nuevo->siguiente = actual;
                    nuevo->anterior = actual->anterior;
                    if(actual->anterior != NULL){
                        actual->anterior->siguiente = nuevo;
                    }else{
                        l->final = nuevo;
                    }
                    actual->anterior = nuevo;
                    l->tam++;
                    return;
                }else if(cmp>0){
                    if(actual->siguiente == NULL){
                        actual->siguiente = nuevo;
                        nuevo->anterior = actual;
                        l->final = nuevo;
                        l->tam++;
                        return;
                    }
                    actual = actual->siguiente;
                }else{
                    printf("Error: la palabra '%s' ya existe en la lista.\n", e.p);
                    free(nuevo);
                    return;
                }
            }
            }
        }
}

//funcion para buscar coincidencia, recibe lista l, elemento e y regresa posicion dentro de la lista en caso de encontrarlo, si no regresa null
posicion buscar (lista *l, elemento e){
    //verificar que no este vacia la lista
    if (l== NULL || l->tam == 0){
        return NULL;
    }
    //iniciar busqueda desde el primer nodo
    posicion actual = l->frente;
    //busqueda entre las palabras buscando coincidencias
    while(actual != NULL){
        //si encuentra alguna coincidencia regresa la ubicacion del nodo
        if (strcmp(actual->e.p, e.p)==0){
            return actual;
        }
        //avanza al siguiente nodo para buscar
        actual = actual->siguiente;
    }
    //la funcion regresa NULL en caso de no encontrar coincidencias
    return NULL;
}