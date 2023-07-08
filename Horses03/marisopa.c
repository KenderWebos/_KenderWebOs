#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define NUM_CAJAS 2
#define MAX_CLIENTES 10
#define MAX_PRODUCTOS 20

#define CAPACIDAD_ALMACEN_MIN 5
#define CAPACIDAD_ALMACEN_MAX 15

#define TIEMPO_MIN_CAJA 5
#define TIEMPO_MAX_CAJA 15

#define TIEMPO_MIN_CLIENTE 5
#define TIEMPO_MAX_CLIENTE 15

typedef struct {
  int capacidad_almacen;
  int productos[MAX_PRODUCTOS];
  int num_productos;
  sem_t puede_producir;
  sem_t puede_consumir;
  pthread_mutex_t mutex_correa;
} caja_registradora_t;

typedef struct {
  int id;
  caja_registradora_t* caja;
} cliente_t;

caja_registradora_t cajas[NUM_CAJAS];

void* cliente_func(void* data);
void* caja_func(void* data);
int generar_aleatorio(int min, int max);
void dibujar_pantalla(int id, int num_productos);

int main() {
  initscr();  // Inicializar la pantalla ncurses
  srand(time(NULL));

  pthread_t clientes[MAX_CLIENTES];
  pthread_t cajas[NUM_CAJAS];
  int i, j;

  for (i = 0; i < NUM_CAJAS; i++) {
    caja_registradora_t* caja = &cajas[i];
    caja->capacidad_almacen = generar_aleatorio(CAPACIDAD_ALMACEN_MIN, CAPACIDAD_ALMACEN_MAX);
    caja->num_productos = 0;
    sem_init(&caja->puede_producir, 0, caja->capacidad_almacen);
    sem_init(&caja->puede_consumir, 0, 0);
    pthread_mutex_init(&caja->mutex_correa, NULL);

    for (j = 0; j < caja->capacidad_almacen; j++) {
      caja->productos[j] = 0;
    }

    pthread_create(&cajas[i], NULL, caja_func, caja);
  }

  for (i = 0; i < MAX_CLIENTES; i++) {
    cliente_t* cliente = (cliente_t*)malloc(sizeof(cliente_t));
    cliente->id = i + 1;
    cliente->caja = &cajas[i % NUM_CAJAS];
    pthread_create(&clientes[i], NULL, cliente_func, cliente);
  }

  for (i = 0; i < MAX_CLIENTES; i++) {
    pthread_join(clientes[i], NULL);
  }

  for (i = 0; i < NUM_CAJAS; i++) {
    pthread_cancel(cajas[i]);
    pthread_join(cajas[i], NULL);
  }

  endwin();  // Finalizar la pantalla ncurses

  return 0;
}

void* cliente_func(void* data) {
  cliente_t* cliente = (cliente_t*)data;
  caja_registradora_t* caja = cliente->caja;
  int i;
  for (i = 0; i < MAX_PRODUCTOS; i++) {
    usleep(1000 * generar_aleatorio(TIEMPO_MIN_CLIENTE, TIEMPO_MAX_CLIENTE));
    sem_wait(&caja->puede_producir);
    pthread_mutex_lock(&caja->mutex_correa);
    caja->productos[caja->num_productos++] = cliente->id;
    pthread_mutex_unlock(&caja->mutex_correa);
    sem_post(&caja->puede_consumir);
    dibujar_pantalla(cliente->id, caja->num_productos);
  }
  return NULL;
}


