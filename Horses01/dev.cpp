#include <iostream> // esto hace que podamos usar cout y cin

// #include <ncurses.h>

#include <vector> // esto hace que podamos usar vector
#include <list> // esto hace que podamos usar list
#include <unistd.h> // esto hace que podamos usar sleep y manejar intervalos de tiempo

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

Carrera carrera; // declaramos la estructura carrera
vector<Caballo> caballos; // declaramos la lista de caballos

// void pedirDatosCarrera(Carrera& carrera) { // & para usar la estructura original
//     cout << "Ingrese la cantidad de caballos que participarán: ";
//     cin >> carrera.cantCaballos;
//     cout << "Ingrese la cantidad de metros de la pista: ";
//     cin >> carrera.canMetrosPista;
//     cout << "Ingrese la cantidad de vueltas: ";
//     cin >> carrera.cantVueltas;
// }

void pedirDatosCarrera(Carrera& carrera) { // & para usar la estructura original
    carrera.cantCaballos = 3;
    carrera.canMetrosPista = 100;
    carrera.cantVueltas = 1;
}

void pedirDatosCaballo(Caballo& caballo){ // & para usar la estructura original
    cout << "Ingrese el nombre del caballo: ";
    cin >> caballo.nombre;
    cout << "Ingrese el numero del caballo: ";
    cin >> caballo.numero;
}

int getRandomNumber(int min, int max){
    int random = rand() % max + min;
    return random;
}

void avanceCaballo(int index, vector<Caballo>& caballos, Carrera carrera){
    int avance = getRandomNumber(0, 4);
    caballos[index].posicion += avance;
}

void iniciarCarrera(vector<Caballo>& caballos, Carrera carrera){
    while(true){
        system("clear");
        cout << ">> 🐴 Carrera de caballos 🐴 <<" << endl;

        for(int i = 0; i < caballos.size(); i++){
            avanceCaballo(i, caballos, carrera );

            if(caballos[i].posicion >= carrera.canMetrosPista){
                system("clear");
                cout << ">> 🐴 Carrera de caballos 🐴 <<" << endl;
                cout << "🎉✨¡Felicitaciones!✨🎉"<<endl;
                cout << "¡El caballo 🐴 " << caballos[i].nombre << " ganó la carrera!" << endl;
                return;
            }
            
            for(int j = 0; j < caballos[i].posicion; j++){
                
                cout << "-";
            }
            cout << caballos[i].nombre << " 🐎: " << caballos[i].posicion << endl;
        }

        usleep(500000);
    }
}

/*
pediremos al usuario:
    al inicio:
        cuantos caballos participaran (int)
        cantidad de metros de la pista (int)
        _cantidad de vueltas (int)
    por cada caballo:
        nombre (string)
        numero (int)
    
    una vez tengamos todos los datos:
        iniciamos la carrera con la funcion iniciarCarrera();
    ideas:
    1. apuestas
    2. colores
    3. animaciones graficas
/*
podria ser cantidad de vueltas y apuestas
*/

int main(){

    pedirDatosCarrera(carrera); // pedimos los datos base de la carrera

    for (int i = 0; i < carrera.cantCaballos; i++) { // iniciamos un bucle para pedir los datos de cada caballo
        Caballo caballo;

        cout << ">> Datos del caballo [" << i + 1 <<"] <<" << endl; // pedimos los datos del caballo
        pedirDatosCaballo(caballo);
        caballos.push_back(caballo);
    }

    iniciarCarrera(caballos, carrera);

    return 0;
}
