#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Gram{
    char tproducciones[11][10];
    char noterminales[5];
    char terminales[5];
    char axioma;
} Gramatica;

// PUNTO 1
void cargarNoTerminales(Gramatica *g){
    printf("Ingresar los no terminales: ");
    scanf("%s", g->noterminales);
}

void cargarTerminales(Gramatica *g){
   printf("Ingresar los terminales: ");
   scanf("%s", g->terminales);
}

void cargarAxioma(Gramatica *g){
    printf("Ingresar el axioma: ");
    scanf(" %c", &g->axioma);
}

void cargarProducciones(Gramatica *g){
    int num_producciones;
    printf("Ingresar la cantidad de producciones (maximo 10): ");
    scanf("%d", &num_producciones);
    if(num_producciones < 1 || num_producciones > 10){
        printf("Numero de producciones invalido. \n");
        num_producciones = 10;
    }
    for(int i = 0; i < num_producciones; i++){
        printf("Ingresar la produccion %d: ", i+1);
        scanf("%s", g->tproducciones[i]);  
    }
    g->tproducciones[num_producciones][0] = '\0';
}

// PUNTO 2
_Bool contains(const char *cadena, const char caracter){
    for( int i=0; cadena[i] != '\0'; i++ )
        if( cadena[i] == caracter)
            return 1; 
    return 0; 
}

_Bool terminalSolo(const char *produccion, const Gramatica g){
    return (contains(g.terminales, produccion[3]) || produccion[3] == 'e' ) && produccion[4] == '\0';
}

_Bool terminalMasNoTerminal(const char *produccion, const Gramatica g){
    return contains(g.terminales, produccion[3]) && contains(g.noterminales, produccion[4]);
}

_Bool esLinealPorDerecha(const Gramatica g){
    for(int i=0; g.tproducciones[i][0] != '\0'; i++)
        if( !terminalMasNoTerminal(g.tproducciones[i], g) && !terminalSolo(g.tproducciones[i], g) )
            return 0; 
    return 1; 
}

_Bool noTerminalMasTerminal(const char *produccion, const Gramatica g){
    return contains(g.noterminales, produccion[3]) && contains(g.terminales, produccion[4]);
}

_Bool esLinealPorIzquierda(const Gramatica g){
    for( int i=0; g.tproducciones[i][0] != '\0'; i++)
        if(!terminalSolo(g.tproducciones[i],g) && !noTerminalMasTerminal(g.tproducciones[i],g))
            return 0; 
    return 1; 
}

_Bool verificarEpsilon(const Gramatica g){ //busca el no terminal que produce epsilon y verifica que no se use en otra produccion
    int posicionProdEpsilon;
    char noTerminalProdEpsilon;
    for(int i=0; g.tproducciones[i][0] != '\0'; i++){
        if(g.tproducciones[i][3] == 'e'){
            posicionProdEpsilon = i;
            noTerminalProdEpsilon = g.tproducciones[i][0];
            for( int x=0; g.tproducciones[x][0] != '\0'; x++ ) // verifica que no se use en el lado derecho de otra produccion
                if( g.tproducciones[x][4] == noTerminalProdEpsilon || g.tproducciones[x][3] == noTerminalProdEpsilon )
                    return 0;
        }
    }
    return 1; 
}

_Bool verificarDerecha(const Gramatica g){
   return (esLinealPorDerecha(g) || esLinealPorIzquierda(g)) && verificarEpsilon(g);
} 

_Bool verificarIzquierda(const Gramatica g){
    for( int i=0 ; g.tproducciones[i][0] != '\0'; i++ ) 
        if ( !contains(g.noterminales, g.tproducciones[i][0]) )
            return 0;
    return 1;
}

_Bool verificarMedio(const Gramatica g){
    for( int i=0; g.tproducciones[i][0] != '\0'; i++ )
        if( g.tproducciones[i][1] != '-' && (g.tproducciones[i][2] != '-' || g.tproducciones[i][2] != '>'))
            return 0;
    return 1;
}

int posNoTerminal(const Gramatica g){
    if(esLinealPorDerecha(g))
        return 4;
    else
        return 3;
}

int posTerminal(const Gramatica g){
    if(esLinealPorDerecha(g))
        return 3;
    else
        return 4;
}

void elegirProducciones(char producciones[][10], char noTerminal, Gramatica g){
    int cont = 0;
    for( int i=0; g.tproducciones[i][0] != '\0'; i++ ){
        if( g.tproducciones[i][0] == noTerminal ) //comienzan con el no terminal
            strcpy(producciones[cont++],g.tproducciones[i]);
    }
    int posNT = posNoTerminal(g);
    producciones[cont][0] = '\0';
    for( int i=0; producciones[i][0] != '\0'; i++ ){
        if( contains(g.noterminales,producciones[i][posNT]) && producciones[i][posNT] != noTerminal){
            char nuevoNoTerminal = producciones[i][posNT]; //nuevo no terminal = T
            elegirProducciones(producciones, nuevoNoTerminal, g);
        }               
    }
}

