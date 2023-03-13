# algo3-tp1


## Compilacion rapida
Para compilar corra los siguientes comandos, estos generan los 5 ejecutables en la subcarpeta test

```bash
cmake .
make
```

de querer usar docker hacer lo siguiente:
```bash
docker-compose up -d --build
docker ps #buscamos la id del contenedor
docker exec -ti <container-id> bash
cd data/test #lugar por defecto de los binarios
```

## Mas detalles

para reducir la cantidad de archivos pero manteniendo todos los algoritmos individuales, se pusieron `#ifdef`    
y al momento de compilar dependiendo de que se define:

- ejercicio 1 sin podas:
  - archivos: ej12.cpp y ej12.h 
  - no definimos nada
  - ejecutable default: redSocial
- ejercicio 2 con poda:
  - archivos: ej12.cpp y ej12.h
  - definimos: ` -DPODAR`
  - ejecutable default: redSocial_poda
- ejercicio 3 TopDown:
  - archivo actividades_dinamico.cpp
  - definimos: `-DTOPDOWN`
  - ejecutable default: actividades_topDown
- ejercicio 3 BottomUp:
  - archivo actividades_dinamico.cpp
  - definimos: `-DBOTTOMUP`
  - ejecutable default: actividades_bottomUp
- ejercicio 4 :
  - archivo actividades_dinamico.cpp
  - definimos: `-DGOLOSO`
  - ejecutable default: actividades_goloso

