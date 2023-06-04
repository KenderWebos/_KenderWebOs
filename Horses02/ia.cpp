#include <iostream>
#include <ncurses.h>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Estructura de la carrera
struct Carrera {
    int cantCaballos;
    int canMetrosPista;
    int cantVueltas;
};

// Estructura de los caballos
struct Caballo {
    string nombre;
    int numero;
    int posicion;
};

// Variables globales
Carrera carrera;
vector<Caballo> caballos;
bool carreraTerminada = false;

// Función para generar un número aleatorio en un rango
int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Función para actualizar la posición del caballo
void avanzarCaballo(Caballo& caballo) {
    int avance = getRandomNumber(1, 2);
    caballo.posicion += avance;
}

// Función que representa la carrera
void* carreraThread(void* arg) {
    while (!carreraTerminada) {
        for (int i = 0; i < caballos.size(); i++) {
            avanzarCaballo(caballos[i]);

            if (caballos[i].posicion >= (carrera.canMetrosPista * carrera.cantVueltas)) {
                carreraTerminada = true;
                break;
            }

            usleep(500000);  // Esperar 500ms entre movimientos
        }
    }

    pthread_exit(NULL);
}

// Función para imprimir el estado de la carrera
void imprimirEstadoCarrera() {
    system("clear");
    cout << ">> 🐴 Carrera de caballos 🐴 <<" << endl;
    cout << "Vueltas: " << carrera.cantVueltas << " / Pista: " << carrera.canMetrosPista << " metros" << endl;
    cout << "------------------------" << endl;

    for (int i = 0; i < caballos.size(); i++) {
        cout << caballos[i].nombre << " 🐎: " << caballos[i].posicion << " metros" << endl;
    }
}

// Función para presentar el resultado de la carrera
void presentarResultado() {
    cout << endl;
    cout << ">> 🎉 Resultado de la carrera 🎉 <<" << endl;
    cout << "¡La carrera ha terminado!" << endl;
    cout << "------------------------" << endl;

    for (int i = 0; i < caballos.size(); i++) {
        cout << "Caballo " << caballos[i].numero << " - " << caballos[i].nombre << endl;
        cout << "Posición final: " << caballos[i].posicion << " metros" << endl;
        cout << endl;
    }
}

int main() {
    srand(time(NULL));  // Semilla para números aleatorios

    // Pedir los datos de la carrera
    cout << "Ingrese la cantidad de caballos que participarán: ";
    cin >> carrera.cantCaballos;
    cout << "Ingrese la cantidad de metros de la pista (30, 40, 50 o 60): ";
    cin >> carrera.canMetrosPista;
    cout << "Ingrese la cantidad de vueltas (1, 2, 3 o 4): ";
    cin >> carrera.cantVueltas;

    // Pedir los datos de cada caballo
    for (int i = 0; i < carrera.cantCaballos; i++) {
        Caballo caballo;
        cout << "Ingrese el nombre del caballo " << i + 1 << ": ";
        cin >> caballo.nombre;
        caballo.numero = i + 1;
        caballo.posicion = 0;
        caballos.push_back(caballo);
    }

    // Inicializar la librería ncurses
    initscr();
    cbreak();
    noecho();

    // Crear el hilo para la carrera
    pthread_t carreraThreadID;
    pthread_create(&carreraThreadID, NULL, carreraThread, NULL);

    // Mostrar el estado de la carrera hasta que termine
    while (!carreraTerminada) {
        imprimirEstadoCarrera();
        usleep(500000);  // Esperar 500ms entre actualizaciones
    }

    // Esperar a que termine el hilo de la carrera
    pthread_join(carreraThreadID, NULL);

    // Presentar el resultado de la carrera
    imprimirEstadoCarrera();
    presentarResultado();

    // Restaurar la configuración de la consola
    endwin();

    return 0;
}
