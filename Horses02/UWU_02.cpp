#include <iostream> // esto hace que podamos usar cout y cin

// #include <ncurses.h>

#include <vector>   // esto hace que podamos usar vector
#include <list>     // esto hace que podamos usar list
#include <unistd.h> // esto hace que podamos usar sleep y manejar intervalos de tiempo

using namespace std; // esto hace que no tengamos que poner std:: antes de cada funcion

struct Carrera
{ // declaramos la estructura de la carrera
    int cantCaballos;
    int canMetrosPista;
    int cantVueltas;
};

struct Caballo
{ // declaramos la estructura de los caballos
    string nombre;
    int numero;
    int posicion;
};

// variable pantalla tipo mutex para sincronizar el acceso a la pantalla
#ifdef MUTEX
pthread_mutex_t pantalla;
#endif

Carrera carrera;          // declaramos la estructura carrera
vector<Caballo> caballos; // declaramos la lista de caballos

void pedirDatosCarrera(Carrera &carrera)
{ // & para usar la estructura original
    cout << "Ingrese la cantidad de caballos que participarán: ";
    cin >> carrera.cantCaballos;
    cout << "Ingrese la cantidad de metros de la pista: ";
    cin >> carrera.canMetrosPista;
    // cout << "Ingrese la cantidad de vueltas: ";
    // cin >> carrera.cantVueltas;
}

// void pedirDatosCarrera(Carrera& carrera) { // & para usar la estructura original
//     carrera.cantCaballos = 3;
//     carrera.canMetrosPista = 100;
//     carrera.cantVueltas = 1;
// }

void pedirDatosCaballo(Caballo &caballo)
{ // & para usar la estructura original
    cout << "Ingrese el nombre del caballo: ";
    cin >> caballo.nombre;
    cout << "Ingrese el numero del caballo: ";
    cin >> caballo.numero;
}

int getRandomNumber(int min, int max)
{
    int random = rand() % max + min;
    return random;
}

void avanceCaballo(int index, vector<Caballo> &caballos, Carrera carrera)
{
    // int avance = getRandomNumber(0, 4);
    // caballos[index].posicion += avance;

    cout << "me crearoooon";

#ifdef MUTEX
    pthread_mutex_lock(&pantalla); // bloquea la pantalla (seccion critica)
#endif

    // imprimir

#ifdef MUTEX
    pthread_mutex_unlock(&pantalla); // desbloquea la pantalla (seccion critica)
#endif
}

void iniciarCarrera(vector<Caballo> &caballos, Carrera carrera)
{
    // error1 = pthread_create(&idHilo1, &atributo1, (void *)funcionThread, (void *)&in1);

    pthread_t threads[carrera.cantCaballos];
    int error[carrera.cantCaballos];

    for (int i = 0; i < carrera.cantCaballos; i++)
    {
        error[i] = pthread_create(&threads[i], NULL, avanceCaballo(i, caballos, carrera), (void *)&caballos[i]);

        if (error[i] != 0)
        {
            cout << "No puedo crear el caballo: " + i << endl;
        }
        else
        {
            cout << "Caballo creado: " + i << endl;
        }
    }

    // while (true)
    // {
    //     system("clear");
    //     cout << ">> 🐴 Carrera de caballos 🐴 <<" << endl;

    //     for (int i = 0; i < caballos.size(); i++)
    //     {
    //         avanceCaballo(i, caballos, carrera);

    //         if (caballos[i].posicion >= carrera.canMetrosPista)
    //         {
    //             system("clear");
    //             cout << ">> 🐴 Carrera de caballos 🐴 <<" << endl;
    //             cout << "🎉✨¡Felicitaciones!✨🎉" << endl;
    //             cout << "¡El caballo 🐴 " << caballos[i].nombre << " ganó la carrera!" << endl;
    //             return;
    //         }

    //         for (int j = 0; j < caballos[i].posicion; j++)
    //         {

    //             cout << "_";
    //         }
    //         cout << caballos[i].nombre << " 🐎: " << caballos[i].posicion << endl;
    //     }

    //     usleep(500000);
    // }
}

int main()
{

#ifdef MUTEX
    pthread_mutex_init(&pantalla, NULL); // inicializa variable pantalla tipo mutex
#endif

    pedirDatosCarrera(carrera); // pedimos los datos base de la carrera

    for (int i = 0; i < carrera.cantCaballos; i++)
    { // iniciamos un bucle para pedir los datos de cada caballo
        Caballo caballo;

        cout << ">> Datos del caballo [" << i + 1 << "] <<" << endl; // pedimos los datos del caballo
        pedirDatosCaballo(caballo);
        caballos.push_back(caballo);
    }

    iniciarCarrera(caballos, carrera);

    return 0;
}