#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <ncurses.h>
#include <random>
#include <unistd.h>
#include <mutex>
#include <bits/stdc++.h>

using namespace std;

#ifdef MUTEX
pthread_mutex_t pantalla;
#endif

#define MAX_CAJAS 2
#define MAX_CLIENTES 10
#define MAX_PRODUCTOS 20
#define CAPACIDAD_ALMACENAMIENTO_MIN 5
#define CAPACIDAD_ALMACENAMIENTO_MAX 15

#define tiempo_minimo_procesamiento 1000
#define tiempo_maximo_procesamiento 3000

struct CajaRegistradora
{
    int id;
    int capacidadAlmacenamiento;
    sem_t mutex;
    sem_t espaciosVacios;
    sem_t productosDisponibles;
};

struct Cliente
{
    int id;
    int numProductos;
};

CajaRegistradora cajas[MAX_CAJAS];

// para guardar nuestros hilos de ejecucion
pthread_t cajeros[MAX_CAJAS];
pthread_t clientes[MAX_CLIENTES];

//número aleatorio entre min y max
int generarNumeroAleatorio(int min, int max)
{
    int aux = 0;
    if (max > min)
        aux = random() % (max - min);
    return min + aux;
}

void procesarProducto(CajaRegistradora &caja, Cliente &cliente)
{
    int tiempoProcesamiento = generarNumeroAleatorio(tiempo_minimo_procesamiento, tiempo_maximo_procesamiento);
    usleep(tiempoProcesamiento);

    sem_wait(&caja.espaciosVacios);
    sem_wait(&caja.mutex);

    caja.capacidadAlmacenamiento--;

    sem_post(&caja.mutex);
    sem_post(&caja.productosDisponibles);

    usleep(generarNumeroAleatorio(500, 1500));
}

//productor
void *procesoCliente(void *arg)
{
    Cliente *cliente = reinterpret_cast<Cliente *>(arg);

    CajaRegistradora *caja = nullptr;
    int minCapacidad = MAX_PRODUCTOS + 1;
    for (int i = 0; i < MAX_CAJAS; i++)
    {
        if (cajas[i].capacidadAlmacenamiento < minCapacidad)
        {
            minCapacidad = cajas[i].capacidadAlmacenamiento;
            caja = &cajas[i];
        }
    }

    if (caja == nullptr)
    {
        mvprintw(cliente->id, 0, "Cliente %d se fue sin comprar\n", cliente->id);
        refresh();
        pthread_exit(nullptr);
    }

    sem_wait(&caja->espaciosVacios);

    for (int i = 0; i < cliente->numProductos; i++)
    {
        procesarProducto(*caja, *cliente);
    }

    mvprintw(cliente->id, 0, "Cliente %d terminó de comprar\n", cliente->id);
    refresh();

    pthread_exit(nullptr);
}

//consumidor
void *procesoCaja(void *arg)
{
    CajaRegistradora *caja = reinterpret_cast<CajaRegistradora *>(arg);

    while (true)
    {
        sem_wait(&caja->productosDisponibles);
        sem_wait(&caja->mutex);

        int tiempoProcesamiento = generarNumeroAleatorio(1000, 3000);
        usleep(tiempoProcesamiento);

        caja->capacidadAlmacenamiento++;

        sem_post(&caja->mutex);
        sem_post(&caja->espaciosVacios);
    }

    pthread_exit(nullptr);
}

int main()
{
    initscr();
    curs_set(0);

    struct timespec tiempo_ini;
    struct timespec tiempo_fin;

    clock_gettime(CLOCK_MONOTONIC, &tiempo_ini);

    for (int i = 0; i < MAX_CAJAS; i++)
    {
        sem_init(&cajas[i].mutex, 0, 1);
        sem_init(&cajas[i].espaciosVacios, 0, CAPACIDAD_ALMACENAMIENTO_MAX);
        sem_init(&cajas[i].productosDisponibles, 0, 0);
        cajas[i].id = i;
        cajas[i].capacidadAlmacenamiento = CAPACIDAD_ALMACENAMIENTO_MAX;

        pthread_create(&cajeros[i], nullptr, procesoCaja, &cajas[i]);
    }

    for (int i = 0; i < MAX_CLIENTES; i++)
    {
        Cliente cliente;
        cliente.id = i;
        cliente.numProductos = generarNumeroAleatorio(1, MAX_PRODUCTOS);

        mvprintw(i, 0, "Cliente %d llegó con %d productos\n", cliente.id, cliente.numProductos);
        refresh();

        pthread_create(&clientes[i], nullptr, procesoCliente, &cliente);

        usleep(generarNumeroAleatorio(1000, 3000));
    }

    // Esperamos a que todos los clientes terminen de procesar sus productos
    for (int i = 0; i < MAX_CLIENTES; i++)
    {
        pthread_join(clientes[i], nullptr);
    }

    clock_gettime(CLOCK_MONOTONIC, &tiempo_fin);

    float periodo = (tiempo_fin.tv_sec - tiempo_ini.tv_sec) +
                    (tiempo_fin.tv_nsec - tiempo_ini.tv_nsec) * 1e-9;
    mvprintw(12, 0, "Tiempo de ejecución: %.9lfs\n", periodo);
    refresh();

    // TODO: mostramos el tiempo total

    // endwin();
    // for (int i = 0; i < MAX_CAJAS; i++) {
    //     pthread_cancel(cajeros[i]);
    //     sem_destroy(&cajas[i].mutex);
    //     sem_destroy(&cajas[i].espaciosVacios);
    //     sem_destroy(&cajas[i].productosDisponibles);
    // }

    return 0;
}