#include <vector>
#include <iostream>
#include <tuple>
#include <climits>
#include <queue>
using namespace std;

vector<vector<int>> V, V_conNodo;
vector<vector<int>> W, W_limpiado;
vector<vector<int>> dist_BF, dist_D;
vector<int> ciclo_negativo;
vector<int> pred_BF;
int vertices, aristas;
int vertices_conNodo, aristas_conNodo;
int origenD;

void printMatriz(vector<vector<int>> *matriz){
    for(int i = 0; i < matriz->size(); i++){
        for(int j = 0; j< matriz->at(i).size(); j++){
            cout << (matriz->at(i)).at(j)<< " ";
        }
        cout << endl;
    }
}

void printVector(vector<int> *vector){
    for(int i = 0; i < vector->size(); i++){
        cout << vector->at(i)<< " ";
    }
    cout << endl;
}

vector<int> crearSegmento(int p){
    int i = 0;
    vector<int> ciclo_fin;
    //Ignoro los vertices recorridos hasta encontrar el primer ciclo negativo
    while(ciclo_negativo[i] != p){
        i++;
    }

    for(i; i<ciclo_negativo.size(); i++){
        ciclo_fin.push_back(ciclo_negativo[i]);
    }

    return ciclo_fin;
}

bool incluido(int n){
    for(int i = 0; i < ciclo_negativo.size(); i++){
        if(ciclo_negativo[i] == n){
            return true;
        }
    }
    return false;
}

void ArmarCiclo(int j){
    ciclo_negativo = vector<int>();
    ciclo_negativo.push_back(j);
    int actual = pred_BF[j];
    while(!incluido(actual)){
        ciclo_negativo.push_back(actual);
        actual = pred_BF[actual];
    }
    ciclo_negativo = crearSegmento(actual);
    ciclo_negativo.push_back(actual);
}

bool bellmanFord(int origen){
    //INICIALIZO LAS DISTANCIAS
    dist_BF = vector(vertices_conNodo, vector<int>(vertices_conNodo, INT_MAX));
    pred_BF = vector(vertices_conNodo, -1);
    //Al origen vale 0
    dist_BF[origen][origen] = 0;
    pred_BF[origen] = origen; 
    
    //ASIGNO LAS DISTANCIAS DEL VERTICE DADO HASTA EL RESTO
    int nodoActual;
    //Repito la busqueda |V|-1 veces
    for(int i = 0; i<V_conNodo.size()-1; i++){
        //Itero en los vertices
        for(int j = 0; j<V_conNodo.size(); j++){
            //Itero en sus adyacentes
            for(int k = 0; k<V_conNodo[j].size(); k++){
                nodoActual = V_conNodo[j][k];
                if(dist_BF[origen][j]!=INT_MAX && W[j][nodoActual]!=INT_MAX && dist_BF[origen][nodoActual] > dist_BF[origen][j] + W[j][nodoActual]){
                    dist_BF[origen][nodoActual] = dist_BF[origen][j] + W[j][nodoActual];
                    pred_BF[nodoActual] = j;
                }
            }
        }
    }

    //BUSCO CICLOS NEGATIVOS
    //Itero en los vertices
    for(int j = 0; j<V_conNodo.size(); j++){
        //Itero en sus adyacentes
        for(int k = 0; k<V_conNodo[j].size(); k++){
            nodoActual = V_conNodo[j][k];
            //Chequeo si se podria achicar aun mas la distancia a un nodo
            if(dist_BF[origen][j]!=INT_MAX && W[j][nodoActual]!=INT_MAX && dist_BF[origen][nodoActual] > dist_BF[origen][j]+ W[j][nodoActual]){
                //Significa que hay un ciclo negativo
                //Le asigno falso al predicado "no hay ciclos negativos"
                //Me armo un ciclo negativo
                dist_BF[origen][nodoActual] = dist_BF[origen][j] + W[j][nodoActual];
                pred_BF[nodoActual] = j;
                ArmarCiclo(nodoActual);
                return false;
            }
        }
    }
    //Si no encontro ningun ciclo, devuelvo que no hay (o sea, true)
    return true;
}

