#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <climits>
#include <vector>
using namespace std;
// OPCION CON 03: g++ -no-pie -c -m64 -O3 ej2.cpp -o ej2f.o && g++ -no-pie -O3 -o ./ej2 -m64 ej2f.o
// Compilacion tranqui: g++ -c ej2.cpp && g++ -o ej2.exe ej2.o

int CANT_FILAS; // Se inicializa en el main
int CANT_COLS; // Se inicializa en el main
int etiqDisp = 0;
int compConex = 0;
struct componenteConexa{
    int etiqueta;
    int ultimaFilaEnUso;
    int cantidadUsosEnFila;
};

// Funcion de Debug. Se puede comentar en la entrega
void debugDePobre(vector<componenteConexa*> *K){
    cout<<"[ ";
    for (int i = 0; i < K->size(); ++i){
        if(K->at(i)==nullptr){
            cout<<"0 ";
        }else{
            cout<<K->at(i)->etiqueta<<" ";
        }
    }
    cout<<"]"<<endl;
}

void borroComponentes(vector<componenteConexa*> *v_1, int actual){
    //Cuento la cantidad de veces que aparece el puntero en la fila
    for(int i = 0; i < v_1->size(); i++){
      if(v_1->at(i) != nullptr){
        v_1->at(i)->cantidadUsosEnFila = 0;
      }
    }
    for(int i = 0; i < v_1->size(); i++){
        if(v_1->at(i) != nullptr){
            v_1->at(i)->cantidadUsosEnFila++;
        } 
    }
    //Borro los punteros al llegar a 1 aparicion
    for(int i = 0; i < v_1->size(); i++){
        if(v_1->at(i) != nullptr && v_1->at(i)->ultimaFilaEnUso < actual && v_1->at(i)->cantidadUsosEnFila == 1){
            delete v_1->at(i);
            //cout << "Se borro la memoria de " << punteroABorrar << ", ";
        }else if(v_1->at(i) != nullptr && v_1->at(i)->ultimaFilaEnUso < actual && v_1->at(i)->cantidadUsosEnFila > 1){
            v_1->at(i)->cantidadUsosEnFila--;
            //cout << "Sigue figurando en la fila, espero a la siguiente aparicion " << punteroABorrar << ", ";
        }else if(v_1->at(i) == nullptr){
            //cout << "Era null, ";
        }
    }
    //cout << endl;
}

void asignoComponenteConEtiqueta(vector<componenteConexa*> *v, int indice, int etiqueta, int fila){
    struct componenteConexa *componente;
    componente = (componenteConexa*) malloc(sizeof(struct componenteConexa));
    componente->etiqueta = etiqueta;
    componente->ultimaFilaEnUso = fila;
    componente->cantidadUsosEnFila = 0;
    v->at(indice) = componente;
    //cout << "Agregue una nueva componente en el indice "<< indice << " con la etiqueta: "<<etiqueta<<" guardando la posicion "<< v->at(indice) <<endl;
}

void apuntoAComponenteDeIzq(vector<componenteConexa*> *v, int indice){
    v->at(indice) = v->at(indice-1); 
    //cout << "En la posicion "<<indice<< " asigne la direccion: "<< v->at(indice)<< " que debe concordar con el de la izq, que es: "<< v->at(indice-1) <<endl;
}

void apuntoAComponenteDeArriba(vector<componenteConexa*> *v_2, vector<componenteConexa*> *v_1, int indice, int fila){
    v_2->at(indice) = v_1->at(indice); 
    v_2->at(indice)->ultimaFilaEnUso = fila;
    //cout << "En la posicion "<<indice<< " asigne la direccion: "<< v->at(indice)<< " que debe concordar con el de la izq, que es: "<< v->at(indice-1) <<endl;
}

void cargoPrimeraFila(vector<componenteConexa*> *v_1){
    int valorEnMatriz;
    for (int j = 0; j < CANT_COLS; j++){ 
        cin>>valorEnMatriz;
        if(j>0 && v_1->at(j-1)!=nullptr && valorEnMatriz==1){ 
            // Caso 1 1: Apunto a la misma componente que el de la izquierda
            //cout<< "--Estoy en la primera fila y hay un 1 1. Copio la componente de la izq" << endl;
            apuntoAComponenteDeIzq(v_1, j);
        } else if((j==0 && valorEnMatriz==1) || (j>0 && v_1->at(j-1)==nullptr && valorEnMatriz==1)){ 
            // Caso - 1 y Caso 0 1: Aumento mi etiqueta. Es una nueva componente
            etiqDisp++;
            compConex++; 
            //cout<< "--Estoy en la primera fila y hay un 0 1 o - 1. Creo una nueva" << endl;
            asignoComponenteConEtiqueta(v_1, j, etiqDisp, 0);
            } // Caso 1 0 y Caso 0 0: no hago nada
    }
    /*for(int k = 0; k < CANT_COLS; k++){
        if(v_1->at(k) ==nullptr){
            cout<< v_1->at(k)<< " ";
        }else{
            cout<< v_1->at(k)->etiqueta<< " ";
        }
    }
    cout << endl;
    cout << "+++ Hay "<<compConex<<" componentes conexas en la fila 0"<<endl;*/
}