_Bool verificarProducciones(char producciones[][10], Gramatica g){
    for( int i=0; producciones[i][0] != '\0'; i++ ){
        if(contains(g.terminales, producciones[i][3]) && producciones[i][4] == '\0') //S--a (alguna tiene un terminal SOLO)
            return 1;
    }
    return 0;
}

_Bool esInfinita(const Gramatica g){
    _Bool flag = 1;
    char producciones[11][10];
    for( int i = 0; g.tproducciones[i][0] != '\0'; i++ ){
        if(g.tproducciones[i][0] == g.tproducciones[i][4] || g.tproducciones[i][0] == g.tproducciones[i][3]){ //S--aS || S--Sa
            char noTerminal = g.tproducciones[i][0];
            elegirProducciones(producciones, noTerminal, g);
            if( !verificarProducciones(producciones, g) ){
                printf("Es infinita\n");
                return 1;
            }
            flag = 0;
        }
        flag = 0;
    }
    return flag; 
}   

_Bool es_gramatica_regular(const Gramatica g){
    if( verificarIzquierda(g) && verificarDerecha(g) && verificarMedio(g) && !esInfinita(g)){
        printf("La Gramatica es regular\n");
        return 1;
    }
    else{
        printf("La Gramatica no es regular\n");
        return 0;
    }
}

// PUNTO 3
const char *elegirProduccionRandom(const char noTerminal, const Gramatica *g){
    int aparicionesNoTerminal [10]; 
    int cantApariciones = 0;
    for(int i=0; g->tproducciones[i][0]!='\0' ; i++){
        if(g->tproducciones[i][0]==noTerminal){
            aparicionesNoTerminal[cantApariciones] = i; 
            cantApariciones++;
        }
    }
    int noTerminalRandom = rand () % cantApariciones;
    return g->tproducciones[aparicionesNoTerminal[noTerminalRandom]];
}


char *reverse(char *cadena) {
    int longitud = strlen(cadena);
    int i = 0;
    int j = longitud - 1;
    char temp;
    while (i < j) {
        temp = cadena[i];
        cadena[i] = cadena[j];
        cadena[j] = temp;
        i++;
        j--;
    }
    return cadena;
}

void cadenaDeDerivacion(char *palabraprovisoria, char *produccion, int posNT){
    int len = strlen(palabraprovisoria);
    if (posNT==4){
        palabraprovisoria[len] = produccion[3];
        palabraprovisoria[len + 1] = '\0';
        printf("Cadena de derivacion: %s%c\n", palabraprovisoria, produccion[posNT]);
    } else { 
        for (int j = len; j >= 0; j--) {
            palabraprovisoria[j + 1] = palabraprovisoria[j];
        }
        palabraprovisoria[0] = produccion[4];
        palabraprovisoria[len + 1] = '\0';
        printf("Cadena de derivacion: %c%s\n", produccion[posNT], palabraprovisoria);
    }
}

void derivar(const Gramatica g) {
    char palabra[1024] = "";
    char palabraprovisoria[1024] = "";
    char produccion[10];
    int i = 0;
    int posNT = posNoTerminal(g);
    int posT = posTerminal(g);
    srand(time(NULL));
    strcpy(produccion, elegirProduccionRandom(g.axioma, &g));
    while ((produccion[4] != '\0' && produccion[3] == 'e') || produccion[4] != '\0') {
        printf("Produccion elegida: %s\n", produccion);
        cadenaDeDerivacion(palabraprovisoria, produccion, posNT);
        palabra[i++] = produccion[posT];
        char noTerminal = produccion[posNT];
        strcpy(produccion, elegirProduccionRandom(noTerminal, &g));
    }
    printf("Produccion elegida: %s\n", produccion);
    if (produccion[3] == 'e') {
        palabra[0] = '\0';
    } else {
        palabra[i++] = produccion[3];
        palabra[i] = '\0';
    }
    if (esLinealPorIzquierda(g)) {
        printf("Palabra generada = %s\n", reverse(palabra));
    } else {
        printf("Palabra generada = %s\n", palabra);
    }
}

// MAIN
int main(){
    Gramatica g;
    int escape = 1;
    do{
        printf("Ingresar una Gramatica que sea regular\n");
        cargarNoTerminales(&g);
        cargarTerminales(&g);
        cargarProducciones(&g);
        cargarAxioma(&g);
    }while(! es_gramatica_regular(g));
    while( escape ){
        derivar(g);
        printf("Ponga 1 para generar otra palabra o 0 para terminar\n");
        scanf("%d", &escape);
    }
    return 0;
}