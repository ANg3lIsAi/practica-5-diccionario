/* 
autor: Rodriguez Flores Angel Isaias
fecha: 17/12/2024
V 1.0
ejecucion:  gcc ./../lib/presentacionWin.c ./../lib/TADListaDL.h ./arbol_binario.c ./hashes.c -c
            gcc TADListaDL.o presentacionWin.o arbol_binario.o main.o -o diccionario
 */

//inlcusion de librerias necesarias
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include "./../lib/TADListaDL.h"
#include "./../lib/presentacion.h"
#include "./arbol_binario.h"
#include "./hashes.h"

//Definicion de constantes y variables
//Tamaño de la tabla hash
#define TABLE_SIZE 73
#define ALTO 24
#define ANCHO 74
//eleccion de opciones dentro del menu
int seleccion;
int fila, columna;
char aux[256];
char ruta[256];
FILE *archivo = NULL;
lista tablaHash[TABLE_SIZE];

//impresion de menu de opciones
void menu(void){
    BorrarPantalla();
    fila = (ANCHO-16)/2;
    MoverCursor(fila,0);
    printf("Diccionario Hash\n");
    MoverCursor(0,2);
    printf("Seleccione una opcion:\n");
    printf("1. Cargar un archivo de definiciones.\n");
    printf("2. Agregar una palabra y su definicion,\n");
    printf("3. Buscar una palabra y ver su definicion.\n");
    printf("4. Modificar una definicion.\n");
    printf("5. Eliminar una palabra.\n");
    printf("6. Ver Estadisticas Hash.\n");
    printf("7. Exportar\n");
    printf("8. Salir.\n");
    scanf("%d",&seleccion);
    //limpiar buffer de entrada
    while (getchar() != '\n');
}

//pausar programa
void pausa (void){
    printf("Presione cualquier tecla para continuar...");
    getchar();
}

//impresion de menu de busqueda
void menu_busqueda(void){
    printf("Busqueda\n");
    printf("Que desea buscar?\n");
    printf("1. Por palabra.\n");
    printf("2. Por letra\n");
    printf("3. Por frase\n");
    printf("4. Subcadena\n");
}

//funcion para limpiar buffer de entrada y evitar caracteres basura
void limpiarBuffer(void){
    int c;
    while ((c=getchar())!= '\n' && c !=EOF){
    }
}

