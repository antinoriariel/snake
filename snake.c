//Copyright (c) - Ariel Antinori 2021

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

//Aca definimos el alto x el ancho del tablero
//por el que se desplazara la serpiente
#define ALTO 25
#define ANCHO 55

//Definimos el valor de lo que sera una manzana en la matriz
#define MANZANA -1

//Definimos el valor que representa que no hay nada en la matriz
#define VACIO 0

//Aca definimos los distintos caracteres especiales que se utilizan para dibujar el recuadro
#define LINEA_VERTICAL 186
#define LINEA_HORIZONTAL 205
#define ESQ_SUP_IZQ 201
#define	ESQ_SUP_DER 187
#define ESQ_INF_IZQ 200
#define ESQ_INF_DER 188
#define CHAR_MANZANA 254
#define CHAR_CABEZA 219
#define CHAR_CUERPO 178
#define ENTER 13
#define ESC 27

//Prototipado
void crearUsuario();
void grabarRegistro();
void crearSerpiente();
void finDelJuego();
void dibujarTablero();
void ponerCursorEnCero();
void crearManzana();
void movimiento(int);
void eliminarCola();
int teclaPresionada();

int i,j;
int campo[ALTO][ANCHO]; //Aca definimos el tamaño del campo
int y, x, Gx;
int cabeza, cola; //aca definimos las variables a usar en el cuerpo
int estadoDelJuego, manzanas, var, dir;
int puntos, velocidad;

FILE *vf_archivo;

typedef char cadena[40];
typedef struct{
	int puntos;
	cadena tc_nombre;
}registro;

registro vr_jugador;

void crearUsuario(){
	char crear;
	
	vf_archivo = fopen("datos.dat", "rb");
	if(vf_archivo == NULL){
		printf("No se ha detectado un archivo de jugadores\n");
		printf("Creacion de usuario: ");
		fflush(stdin);
		scanf("%[^\n]s", vr_jugador.tc_nombre);
		system("cls");
	}else{
		printf("Se ha encontrado un archivo de usuarios\n");
		printf("Desea crear otro usuario? (s/n): ");
		fflush(stdin);
		scanf("%c", &crear);
		if(crear == 's'){
			printf("Creacion de usuario: ");
			fflush(stdin);
			scanf("%[^\n]s", vr_jugador.tc_nombre);
			system("cls");
		}else{
			system("cls");
			fread(&vr_jugador.tc_nombre, sizeof(cadena), 1, vf_archivo);
			fread(&vr_jugador.puntos, sizeof(int), 1, vf_archivo);
		}
		
	}
}

void grabarRegistro(){
	vf_archivo = fopen("datos.dat", "wb");
	if(vf_archivo != NULL){
		fwrite(&vr_jugador.tc_nombre, sizeof(cadena), 1, vf_archivo);
		fwrite(&vr_jugador.puntos, sizeof(int), 1, vf_archivo);
	}
	fclose(vf_archivo);
}

void crearSerpiente(){
	//Inicializamos la matriz a 0
	//******************************
    for(i = 0; i < ALTO; i++){
        for(j = 0; j < ANCHO; j++){
            campo[i][j] = VACIO;
        }
    }
    //******************************
    
    //Si modificamos estos valores al inicio la serpiente va a aparecer en donde indiquen las coordenadas
	//*********************************************************************************
    y = (ALTO / 2);//Ponemos la coordenada X en la mitad del campo de juego (de la matriz)
	x = (ANCHO / 2);//Ponemos la coordenada Y en la mitad del campo de juego (de la matriz)
	//*********************************************************************************
	Gx = x;
	//-----------
	cabeza = 5;
	cola = 1;
	//-----------
	estadoDelJuego = 0;	//En 0 indica que el bucle de la línea 365 puede funcionar, o sea "jugable". 0 indica que se eligió salir.
						//el valor de esta variable cambia segun lo que se elija en la línea 153
	manzanas = 0; //Indica el número de manzanas. Siempre que sea 0 indica a la funcion crearManzana() que tiene que crearse una manzana, si es 1 no
	//-----------
	dir = 'd'; //Indica la dirección de la serpiente. W-arriba|S-abajo|A-izquierda|D-derecha
	puntos = 0;
	//-----------
	velocidad = 99;
	//-----------
	
    for(i = 0; i < cabeza; i++){
        Gx++;
        campo[y][Gx - cabeza] = i + 1; //Llena los espacios de memoria desde [1 ... (cabeza - 1)] con números distintos de 0 x menores a "cabeza"
    }
}

