#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <unistd.h>
#include <ctype.h>
#include "list.h"
#include "list.c"

typedef struct main
{
    char reproduccion[15];
    int total;
}totalListas;

typedef struct
{
    char title[40];
    char artist[30];
    char genre[20];
    char year[5];
    char playlist[15];
} cancion;

char *obtener(char * linea, int k)
{
    int marca = 0;
    char* aux=(char*) malloc (100*sizeof(char));
    int inicio = 0, i = 0;
    int j=0;
    while(linea[i+1]!='\0')
    {
        if(linea[i]== '\"')
        {
            marca = 1-marca;
            if(marca) inicio = i+1;
            i++;
            continue;
        }
        if(marca || linea[i]!= ',')
        {
            if(k==j) aux[i-inicio] = linea[i];
            i++;
            continue;
        }
        if(linea[i]== ',')
        {
            if(k==j)
            {
                aux[i-inicio] = 0;
                return aux;
            }
            j++; inicio = i+1;
        }
        i++;
    }
    if(k==j)
    {
        aux[i-inicio] = 0;
        return aux;
    }
    return NULL;
}

List* importar(const char *casteo)
{
    FILE *fp = fopen((casteo), "r");
    char linea[1024];
    int i;
    List* canciones = createList();
    while (fgets (linea, 1023, fp) != NULL)
    {
        cancion * aux = (cancion*) malloc (sizeof(cancion));
        for(i = 0 ; i < 5 ; i++)
        {
            char *aux2 = obtener(linea, i);
            switch (i)
            {
            case 0: strcpy(aux->title,aux2); break;
            case 1:
            {
                aux2[0] = toupper(aux2[0]);
                for (int j = 1; j < strlen(aux2); j++)
                {
                    if (aux2[j-1] == ' ')
                    {
                        aux2[j] = toupper(aux2[j]);
                        j++;
                    }
                    aux2[j] = tolower(aux2[j]);
                }
                strcpy(aux->artist,aux2);
                break;
            }
            case 2: strcpy(aux->genre,aux2); break;
            case 3: strcpy(aux->year,aux2); break;
            case 4: strcpy(aux->playlist,aux2); break;
            }
        }
        pushBack(canciones,aux);
    }
    if (fclose(fp) == 0)
    {
        printf("\n NO HUBO ERROR EN LA EJECUCION\n");
        return canciones;
    }
}

void exportar(const char *casteo, List* canciones)
{
    FILE *f;
    f = fopen((casteo), "a");
    fclose(f);
    cancion* aux = firstList(canciones);
    while (aux != NULL)
    {
        char linea[1024];
        for (int i = 0; i < 5; i++)
        {
            switch(i)
            {
                case 0:
                {
                    strcpy(linea,aux->title);
                    strcat(linea,",");
                    break;
                }
                case 1:
                {
                    strcat(linea,aux->artist);
                    strcat(linea,",");
                    break;
                }
                case 2:
                {
                    strcat(linea,aux->genre);
                    strcat(linea,",");
                    break;
                }
                case 3:
                {
                    strcat(linea,aux->year);
                    strcat(linea,",");
                    break;
                }
                case 4:
                {
                    strcat(linea,aux->playlist);
                    strcat(linea,"\n");
                    break;
                }
            }
        }
        f = fopen((casteo), "a");
        fprintf(f, linea, 1023);
        fclose(f);
        aux = nextList(canciones);
    }
}

void agregarCancion(char *titulo, char  *artista, char  *genero, char  *anyo, char  *lista, List *canciones)
{
    int i = 0;
    titulo[i] = toupper(titulo[i]);
    artista[i] = toupper(artista[i]);
    genero[i] = toupper(genero[i]);
    lista[i] = toupper(lista[i]);
    for (i = 1; i < strlen(titulo); i++)
    {
        titulo[i] = tolower(titulo[i]);
    }
    for (i = 1; i < strlen(artista); i++)
    {
        if (artista[i-1] == ' ')
        {
            artista[i] = toupper(artista[i]);
            i++;
        }
        artista[i] = tolower(artista[i]);
    }
    for (i = 1; i < strlen(genero); i++)
    {
        genero[i] = tolower(genero[i]);
    }
    for (i = 1; i < strlen(lista); i++)
    {
        lista[i] = tolower(lista[i]);
    }
    
    cancion* aux = (cancion*) malloc (sizeof(cancion));
    strcpy(aux->title, titulo);
    strcpy(aux->artist, artista);
    strcpy(aux->genre, genero);
    strcpy(aux->year, anyo);
    strcpy(aux->playlist, lista);
    pushBack(canciones,aux);
}

