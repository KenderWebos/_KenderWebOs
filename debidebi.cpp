#include <vector>    //esto hace que podamos usar vector
#include <stdlib.h>  //bibliotecas funciones estandar
#include <unistd.h>  //bibliotecas de sistema Unix
#include <ncurses.h> //bibliotecas manejo de pantalla
#include <pthread.h> //bibliotecas de threads
#include <string>
#include <iostream>

using namespace std;

struct Carrera
{ // declaramos la estructura de la carrera
    int cantCaballos;
    int canMetrosPista;
    int cantVueltas;
};

struct Caballo // no es directamente un caballo, es un caballo en la carrera
{              // declaramos la estructura de los caballos
    string nombre;
    int numero;
    int vueltas;
    int posicion;
    int carril;
};

#ifdef MUTEX
pthread_mutex_t pantalla;
#endif

Carrera carrera;          // declaramos la estructura carrera
vector<Caballo> caballos; // declaramos la lista de caballos

vector<pthread_t> threads;

int carrilLibre = 0;

void pedirDatosCarrera()
{
    printw("Ingrese la cantidad de caballos que participarán: ");
    refresh();
    scanw("%d", &carrera.cantCaballos);

    printw("Ingrese la cantidad de metros de la pista: ");
    refresh();
    scanw("%d", &carrera.canMetrosPista);

    printw("Ingrese la cantidad de vueltas a la pista: ");
    refresh();
    scanw("%d", &carrera.cantVueltas);

    clear();
}

void pedirDatosCaballo(int index)
{
    Caballo caballo;

    printw(">> Datos del caballo [%d] <<\n", index + 1);
    refresh();

    // printw("Ingrese el nombre del caballo %i: ", index + 1);
    // refresh();
    // scanw("%s", &caballo.nombre);

    printw("Ingrese el numero del caballo %i: ", index + 1);
    refresh();
    scanw("%d", &caballo.numero);

    // char aux[50];
    // scanw("%s", aux);
    // caballo.nombre = aux;

    caballo.posicion = 0;
    caballo.vueltas = 0;

    caballo.carril = carrilLibre;
    carrilLibre++;

    caballos.push_back(caballo);

    clear();
}

void printTitle()
{
    const char *title = R"(
 __      _  _                            ___                           __
 \ \    | || |  ___   _ _   ___  ___    | __|  __ _   _ _   _ __      / /
  > >   | __ | / _ \ | '_| (_-< / -_)   | _|  / _` | | '_| | '  \    < < 
 /_/    |_||_| \___/ |_|   /__/ \___|   |_|   \__,_| |_|   |_|_|_|    \_\
 
                         ,  ,
                        _)\/)
                       /  9)>
                      //  _ /\
                     /(,_//   \           ____
                     \_('  \   )         /__  \
                           /    \>___,----. )  )
                          (                 \  (
                          /                 |  )
                          |  '          _   | /
                          | \__|  /___,/    /(
                           \ | | / (  (   _/
                           ('| ( )  \_ \_ >
                            \\ |/      | )/
                             \)\\      )//
                             /,/,     /(<,
                          ,,/_/_).   / )_),,, ,,_
)";
    mvprintw(0, 0, "%s", title);
    refresh();

    sleep(4);
    clear();
}

void printTitle2()
{
    const char *title = R"(
          _   _           _          _      _                                                        _ 
  __ _  | | | |    _ _  (_)  __ _  | |_   | |_   ___    _ _   ___   ___  ___   _ _  __ __  ___   __| |
 / _` | | | | |   | '_| | | / _` | | ' \  |  _| (_-<   | '_| / -_) (_-< / -_) | '_| \ V / / -_) / _` |
 \__,_| |_| |_|   |_|   |_| \__, | |_||_|  \__| /__/   |_|   \___| /__/ \___| |_|    \_/  \___| \__,_|
                            |___/                                                                     
               
)";
    mvprintw(0, 0, "%s", title);
    refresh();

    sleep(4);
    clear();
}

int getRandomNumber(int min, int max)
{
    int random = rand() % max + min;
    return random;
}

