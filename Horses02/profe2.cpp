//para compilar usar: gcc -o carrera carrera.c -lncurses -lpthread -DMUTEX

#include<stdlib.h>       //bibliotecas funciones estandar
#include<unistd.h>       //bibliotecas de sistema Unix
#include<ncurses.h>      //bibliotecas manejo de pantalla
#include<pthread.h>      //bibliotecas de threads

typedef struct{          //se define una estructura para pasar los
    int carril;          //parametros de entrada a la funcion que
    char caballo;        //va a ser ejecutada por un thread
} parametros_t;

//Prototipo de la funcion que va a ejecutar el thread hijo
void *funcionThread(parametros_t *parametro);

//variable pantalla tipo mutex para sincronizar el acceso a la pantalla
#ifdef MUTEX
pthread_mutex_t pantalla;
#endif

//Main: Ejecuta threads para la funcion funcionThread
int main(void){
  int x=1,y=15;                             //para posicionamiento de pantalla
  int error1,error2;                        //para verificar error al crear el thread
  pthread_attr_t atributo1,atributo2;       //atributos para la creacion del thread
  pthread_t idHilo1,idHilo2;                //identificador del thread hijo
  parametros_t in1,in2;                     //estructura con los parametros de entrada

  initscr();                                //inicializa la pantalla para ncurses
  curs_set(0);                              //elimina el cursor
  erase();                                  //limpia pantalla
  refresh();                                //refresca los cambion en la pantalla
  srand(time(NULL));                        //inicializa semilla de la funcion random

  #ifdef MUTEX
  pthread_mutex_init (&pantalla, NULL);     //inicializa variable pantalla tipo mutex
  #endif

  pthread_attr_init(&atributo1);            //Inicializa atributos con valores por defecto
  pthread_attr_init(&atributo2);            //Inicializa atributos con valores por defecto
  in1.carril=5;
  in1.caballo='h';
  in2.carril=7;
  in2.caballo='H';

  /*Creamos el thread.
  En idHilo nos devuelve un identificador para el nuevo thread,
  Pasamos atributos del nuevo thread con valores por defecto
  Pasamos la funcion que se ejecutara en el nuevo hilo
  Pasamos la estructura in[i] como parametro para esa i-esima funcion. */
  error1=pthread_create(&idHilo1, &atributo1, (void *)funcionThread, (void *)&in1);
  error2=pthread_create(&idHilo2, &atributo2, (void *)funcionThread, (void *)&in2);
  if (error1 != 0){                         //verificamos creacion del thread
    perror ("No puedo crear la hebra 1.");
    exit (-1);
  }
  if (error2 != 0){                         //verificamos creacion del thread
    perror ("No puedo crear la hebra 2.");
    exit (-1);
  }

  while (x<40){                
    #ifdef MUTEX
    pthread_mutex_lock(&pantalla);          //bloquea la pantalla (seccion critica)
    #endif

    mvprintw(y,x," p");                     //posiciona e imprime el competidor p
    refresh();                              //refresca los cambion en la pantalla

    #ifdef MUTEX
    pthread_mutex_unlock(&pantalla);        //desbloquea la pantalla (seccion critica)
    #endif

    usleep(1000000/4);                      //duerme por 1/4 de segundo
    if(drand48()<0.5) x++;                  //avanza una posicion con probabilidad 0.5
  }

 getch();                                   //espera que se ingrese cualquier tecla para salir
 endwin();                                  //cierra la pantalla de ncurses
}

/* Funcion que se ejecuta en el thread hijo.*/
void *funcionThread(parametros_t *parametro){
  int x=1,y=parametro->carril;              //para posicionamiento de pantalla
 
  while (x<40){
    #ifdef MUTEX
    pthread_mutex_lock(&pantalla);          //bloquea la pantalla (seccion critica)
    #endif
 
    mvprintw(y,x," %c",parametro->caballo); //posiciona e imprime el competidor h
    refresh();                              //refresca los cambion en la pantalla
 
    #ifdef MUTEX
    pthread_mutex_unlock(&pantalla);        //desbloquea la pantalla (seccion critica)
    #endif
 
    usleep(1000000/4);                      //duerme por 1/4 de segundo
    if(drand48()<0.5) x++;                  //avanza una posicion con probabilidad 0.5
  }
}