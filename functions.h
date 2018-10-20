#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_          

#include "headers.h"
#include "globals.h"


/*************** READ FUNCTIONS ***************/
void            readCities                  ();

/************ CLUSTERING FUNCTIONS ************/
void            createClusters              ();
void            kMeans                      ();
void            getSmallerDist              (); //GET SMALLER DISTANCES BETWEEN CITIES AND CLUSTERS, build mapCityCluster array
void            updateCentroids             ();
int             getCentroidX                (int clusterId);
int             getCentroidY                (int clusterId);

/*********** TABU SEARCH FUNCTIONS ***********/
void            tabuSearch                  (int tabuListSize, int maxIter, int maxIterImpr);
int             getPathValue                (int *visitedCities, int len);
int             getNumberCitiesInCluster    (int clusterId);
Neighbor*       getNeighbors                (int *bestNeighbor, int len );
void            shakePath                   (int* pathArray, int len);
int             checkIfMovIsTabu            (TabuMoviment *tabuList, int len, TabuMoviment m);

/*************** AUX FUNCTIONS ***************/
void            RemoveSpaces                (char*);
int             euclDist                    (Coord c1, Coord c2);
int*            copyArray                   (int* array, int len);
void            printArray                  (int *array, int len);
void            freeNeighborArray           (Neighbor *array, int len);
int             neighborArrayLen            (int len);
void            permute                     (int* array,int len, int l);
void            swap                        (int* x, int* y);
void            findNearestCity             (int* cts, int len, int idCity);

#endif 
