#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <ncurses.h>
#include <random>
#include <unistd.h>
#include <chrono>

#define MAX_CAJAS 2
#define MAX_CLIENTES 10
#define MAX_PRODUCTOS 20
#define CAPACIDAD_ALMACENAMIENTO_MIN 5
#define CAPACIDAD_ALMACENAMIENTO_MAX 15

// Estructura para representar a un cliente
struct Cliente {
    int id;
    int numProductos;
};

// Estructura para representar a una caja registradora
struct CajaRegistradora {
    int id;
    int capacidadAlmacenamiento;
    sem_t mutex;
    sem_t espaciosVacios;
    sem_t productosDisponibles;
};

CajaRegistradora cajas[MAX_CAJAS];
pthread_t cajeros[MAX_CAJAS];
pthread_t clientes[MAX_CLIENTES];

// Genera un número aleatorio entre min y max (ambos inclusive)
int generarNumeroAleatorio(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// Función que simula el procesamiento de un producto
void procesarProducto(CajaRegistradora& caja, Cliente& cliente) {
    // Simulamos el tiempo de procesamiento del producto
    int tiempoProcesamiento = generarNumeroAleatorio(1000, 3000);
    usleep(tiempoProcesamiento);

    // Tomamos un espacio de almacenamiento en la caja
    sem_wait(&caja.espaciosVacios);
    sem_wait(&caja.mutex);

    // Simulamos el tiempo que tarda en guardar el producto en la caja
    usleep(500);

    // Actualizamos la capacidad de almacenamiento de la caja
    caja.capacidadAlmacenamiento--;

    // Liberamos el mutex y notificamos que hay un producto disponible
    sem_post(&caja.mutex);
    sem_post(&caja.productosDisponibles);

    // Simulamos el tiempo de espera del cliente antes de procesar el siguiente producto
    usleep(generarNumeroAleatorio(500, 1500));
}

// Función que simula el proceso de un cliente en una caja
void* procesoCliente(void* arg) {
    Cliente* cliente = reinterpret_cast<Cliente*>(arg);

    // Buscamos la caja con menor capacidad de almacenamiento disponible
    CajaRegistradora* caja = nullptr;
    int minCapacidad = MAX_PRODUCTOS + 1;
    for (int i = 0; i < MAX_CAJAS; i++) {
        if (cajas[i].capacidadAlmacenamiento < minCapacidad) {
            minCapacidad = cajas[i].capacidadAlmacenamiento;
            caja = &cajas[i];
        }
    }

    // Si no hay cajas disponibles, el cliente se va
    if (caja == nullptr) {
        mvprintw(cliente->id, 0, "Cliente %d se fue sin comprar\n", cliente->id);
        refresh();
        pthread_exit(nullptr);
    }

    // Esperamos a que haya espacio disponible en la caja
    sem_wait(&caja->espaciosVacios);

    // Procesamos los productos del cliente en la caja
    auto inicioCliente = std::chrono::steady_clock::now();
    for (int i = 0; i < cliente->numProductos; i++) {
        procesarProducto(*caja, *cliente);
    }
    auto finCliente = std::chrono::steady_clock::now();
    std::chrono::duration<double> tiempoCliente = finCliente - inicioCliente;

    mvprintw(cliente->id, 0, "Cliente %d terminó de comprar\n", cliente->id);
    refresh();

    pthread_exit(reinterpret_cast<void*>(tiempoCliente.count()));
}

// Función que simula el proceso de una caja registradora
void* procesoCaja(void* arg) {
    CajaRegistradora* caja = reinterpret_cast<CajaRegistradora*>(arg);

    while (true) {
        // Esperamos a que haya productos disponibles en la caja
        sem_wait(&caja->productosDisponibles);
        sem_wait(&caja->mutex);

        // Simulamos el tiempo de procesamiento de los productos en la caja
        int tiempoProcesamiento = generarNumeroAleatorio(1000, 3000);
        usleep(tiempoProcesamiento);

        // Liberamos un espacio de almacenamiento en la caja
        caja->capacidadAlmacenamiento++;

        // Liberamos el mutex y notificamos que hay un espacio vacío en la caja
        sem_post(&caja->mutex);
        sem_post(&caja->espaciosVacios);
    }

    pthread_exit(nullptr);
}

int main() {
    initscr();
    curs_set(0);

    // Inicializamos las cajas registradoras
    for (int i = 0; i < MAX_CAJAS; i++) {
        sem_init(&cajas[i].mutex, 0, 1);
        sem_init(&cajas[i].espaciosVacios, 0, CAPACIDAD_ALMACENAMIENTO_MAX);
        sem_init(&cajas[i].productosDisponibles, 0, 0);
        cajas[i].id = i;
        cajas[i].capacidadAlmacenamiento = CAPACIDAD_ALMACENAMIENTO_MAX;

        pthread_create(&cajeros[i], nullptr, procesoCaja, &cajas[i]);
    }

    // Generamos los clientes
    for (int i = 0; i < MAX_CLIENTES; i++) {
        Cliente cliente;
        cliente.id = i;
        cliente.numProductos = generarNumeroAleatorio(1, MAX_PRODUCTOS);

        mvprintw(i, 0, "Cliente %d llegó a la caja %d con %d productos\n", cliente.id, caja->id, cliente.numProductos);
        refresh();

        pthread_create(&clientes[i], nullptr, procesoCliente, &cliente);

        usleep(generarNumeroAleatorio(1000, 3000));
    }

    // Esperamos a que todos los clientes terminen de procesar sus productos
    double tiempoTotal = 0.0;
    for (int i = 0; i < MAX_CLIENTES; i++) {
        void* tiempoCliente;
        pthread_join(clientes[i], &tiempoCliente);
        tiempoTotal += *reinterpret_cast<double*>(tiempoCliente);
    }

    // Terminamos el programa
    endwin();
    for (int i = 0; i < MAX_CAJAS; i++) {
        pthread_cancel(cajeros[i]);
        sem_destroy(&cajas[i].mutex);
        sem_destroy(&cajas[i].espaciosVacios);
        sem_destroy(&cajas[i].productosDisponibles);
    }

    std::cout << "Tiempo total de procesamiento: " << tiempoTotal << " segundos" << std::endl;

    return 0;
}