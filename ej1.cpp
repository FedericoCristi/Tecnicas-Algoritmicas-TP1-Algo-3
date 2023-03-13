#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <climits>
#include <vector>
#include <list>

using namespace std;
int CANT_NODOS; // Se inicializa en el main
int CANT_ARISTAS; // Se inicializa en el main
vector<vector<int>> V;
vector<vector<int>> predecesor;
int INDEF = -1; 

/*
BFS está modificado de tal modo que mantiene las minimas distancias de la raiz dada a cada vertice.
Eso se usa para cuando por BFS se visita un nodo ya visitado, se pueda comparar la distancia de el 
camino ya establecido y la distancia del nuevo camino por el cual estamos llegando. Si ambas distancias son iguales,
entonces existen 2 caminos minimos a el mismo vertice desde el origen, por lo tanto no es un grafo geodesico
*/

void imprimoVector(vector<int> *A){
  for(int i = 0; i < A->size(); i++){
    cout << A->at(i) << " ";
  }
  cout << endl;
}

void imprimoCaminos(int origen, int jYaVisitado, int nuevoPredecesor){
  //Armo camino A con: jYaVisitado + predecesores de jYaVisitado
  //Armo camino B con: jYaVisitado + nuevoPredecesor + predecesores de nuevoPredecesor
  //Busco el nodo donde coinciden los caminos
  vector<int> A;
  A.push_back(jYaVisitado); 
  A.push_back(predecesor[origen][jYaVisitado]);

  vector<int> B;
  B.push_back(jYaVisitado);
  B.push_back(nuevoPredecesor);
  bool encontre = A.back()==B.back();
  while (!encontre){
    A.push_back(predecesor[origen][A.back()]);
    B.push_back(predecesor[origen][B.back()]);
    encontre = A.back()==B.back();
  }
  
  imprimoVector(&A);
  imprimoVector(&B);
}

bool bfs(int origen) {
  
  //predecesor[origen] = vector<int>(CANT_NODOS, INDEF); // Acá explota ... 

  list<int> queue;
  vector<int> distancias (CANT_NODOS, INT_MAX);
  bool esGeodesico = true;
  
  predecesor[origen][origen] = origen;
  distancias[origen] = 0;
  queue.push_back(origen);

  while (!queue.empty()) {
    int NodoActual = queue.front();
    //cout << "Visitado " << NodoActual << endl;
    queue.pop_front();

    //Recorro los adyacentes del nodoActual: i
    for (auto i: V[NodoActual]){
        if (predecesor[origen][i] == INDEF) {
            //Si no lo visite, la actual es la distancia mas corta
            predecesor[origen][i] = NodoActual;
            distancias[i] = distancias[NodoActual] + 1;
            queue.push_back(i);
        }else{
            //Si ya lo visite, chequeo si encontre otra distancia minima
            if(distancias[i] == distancias[NodoActual]+1){
                esGeodesico = false;
                cout << "0" << endl;
                imprimoCaminos(origen, i, NodoActual);
                return esGeodesico;
            }
        }
    }
  }
  return esGeodesico;
}



int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    //*** Leo la primera fila
    cin >> CANT_NODOS >> CANT_ARISTAS;
    
    //*** Caso base
    if(CANT_NODOS == 0 && CANT_ARISTAS == 0){cout<<"NO HAY GRAFO"<<endl; return 0;}
    if(CANT_NODOS == 0 && CANT_ARISTAS != 0){cout<<"GRAFO NO VALIDO"<<endl; return 0;}
    if(CANT_NODOS != 0 && CANT_ARISTAS == 0){cout<<"GRAFO NO CONEXO"<<endl; return 0;}
    vector<vector<int> > V_temp(CANT_NODOS, vector<int>(0)); // Variable temporal
    // Si no la uso da Segmentation Fault

    //*** Cargo el grafo
    int a, b;
    for(int i = 0; i<CANT_ARISTAS; i++){
      cin >> a >> b;
      V_temp[a].push_back(b);
      V_temp[b].push_back(a);
    }
    V = V_temp; // Paso data al global

    // Hago el traslado temporal. Si lo hago en el BFS da Segmentation Fault
    vector<vector<int> > pred_temp (CANT_NODOS, vector<int>(CANT_NODOS, INDEF));
    predecesor = pred_temp;

    for(int i = 0; i<CANT_NODOS; i++){
      //cout<<"Iteracion numero "<<i<<endl;
      if(!bfs(i)){
        //Si no es geodesico, devolver 0 seguido de vertices de dos caminos minimos de i a j
        //cout<< "No es geodesico, demostrado por minimos caminos del BFS con raiz " << i<< endl;
        //return 1;
        return 0;
      }
    }
    //Si es geodesico, devolver 1 seguido de n lineas: en la i-esima, pos j tiene el predecesor del j-esimo en el camino de i a j (i=j tiene i)
    cout << "1" << endl;
    for(int i = 0; i<predecesor.size(); i++){
      imprimoVector(&predecesor[i]);
    }
    return 0;
}