bool buscarPorNombre(char *nombre, List* canciones)
{
    cancion* aux = firstList(canciones);
    for (int i = 0; i < strlen(nombre); i++)
    {
        if (i == 0) nombre[i] = toupper(nombre[i]);
        else nombre[i] = tolower(nombre[i]);
    }
    bool existe = false;
    while (aux != NULL)
    {
        if (strcmp(nombre, aux->title) == 0)
        {
            existe = true;
            printf("titulo: %s\n", aux->title);
            printf("artista: %s\n", aux->artist);
            printf("genero: %s\n", aux->genre);
            printf("fecha de salida: %s\n", aux->year);
            printf("playlist: %s\n\n", aux->playlist);
        }
        aux = nextList(canciones);
    }
    return existe;
}

bool buscarPorArtista(char *artista, List* canciones)
{
    cancion* aux = firstList(canciones);
    for (int i = 0; i < strlen(artista); i++)
    {
        if (i == 0) artista[i] = toupper(artista[i]);
        else artista[i] = tolower(artista[i]);
    }
    bool existe = false;
    while (aux != NULL)
    {
        if (strcmp(artista, aux->artist) == 0)
        {
            existe = true;
            printf("Titulo: %s\n", aux->title);
            printf("Artista: %s\n", aux->artist);
            printf("Genero: %s\n", aux->genre);
            printf("Fecha de salida: %s\n", aux->year);
            printf("Playlist: %s\n\n", aux->playlist);
        }
        aux = nextList(canciones);
    }
    return existe;
}

char *conseguir(char *genero, int k)
{
    int i = 0;
    int inicio = 0;
    int j = 0;
    char* aux = (char *) malloc (100*sizeof(char));
    while(genero[i]!='\0')
    {
        if(genero[i]!= ',')
        {
            if(k==j) aux[i-inicio] = genero[i];
            i++;
            continue;
        }
        if(genero[i]== ',')
        {
            if(k==j)
            {
                aux[i-inicio] = 0;
                return aux;
            }
            j++; inicio = i+1;
        }
        i++;
    }
    if(k==j)
    {
        aux[i-inicio] = 0;
        return aux;
    }
    return NULL;
}

bool buscarPorGenero(char *genero, List* canciones)
{
    int k;
    int a = 1;
    cancion* aux = firstList(canciones);
    for (int i = 0; i < strlen(genero); i++)
    {
        if (i == 0) genero[i] = toupper(genero[i]);
        else genero[i] = tolower(genero[i]);
    }
    bool existe = false;
    while (aux != NULL)
    {
        for(k = 0; k<5; k++)
        {
            if(strcmp(genero, aux->genre) == 0)
            {
                existe = true;
                printf("titulo: %s\n", aux->title);
                printf("artista: %s\n", aux->artist);
                printf("genero: %s\n", aux->genre);
                printf("fecha de salida: %s\n", aux->year);
                printf("playlist: %s\n\n", aux->playlist);
                break;
            }
            char *aux2 = conseguir(aux->genre, k);
            if(aux2 == NULL) break;
            if(k>0)
            {
                for(int j = 0; j < strlen(aux2); j++)
                {
                    aux2[j] = aux2[j+1];
                    if((j+1) == strlen(aux2))
                    {
                        aux2[j+1] = 0;
                    }
                }
            }
            if (strcmp(genero, aux2) == 0)
            {
                existe = true;
                printf("titulo: %s\n", aux->title);
                printf("artista: %s\n", aux->artist);
                printf("genero: %s\n", aux->genre);
                printf("fecha de salida: %s\n", aux->year);
                printf("playlist: %s\n", aux->playlist);
                break;
            }
        }
        aux = nextList(canciones);
        a++;
    }
    return existe;
}

