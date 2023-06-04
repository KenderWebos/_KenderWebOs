#include <vector>    //esto hace que podamos usar vector
#include <stdlib.h>  //bibliotecas funciones estandar
#include <unistd.h>  //bibliotecas de sistema Unix
#include <ncurses.h> //bibliotecas manejo de pantalla
#include <pthread.h> //bibliotecas de threads
#include <string>
#include <iostream>
#include <thread>

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

char letras[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

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

    printw("Ingrese el numero del caballo %c: ", letras[index]);
    refresh();
    scanw("%d", &caballo.numero);

    caballo.carril = carrilLibre;
    carrilLibre++;

    caballo.posicion = 0;
    caballo.vueltas = 0;

    caballos.push_back(caballo);

    clear();
}

void comprobarGanador()
{
    // if (caballos[i].posicion >= carrera.canMetrosPista)
    // {
    //     system("clear");
    //     cout << ">> 🐴 Carrera de caballos 🐴 <<" << endl;
    //     cout << "🎉✨¡Felicitaciones!✨🎉" << endl;
    //     cout << "¡El caballo 🐴 " << caballos[i].nombre << " ganó la carrera!" << endl;
    //     return;
    // }
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
    int index = 0;
    // Caballo caballo = caballos[*(int *)param];

    printw("Soy un caballo y estoy vivoooo!! soy el numero {%d}! \n", *(int *)param);
    refresh();

#ifdef MUTEX
    pthread_mutex_lock(&pantalla);
#endif

    while (true)
    {
        caballos[index].posicion += getRandomNumber(0, 3);

        mvprintw(20, 20 + caballos[index].posicion, "%c", letras[index]);
        refresh();
        sleep(1);
    }

#ifdef MUTEX
    pthread_mutex_unlock(&pantalla);
#endif

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

#ifdef MUTEX
        pthread_mutex_lock(&pantalla);
#endif
        clear(); // Limpiar la pantalla

        // Obtener la hora actual del sistema
        time_t now = time(0);
        struct tm *timeinfo = localtime(&now);

        // Obtener los componentes de la hora
        int hours = timeinfo->tm_hour;
        int minutes = timeinfo->tm_min;
        int seconds = timeinfo->tm_sec;

        // Obtener el tamaño de la ventana
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

#ifdef MUTEX
        pthread_mutex_unlock(&pantalla);
#endif
    }
}

void printStats()
{
    while (true)
    {
#ifdef MUTEX
        pthread_mutex_lock(&pantalla);
#endif
        clear();

        printw("> Estadisticas < \n");

        for (int i = 0; i < carrera.cantCaballos; i++)
        {
            // printw("Caballo A: Vueltas %i - %i Metros\n", caballos[i].vueltas, caballos[i].posicion);
            printw("Caballo %c: Vueltas %i - %i Metros\n", letras[i], caballos[i].vueltas, caballos[i].posicion);
        }

        printw("Totales: %d vueltas - %d Metros\n", carrera.cantVueltas, carrera.canMetrosPista);

        refresh();

        sleep(1);
#ifdef MUTEX
        pthread_mutex_unlock(&pantalla);
#endif
    }
}

void iniciarCarrera()
{
    for (int i = 0; i < carrera.cantCaballos; i++)
    {
        pthread_t thread;
        int threadParam = i;
        threads.push_back(thread);

        int result = pthread_create(&threads[i], NULL, horseMainProcess, &i);

        if (result != 0)
        {
            printw("Error al crear el hilo\n");
            refresh();

            sleep(5);
            endwin();
        }
    }
}

int main()
{
    initscr();

    // bool debugMode = true;

    // if (!debugMode)
    // {
    //     printTitle();
    //     printTitle2();
    // }

    // pedirDatosCarrera();

    // for (int i = 0; i < carrera.cantCaballos; i++)
    // {
    //     pedirDatosCaballo(i); // pedimos los datos por cada caballo
    // }

    printw("> tamos listeilor con los datos \n");

    // iniciarCarrera();

    // std::thread threadObj(printStats);

    std::thread threadObj02(printClock);

    // printClock();
    // printChrono();

    // printTitle();

    for (int i = 0; i < threads.size(); i++)
    {
        pthread_join(threads[i], NULL);
    }

    threadObj.join();

    sleep(5);
    endwin();

    return 0;
}
