#include <vector>    //esto hace que podamos usar vector
#include <stdlib.h>  //bibliotecas funciones estandar
#include <unistd.h>  //bibliotecas de sistema Unix
#include <ncurses.h> //bibliotecas manejo de pantalla
#include <pthread.h> //bibliotecas de threads
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

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

struct Params
{
    int index;
};

#ifdef MUTEX
pthread_mutex_t pantalla;
#endif

std::mutex pantallaX;

Carrera carrera;          // declaramos la estructura carrera
vector<Caballo> caballos; // declaramos la lista de caballos

vector<pthread_t> threads;

int carrilLibre = 0;

int padding = 20;

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

void printUI()
{
    const char *title = R"(
                                                          :::::::::      :::      ::::::::  :::::::::: 
                                                         :+:    :+:   :+: :+:   :+:    :+: :+:         
                                                        +:+    +:+  +:+   +:+  +:+        +:+          
                                                       +#++:++#:  +#++:++#++: +#+        +#++:++#      
                                                      +#+    +#+ +#+     +#+ +#+        +#+            
                                                     #+#    #+# #+#     #+# #+#    #+# #+#             
                                                    ###    ### ###     ###  ########  ##########       
)";
    mvprintw(0, 0, "%s", title);
    refresh();

    sleep(4);
}

int getRandomNumber(int min, int max)
{
    int random = rand() % max + min;
    return random;
}

void *horseMainProcess(void *param)
{
    // int index = param.index;

    Params *p = (Params*)param;
    int index = p->index;

    Caballo caballo = caballos[index];

    while (true)
    {
        pantallaX.lock(); // aqui para

        caballos[index].posicion += getRandomNumber(0, 3);

        caballos[index].vueltas = caballos[index].posicion / carrera.canMetrosPista;

        mvprintw(index+padding, 0 + caballos[index].posicion, "%c", letras[index]);
        refresh();

        pantallaX.unlock();
        sleep(1);
    }

    return NULL;
}

void printChrono()
{
    int maxRows, maxCols;
    getmaxyx(stdscr, maxRows, maxCols);

    int contador = 0;

    while (true)
    {
        pantallaX.lock();

        mvprintw(maxRows - 1, (maxCols / 2) - 12, "Tiempo transcurrido: %ds", contador);

        refresh();
        contador++;

        pantallaX.unlock();
        sleep(1);
    }
}

void printClock()
{
    while (true)
    {
        pantallaX.lock();

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
        int clockRow = maxRows - 2;      // Fila
        int clockCol = maxCols / 2 - 12; // Columna

        // Imprimir el emoji de reloj y la hora en la posición calculada
        mvprintw(clockRow, clockCol, "Reloj -> %02d:%02d:%02d", hours, minutes, seconds);

        refresh(); // Actualizar la pantalla

        pantallaX.unlock();

        sleep(1);
    }
}

void printStats()
{
    while (true)
    {
        pantallaX.lock();
        mvprintw(0, 0, "||> Estadisticas <||\n");

        for (int i = 0; i < carrera.cantCaballos; i++)
        {
            mvprintw(i + 1, 0, "Caballo %c: Vueltas %i - %i Metros\n", letras[i], caballos[i].vueltas, caballos[i].posicion);
        }

        mvprintw(carrera.cantCaballos + 1, 0, "Totales: %d vueltas - %d Metros\n", carrera.cantVueltas, carrera.canMetrosPista);

        refresh();

        pantallaX.unlock();

        sleep(1);
    }
}

void iniciarCarrera()
{

    Params params;

    for (int i = 0; i < carrera.cantCaballos; i++)
    {
        pthread_t thread;
        int threadParam = i;
        threads.push_back(thread);

        params.index = i;

        int result = pthread_create(&threads[i], NULL, horseMainProcess, &params);

        sleep(1);

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

    printUI();

    std::thread threadObj(printStats);
    std::thread threadObj02(printClock);
    std::thread threadObj03(printChrono);

    iniciarCarrera();

    for (int i = 0; i < threads.size(); i++)
    {
        pthread_join(threads[i], NULL);
    }

    threadObj.join();
    threadObj02.join();
    threadObj03.join();

    printTitle();

    sleep(5);
    endwin();

    return 0;
}
