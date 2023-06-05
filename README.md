# Carrera de Caballos 🐴

Este programa simula una carrera de caballos en la terminal utilizando la biblioteca ncurses en C++.

## Requisitos previos

Es necesario tener instalada la biblioteca ncurses en el sistema antes de compilar y ejecutar el programa. Es posible instalarla en sistemas basados en Debian ejecutando el siguiente comando:

> sudo apt-get install libncurses5-dev libncursesw5-dev

## Compilación y ejecución

El programa puede ser compilado utilizando el siguiente comando:

> g++ final.cpp -o final -lncurses -lpthread -DMUTEX

Luego, para ejecutar el programa, utilice el siguiente comando:

> final

## Uso del programa

El programa solicitara los siguientes datos de la carrera:

- Cantidad de caballos que participarán (entre 2 y 7).
- Cantidad de metros de la pista (30, 40, 50 o 60 metros).
- Cantidad de vueltas a la pista (entre 1 y 4).

Luego, para cada caballo, se debe ingresar el número del caballo.

Una vez ingresados los datos, se dará comienzo a la simulación de la carrera en la terminal. Los caballos se moverán aleatoriamente en la pista y se mostrarán las estadísticas en tiempo real, incluyendo el tiempo transcurrido, las vueltas y metros recorridos por cada caballo.

## Créditos

Este programa fue creado por Kevin Campos, Maritxiu Castro y Debora Huerta como parte del curso de Sistemas Operativos de la UCSC.

![Imagen de sistemas operativos](https://pbs.twimg.com/media/DMPLIikU8AA6_GR?format=jpg&name=small)