#include<vector>         //esto hace que podamos usar vector
#include<stdlib.h>       //bibliotecas funciones estandar
#include<unistd.h>       //bibliotecas de sistema Unix
#include<ncurses.h>      //bibliotecas manejo de pantalla
#include<pthread.h>      //bibliotecas de threads
#include<string>
#include<iostream>

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
{
    printw("Ingrese la cantidad de caballos que participarán: ");
    refresh();
    scanw("%d", &carrera.cantCaballos);

    printw("Ingrese la cantidad de metros de la pista: ");
    refresh();
    scanw("%d", &carrera.canMetrosPista);
}

void pedirDatosCaballo(Caballo &caballo)
{
    printw("Ingrese el nombre del caballo: ");
    refresh();
    scanw("%s", &caballo.nombre[0]);

    printw("Ingrese el numero del caballo: ");
    refresh();
    scanw("%d", &caballo.numero);
}

int getRandomNumber(int min, int max)
{
    int random = rand() % max + min;
    return random;
}

void *avanceCaballo(int param)
{
    printw("me crearoooon: ");
    refresh();

// #ifdef MUTEX
//     pthread_mutex_lock(&pantalla); // bloquea la pantalla (seccion critica)
// #endif

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

// #ifdef MUTEX
//     pthread_mutex_unlock(&pantalla); // desbloquea la pantalla (seccion critica)
// #endif
}

void iniciarCarrera()
{
    pthread_t threads[carrera.cantCaballos];
    int error[carrera.cantCaballos];

    for (int i = 0; i < carrera.cantCaballos; i++)
    {
        error[i] = pthread_create(&threads[i], NULL, (void *)avanceCaballo, 1);

        if (error[i] != 0)
        {
            printw("No puedo crear el caballo: %d", i);
            refresh();
        }
        else
        {
            printw("Caballo creado: %d", i);
            refresh();
        }
    }
}

int main()
{
    initscr();

    #ifdef MUTEX
        pthread_mutex_init(&pantalla, NULL); // inicializa variable pantalla tipo mutex
    #endif

    pedirDatosCarrera(carrera);

    for (int i = 0; i < carrera.cantCaballos; i++)
    {
        Caballo caballo;

        printw(">> Datos del caballo [%d] <<",(i + 1));
        refresh();
        pedirDatosCaballo(caballo);
        caballos.push_back(caballo);
    }

    iniciarCarrera();

    endwin();
    return 0;
}