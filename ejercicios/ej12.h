using namespace std;

//Declaro variables globales
int INDEF = std::numeric_limits<int>::min();
int CANT_ACTORES; // Se inicializa en el main
int CANT_AMISTADES; // Se inicializa en el main
int sumaMax = 0;
vector<vector<bool>> sonAmigos;
vector<int> influencia;
vector<int> cantVecinos;
vector<int> actoresSolucion;
int nodosRecorridosDB = 0; // PARA DEBUG

void imprimirActores(){
    int tam = actoresSolucion.size();
    for (int j = 0; j < tam-1; ++j){
        cout<<actoresSolucion[j]+1<<" ";
    }
    if(tam >= 1){cout<<actoresSolucion[tam-1] + 1;}
    cout<<""<<endl;
}

//Para ordenar actores segun su influencia, de menor a mayor y preservando el orden
bool menorActorSegunInfluencia(int i, int j){
    return (influencia.at(i) < influencia.at(j) || (influencia.at(i) == influencia.at(j) && i < j));
}

bool mayorActorSegunInfluencia(int i, int j){
    return (influencia.at(i) > influencia.at(j) || (influencia.at(i) == influencia.at(j) && i > j)) ;
}

//Para poder podar, se usa para calcular la suma de los conjuntos de actores K y Q
int sumaPrioridades(vector<int> *actores){
    int suma = 0;
    for(int i = 0; i<actores->size(); i++){
        suma += influencia.at(actores->at(i));
    }
    return suma;
}

//Saco a los actores de K si son el actorADecidir o si no son sus amigos 
//y en este ultimo caso, resto su influencia de sumaParcial
void sacoActorYSusNoAmigos(vector<int> *K, int actorADecidir){
    for (int t = 0; t < K->size(); t++) {
        //cout<< "Comparo para sacar de K: "<< actorADecidir <<"," << K->at(t) <<endl; //COMENTARIO DE DEBUG
        if (K->at(t)==actorADecidir){
            //Si el es actor que agregue a Q, lo saco de K pero no lo descuento de la sumaParcial
            K->erase(K->begin()+t);
            t--;
        } else if (not sonAmigos[actorADecidir][K->at(t)]){
            //Si es un actor que no es amigo del actorADecidir, lo saco de K y lo descuento
            K->erase(K->begin()+t);
            t--;
        }
    }
}

//Funcion que pasa a Q los que estan en K y son amigos de todos ahí
void agregoAQLosQueSonAmigosDeTodosEnK(vector<int> *Q, vector<int> *K){
    bool esAmigoDeTodos;
    //Uso i: indice para chequear si el i-esimo es amigo de todos o no
    for (int i = 0; i < K->size(); i++) {
        //Si no hay otros actores en K => hay que agregarlo a Q
        esAmigoDeTodos = true;
        //Uso j: indice para chequear si el j-esimo es amigo del i-esimo
        for (int j = 0; j < K->size() && esAmigoDeTodos; j++) {
            //Si hay uno que NO es amigo => NO es amigo de todos y se mantiene en K
            if (K->at(i)!=K->at(j) && not sonAmigos[K->at(i)][K->at(j)]){
                esAmigoDeTodos = false;
            }
        }
        //Si es amigo de todos => Lo agrego a Q y lo saco de K 
        //No van a haber actores en K que no sean amigos del i-esimo => no hace falta chequearlo
        if(esAmigoDeTodos){
            Q->push_back(K->at(i));
            K->erase(K->begin()+i);
            i--;
        }
    }
    /* COMENTARIO DE DEBUG
    cout<<" Imrpimo Q= ";
    for (int i = 0; i < Q->size(); ++i){
        cout<<Q->at(i)<<",";
    }
    cout<<"; Imprimo K= ";
    for (int i = 0; i < K->size(); ++i){
        cout<<K->at(i)<<",";
    }
    cout<<""<<endl;
    */
}

int cliqueMasInfluyente(vector<int> *Q, vector<int> *K){
    nodosRecorridosDB++; // DEBUG
    int sumaQ = sumaPrioridades(Q);
    if (K->size() == 0){
        //K vacia, en Q esta la clique formada
        //chequeo si la prioridad de la clique es la maxima encontrada y la seteo como tal
        if(sumaQ > sumaMax){
            sumaMax = sumaQ;
            actoresSolucion = *Q;
        }
        return sumaQ;
    }
    if(sumaQ + sumaPrioridades(K) <= sumaMax){
        //Si no se podria superar al maximo de influencia actual, incluso agregando todos los de k, corto la rama
        return INDEF;
    } else {
        //K sigue teniendo actores, Q no esta completa
        //Tomo el siguiente de la lista de K
        int actorADecidir = K->front();
        
        //CASO: AGREGO 
        vector<int> KAgregando = *K;
        vector<int> QAgregando = *Q;
        //Agrego el actorADecidir a Q
        QAgregando.push_back(actorADecidir);
        //Saco de K al actorADecidir y a los que no sean amigos (resto sus influencias)
        sacoActorYSusNoAmigos(&KAgregando, actorADecidir); 
        //Valido si entre los que quedan, hay algunos que sean amigos de todos y los saco 
        agregoAQLosQueSonAmigosDeTodosEnK(&QAgregando, &KAgregando);

        int maxAgregando = cliqueMasInfluyente(&QAgregando,&KAgregando);

        //CASO: NO AGREGO
        vector<int> KSinAgregar = *K;
        vector<int> QSinAgregar = *Q;
        //Saco de K al actorADecidir (era el elemento 0 en k)
        KSinAgregar.erase(KSinAgregar.begin());
        //std::remove(KSinAgregar.begin(), KSinAgregar.end(), actorADecidir);
        //Valido si entre los que quedan, hay algunos que sean amigos de todos y los saco
        agregoAQLosQueSonAmigosDeTodosEnK(&QSinAgregar, &KSinAgregar);

        int maxNoAgregando = cliqueMasInfluyente(&QSinAgregar,&KSinAgregar);

        //Tomo el maximo de los dos y lo devuelvo
        int maximo = max(maxAgregando, maxNoAgregando);
        return maximo;
    }
}