void finDelJuego(){
    printf("\a"); //Hace un timbrado
    Sleep(1500);
    system("cls");

    if(puntos > vr_jugador.puntos){
        printf("Nueva puntuacion %d!!!!!!\n\n",puntos);
        vr_jugador.puntos = puntos;
        system("pause");
    }

    system("cls");
    printf("FIN DEL JUEGO !!!!!!\n");
    printf("Puntuacion: %d\n\n", puntos);
    printf("Presiona ENTER para volver a jugar o ESC para salir...\n");
	
    while(1){
        var = teclaPresionada();
        if(var == ENTER){
            estadoDelJuego = 0;
            crearSerpiente();
            break;
        }else if(var == ESC){
            estadoDelJuego = 1;
            	grabarRegistro();
            break;
        }
    }
    
    system("cls");
}

void dibujarTablero(){
	//Mediante este bloque de código mostramos la parte de arriba deelcuadrado donde se desplaza la viborita
	//************************************************************************************************************************
    for(i = 0; i <= ANCHO + 1; i++){
        if(i == 0){
            printf("%c", ESQ_SUP_IZQ);//Muestra el primer caracter de la esquina superior izquierda
        }else if(i == ANCHO + 1){
            printf("%c", ESQ_SUP_DER);//Muestra el ultimo caracter de la esquina superior derecha
        }else{
            printf("%c", LINEA_HORIZONTAL);//Muestra los caracteres horizontales entre ambas esquinas
        }
    }
    //************************************************************************************************************************
    
    
	//--------------------------------------------------------------------------------------
    printf("\tJugador: %s Puntuacion: %d  Maximo: %d", vr_jugador.tc_nombre, puntos, vr_jugador.puntos);
    printf("\n");
	//--------------------------------------------------------------------------------------
	
    for(i = 0; i < ALTO; i++){
        printf("%c", LINEA_VERTICAL); //Muestra las barras verticales derechas del tablero

        for(j = 0; j < ANCHO; j++){
            if(campo[i][j] == VACIO){//Si en el espacio de la matriz se encuentra un 0 indica que no hay nada
            	printf(" "); //En donde la matriz "campo" indica que no hay nada solamente imprime un espacio vacio
			}
            
			if(campo[i][j] > VACIO && campo[i][j] != cabeza){//Todo número (comprendido entre 1 x "cabeza") en la matriz que sea distinto de -1 (MANZANA) x distinto de "cabeza" es el cuerpo
				printf("%c", CHAR_CUERPO); //esto es el cuerpo ***** 176
			}
			
			if(campo[i][j] == cabeza){//El número máximo que indica a su vez la longitud total de elementos es "cabeza"
				printf("%c", CHAR_CABEZA); //esto es la cabeza ****** 178
			}
			
			if(campo[i][j] == MANZANA){//Si en el espacio de la matriz se encuentra un -1 indica una manzana
				printf("%c", CHAR_MANZANA); //esto es una manzana ******
			}
            
			if(j == ANCHO - 1){
				printf("%c\n", LINEA_VERTICAL); //Muestra las barras verticales izquierdas del tablero
			}
        }
    }
	
	//Mediante este bloque de código mostramos la parte de abajo del cuadrado donde se desplaza la viborita
	//************************************************************************************************************************
    for(i = 0; i <= ANCHO+1; i++){
        if(i == 0){
            printf("%c", ESQ_INF_IZQ);//Muestra el primer caracter de la esquina inferior izquierda
        }else if(i == ANCHO+1){
        	printf("%c", ESQ_INF_DER);//Muestra el ultimo caracter de la esquina inferior derecha
        }else{
            printf("%c", LINEA_HORIZONTAL);//Muestra los caracteres horizontales entre ambas esquinas
        }
    }
    //************************************************************************************************************************
}

void ponerCursorEnCero(){
    HANDLE controlSalida;
    COORD Posicion; //Declara una variable de tipo coordenada
    
    controlSalida = GetStdHandle(STD_OUTPUT_HANDLE);
    Posicion.X = 0; //Pone la coordenada (x) en 0
    Posicion.Y = 0; //Pone la coordenada (y) en 0
    SetConsoleCursorPosition(controlSalida, Posicion); //Mueve el cursor a las coordenadas establecidas en las líneas anteriores
}

void crearManzana(){
	int a, b;
	
    srand(time(0)); //Crea una seed para un número aleatorio
    a = 2 + rand() % ALTO - 2;
    b = 2 + rand() % ANCHO - 2;
	
    if(manzanas == 0 && campo[a][b] == 0){
        campo[a][b] = MANZANA; //Indica las coordenadas de la matriz en donde se encuentra la manzana llenando ese espacio con un -1
        manzanas = 1; //Indica que ya hay una manzana para evitar que se creen mas
        
		if(velocidad > 10 && puntos != 0){
			velocidad = velocidad - 5;//Acelera la velocidad incrementando la dificultad
		}
    }
}