void *horseMainProcess(void *param)
{
    // Caballo caballo = caballos[*(int *)param];
    printw("Soy un caballo y estoy vivoooo!! soy el numero {%d}! \n", *(int *)param);
    refresh();

    // #ifdef MUTEX
    //     pthread_mutex_lock(&pantalla); // bloquea la pantalla (seccion critica)
    // #endif

    // cout << ">> 🐴 Carrera de caballos 🐴 <<" << endl;

    // for (int i = 0; i < caballos.size(); i++)
    // {
    //     avanceCaballo(i, caballos, carrera);

    //     if (caballos[i].posicion >= carrera.canMetrosPista)
    //     {
    //         system("clear");
    //         cout << ">> 🐴 Carrera de caballos 🐴 <<" << endl;
    //         cout << "🎉✨¡Felicitaciones!✨🎉" << endl;
    //         cout << "¡El caballo 🐴 " << caballos[i].nombre << " ganó la carrera!" << endl;
    //         return;
    //     }

    //     for (int j = 0; j < caballos[i].posicion; j++)
    //     {

    //         cout << "_";
    //     }
    //     cout << caballos[i].nombre << " 🐎: " << caballos[i].posicion << endl;
    // }

    //deb
    // void dibujarCaballos()
    // {
    //     clear();
    //     for (int i = 0; i < numCaballos; i++)
    //     {
    //         mvprintw(i + 1, posiciones[i], "Caballo %d", i + 1);
    //     }
    //     refresh();
    // }

    sleep(1);
    clear();

    // #ifdef MUTEX
    //     pthread_mutex_unlock(&pantalla); // desbloquea la pantalla (seccion critica)
    // #endif
    return NULL;
}

void printChrono()
{
    int maxRows, maxCols;
    getmaxyx(stdscr, maxRows, maxCols);

    int contador = 0;

    while (true)
    {
        mvprintw(maxRows - 1, (maxCols / 2) - 12, "Tiempo transcurrido: %ds", contador);

        refresh();
        contador++;
        sleep(1);
    }
}

void printClock()
{
    while (true)
    {
        clear(); // Limpiar la pantalla

        // Obtener la hora actual del sistema
        time_t now = time(0);
        struct tm *timeinfo = localtime(&now);

        // Obtener los componentes de la hora
        int hours = timeinfo->tm_hour;
        int minutes = timeinfo->tm_min;
        int seconds = timeinfo->tm_sec;

        // Obtener el tamano de la ventana
        int maxRows, maxCols;
        getmaxyx(stdscr, maxRows, maxCols);

        // Calcular las coordenadas para imprimir el reloj en la parte inferior central
        int clockRow = maxRows - 2;     // Fila
        int clockCol = maxCols / 2 - 4; // Columna

        // Imprimir el emoji de reloj y la hora en la posición calculada
        mvprintw(clockRow, clockCol, "Reloj -> %02d:%02d:%02d", hours, minutes, seconds);

        refresh(); // Actualizar la pantalla

        // Pausar el programa durante 1 segundo
        napms(1000);
    }
}

void printStats()
{
    while (true)
    {
        clear();

        printw("> Estadisticas < \n");

        for (int i = 0; i < carrera.cantCaballos; i++)
        {
            printw("Caballo %c: Vueltas %i - %i Metros\n", letras[i+1], caballos[i].vueltas, caballos[i].posicion);
        }

        printw("Totales: %d vueltas - %d Metros\n", carrera.cantVueltas, carrera.canMetrosPista);

        refresh();
            
        sleep(1);
    }
}

// void iniciarCarrera()
// {
//     pthread_t threads[carrera.cantCaballos];
//     int error[carrera.cantCaballos];

//     for (int i = 0; i < carrera.cantCaballos; i++)
//     {
//         error[i] = pthread_create(&threads[i], NULL, (void *)avanceCaballo, 1);

//         if (error[i] != 0)
//         {
//             printw("No puedo crear el caballo: %d", i);
//             refresh();
//         }
//         else
//         {
//             printw("Caballo creado: %d", i);
//             refresh();
//         }
//     }
// }

int main()
{
    initscr();

    bool debugMode = true;

    if (!debugMode)
    {
        printTitle();
        printTitle2();
    }

    pedirDatosCarrera();

    for (int i = 0; i < carrera.cantCaballos; i++)
    {
        pedirDatosCaballo(i); // pedimos los datos por cada caballo
    }

    printw("> tamos listeilor con los datos \n");

    // for (int i = 0; i < carrera.cantCaballos; i++) // iniciamos la rutina de carrera dando hilo a cada proceso de la funcion
    // {
    //     pthread_t thread;
    //     int threadParam = i + 1;
    //     threads.push_back(thread);

    //     int result = pthread_create(&threads[i], NULL, horseMainProcess, &i);

    //     // pthread_create(&hebras[i], nullptr, correrCarrera, &i);

    //     if (result != 0)
    //     {
    //         printw("Error al crear el hilo\n");
    //         refresh();
    //         endwin();
    //         return 1;
    //     }
    // }

    printStats();

    // printClock();

    // printChrono();

    sleep(5);
    endwin();

    return 0;
}