///////////////////////////////////////////////EJERCICIO 2///////////////////////////////////////////////////////////////

vector<vector<int>> particionConMinInflu(vector<int> *K){
    vector<vector<int>> particion (0, vector<int>(0,-1));
    bool puedoAgregarlo;
    //Armo la particion
    for(int actorDeK: *K){
        //si no hay particion, implica crear una; sino, hay que ver si se puede sumar a algun conjunto
        puedoAgregarlo = particion.size()>0;
        //Recorro la particion buscando un conjunto donde el actor sea NO AMIGO de todos en el
        for (int i = 0; i < particion.size(); ++i){
            puedoAgregarlo = true;
            if(particion.at(i).size() == 0){ 
                particion.at(i).push_back(actorDeK);
                //Dejo de buscar a qué conjunto de la particion agregar
                break;
            }
            for(int actorDeI: particion.at(i)){
                if(sonAmigos.at(actorDeK).at(actorDeI)){
                    //Si hay un actor del conjunto independiente que es AMIGO de el, no puede ser ingresado ahi
                    puedoAgregarlo = false;
                    //Dejo de revisar a los actores de ese conjunto
                    break;
                }
            }
            if(puedoAgregarlo){//para asegurarse que siempre
                //El actor de K encontro un conj independiente donde es NO AMIGO con todos los que estan, lo sumo ahi
                if(influencia.at(particion.at(i).front()) < influencia.at(actorDeK)){
                    particion.at(i).push_back(particion.at(i).front());
                    particion.at(i).front() = actorDeK;
                } else{
                    (particion.at(i)).push_back(actorDeK);
                }
                //Dejo de buscar a qué conjunto de la particion agregar
                break;
            }
        }
        //Si recorri todos los conjuntos de la particion y en todos tiene algun amigo, lo agrego a una particion nueva
        if(!puedoAgregarlo){
            particion.push_back(vector<int> (1, actorDeK));
        }
    }
    return particion;
}

int influenciaMindePart(vector<int> *K){
    vector<vector<int>> particionK = particionConMinInflu(K);
    int minInf = 0;
    for (int i = 0; i < particionK.size(); i++) {
        minInf += influencia.at(particionK[i].front());
    }
    return minInf;
}

int cliqueMasInfluyenteConPoda(vector<int> *Q, vector<int> *K){
    nodosRecorridosDB++; // DEBUG
    int sumaQ = sumaPrioridades(Q);
    if (K->size() == 0){
        //K vacia, en Q esta la clique formada
        //chequeo si la prioridad de la clique es la maxima encontrada y la seteo como tal
        if(sumaQ > sumaMax){
            sumaMax = sumaQ;
            actoresSolucion = *Q;
        }
        return sumaQ;
    }
    if(sumaQ + influenciaMindePart(K) <= sumaMax){
        //Si no se podria superar al maximo de influencia actual, tomando la mejor lista de K
        // teniendo en cuenta que tiene el maximo largo posible que se le puede agtrega a Q y un elemento que es el elemento correspontiente o lo mejor posible como reemplazo
        return INDEF;
    } else {
        //K sigue teniendo actores, Q no esta completa
        //Tomo el siguiente de la lista de K
        int actorADecidir = K->front();

        //CASO: AGREGO
        vector<int> KAgregando = *K;
        vector<int> QAgregando = *Q;
        //Agrego el actorADecidir a Q
        QAgregando.push_back(actorADecidir);
        //Saco de K al actorADecidir y a los que no sean amigos (resto sus influencias)
        sacoActorYSusNoAmigos(&KAgregando, actorADecidir);
        //Valido si entre los que quedan, hay algunos que sean amigos de todos y los saco
        agregoAQLosQueSonAmigosDeTodosEnK(&QAgregando, &KAgregando);

        int maxAgregando = cliqueMasInfluyenteConPoda(&QAgregando,&KAgregando);

        //CASO: NO AGREGO
        vector<int> KSinAgregar = *K;
        vector<int> QSinAgregar = *Q;
        //Saco de K al actorADecidir (era el elemento 0 en k)
        KSinAgregar.erase(KSinAgregar.begin());
        //std::remove(KSinAgregar.begin(), KSinAgregar.end(), actorADecidir);
        //Valido si entre los que quedan, hay algunos que sean amigos de todos y los saco
        agregoAQLosQueSonAmigosDeTodosEnK(&QSinAgregar, &KSinAgregar);

        int maxNoAgregando =cliqueMasInfluyenteConPoda(&QSinAgregar,&KSinAgregar);

        //Tomo el maximo de los dos y lo devuelvo
        int maximo = max(maxAgregando, maxNoAgregando);
        return maximo;
    }
}
