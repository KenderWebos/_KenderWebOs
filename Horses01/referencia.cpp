#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

struct Caballo {
    string nombre;
    string color;
    int numero;
    int posicion;
};

void iniciarCarrera(vector<Caballo>& caballos) {
    int meta = 100;

    while (true) {
        for (auto& caballo : caballos) {
            int avance = rand() % 3 + 1;
            caballo.posicion += avance;

            // Verificar si el caballo llegó a la meta
            if (caballo.posicion >= meta) {
                cout << "¡El caballo " << caballo.nombre << " ganó la carrera! ¡Felicitaciones!" << endl;
                return;
            }
        }

        // Imprimir pista y caballos
        system("clear");
        for (auto& caballo : caballos) {
            cout << "Pista del caballo " << caballo.nombre << ": ";
            for (int i = 0; i < meta; i++) {
                if (caballo.posicion == i) {
                    cout << caballo.nombre[0] << caballo.numero;
                } else {
                    cout << "-";
                }
            }
            cout << endl;
        }

        // Esperar medio segundo antes de actualizar de nuevo
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main() {
    srand(time(NULL));

    int numCaballos;
    cout << "Ingrese la cantidad de caballos que participarán: ";
    cin >> numCaballos;

    vector<Caballo> caballos;
    for (int i = 1; i <= numCaballos; i++) {
        Caballo caballo;
        cout << "Ingrese el nombre del caballo " << i << ": ";
        cin >> caballo.nombre;
        cout << "Ingrese el color del caballo " << i << ": ";
        cin >> caballo.color;
        caballo.numero = i;
        caballo.posicion = 0;
        caballos.push_back(caballo);
    }

    iniciarCarrera(caballos);

    return 0;
}