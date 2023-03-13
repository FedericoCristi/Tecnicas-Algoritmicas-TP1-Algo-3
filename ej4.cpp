//
// Created by Pablo on 18/05/2022.
//
#include <iostream>
#include <queue>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;

enum tipo { IN, OUT };

struct nodo {
  int index;
  tipo nodeType; // 0 = in , 1 = out
  int a;
  int b;
};

bool porInicio(nodo a, nodo b) { return a.a < b.a; }

vector<nodo> intervalos{};
int max_index = -1;
bool unicoContenido = false;
int contendor = -1;

void lecturaDeArch() {
  // Comienzo a cargar el archivo
  // Leo la primera fila
  int CANT_INTERVALOS;
  cin >> CANT_INTERVALOS; // Esta seria la variable global que corresponde a
                          // #INTERVALOS

  // Leo los intervalos
  int a, b;

  intervalos.push_back({-1, IN, -2, -1});
  for (int j = 0; j < CANT_INTERVALOS; j++) {
    cin >> a >> b; // {T_inicial , T_final}
    intervalos.push_back({j, IN, a, b});
    intervalos.push_back({j, OUT, a, b});
  }
  intervalos.push_back(
      {CANT_INTERVALOS, OUT, CANT_INTERVALOS * 2 + 1, CANT_INTERVALOS * 2 + 2});

  //sort(intervalos.begin(), intervalos.end(), porInicio);
}

vector<vector<pair<int, int>>> matriz;

void agrego_aristas() {
  int limiteParaC;
  for (int k = 0; k < intervalos.size(); ++k) {
    matriz.push_back(vector<pair<int, int>>{});
    nodo i = intervalos[k];
    limiteParaC = (int)intervalos.size() * 2 + 3;
    for (int h = 0; h < intervalos.size(); ++h) {
      nodo j = intervalos[h];

      // aristas de in -> out del mismo nodo
      if (i.nodeType == IN and j.nodeType == OUT and i.index == j.index) {
        matriz[k].push_back(make_pair(h, 0));
        continue;
      }

      // arista para B
      if (i.nodeType == OUT and j.nodeType == IN and i.a < j.a and j.a < i.b and
          i.b < j.b) {
        matriz[k].push_back(make_pair(h, 1));
      }

      // aristas para C
      if (i.nodeType == IN and j.nodeType == OUT and i.b < j.a) {
        if (j.a < limiteParaC) {
          matriz[k].push_back(
              make_pair(h, j.index == max_index or i.index == -1 ? 0 : 1));
        }
        limiteParaC = min(limiteParaC, j.b);
      }
    }
  }
}

vector<int> dist_D;
vector<int> padre;

void dijkstra(int origen) {
  // Inicializo las distancias al origen, el predecesor y si ya vi a cada nodo
  dist_D = vector<int>(matriz.size(), INT_MAX);
  padre = vector<int>(matriz.size(), -1);
  vector<bool> ya_lo_vi(matriz.size(), false);
  // Inicializo el caso del nodo origen
  dist_D[origen] = 0;
  // Inicializo cola y le agrego el nodo origen
  vector<int> colaNodos;
  colaNodos.push_back(origen);
  // Mientras que la cola no este vacia
  int nodoActual;
  int adyacente;
  //cout << "dijkstra 1"<< endl;

  while (!colaNodos.empty()) {
    // Tomo un nodo y lo marco como visto
    int min_index = 0;
    for (int i = 0; i < colaNodos.size(); ++i) {
      if (dist_D[colaNodos[i]] < dist_D[colaNodos[min_index]]){
        min_index = i;
      }
    }
    swap(colaNodos[min_index], colaNodos[colaNodos.size()-1]);
    nodoActual = colaNodos.back();
    colaNodos.pop_back();
    ya_lo_vi[nodoActual] = true;
    // Recorro los adyacentes al nodo actual
    for (int i = 0; i < matriz[nodoActual].size(); i++) {
      adyacente = matriz[nodoActual][i].first;
      // Si no lo vi...
      if (!ya_lo_vi[adyacente]) {
        // Si pasando por el nodo actual, la distancia desde el origen al nodo
        // adyacente es menor...
        if (dist_D[nodoActual] != INT_MAX and
            matriz[nodoActual][i].second != INT_MAX and
            dist_D[adyacente] >
                dist_D[nodoActual] + matriz[nodoActual][i].second) {
          // Tomo dicha distancia
          dist_D[adyacente] =
              dist_D[nodoActual] + matriz[nodoActual][i].second;
          // Actualizo al nodo actual como padre
          padre[adyacente] = nodoActual;
          // Agrego al adyacente en la cola para revisar sus adyacentes
          colaNodos.push_back(adyacente);
        }
      }
    }
  }
}

void escribirResultado() {
  vector<int> resultado;
  int pre = padre[intervalos.size() - 1];
  if (!unicoContenido){
    while (pre != 0) {
      if (resultado.size() == 0 or intervalos[pre].index !=  resultado[resultado.size()-1]){
        resultado.push_back(intervalos[pre].index);
      }
      pre = padre[pre];
    }
    reverse(resultado.begin(), resultado.end());
  }else {
    resultado.push_back(contendor);
      if (resultado[0] == max_index){
        resultado.push_back(0);
      }else{
        resultado.push_back(max_index-1);
      }
  }


  cout << resultado.size() << endl;
  for (auto r : resultado) {
    cout << r << " ";
  }
  cout << endl;
}




void sacoContenidos() {
  vector<bool> contenidos(max_index, false);
  for (auto &i : intervalos) {
    bool estaContenido = false;
    for (auto &tmp : intervalos) {
      // i.index != tmp.index and
      if (tmp.a < i.a and i.b < tmp.b and i.index != -1 and
          i.index != max_index) {
        contenidos[i.index] = true;
        break;
      }
    }
  }
  int a = 0;
  for (int k = 0; k < contenidos.size(); ++k) {
    if (!contenidos[k]) contendor = k;
    a += contenidos[k];
  }
  unicoContenido = (max_index-a) == 1;


  for (int j = 0; j < matriz.size(); ++j) {
    if (intervalos[j].index != -1 and intervalos[j].index != max_index and
        contenidos[intervalos[j].index]) {
      matriz[j] = vector<pair<int, int>>();
      continue;
    }
    vector<pair<int, int>> tmp=  vector<pair<int, int>>();
    for (int i = 0; i < matriz[j].size(); ++i) {
      if ((intervalos[matriz[j][i].first].index == -1 or
          intervalos[matriz[j][i].first].index == max_index) or
          !contenidos[intervalos[matriz[j][i].first].index]) {
        tmp.push_back(matriz[j][i]);
      }
    }
    matriz[j] = vector<pair<int,int>>();
    matriz[j] = tmp;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie();
  lecturaDeArch();
  max_index = (int)intervalos.size() / 2 - 1;
  agrego_aristas();
  sacoContenidos();
  dijkstra(0);
  escribirResultado();
  return 0;
}