void devuelvoCicloLongitudNegativa(){
    for(int i = 0; i < ciclo_negativo.size(); i++){
        cout << ciclo_negativo[i] << " ";
    }
    cout << endl;
}

bool tieneDistanciaMenor(int a, int b){
    return dist_D[origenD][a] < dist_D[origenD][b];
}

class BinaryHeap {
private:
    const static int MAX_SIZE = 99999;
    int heap[MAX_SIZE];
    int size;

public:
    BinaryHeap() {
        size = 0;
    }

    // returns the index of the parent node
    static int parent(int i) {
        return (i - 1) / 2;
    }

    // return the index of the left child 
    static int leftChild(int i) {
        return 2*i + 1;
    }

    // return the index of the right child 
    static int rightChild(int i) {
        return 2*i + 2;
    }

    static void swap(int *x, int *y) {
        int temp = *x;
        *x = *y;
        *y = temp;
    }

    // insert the item at the appropriate position
    void insert(int data) {
        if (size >= MAX_SIZE) {
            cout<<"The heap is full. Cannot insert"<<endl;
            return;
        }

        // first insert the time at the last position of the array 
        // and move it up
        heap[size] = data;
        size = size + 1;

        // move up until the heap property satisfies
        int i = size - 1;
        while (i != 0 && tieneDistanciaMenor(heap[i], heap[BinaryHeap::parent(i)])) {
            BinaryHeap::swap(&heap[BinaryHeap::parent(i)], &heap[i]);
            i = BinaryHeap::parent(i);
        }
    }

    // moves the item at position i of array a
    // into its appropriate position
    void minHeapify(int i){
        // find left child node
        int left = BinaryHeap::leftChild(i);

        // find right child node
        int right = BinaryHeap::rightChild(i);

        // find the smallest among 3 nodes
        int smallest = i;

        // check if the left node is smaller than the current node
        if (left <= size && tieneDistanciaMenor(heap[left], heap[smallest])) {
            smallest = left;
        }

        // check if the right node is smaller than the current node 
        // and left node
        if (right <= size && tieneDistanciaMenor(heap[right], heap[smallest])) {
            smallest = right;
        }

        // swap the smallest node with the current node 
        // and repeat this process until the current node is smaller than 
        // the right and the left node
        if (smallest != i) {
            int temp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = temp;
            minHeapify(smallest);
        }

    }

    // returns the  minimum item of the heap
    int getMin() {
        return heap[0];
    }

    // deletes the min item and return
    int extractMin() {
        int minItem = heap[0];

        // replace the first item with the last item
        heap[0] = heap[size - 1];
        size = size - 1;

        // maintain the heap property by heapifying the 
        // first item
        minHeapify(0);
        return minItem;
    }

    // prints the heap
    void printHeap() {
        for (int i = 0; i < size; i++) {
            cout<<heap[i];
        }
        cout<<endl;
    }

    // returns the size of the heap
    int getSize(){
        return size;
    }
};

void dijkstra(int origen){
    //Corro DIJKSTRA en el grafo sin el vertice agregado, con los pesos limpiados, desde el nodo i
    //Inicializo el predecesor y si ya vi a cada nodo
    vector<int> padre(V.size(), -1);
    vector<bool> ya_lo_vi(V.size(), false);
    //Inicializo el caso del nodo origen
    dist_D[origen][origen] = 0;
    //Inicializo cola y le agrego el nodo origen
    origenD = origen;
    BinaryHeap min_heap;
    min_heap.insert(origen);
    //Mientras que la cola no este vacia
    int nodoActual;
    int adyacente;
    while(min_heap.getSize() != 0){
        //Tomo un nodo y lo marco como visto
        nodoActual = min_heap.extractMin();
        ya_lo_vi[nodoActual] = true;
        //Recorro los adyacentes al nodo actual
        for(int i = 0; i < V[nodoActual].size(); i++){
            adyacente = V[nodoActual][i];
            //Si no lo vi...
            if(!ya_lo_vi[adyacente]){
                //Si pasando por el nodo actual, la distancia desde el origen al nodo adyacente es menor...
                if(dist_D[origen][nodoActual]!=INT_MAX && W_limpiado[nodoActual][adyacente]!=INT_MAX && dist_D[origen][adyacente] > dist_D[origen][nodoActual] + W_limpiado[nodoActual][adyacente]){
                    //Tomo dicha distancia
                    dist_D[origen][adyacente] = dist_D[origen][nodoActual] + W_limpiado[nodoActual][adyacente];
                    //Actualizo al nodo actual como padre
                    padre[adyacente] = nodoActual;
                    //Agrego al adyacente en la cola para revisar sus adyacentes
                    min_heap.insert(adyacente);
                }
            }
        }
    }
}

