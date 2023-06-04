#include <iostream>
#include <vector>
#include <pthread.h>
#include <random>
#include <unistd.h>

struct Carrera {
    int cantCaballos;
    int canMetrosPista;
    int cantVueltas;
};

struct Caballo {
    std::string nombre;
    int numero;
    int posicion;
};

Carrera carrera;
std::vector<Caballo> caballos;

pthread_mutex_t pantalla_mutex = PTHREAD_MUTEX_INITIALIZER;

void pedirDatosCarrera(Carrera& carrera) {
    std::cout << "Ingrese la cantidad de caballos que participarán: ";
    std::cin >> carrera.cantCaballos;
    std::cout << "Ingrese la cantidad de metros de la pista: ";
    std::cin >> carrera.canMetrosPista;
    std::cout << "Ingrese la cantidad de vueltas: ";
    std::cin >> carrera.cantVueltas;
}

void pedirDatosCaballo(Caballo& caballo) {
    std::cout << "Ingrese el nombre del caballo: ";
    std::cin >> caballo.nombre;
    std::cout << "Ingrese el número del caballo: ";
    std::cin >> caballo.numero;
}

int getRandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

void avanceCaballo(Caballo& caballo, Carrera carrera) {
    while (caballo.posicion < carrera.canMetrosPista * carrera.cantVueltas) {
        int avance = getRandomNumber(1, 3);
        caballo.posicion += avance;
        usleep(500000);
    }
}

void* correrCarrera(void* arg) {
    Caballo* caballo = reinterpret_cast<Caballo*>(arg);
    avanceCaballo(*caballo, carrera);
    return nullptr;
}

void iniciarCarrera() {
    pthread_t threads[carrera.cantCaballos];

    for (int i = 0; i < carrera.cantCaballos; i++) {
        Caballo& caballo = caballos[i];
        pthread_create(&threads[i], nullptr, correrCarrera, &caballo);
    }

    for (int i = 0; i < carrera.cantCaballos; i++) {
        pthread_join(threads[i], nullptr);
    }
}

void presentarResultado() {
    pthread_mutex_lock(&pantalla_mutex);

    std::cout << std::endl;
    std::cout << "¡Carrera finalizada!" << std::endl;
    std::cout << "Características de la carrera:" << std::endl;
    std::cout << "Cantidad de caballos: " << carrera.cantCaballos << std::endl;
    std::cout << "Longitud de la pista: " << carrera.canMetrosPista << " metros" << std::endl;
    std::cout << "Cantidad de vueltas: " << carrera.cantVueltas << std::endl;
    std::cout << std::endl;

    std::cout << "Resultado de la carrera:" << std::endl;
    for (const auto& caballo : caballos) {
        std::cout << "Caballo " << caballo.numero << ": " << caballo.posicion << " metros" << std::endl;
    }

    pthread_mutex_unlock(&pantalla_mutex);
}

int main() {
    pedirDatosCarrera(carrera);

    for (int i = 0; i < carrera.cantCaballos; i++) {
        Caballo caballo;
        std::cout << ">> Datos del caballo [" << i + 1 <<"] <<" << std::endl;
        pedirDatosCaballo(caballo);
        caballos.push_back(caballo);
    }

    iniciarCarrera();

    presentarResultado();

    return 0;
}
