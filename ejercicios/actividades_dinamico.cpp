
#include <iostream>
#include <vector>
using namespace std;
int INDEF = -10;
vector<int> tInicial;
vector<int> tFinal;
vector<int> beneficio;
int CANT_ACTIV;
//vector<int> A; //Se supone que esta ordenado en orden de comienzo de actividad
vector<int> maxBenef_vector;


vector<int> siguienteActividad;

void construyeEstructurasAux() {
    siguienteActividad = vector(CANT_ACTIV * 2 + 2, INDEF);
    //pongo el index de la primera actividad que empieza en cada hora
    for (int i = 0; i < CANT_ACTIV; ++i) {
        if (siguienteActividad[tInicial[i]] == INDEF) {
            siguienteActividad[tInicial[i]] = i;
        }
    }
    //arrastro el index para completar los espacio en blanco
    int ultimo = CANT_ACTIV;
    for (int i = (int) siguienteActividad.size() - 1; i >= 0; --i) {
        if (siguienteActividad[i] == INDEF) {
            siguienteActividad[i] = ultimo;
        } else {
            ultimo = siguienteActividad[i];
        }
    }
}

int sigCompatible(int actActual) {
    //Usar el codigo de pablo para armar un nuevo vector o simplemente que tome el valor de la siguiente por hora
    //cout << actActual << "|"<< tFinal[actActual] + 1 << "|" << siguienteActividad[tFinal[actActual] + 1]<< endl;
    return siguienteActividad[tFinal[actActual] + 1];
}

//Funcion que busca maximixar el beneficio al elegir actividades compatibles
//busca hacerlo evaluando desde la actividad actActual (indice que va del 0 al |A|-1)
//se guarda desde que tiempo que tiene disponible (arranca en 0, al agregar uno, pasa a tFinal(i)+1 para evitar superposicion)
int maxBenef(int actActual) {
    if (actActual == CANT_ACTIV) {
        //Caso base: ya no hay mas actividades y/o tiempo, devuelvo 0 (el neutro de la suma)
        return 0;
    } else {
        //cout<<actActual<<endl;
        if (maxBenef_vector.at(actActual) == -1) {
            //Caso recursivo: evaluo que pasa con la actividad i-esima
            //La actividad es compatible, entonces tomo el maximo beneficio entre tenerla en cuenta o no hacerlo, y evaluar desde la siguiente
            maxBenef_vector.at(actActual) = max(maxBenef(actActual+1), maxBenef(sigCompatible(actActual)) + beneficio[actActual]);
        }
        return maxBenef_vector.at(actActual);
    }
}

int maxBenefCompatibles_bottomup() {
    for (int i = CANT_ACTIV - 1; i >= 0; i--) {
        if (i == CANT_ACTIV - 1) {
            //Relleno la tabla en el caso de la ultima actividad
            maxBenef_vector.at(i) = beneficio[CANT_ACTIV - 1];
        } else {
            //Relleno el resto
            maxBenef_vector.at(i) = max(maxBenef(i + 1), maxBenef(sigCompatible(i)) + beneficio[i]);
        }
    }
    return maxBenef_vector[0];
}

vector<int> reconstruyendoVectorSolucion() {
    vector<int> solucion;
    int maxBeneficio = maxBenef_vector.at(0);
    for (int i = 0; i < CANT_ACTIV - 1; i++) {
        //Si al evaluar en una actividad mas, el maximo beneficio no es el mismo...
        if (maxBenef_vector.at(i) != maxBenef_vector.at(i + 1)) {
            //Entonces esa nueva actividad fue tomada en cuenta
            solucion.push_back(i);
            //Le resto el beneficio de la actividad actual a la suma obtenida
            maxBeneficio -= beneficio[i];
            //Chequeo desde la siguiente actividad compatible
            i = sigCompatible(i);
        }
    }
    //Si en la suma quedaba la ultima actividad, la agrego
    if (maxBeneficio == beneficio[CANT_ACTIV - 1]) {
        solucion.push_back(CANT_ACTIV - 1);
    }
    return solucion;
}

void lecturaDeArch() {
    // Comienzo a cargar el archivo
    // Leo la primera fila
    cin >> CANT_ACTIV;// Esta seria la variable global que corresponde a #Actividades
#ifdef GOLOSO
    vector<int> vecte(2, 0);
    for (int j = 0; j < CANT_ACTIV; j++) {
        cin >> vecte[0] >> vecte[1];// {T_inicial , T_final , Beneficio}
        tInicial.push_back(vecte[0]);
        tFinal.push_back(vecte[1]);
    }
    return;
#endif

    // Leo las actividades
    vector<int> vect = vector(3, 0);

    for (int j = 0; j < CANT_ACTIV; j++) {
        cin >> vect[0] >> vect[1] >> vect[2];// {T_inicial , T_final , Beneficio}
        tInicial.push_back(vect[0]);
        tFinal.push_back(vect[1]);
        beneficio.push_back(vect[2]);
    }
}
vector<int> solucion;
void maximo_goloso() {
    vector<int> actividadesPorFinal;
    //ordenamos las activides por tiempo final
    vector<vector<int>> tmp = vector(CANT_ACTIV * 2 + 1, vector<int>());
    for (int i = 0; i < CANT_ACTIV; ++i) {
        tmp[tFinal[i]].push_back(i);
    }
    for (const auto &actividadesPorHora: tmp) {
        for (auto actividad: actividadesPorHora) {
            actividadesPorFinal.push_back(actividad);
        }
    }
    solucion = vector<int>();
    for (int i = 0; i < actividadesPorFinal.size(); ++i) {
        if (i == 0) solucion.push_back(actividadesPorFinal[0]);
        //checkeo si la i-esima es compatible con la ultima actividad agregada
        if (tInicial[actividadesPorFinal[i]] > tFinal[solucion.back()]) {
            solucion.push_back(actividadesPorFinal[i]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    lecturaDeArch();
#ifdef GOLOSO
    maximo_goloso();
    cout << solucion.size() << endl;
    for (int act: solucion) {
        cout << act << " ";
    }
    cout << endl;
    return 0;
#endif
    maxBenef_vector = vector(CANT_ACTIV, -1);
    construyeEstructurasAux();
#ifdef TOPDOWN
    cout << maxBenef(0) << endl;
    vector<int> soluciontd = reconstruyendoVectorSolucion();
    for (int act: soluciontd) {
        cout << act << " ";
    }
    cout << endl;
    return 0;
#endif

#ifdef BOTTOMUP
    cout << maxBenefCompatibles_bottomup() << endl;
    vector<int> solucionbp= reconstruyendoVectorSolucion();
    for (int act: solucionbp) {
        cout << act << " ";
    }
    cout << endl;
    return 0;
#endif

    return 1;
}