List* eliminarCancion(char *titulo ,char *artista ,char *anyo, List *canciones)
{
    int i = 0;
    titulo[i] = toupper(titulo[i]);
    artista[i] = toupper(artista[i]);
    for (i = 1; i < strlen(titulo); i++)
    {
        titulo[i] = tolower(titulo[i]);
    }
    for (i = 1; i < strlen(artista); i++)
    {
        if (artista[i-1] == ' ')
        {
            artista[i] = toupper(artista[i]);
            i++;
        }
        artista[i] = tolower(artista[i]);
    }
    cancion* aux = firstList(canciones);
    if(aux==NULL)
    {
        printf("No existen canciones que eliminar\n");
        return NULL;
    } 
    List* nuevaLista = createList();
    int j = 0;
    while (aux != NULL)
    {
        if (strcmp(aux->title, titulo) == 0)
        {
            if (strcmp(aux->artist, artista) == 0)
            {
                if (strcmp(aux->year, anyo) == 0)
                {
                    j++;
                }
                else
                {
                    pushBack(nuevaLista,aux);
                }
            }
            else
            {
                pushBack(nuevaLista,aux);
            }
        }
        else
        {
            pushBack(nuevaLista,aux);
        }
        aux = nextList(canciones);
    }
    if (j == 0)
    {
        printf("La cancion que desea eliminar no se encuentra en la lista\n");
    }
    else
    {
        printf("Su cancion ha sido eliminada correctamente \n");
    }
    return nuevaLista;
}

bool existelista(char *lista, List* canciones)
{
    cancion* aux = firstList(canciones);
    for (int i = 0; i < strlen(lista); i++)
    {
        if (i == 0) lista[i] = toupper(lista[i]);
        else lista[i] = tolower(lista[i]);
    }
    bool existe = false;
    while (aux != NULL)
    {
        if (strcmp(lista, aux->playlist) == 0)
        {
            existe = true;
            printf("titulo: %s\n", aux->title);
            printf("artista: %s\n", aux->artist);
            printf("genero: %s\n", aux->genre);
            printf("fecha de salida: %s\n", aux->year);
            printf("playlist: %s\n\n", aux->playlist);
        }
        aux = nextList(canciones);
    }
    return existe;
}

void MostrarListas(List * canciones)
{
    int cont=1;
    int i=0;
    totalListas *aux =(totalListas*) malloc(sizeof(totalListas));
    cancion *aux2 = firstList(canciones);
    if(aux2==NULL)
    {
        printf("No existen listas\n");
        return ;
    } 
    strcpy(aux[0].reproduccion,aux2->playlist);
    (aux[0].total) = 1;
    aux2 =nextList(canciones); 
    while(aux2!=NULL)
    {
        int cont2=0;
        for ( i = 0; i < cont; i++) 
        {
            if(strcmp(aux[i].reproduccion,aux2->playlist) == 0)
            {
                (aux[i].total)++;
                cont2++;
                break;
            }
        }
        if(cont2 == 0)
        {
            cont++;
            aux = (totalListas*) realloc(aux,sizeof(totalListas)*cont);
            strcpy(aux[cont-1].reproduccion,aux2->playlist);
            (aux[cont-1].total)=1;
        }
        aux2 =nextList(canciones); 
    }
    for (int k = 0; k < cont; k++)
    {
        if(aux[k].total == 1) printf("La %s contiene: %d cancion.\n",aux[k].reproduccion,aux[k].total);
        else printf("La %s contiene: %d canciones.\n",aux[k].reproduccion,aux[k].total);
    }
}

