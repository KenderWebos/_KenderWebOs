#include <iostream> // esto hace que podamos usar cout y cin

#include <list> // esto hace que podamos usar list

using namespace std; // esto hace que no tengamos que poner std:: antes de cada funcion

struct Carrera { // declaramos la estructura de la carrera
    int cantCaballos;
    int canMetrosPista;
    int cantVueltas;
};

struct Caballo { // declaramos la estructura de los caballos
    string nombre;
    int numero;
    int posicion;
};

void pedirDatosCarrera(Carrera& carrera) { // & para usar la estructura original
    cout << "Ingrese la cantidad de caballos que participarán: ";
    cin >> carrera.cantCaballos;
    cout << "Ingrese la cantidad de metros de la pista: ";
    cin >> carrera.canMetrosPista;
    cout << "Ingrese la cantidad de vueltas: ";
    cin >> carrera.cantVueltas;
}

void pedirDatosCaballo(Caballo& caballo){ // & para usar la estructura original
    cout << "Ingrese el nombre del caballo: ";
    cin >> caballo.nombre;
    cout << "Ingrese el numero del caballo: ";
    cin >> caballo.numero;
}

/*
pediremos al usuario:
    al inicio:
        cuantos caballos participaran (int)
        _cantidad de metros de la pista (int)
        _cantidad de vueltas (int)
    por cada caballo:
        nombre (string)
        numero (int)
    se lanza la Funcion inicializarCarrera():
        esta 

*/
// Funcion para iniciar la carrera
// Parametros:
// 
// podria ser cantidad de vueltas y apuestas

int main(){
    Carrera carrera;
    list<Caballo> caballos;

    pedirDatosCarrera(carrera);

    cout << carrera.cantCaballos << endl;
    cout << carrera.canMetrosPista << endl;
    cout << carrera.cantVueltas << endl;

    return 0;
}