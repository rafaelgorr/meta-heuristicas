#ifndef GLOBALS_H_
#define GLOBALS_H_
#include "structs.h"

extern int numberCities;
extern int numberClusters;
extern City *cities;
extern Cluster *clusters;

extern int *mapCityCluster; //map city id with cluster id, the array index represents city's id and the index position cluster's id

extern int greatestX;
extern int greatestY;

#endif