void johnson(){
    //AGREGO A G UN VERTICE NUEVO QUE SE CONECTE CON TODOS
    aristas_conNodo = aristas + vertices;
    vertices_conNodo = vertices + 1;
    //Lo agrego a la lista de adyacencias
    V_conNodo = V;
    V_conNodo.push_back(vector<int>());
    //Agrego en matriz W una nueva fila de pesos 0 que corresponde
    //a todas las aristas desde este nuevo vertice al resto (no incluyo la de si mismo)
    W.push_back(vector<int>(vertices_conNodo-1, 0));
    int origen = V_conNodo.size()-1;
    for(int i = 0; i<vertices; i++){
        //Agrego arista origen->i
        V_conNodo[origen].push_back(i);
        //Agrego a la matriz de pesos la posicion W[i][origen] con INT_MAX
        W[i].push_back(INT_MAX);
    }
    //Agrego el peso del nuevo nodo a si mismo como infinito
    W[origen].push_back(INT_MAX);

    //Corro BELLMAN-FORD para descubrir si tiene ciclos negativos
    if(! bellmanFord(origen)){
        //Si encontro ciclos negativos, hago que devuelva 0
        cout << "0" << endl;
        devuelvoCicloLongitudNegativa();
        return;
    }else{
        //No encontro ciclos negativos, hago que devuelva 1
        cout << "1" << endl;

        //Me guardo distancias al vertice nuevo
        vector<int> h(vertices_conNodo, -1);
        for(int v = 0; v<vertices_conNodo; v++){
            h[v] = dist_BF[origen][v];
        }
        
        //Me guardo pesos restando las distancias de la cabeza y sumando al de la punta
        W_limpiado = vector(vertices_conNodo, vector<int>(vertices_conNodo, INT_MAX));
        //Itero en los vertices
        for(int j = 0; j<W.size(); j++){
            //Itero en sus adyacentes
            for(int k = 0; k<W[j].size(); k++){
                if(W[j][k] != INT_MAX){
                    W_limpiado[j][k] = W[j][k] + h[j] - h[k];
                }else{
                    W_limpiado[j][k] = W[j][k];
                }
            }
        }

        //Creo matriz de tamaño n x n (sin incluir el nodo agregado)
        vector<vector<int>> D(vertices, vector<int>(vertices, INT_MAX));
    
        //Itero en los vertices sin incluir al agregado
        //Inicializo la matriz de distancias de dijkstra
        dist_D = vector(vertices, vector<int>(vertices, INT_MAX));
        for(int i = 0; i<vertices; i++){
            //Corro DIJKSTRA en el grafo sin el vertice agregado, con los pesos limpiados, desde el nodo i
            dijkstra(i);
            //Me guardo la distancia limpiada segun dijkstra
            for(int j = 0; j<vertices; j++){
                //Imprimo los valores por consola
                if(dist_D[i][j] != INT_MAX){
                    D[i][j] = dist_D[i][j] - h[i] + h[j];
                    cout << D[i][j] << " ";
                }else{
                    cout << "INF ";
                }
            }
            cout << endl;
        }
        return;
    }
}

int main(int argc, char const *argv[]){
    int a, b, w;
    //Obtengo las cantidades de vertices y aristas
    cin >> vertices >> aristas;
    //Inicializo la lista de adyacencias
    V = vector(vertices, vector<int>());
    //Inicializo pesos w
    W = vector(vertices, vector<int>(vertices, INT_MAX));
    for(int i = 0; i<aristas; i++){
        cin >> a >> b >> w;
        //Agrego la arista a V
        V.at(a).push_back(b);
        //Agrego el peso en w
        W.at(a).at(b) = w;
    }
    johnson();
    return 0;
}
