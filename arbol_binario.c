#include "./arbol_binario.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./../lib/presentacion.h"
#include <ctype.h>

// Función para eliminar acentos y convertir a minúsculas
void eliminar_acentos(char *dest, const char *src) {
    while (*src) {
        switch (*src) {
            case 'á': case 'à': case 'ä': case 'â': case 'ã': case 'Á': case 'À': case 'Ä': case 'Â': case 'Ã':
                *dest = 'a';
                break;
            case 'é': case 'è': case 'ë': case 'ê': case 'É': case 'È': case 'Ë': case 'Ê':
                *dest = 'e';
                break;
            case 'í': case 'ì': case 'ï': case 'î': case 'Í': case 'Ì': case 'Ï': case 'Î':
                *dest = 'i';
                break;
            case 'ó': case 'ò': case 'ö': case 'ô': case 'õ': case 'Ó': case 'Ò': case 'Ö': case 'Ô': case 'Õ':
                *dest = 'o';
                break;
            case 'ú': case 'ù': case 'ü': case 'û': case 'Ú': case 'Ù': case 'Ü': case 'Û':
                *dest = 'u';
                break;
            case 'ñ': case 'Ñ':
                *dest = 'n';
                break;
            default:
                *dest = *src;
        }
        src++;
        dest++;
    }
    *dest = '\0';
}

// Función para insertar un elemento e en orden alfabético dentro de una lista l
void btsort(lista *l, elemento e) {
    posicion actual = First(l);

    // Si la lista está vacía o tiene un solo elemento, se agrega directamente
    if (Empty(l) || Size(l) == 1) {
        Add(l, e);
        return;
    }

    // Normalizar la palabra del nuevo elemento
    char palabraNormalizada[101];
    eliminar_acentos(palabraNormalizada, e.p);

    boolean listo = FALSE;
    while (actual != NULL && listo == FALSE) {
        // Normalizar la palabra del elemento actual
        char palabraActualNormalizada[101];
        eliminar_acentos(palabraActualNormalizada, actual->e.p);

        if (strcmp(palabraNormalizada, palabraActualNormalizada) < 0) { // e.p va antes que actual
            Insert(l, actual, e, TRUE);
            listo = TRUE;
        } else if (strcmp(palabraNormalizada, palabraActualNormalizada) > 0) { // e.p va después de actual
            if (Following(l, actual) == NULL) { // Si es el último nodo
                Insert(l, NULL, e, FALSE); // Insertar al final
                listo = TRUE;
            } else {
                actual = Following(l, actual);
            }
        } else { // Las palabras son iguales
            BorrarPantalla();
            MoverCursor(0, 0);
            printf("ERROR: término ya enlistado.\n");
            return;
        }
    }
}

// Función para buscar usando la palabra como criterio
// Recibe lista l, char *p que es la palabra a buscar
// Si no hay coincidencias, regresará NULL; si hay coincidencia, regresará la posición de la palabra dentro de la lista
posicion buscar(lista *l, char *p) {
    char normalizada[101];
    eliminar_acentos(normalizada, p);

    posicion aux = l->frente;
    while (aux != NULL) {
        char normalizadaNodo[101];
        eliminar_acentos(normalizadaNodo, aux->e.p);

        if (strcmp(normalizada, normalizadaNodo) == 0) {
            return aux; // Palabra encontrada
        }
        aux = aux->siguiente;
    }
    return NULL; // No se encontró la palabra
}

posicion buscarPorLetra(lista *l, char letra) {
    char normalizadaLetra[2];
    eliminar_acentos(normalizadaLetra, &letra);
    normalizadaLetra[1] = '\0'; // Asegurar que sea una cadena terminada en null

    posicion aux = l->frente;
    while (aux != NULL) {
        char normalizadaPalabra[101];
        eliminar_acentos(normalizadaPalabra, aux->e.p);

        char normalizadaDefinicion[251];
        eliminar_acentos(normalizadaDefinicion, aux->e.d);

        if (strchr(normalizadaPalabra, normalizadaLetra[0]) != NULL || strchr(normalizadaDefinicion, normalizadaLetra[0]) != NULL) {
            return aux; // Nodo encontrado con la letra en la palabra o definición
        }
        aux = aux->siguiente;
    }
    return NULL; // No se encontró ningún nodo con la letra
}

posicion buscarPorDefinicion(lista *l, char *subcadena) {
    char normalizadaSubcadena[251];
    eliminar_acentos(normalizadaSubcadena, subcadena);

    posicion aux = l->frente;
    while (aux != NULL) {
        char normalizadaDefinicion[251];
        eliminar_acentos(normalizadaDefinicion, aux->e.d);

        if (strstr(normalizadaDefinicion, normalizadaSubcadena) != NULL) {
            return aux; // Nodo encontrado con la subcadena en la definición
        }
        aux = aux->siguiente;
    }
    return NULL; // No se encontró ningún nodo con la subcadena en la definición
}
