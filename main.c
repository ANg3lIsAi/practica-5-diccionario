/* 
ejecucion:  ./../lib/presentacionWin.c ./../lib/TADListaDL.h ./arbol_binario.c ./hashes.c -c
            gcc TADListaDL.o presentacionWin.o arbol_binario.o main.o hashes.o -o diccionario
 */

//inlcusion de librerias necesarias
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
char aux[251];
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
    printf("7. Salir.\n");
    scanf("%d",&seleccion);
    //limpiar buffer de entrada
    while (getchar() != '\n');
}

//pausar programa
void pausa (void){
    printf("Presione cualquier tecla para continuar...");
    getchar();
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
void limpiarbuffer(void){
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
    int c;
    elemento e;
    
    do{
        //llamado a funcion de impresion de menu
        menu();
        switch (seleccion){
            //Cargar desde un archivo
            case 1:
                break;
            //Agregar palabra y su definicion
            case 2:
                BorrarPantalla();
                fila = (ANCHO-15)/2;
                MoverCursor(fila,0);
                printf("Agregar Palabra\n");
                printf("\nIngrese la palabra que desea agregar: ");
                fgets(e.p, sizeof(e.p), stdin);
                e.p[strcspn(e.p, "\n")]='\0';
                printf("\nIngrese la definicion de la palabra %s:\n",e.p);
                fgets(e.d, sizeof(e.d), stdin);
                e.d[strcspn(e.d, "\n")] = '\0';
                e.c=0;
                //calculo de indice hash
                c = hash1(e.p);
                //comprobar colision
                if (Size(&tablaHash[c])!=0){//despues de primer hash, si la lista no esta vacia, revisar que la palabra no exista, si no existe, revisar segundo hash, si tampoco existe, agregar a segundo hash
                    //funcion para buscar coincidencias
                    posicion s = buscar(&tablaHash[c],e);
                    //si ya existe la palabra, imprimir advertencia y regresar a menu.
                    if (s != NULL){
                        BorrarPantalla();
                        MoverCursor(0,0);
                        printf("ERROR, Intenta ingresar una palabra que ya existe.\n");
                        pausa();
                        break;
                    }
                    //si no existe en primer hash, hacer segundo hash
                    int d = hash2(e.p);
                    //comprobar si la lista del segundo hash esta vacia
                    if(Size(&tablaHash[d])!= 0){
                        //si no volver a buscar coincidencia, si no hay coincidencia agregar
                        s = buscar(&tablaHash[d],e);
                        //si hay coincidencia, imprimir advertencia
                        if( s != NULL){
                            BorrarPantalla();
                            MoverCursor(0,0);
                            printf("ERROR, Intenta ingresar una palabra que ya existe.\n");
                            pausa();
                            break;
                        }
                    }
                    //si no hay coincidencia o esta vacia la lista, agregar a la lista del segundo hash
                    Add(&tablaHash[d],e);
                    c=d;
                    e.c++;
                }
                //en caso de que la lista del primer hash este vacia, agregar a esa lista
                else{
                    Add(&tablaHash[c],e);
                }
                //impresion de estadisticas
                BorrarPantalla();
                MoverCursor(0,0);
                printf("Termino guardado correctamente\n");
                printf("Estadisitcas.\n");
                printf("Termino guardado en lista N° %d detras de %d palabras, despues de %d pasos.\n", c, e.c, e.c);
                printf("Resultado de hash: %d\n",c);
                if(e.c != 0){
                    printf("Numero de colisiones: %d",e.c);
                }else{
                    printf("Sin colisiones.");
                }
                break;

            //Buscar palabra y su definicion
            case 3:
                
                BorrarPantalla();
                fila = (ANCHO-8)/2;
                MoverCursor(fila,0);
                menu_busqueda();
                scanf("%d",seleccion);
                switch (seleccion)
                {
                    //por palabra
                case 1:
                    BorrarPantalla();
                    fila = (ANCHO-20)/2;
                    MoverCursor(fila,0);
                    char palabra;
                    printf("Busqueda por Palabra\n");
                    printf("Ingrese la palabra a buscar: ");
                    scanf("%s",palabra);
                    
                    break;
                
                //por letra
                case 2:
                    break;

                //por frase
                case 3:
                    break;

                //por subcadena
                case 4:
                    break;
                
                default:
                    break;
                }
                break;

            //modificar una definicion
            case 4:
                BorrarPantalla();
                fila = (ANCHO-9)/2;
                MoverCursor(fila,0);
                printf("Modificar\n");
                printf("Ingrese la palabra de la definicion a modificar: ");
                fgets(e.p, sizeof(e.p), stdin);
                limpiarbuffer();
                /* funcion para busqueda de palabra y debe regresar la lista y ubicacion de la palabra o en caso de no estar imprimir el error*/
                printf("Palabra encontrada.\n Ingrese la nueva definicion para la palabra %s",e.p);
                fgets(e.d, sizeof(e.d), stdin);
                limpiarbuffer();
                //funcion para reemplazar nodo anterior por el nuevo
                BorrarPantalla();
                MoverCursor(0,0);
                printf("Definicion cambiada correctamente.\n");
                printf("Estadisticas:\n");
                //imprimir estadisticas solicitadas
                break;

            //eliminar una palabra(definicion incluida)
            case 5:
                BorrarPantalla();
                fila = (ANCHO-8)/2;
                MoverCursor(fila,0);
                printf("Eliminar\n");
                printf("Ingrese la palabra que desea eliminar: ");
                fgets(e.p, sizeof(e.p), stdin);
                //funcion para buscar ubicacion de la palabra
                //funcion para borrar nodo y reconectar en caso de ser necesario
                printf("\nPalabra Eliminada con exicto.\n");
                printf("Estadisticas:\n");
                //imprimir estadisticas
                break;

            //ver estadisticas hash
            case 6:
                BorrarPantalla();
                fila = (ANCHO-12)/2;
                MoverCursor(fila,0);
                printf("Estadisticas");
                printf("\n");
                break;
            //salir
            case 7:
                BorrarPantalla();
                MoverCursor(0,0);
                printf("Saliendo...");
                // Sleep(500);
                BorrarPantalla();
                break;
            
            default:
                printf("Opcion no valida. por favor intente de nuevo.\n");
                break;
        }
    } while (seleccion !=7);
    return 0;
}