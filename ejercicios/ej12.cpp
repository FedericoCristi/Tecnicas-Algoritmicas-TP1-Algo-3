#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector> // Usado
#include <limits> // menos infinito
#include <string> 
#include <algorithm>
#include <list>
#include "ej12.h"

using namespace std;

// OPCION CON 03 Ej1: g++ -no-pie -c -m64 -O3 ej12.cpp -o ej1f.o && g++ -no-pie -O3 -o ./ej1 -m64 ej1f.o
// OPCION CON 03 Ej2: g++ -no-pie -c -DPODAR -m64 -O3 ej12.cpp -o ej2f.o && g++ -no-pie -O3 -o ./ej2 -m64 ej2f.o
// Compilar los 2:
// g++ -no-pie -c -m64 -O3 ej12.cpp -o ej1f.o && g++ -no-pie -O3 -o ./ej1 -m64 ej1f.o && g++ -no-pie -c -DPODAR -m64 -O3 ej12.cpp -o ej2f.o && g++ -no-pie -O3 -o ./ej2 -m64 ej2f.o
// No hace falta agregar los .h

vector<pair<int, int>> actoresInfluencia;
vector<pair<int, int>> amistades;




int main(int argc, char *argv[]){
    ios::sync_with_stdio(false);
    cin.tie(0);

    // 1.0 Comienzo a cargar el archivo
    //cout<<"Cargando archivo...\n";
    
    // 1.1 Leo la primera fila
    char id; string edge;
    cin >> id >> edge >> CANT_ACTORES >> CANT_AMISTADES;
    //*** DEBUG DE ENTRADA = cout<<id<<" "<<edge<<" "<<CANT_ACTORES<<" "<<CANT_AMISTADES<<endl;


    // 1.2
    // Se resta cuando el numero representa un actor. De esta manera accedemos mejor a los vectores.
    // 1.2.1 Cargo los actores
    std::pair<int, int> vect;
    for (int j = 0; j < CANT_ACTORES; j++){
        cin >> id >> vect.first >> vect.second;

        //*** DEBUG DE ENTRADA = cout<<id<<" "<<vect.first<<" "<<vect.second<<endl;

        vect.first--;
        actoresInfluencia.push_back(vect);

    }

    // 1.2.2  Cargo las amistades
    for (int j = 0; j < CANT_AMISTADES; j++){
        cin >> id >> vect.first >> vect.second;

        //*** DEBUG DE ENTRADA = cout<<id<<" "<<vect.first<<" "<<vect.second<<endl;

        vect.first--;
        vect.second--;
        amistades.push_back(vect);
    }

    // 2.0 Funcion que inicializa la MATRIZ donde guardamos los amigos de cada vector y
    // el vector donde se guarda la cantidad de amigos de un usuario
    vector<vector<bool>> sonAmigos2 (CANT_ACTORES, vector<bool> (CANT_ACTORES, false));
    vector<int> cantVecinos2 (CANT_ACTORES,0);
    for(int i = 0; i<amistades.size(); i++){
        sonAmigos2[(amistades[i]).first][(amistades[i]).second] = true;
        sonAmigos2[(amistades[i]).second][(amistades[i]).first] = true;
        cantVecinos2[(amistades[i]).first]++;
        cantVecinos2[(amistades[i]).second]++;
    }
    sonAmigos = sonAmigos2;
    cantVecinos = cantVecinos2;

    // 3.0 Ejecuto lo pedido

    // Inicializo, armo el vector de actores (K) y armo el vector de influencias
    vector<int> Q;
    vector<int> K(CANT_ACTORES,0);
    for (int j = 0; j < CANT_ACTORES; ++j){
        K[j] = j;
        influencia.push_back((actoresInfluencia[j]).second);
    }
    //Ordeno los actores en orden segun su influencia

        // Orden Menor a Mayor
    sort(K.begin(), K.end(), mayorActorSegunInfluencia);
    
        // Orden Mayor a Menor
    //sort(K.begin(), K.end(), menorActorSegunInfluencia);

    //for (int i = 0; i < K.size(); ++i){cout<<K[i]<<",";} // DEBUG


    agregoAQLosQueSonAmigosDeTodosEnK(&Q,&K); // COLO: Para mantener el invariante
    sumaMax = sumaPrioridades(&Q);

    int infMax;
    // Ejercicio 2
    #ifdef PODAR
    //std::cout<<"Ejecutando el Ejercicio 2"<<std::endl;
    infMax = cliqueMasInfluyenteConPoda(&Q,&K);
    //cout<<"La influencia maxima es: "<< infMax <<endl;
    cout<< infMax <<endl;
    //cout<<"El grupo de influencia maxima fue: ";
    imprimirActores();
    //SALIDAS DE DEBUG
    cout<<" "<< endl;
    //cout<<"SALIDAS DE DEBUG"<< endl;
    //cout<<"#Nodos recorridos= "<<nodosRecorridosDB<<endl;
    return 0;
    #endif

    // Ejercicio 1
    //cout<<"Ejecutando el Ejercicio 1"<<std::endl;
    infMax = cliqueMasInfluyente(&Q,&K);
    //cout<<"La influencia maxima es: "<< infMax <<std::endl;
    cout<< infMax <<std::endl;
    //cout<<"El grupo de influencia maxima fue: ";
    imprimirActores();
    //SALIDAS DE DEBUG
    cout<<" "<< endl;
    //cout<<"SALIDAS DE DEBUG"<< endl;
    //cout<<"#Nodos recorridos= "<<nodosRecorridosDB<<endl;
    return 0;
}