int main(void){
    //inicializacion de la tabla hash
    for (int i=0; i< TABLE_SIZE; i++){
        Initialize(&tablaHash[i]);
    }
    //variable para guardar resultado de hash
    int g;
    elemento e;
    
    do{
        //llamado a funcion de impresion de menu
        menu();
        switch (seleccion){
            //Cargar desde un archivo
            case 1:
                BorrarPantalla();
                fila=(ANCHO-24)/2;
                MoverCursor(fila,0);
                printf("Cargar desde un archivo\n");
                printf("\nIntroduzca la ruta relativa de el archivo a cargar (solo formato txt): ");
                
                // Escaneo de la ruta relativa para archivo a cargar
                char ruta[256];
                scanf("%s", ruta);

                // Apertura del archivo
                FILE *archivo = fopen(ruta, "r");
                if (archivo == NULL) {
                    BorrarPantalla();
                    MoverCursor(0, 0);
                    printf("ERROR: No se pudo abrir el archivo %s.\n", ruta);
                    pausa();
                    break;
                }

                // Variables para la lectura de líneas
                char linea[512];
                char palabra[101];
                char definicion[251];
                int colisionesTotales = 0;
                int palabrasIngresadas = 0;

                // Lectura de cada línea del archivo
                while (fgets(linea, sizeof(linea), archivo)) {
                    // Búsqueda del carácter ':'
                    char *delim = strchr(linea, ':');
                    if (delim != NULL) {
                        // Separación de la palabra y la definición
                        *delim = '\0';
                        strcpy(palabra, linea);
                        strcpy(definicion, delim + 1);

                        // Eliminación de salto de línea al final de la definición
                        definicion[strcspn(definicion, "\n")] = '\0';

                        // Creación del elemento
                        elemento e;
                        strcpy(e.p, palabra);
                        strcpy(e.d, definicion);
                        e.c = 0;

                        // Calculo de índice hash y comprobación de colisión
                        int g = hash1(e.p);
                        posicion s = buscar(&tablaHash[g], e.p);
                        if (s != NULL) {
                            colisionesTotales++;
                            int d = hash2(e.p);
                            s = buscar(&tablaHash[d], e.p);
                            if (s != NULL) {
                                colisionesTotales++;
                            } else {
                                btsort(&tablaHash[d], e);
                                palabrasIngresadas++;
                            }
                        } else {
                            btsort(&tablaHash[g], e);
                            palabrasIngresadas++;
                        }
                    }
                }

                // Cierre del archivo
                fclose(archivo);

                // Impresión de estadísticas
                BorrarPantalla();
                MoverCursor(0, 0);
                printf("Archivo cargado correctamente.\n");
                printf("Estadísticas:\n");
                printf("Número total de colisiones: %d\n", colisionesTotales);
                printf("Número total de palabras ingresadas: %d\n", palabrasIngresadas);
                pausa();
                pausa();
                break;

            case 2:
                BorrarPantalla();
                fila = (ANCHO-15)/2;
                MoverCursor(fila, 0);
                printf("Agregar Palabra\n");
                printf("\nIngrese la palabra que desea agregar: ");
                fgets(e.p, sizeof(e.p), stdin);
                e.p[strcspn(e.p, "\n")] = '\0';
                printf("\nIngrese la definición de la palabra %s:\n", e.p);
                fgets(e.d, sizeof(e.d), stdin);
                e.d[strcspn(e.d, "\n")] = '\0';
                e.c = 0;

                // Calculo de índice hash
                int g = hash1(e.p);

                // Comprobar colisión
                if (Size(&tablaHash[g]) != 0) {  // Después de primer hash, si la lista no está vacía
                    // Función para buscar coincidencias
                    posicion s = buscar(&tablaHash[g], e.p);
                    // Si ya existe la palabra, imprimir advertencia y regresar a menú.
                    if (s != NULL) {
                        BorrarPantalla();
                        MoverCursor(0, 0);
                        printf("ERROR, Intenta ingresar una palabra que ya existe.\n");
                        pausa();
                        break;
                    }
                    // Si no existe en primer hash, hacer segundo hash
                    int d = hash2(e.p);
                    // Comprobar si la lista del segundo hash está vacía
                    if (Size(&tablaHash[d]) != 0) {
                        // Si no, volver a buscar coincidencia, si no hay coincidencia agregar
                        s = buscar(&tablaHash[d], e.p);
                        // Si hay coincidencia, imprimir advertencia
                        if (s != NULL) {
                            BorrarPantalla();
                            MoverCursor(0, 0);
                            printf("ERROR, Intenta ingresar una palabra que ya existe.\n");
                            pausa();
                            break;
                        }
                    }
                    // Si no hay coincidencia o está vacía la lista, agregar a la lista del segundo hash
                    btsort(&tablaHash[d], e);
                    g = d;  // Actualizar índice para reflejar la nueva posición
                    e.c++;
                } else {
                    // En caso de que la lista del primer hash esté vacía, agregar a esa lista
                    Add(&tablaHash[g], e);
                }

                // Impresión de estadísticas
                BorrarPantalla();
                MoverCursor(0, 0);
                printf("Término guardado correctamente\n");
                printf("Estadísticas.\n");
                printf("Término guardado en lista N° %d detrás de %d palabras, después de %d pasos.\n", g, e.c, e.c);
                printf("Resultado de hash: %d\n", g);
                if (e.c != 0) {
                    printf("Número de colisiones: %d\n", e.c);
                } else {
                    printf("Sin colisiones.\n");
                }
                pausa();
                break;

            //Buscar palabra y su definicion
            case 3:
                BorrarPantalla();
                fila = (ANCHO-8)/2;
                MoverCursor(fila, 0);
                menu_busqueda();
                scanf("%d", &seleccion);  // Corregir: agregar el operador de dirección (&)

                switch (seleccion) {
                    // Búsqueda por palabra
                    case 1:
                        BorrarPantalla();
                        fila = (ANCHO-20)/2;
                        MoverCursor(fila, 0);
                        printf("Búsqueda por Palabra\n");
                        printf("\nIngrese la palabra a buscar: ");
                        char palabra[100];  // Definir un array de caracteres para guardar la palabra
                        scanf("%s", palabra);  // Almacenar la palabra ingresada en el array
                        limpiarBuffer();

                        // Crear un elemento temporal para la búsqueda
                        elemento e_temp;
                        strcpy(e_temp.p, palabra);
                        
                        int p = hash1(palabra);
                        posicion s = buscar(&tablaHash[p], palabra);
                        if (s == NULL) {
                            int d = hash2(palabra);
                            s = buscar(&tablaHash[d], palabra);
                            p = d;  // Actualiza p para reflejar la nueva posición
                        }
                        if (s != NULL) {
                            BorrarPantalla();
                            MoverCursor(0, 0);
                            printf("Palabra encontrada en lista %d.\n", p);
                            printf("Definición: %s\n", s->e.d);
                        } else {
                            BorrarPantalla();
                            MoverCursor(0, 0);
                            printf("Palabra no encontrada.\n");
                        }
                        pausa();
                        break;

                    // Búsqueda por letra
                    case 2:
                        BorrarPantalla();
                        fila = (ANCHO-20)/2;
                        MoverCursor(fila, 0);
                        printf("Búsqueda por Letra\n");
                        printf("\nIngrese la letra a buscar: ");
                        char letra;
                        scanf(" %c", &letra);
                        limpiarBuffer();
                        BorrarPantalla();
                        MoverCursor(0, 0);
                        printf("Palabras y definiciones que contienen la letra '%c':\n", letra);
                        for (int i = 0; i < TABLE_SIZE; i++) {
                            posicion actual = tablaHash[i].frente;
                            while (actual != NULL) {
                                if (strchr(actual->e.p, letra) != NULL || strchr(actual->e.d, letra) != NULL) {
                                    printf("- Palabra: %s\n  Definición: %s\n", actual->e.p, actual->e.d);
                                }
                                actual = actual->siguiente;
                            }
                        }
                        pausa();
                        break;

                    // Búsqueda por frase
                    case 3:
                        BorrarPantalla();
                        fila = (ANCHO-20)/2;
                        MoverCursor(fila, 0);
                        printf("Búsqueda por Frase\n");
                        printf("\nIngrese la frase a buscar: ");
                        char frase[251];
                        fgets(frase, sizeof(frase), stdin);
                        frase[strcspn(frase, "\n")] = '\0';
                        limpiarBuffer();
                        BorrarPantalla();
                        MoverCursor(0, 0);
                        printf("Definiciones que contienen la frase '%s':\n", frase);
                        for (int i = 0; i < TABLE_SIZE; i++) {
                            posicion actual = tablaHash[i].frente;
                            while (actual != NULL) {
                                if (strstr(actual->e.d, frase) != NULL) {
                                    printf("- %s\n", actual->e.p);
                                }
                                actual = actual->siguiente;
                            }
                        }
                        pausa();
                        break;

                    // Búsqueda por subcadena
                    case 4:
                        BorrarPantalla();
                        fila = (ANCHO-20)/2;
                        MoverCursor(fila, 0);
                        printf("Búsqueda por Subcadena\n");
                        printf("\nIngrese la subcadena a buscar: ");
                        char subcadena[101];
                        fgets(subcadena, sizeof(subcadena), stdin);
                        subcadena[strcspn(subcadena, "\n")] = '\0';
                        limpiarBuffer();
                        BorrarPantalla();
                        MoverCursor(0, 0);
                        printf("Definiciones que contienen la subcadena '%s':\n", subcadena);
                        for (int i = 0; i < TABLE_SIZE; i++) {
                            posicion actual = tablaHash[i].frente;
                            while (actual != NULL) {
                                if (strstr(actual->e.d, subcadena) != NULL) {
                                    printf("- %s\n", actual->e.p);
                                }
                                actual = actual->siguiente;
                            }
                        }
                        pausa();
                        break;

                    default:
                        printf("Opción no válida.\n");
                        pausa();
                        break;
                }
                break;

            //modificar una definicion
           case 4:
            BorrarPantalla();
            fila = (ANCHO-9)/2;
            MoverCursor(fila, 0);
            printf("Modificar\n");
            printf("\nIngrese la palabra de la definicion a modificar: ");
            fgets(e.p, sizeof(e.p), stdin);
            e.p[strcspn(e.p, "\n")] = '\0';

            // Calculo de índice hash
            int c = hash1(e.p);

            // Buscar la palabra en la lista del primer hash
            posicion s = buscar(&tablaHash[c], e.p);
            if (s == NULL) {
                // Si no está en la lista del primer hash, buscar en la lista del segundo hash
                int d = hash2(e.p);
                s = buscar(&tablaHash[d], e.p);
                c = d; // Actualizar c para reflejar la nueva posición si la palabra se encuentra en la lista del segundo hash
            }

            if (s != NULL) {
                // Si la palabra fue encontrada, pedir la nueva definición
                BorrarPantalla();
                MoverCursor(0, 0);
                printf("Palabra encontrada.\n Ingrese la nueva definicion para la palabra '%s': ", e.p);
                fgets(e.d, sizeof(e.d), stdin);
                e.d[strcspn(e.d, "\n")] = '\0';
                // Reemplazar la definición anterior con la nueva
                strcpy(s->e.d, e.d);

                BorrarPantalla();
                MoverCursor(0, 0);
                printf("Definición cambiada correctamente.\n");
                // Imprimir estadísticas (si es necesario)
                printf("\nPalabra modificada en la lista N° %d.\n", c);
            } else {
                // Imprimir un mensaje de error si la palabra no fue encontrada
                BorrarPantalla();
                MoverCursor(0, 0);
                printf("ERROR: La palabra '%s' no se encuentra en el diccionario.\n", e.p);
            }
            pausa();
            break;

            //eliminar una palabra(definicion incluida)
          case 5:
            BorrarPantalla();
            fila = (ANCHO-8)/2;
            MoverCursor(fila, 0);
            printf("Eliminar\n");
            printf("\nIngrese la palabra que desea eliminar: ");
            fgets(e.p, sizeof(e.p), stdin);
            e.p[strcspn(e.p, "\n")] = '\0';  // Eliminar el salto de línea al final
            
            // Calculo de índice hash
            int m = hash1(e.p);

            // Buscar la palabra en la lista del primer hash
            posicion w = buscar(&tablaHash[m], e.p);
            if (w == NULL) {
                // Si no está en la lista del primer hash, buscar en la lista del segundo hash
                int d = hash2(e.p);
                w = buscar(&tablaHash[d], e.p);
                if (w != NULL) {
                    // Eliminar el nodo si está en la lista del segundo hash usando función `Remove`
                    Remove(&tablaHash[d], w);
                    m = d;  // Actualizar índice para estadísticas
                } else {
                    // Imprimir un mensaje de error si la palabra no fue encontrada
                    BorrarPantalla();
                    MoverCursor(0, 0);
                    printf("ERROR: La palabra '%s' no se encuentra en el diccionario.\n", e.p);
                    pausa();
                    break;
                }
            } else {
                // Eliminar el nodo si está en la lista del primer hash usando función `Remove`
                Remove(&tablaHash[m], w);
            }
            
            // Imprimir mensaje de éxito y estadísticas
            BorrarPantalla();
            MoverCursor(0, 0);
            printf("\nPalabra Eliminada con éxito.\n");
            printf("Estadísticas:\n");
            printf("Palabra eliminada de la lista N° %d.\n", m);
            pausa();
            break;

            //ver estadisticas hash
            case 6:
                BorrarPantalla();
                fila = (ANCHO-12)/2;
                MoverCursor(fila,0);
                printf("Estadisticas\n\n");

                // Variables para estadísticas
                int totalPalabras = 0;
                int totalColisiones = 0;
                int palabrasPorLista[TABLE_SIZE] = {0}; // Array para contar palabras por lista

                // Recorrido de la tabla hash para calcular estadísticas
                for (int i = 0; i < TABLE_SIZE; i++) {
                    int palabrasEnLista = 0;
                    posicion actual = tablaHash[i].frente;
                    while (actual != NULL) {
                        palabrasEnLista++;
                        totalPalabras++;
                        if (palabrasEnLista > 1) {
                            totalColisiones++;
                        }
                        actual = actual->siguiente;
                    }
                    palabrasPorLista[i] = palabrasEnLista;
                }

                // Impresión de distribución de palabras
                printf("Distribución de palabras en las listas:\n");
                for (int i = 0; i < TABLE_SIZE; i++) {
                    printf("Lista %d: %d palabras\n", i, palabrasPorLista[i]);
                }

                // Impresión de colisiones totales
                printf("\nTotal de colisiones: %d\n", totalColisiones);

                // Impresión de eficiencia de la tabla hash
                int listasVacias = 0;
                for (int i = 0; i < TABLE_SIZE; i++) {
                    if (palabrasPorLista[i] == 0) {
                        listasVacias++;
                    }
                }
                double eficiencia = ((double)(TABLE_SIZE - listasVacias) / TABLE_SIZE) * 100.0;
                printf("\nEficiencia de la tabla hash: %.2f%%\n", eficiencia);

                pausa();
                break;

            //exportacion de lista
            case 7:
                BorrarPantalla();
                fila = (ANCHO-11)/2;
                MoverCursor(fila,0);
                printf("Exportacion");
                printf("\nCon que nombre desea exportar el diccionario?: ");
                scanf("%s", aux);
                printf("\nEn que ruta relativa desea que se guarde su archivo txt?: ");
                scanf("%s", ruta);

                // Crear la ruta completa del archivo
                char ruta_completa[512];
                snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s.txt", ruta, aux);

                // Abrir el archivo para escritura
                archivo = fopen(ruta_completa, "w");
                if (archivo == NULL) {
                    BorrarPantalla();
                    MoverCursor(0, 0);
                    printf("ERROR: No se pudo crear el archivo %s.\n", ruta_completa);
                    pausa();
                    break;
                }

                // Exportación del diccionario
                for (int i = 0; i < TABLE_SIZE; i++) {
                    posicion actual = tablaHash[i].frente;
                    while (actual != NULL) {
                        fprintf(archivo, "%s:%s\n", actual->e.p, actual->e.d);
                        actual = actual->siguiente;
                    }
                }

                // Cerrar el archivo
                fclose(archivo);

                // Confirmación de exportación exitosa
                BorrarPantalla();
                MoverCursor(0, 0);
                printf("Exportacion exitosa.\n");
                pausa();
                pausa();
                break;

            //salir
            case 8:
                BorrarPantalla();
                MoverCursor(0,0);
                printf("Saliendo...");
                Destroy(tablaHash);
                Sleep(500);
                BorrarPantalla();
                break;
            
            default:
                printf("Opcion no valida. por favor intente de nuevo.\n");
                break;
        }
    } while (seleccion !=8);
    return 0;
}