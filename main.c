//inlcusion de librerias necesarias
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "./../lib/TADListaDL.h"
#include "./../lib/presentacion.h"
#include "./../lib/presentacionWin.c"
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
                if (Size(&tablaHash[c])!=0){
                    c = hash2(e.p);
                    e.c++;
                    Add(&tablaHash[c],e);
                }else{
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
                break;
            //modificar una definicion
            case 4:
                break;
            //eliminar una palabra(definicion incluida)
            case 5:
                break;
            //ver estadisticas hash
            case 6:
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