int main( int argc, char *argv[])
{
    int op;
    List* canciones = createList();
    while(op!=11)
    {
        printf("\n1.- Importar canciones desde un archivo CSV\n"); //         LISTO
        printf("2.- Exportar canciones CSV\n"); //                            LISTO
        printf("3.- Agregar cancion\n"); //                                   LISTO 
        printf("4.- Buscar cancion por nombre\n"); //                         LISTO
        printf("5.- Buscar cancion por artista\n"); //                        LISTO
        printf("6.- Buscar cancion por genero\n");//                          LISTO
        printf("7.- Eliminar cancion\n");//                                   LISTO
        printf("8.- Mostrar nombres de las listas de reproduccion\n");//      LISTO        
        printf("9.- Mostrar una lista de reproduccion\n");//                  LISTO
        printf("10.- Mostrar todas las canciones\n");//                       LISTO
        printf("11.- Salir\n");//                                             LISTO
        printf("Indica la opcion:\n");
        scanf("%d", &op);
        if(op>11 || op<1)
        {
            printf("\nHas salido del programa porque ingesaste un numero incorrecto");
            break;
        }
        switch(op)
        {
            case 1:
            {
                char archivo[30];
                printf("Ingrese el nombre del archivo para importar (especifique el tipo de archivo)\n");
                scanf("%s", &archivo);
                const char *casteo = (const char *) archivo;
                if(access(casteo,R_OK) == -1)
                {
                    printf("No se ha encontrado ninguno archivo con ese nombre\n\n");
                    break;
                }
                canciones = importar(casteo);
                printf("\n\nSe han importado sus canciones correctamente\n\n");
                break;
            }
            case 2:
            {
                char archivo[30];
                printf("Ingrese el nombre del nuevo archivo para exportar/crear (especifique el tipo de archivo)\n");
                scanf("%s", &archivo);
                const char *casteo = (const char *) archivo;
                exportar(casteo, canciones);
                printf("\nSe ha creado el archivo: %s\n", archivo);
                break;
            }
            case 3:
            {
                char titulo[40];
                char artista[30];
                char genero[20];
                char anyo[5];
                char lista[15];
                int Messi = 1;
                while(Messi != 2)
                {
                    printf("Ingrese el nombre de la cancion\n");
                    scanf(" %[^\n]s",titulo);
                    printf("Ingrese el nombre del artista\n");
                    scanf(" %[^\n]s",artista);
                    printf("Ingrese los generos de la cancion (Separados por coma)\n");
                    scanf(" %[^\n]s",genero);
                    printf("Ingrese la fecha de salida de la cancion\n");
                    scanf(" %[^\n]s",anyo);
                    printf("Ingrese la lista de reproduccion de la cancion\n");
                    scanf(" %[^\n]s",lista);
                    agregarCancion(titulo, artista, genero, anyo, lista, canciones);
                    printf("Desea agregar otra cancion? 1 = SI, 2 = NO\n");
                    while (Messi < 1 || Messi > 2)
                    {
                        printf("Ingrese una opcion valida, 1 = SI, 2 = NO\n");
                        scanf("%d",&Messi);
                    }
                    scanf("%d", &Messi);
                }
                
                break;
            }
            case 4:
            {
                char nombre[40];
                printf("Ingrese el nombre de la cancion a buscar\n");
                scanf(" %[^\n]s", nombre);
                bool existe = buscarPorNombre(nombre, canciones);
                if (existe == false)
                {
                    printf("No se ha encontrado ninguna cancion con ese nombre\n");
                }
                break;
            }
            case 5:
            {
                char artista[30];
                printf("Ingrese el nombre del artista/banda a buscar\n");
                scanf(" %[^\n]s", artista);
                bool existe = buscarPorArtista(artista, canciones);
                if (existe == false)
                {
                    printf("No se ha encontrado ningun artista/banda con ese nombre\n");
                }
                break;
            }
            case 6:
            {
                char genero[30];
                printf("Ingrese el nombre del genero musical a buscar\n");
                scanf(" %[^\n]s", genero);
                bool existe = buscarPorGenero(genero, canciones);
                if (existe == false)
                {
                    printf("No se ha encontrado ningun genero musical con ese nombre\n");
                }
                break;
            }
            case 7:
            {
                char titulo[40];
                char artista[30];
                char anyo[5];
                printf("Ingrese el nombre de la cancion a eliminar\n");
                scanf(" %[^\n]s",titulo);
                printf("Ingrese el artista de la cancion a eliminar\n");
                scanf(" %[^\n]s",artista);
                printf("Ingrese la fecha de salida de la cancion a eliminar\n");
                scanf(" %[^\n]s",anyo);
                canciones = eliminarCancion(titulo,artista,anyo,canciones);
                break;
            }
            case 8: 
            {
                MostrarListas(canciones);
                break;
            }
            case 9: 
            {
                char lista[15];
                printf("Ingrese la lista de reproduccion que desea ver\n");
                scanf(" %[^\n]s", lista);
                bool existe = existelista(lista, canciones);
                if(existe == false)
                {
                    printf("No se ha encontrado ninguna lista con ese nombre\n");
                }
                break;
            }
            case 10:
            {
                cancion *songs = firstList(canciones);
                if(songs==NULL)
                {
                    printf("No existen canciones\n");
                    break;
                } 
                while(songs!=NULL)
                {
                    printf("Titulo: %s\n", songs->title);
                    printf("Artista: %s\n", songs->artist);
                    printf("Genero: %s\n", songs->genre);
                    printf("Fecha de salida: %s\n", songs->year);
                    printf("Playlist: %s\n\n", songs->playlist);
                    songs=nextList(canciones);
                }
                break;
            }
            case 11:printf("\n Has salido del programa con exito"); break;
        }
    }
    return 0;
}