int teclaPresionada(){ //Esta funcion espera continuamente la presion de una tecla x retorna su valor en caso de ser presionada
    if(_kbhit()){
    	return _getch();
	}else{
		return -1;
	}
}

void movimiento(int pTeclaPresionada){
    var = pTeclaPresionada;
	//var = getch_noblock(); //Solicita un caracter del entorno para almacenarlo en var
    //-------------------------------------------------------------------------------------------------
	var = tolower(var);
	/*Esto se usa en caso de que el "Bloq mayus" este activado para que se pasen a minúscula las letras
      de los controles, que en este caso son la "w", "s", "d", "a"
    --------------------------------------------------------------------------------------------------*/
    if(((var == 'd' || var == 'a') || (var == 'w' || var == 's')) && (abs(dir - var) > 5)){
    	dir = var;
	}
    
	if(dir == 'd'){
        x++;
        if(x == ANCHO){//Si x = ANCHO aparece del otro lado (IZQUIERDA)
        	x = 0;
		}
        if(campo[y][x] != 0 && campo[y][x] != MANZANA){//Si toca alguna parte del cuerpo (número distinto a VACIO y a MANZANA, hace game over)
        	finDelJuego();
		}
        if(campo[y][x] == MANZANA){//Si el campo donde nos ubicamos contiene el valor de MANZANA decrementa la cola para que la funcion eliminarCola() no la borre y aumenta los puntos
            manzanas = 0;
            puntos += 5;
            cola -= 1;
        }

        cabeza++;
        campo[y][x] = cabeza;//Posiciona "cabeza" en la próxima direccion
    }

    if(dir =='a'){
        x--;
        if(x == 0){//Si x = 0 aparece del otro lado (DERECHA)
        	x = ANCHO-1;
		}
			
        if(campo[y][x] != 0 && campo[y][x] != MANZANA){//Si toca alguna parte del cuerpo (número distinto a VACIO y a MANZANA, hace game over)
        	finDelJuego();
		}
        if(campo[y][x] == MANZANA){//Si el campo donde nos ubicamos contiene el valor de MANZANA decrementa la cola para que la funcion eliminarCola() no la borre y aumenta los puntos
            manzanas = 0;
            puntos += 5;
            cola -= 1;
        }
        
		cabeza++;
        campo[y][x] = cabeza;//Posiciona "cabeza" en la próxima direccion
    }

    if(dir =='w'){
        y--;
        if(y == -1){//Si y = -1 aparece del otro lado (ABAJO)
        	y = ALTO-1;
		}
        if(campo[y][x] != 0 && campo[y][x] != MANZANA){//Si toca alguna parte del cuerpo (número distinto a VACIO y a MANZANA, hace game over)
        	finDelJuego();
		}
        if(campo[y][x] == MANZANA){//Si el campo donde nos ubicamos contiene el valor de MANZANA decrementa la cola para que la funcion eliminarCola() no la borre y aumenta los puntos
            manzanas = 0;
            puntos += 5;
            cola -= 1;
        }
        
		cabeza++;
        campo[y][x] = cabeza;//Posiciona "cabeza" en la próxima direccion
    }

    if(dir == 's'){
    	y++;
        if(y == ALTO-1){//Si y = ALTO aparece del otro lado (ARRIBA)
        	y = 0;
		}
        if(campo[y][x] != 0 && campo[y][x] != MANZANA){//Si toca alguna parte del cuerpo (número distinto a VACIO y a MANZANA, hace game over)
        	finDelJuego();
		}
        if(campo[y][x] == MANZANA){//Si el campo donde nos ubicamos contiene el valor de MANZANA decrementa la cola para que la funcion eliminarCola() no la borre y aumenta los puntos
            manzanas = 0;
            puntos += 5;
            cola -= 1;
        }
        cabeza++;
        campo[y][x] = cabeza;//Posiciona "cabeza" en la próxima direccion
    }
}

void eliminarCola(){
    for(i = 0; i < ALTO; i++){
        for(j = 0; j < ANCHO; j++){
            if(campo[i][j] == cola){
                campo[i][j] = VACIO;
            }
        }
    }
    
    cola++;
}

int main(){
    system("color 0a");
    //lee registro
	crearSerpiente();
	crearUsuario();
    while(estadoDelJuego == 0){
        dibujarTablero();
        ponerCursorEnCero(); //Mediante esta funcion se devuelve el cursor del CMD a la posicion (0, 0)
        crearManzana();
        movimiento(teclaPresionada());
        eliminarCola(); //Esta funcion evita que la cola se alargue al final
        Sleep(velocidad);
    }
    
    return 0;
}