void cargoRestoFilas(vector<componenteConexa*> *v_1, vector<componenteConexa*> *v_2){
    //*** Cargo el resto de filas de la Matriz
    for (int i = 1; i < CANT_FILAS; i++){
        int valorEnMatriz;
        for (int j = 0; j < CANT_COLS; j++){
            cin>>valorEnMatriz;
            //cout << "++leyo un "<< valorEnMatriz<<endl;
            if(valorEnMatriz==1){
                if(v_1->at(j)==nullptr){ 
                    //* Caso 0 arriba
                    if (j==0 || (j>0 && v_2->at(j-1)==nullptr)){ 
                        // Caso - 1 o Caso 0 1: Aumento mi etiqueta. Es una nueva componente
                        etiqDisp++;
                        compConex++; 
                        //cout<< "--Estoy en la " << (i+1) << " fila y hay un 0 1 o - 1. Creo una nueva" << endl;
                        asignoComponenteConEtiqueta(v_2, j, etiqDisp, i);
                    } else { 
                        // Caso 1 1: Apunto a la misma componente que el de la izquierda
                        //cout<< "--Estoy en la " << (i+1) << " fila y hay un 1 1. Copio la de la izquierda" << endl;
                        apuntoAComponenteDeIzq(v_2, j);
                    }
                } else { 
                    //* Caso 1 arriba 
                    if (j>0 && v_2->at(j-1)!=nullptr){ 
                        // Caso 1 1: Son todas la misma componente que arriba
                        //cout<< "--Estoy en la " << (i+1) << " fila y hay 1 1 y un 1 arriba. Copio la de la izquierda "<< endl;
                        apuntoAComponenteDeIzq(v_2, j);
                        //cout << "--Comparo etiqueta izq: "<<v_2->at(j-1)->etiqueta<<" con arriba: "<<etiquetaArriba<<endl;
                        int etiquetaArriba = v_1->at(j)->etiqueta;
                        if(v_2->at(j-1)->etiqueta > etiquetaArriba){
                            //Si la etiqueta de la izquierda es mas grande que la de arriba, descuento una componente y pongo la misma etiqueta de arriba
                            compConex--;
                            //cout<< "--Tenian distinta etiqueta, se la asigno a la de la izquierda" << endl;
                            v_2->at(j)->etiqueta = etiquetaArriba;
                        } else if(v_2->at(j-1)->etiqueta < etiquetaArriba){
                            //Si la etiqueta de la izquierda es mas chica que la de arriba, descuento una componente y pongo la misma etiqueta de arriba
                            compConex--;
                            //cout<< "--Tenian distinta etiqueta, se la asigno a la de la izquierda" << endl;
                            int etiquetaIzq = v_2->at(j)->etiqueta;
                            v_1->at(j)->etiqueta = etiquetaIzq;
                        }
                    } else if (j==0 || j>0 && v_2->at(j-1)==nullptr){
                        // Caso 0 1 o - 1: Ya tomo la componente de arriba
                        // Creo una componente conexa nueva y le copio la etiqueta del de arriba (para no depender de CC de filas anteriores)
                        //cout<< "--Estoy en la " << (i+1) << " fila y hay un 1 arriba y 0 1 o - 1. Creo una con esa etiqueta" << endl;
                        apuntoAComponenteDeArriba(v_2, v_1, j, i);
                    }
                }   
            }else{
                v_2->at(j) = nullptr;
            }
            //cout<<"--VALOR EN V_2: "<<v_2->at(j)<<endl;
        }

        //**Borro las componentes de la fila a borrar
        borroComponentes(v_1, i);

        //** El vector de abajo pasa a ser el de arriba
        for(int k = 0; k < CANT_COLS; k++){
            v_1->at(k) = v_2->at(k);
            //v_1->at(k)->cantidadUsosEnFila = 0;
            /*if(v_2->at(k) ==nullptr){
                cout<< v_2->at(k)<< " ";
            }else{
                cout<< v_2->at(k)->etiqueta<< " ";
            }*/
        }
        //cout << endl;
        
        //cout << "+++ Hay "<<compConex<<" componentes conexas en la fila "<<i<<"."<<endl;
    }
}

int main(int argc, char *argv[]){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    //*** Leo la primera fila
    cin >> CANT_FILAS >> CANT_COLS;
    
    //*** Caso base
    if(CANT_FILAS == 0 || CANT_COLS == 0){cout<<0<<endl; return 0;}

    //*** Creo mis dos vectores
    vector<componenteConexa*> v_1(CANT_COLS, nullptr);
    vector<componenteConexa*> v_2(CANT_COLS, nullptr);
    etiqDisp = 0;
    compConex = 0;

    cargoPrimeraFila(&v_1);
    cargoRestoFilas(&v_1, &v_2);
    borroComponentes(&v_1, CANT_FILAS);
    
    cout<<compConex<<endl;
    return 0;
}
