#include "functions.h"

/*************** READ FUNCTIONS ***************/
void readCities()
{
  string str = (string)malloc(sizeof(char) * 9999999);
  float x, y;
  int id;

  scanf("%[^\t]", str); // READ ALL INPUT FROM STDIN
  // printf("%s\n",str);
  string line;
  line = strtok(str, "\n");  //IGNORE LINE 1
  line = strtok(NULL, "\n"); //IGNORE LINE 2
  line = strtok(NULL, "\n"); //IGNORE LINE 3
  line = strtok(NULL, "\n");
  RemoveSpaces(line);
  sscanf(line, "DIMENSION:%d\n", &numberCities); // TAKE NUMBER OF CITIES FROM 4TH LINE
  line = strtok(NULL, "\n");                     //IGNORE LINE 5
  line = strtok(NULL, "\n");                     //IGNORE LINE 6

  line = strtok(NULL, "\n"); // FIRST CITY
  cities = (City *)malloc(sizeof(City) * numberCities);

  for (int i = 0; i < numberCities; i++)
  {
    sscanf(line, "%d %f %f\n", &id, &x, &y);
    cities[i].id = id - 1;
    cities[i].c.x = round(x);
    cities[i].c.y = round(y);

    if (cities[i].c.x > greatestX)
      greatestX = cities[i].c.x;

    if (cities[i].c.y > greatestY)
      greatestY = cities[i].c.y;

    // printf("(%d,%d)\n",cities[i].c.x,cities[i].c.y);
    line = strtok(NULL, "\n");
  }
}
/*********************************************/

/************ CLUSTERING FUNCTIONS ************/
void createClusters()
{
  srand(time(NULL));
  clusters = (Cluster *)calloc(numberClusters, sizeof(Cluster));
  int addPosX = greatestX / numberClusters;
  int addPosY = greatestY / numberClusters;
  int posX = 0;
  int posY = 0;
  for (int i = 0; i < numberClusters; i++)
  {
    posX += addPosX;
    posY += addPosY;
    clusters[i].id = i;
    clusters[i].c.x = posX;
    clusters[i].c.y = posY;
  }
}

void kMeans()
{
  mapCityCluster = (int *)calloc(numberCities, sizeof(int));
  int maxIter = 50;
  for (int it = 0; it < maxIter; it++)
  {
    getSmallerDist(numberClusters);
    updateCentroids(numberClusters);
  }
}

void getSmallerDist()
{
  SmallerDist smallerDist;
  for (int i = 0; i < numberCities; i++)
  {
    smallerDist.dist = __INT_MAX__;
    for (int j = 0; j < numberClusters; j++)
    {
      if (smallerDist.dist > euclDist(cities[i].c, clusters[j].c))
      {
        smallerDist.clusterId = clusters[j].id;
        smallerDist.dist = euclDist(cities[i].c, clusters[j].c);
      }
    }
    mapCityCluster[cities[i].id] = smallerDist.clusterId;
  }
}

void updateCentroids()
{
  for (int i = 0; i < numberClusters; i++)
  {
    if (getCentroidX(i) != 0)
      clusters[i].c.x = getCentroidX(i);
    if (getCentroidY(i) != 0)
      clusters[i].c.y = getCentroidY(i);
  }
}

int getCentroidX(int clusterId)
{
  int sumX = 0, citiesCount = 0;
  City city;
  for (int i = 0; i < numberCities; i++)
  {
    city = cities[i];
    if (mapCityCluster[i] == clusterId)
    {
      sumX += city.c.x;
      citiesCount++;
    }
  }
  if (citiesCount != 0)
    return (int)sumX / citiesCount;
  else
    return sumX;
}

int getCentroidY(int clusterId)
{
  int sumY = 0, citiesCount = 0;
  City city;
  for (int i = 0; i < numberCities; i++)
  {
    city = cities[i];
    if (mapCityCluster[i] == clusterId)
    {
      sumY += city.c.y;
      citiesCount++;
    }
  }
  if (citiesCount != 0)
    return (int)sumY / citiesCount;
  else
    return sumY;
}

/*********************************************/

/*********** TABU SEARCH FUNCTIONS ***********/
void tabuSearch(int tabuListSize, int maxIter, int maxIterImpr)
{
  int *citiesInCluster = NULL;
  int numberCitiesInCluster;
  int visitedCount;
  int totalPathValue = 0;
  ValueId bestNeighborValue;

  Neighbor best;
  best.neighbor = NULL;

  Neighbor bestNeighbor;
  bestNeighbor.neighbor = NULL;

  TabuMoviment *tabuList = (TabuMoviment *)calloc(tabuListSize, sizeof(TabuMoviment));
  int tabuListCount = 0;
  for (int i = 0; i < tabuListSize; i++)
  {
    tabuList[i].i = 0;
    tabuList[i].j = 0;
  }
  Neighbor *neighbors = NULL;

  int lastCityOfCluster = -1, firstCityOfCluster = -1;
  for (int i = 0; i < numberClusters; i++)
  {
    numberCitiesInCluster = getNumberCitiesInCluster(clusters[i].id);
    if (numberCitiesInCluster == 0 || numberCitiesInCluster == 1)
      continue;
    // printf("CLUSTER %d: %d\n", i, numberCitiesInCluster);

    if (citiesInCluster != NULL)
      free(citiesInCluster);
    citiesInCluster = (int *)calloc(numberCitiesInCluster, sizeof(int));
    visitedCount = 0;
    for (int j = 0; j < numberCities; j++)
      if (mapCityCluster[j] == clusters[i].id)
      {
        citiesInCluster[visitedCount] = cities[j].id;
        visitedCount++;
      }
    if (best.neighbor != NULL)
      free(best.neighbor);

    if (bestNeighbor.neighbor != NULL)
      free(bestNeighbor.neighbor);

    // shakePath(citiesInCluster, numberCitiesInCluster);
    best.neighbor = copyArray(citiesInCluster, numberCitiesInCluster);
    bestNeighbor.neighbor = copyArray(citiesInCluster, numberCitiesInCluster);
    tabuListCount = 0;
    for (int i = 0; i < tabuListSize; i++)
    {
      tabuList[i].i = 0;
      tabuList[i].j = 0;
    }

    for (int it = 0; it < maxIter; it++) // TABU SEARCH
    {
      if (neighbors != NULL)
        free(neighbors);

      neighbors = getNeighbors(bestNeighbor.neighbor, numberCitiesInCluster); // CREATE NEIGHBORS      
      if (i != 0)
      {        
        for(int j = 0;j<neighborArrayLen(numberCitiesInCluster);j++)
          findNearestCity(neighbors[j].neighbor, numberCitiesInCluster, lastCityOfCluster);        
      }
      
      bestNeighborValue.value = __INT_MAX__;
      for (int j = 0; j < neighborArrayLen(numberCitiesInCluster); j++) //SEARCH FOR THE BEST NEIGHBOR
      {
        if (!checkIfMovIsTabu(tabuList, tabuListCount, neighbors[j].m)) //CHECK IF MOVIMENT m ISN'T TABU
        {
          if (bestNeighborValue.value > getPathValue(neighbors[j].neighbor, neighbors[j].len)) // COMPARING OBJ FUNCTION
          {
            bestNeighborValue.value = getPathValue(neighbors[j].neighbor, neighbors[j].len);
            bestNeighborValue.id = j;
          }
        }
        else
        { // MOVIMENT IS TABU
          if (getPathValue(best.neighbor, numberCitiesInCluster) >
              getPathValue(neighbors[j].neighbor, neighbors[j].len)) //ASPIRATION CRITERION
          {
            bestNeighborValue.value = getPathValue(neighbors[j].neighbor, neighbors[j].len);
            bestNeighborValue.id = j;
          }
        }
      }      
      
      free(bestNeighbor.neighbor);
      bestNeighbor.neighbor = copyArray(neighbors[bestNeighborValue.id].neighbor, // FIND BEST LOCAL NEIGHBOR
                                        neighbors[bestNeighborValue.id].len);
      bestNeighbor.m = neighbors[bestNeighborValue.id].m; // SAVE HIS MOVIMENT

      if (getPathValue(best.neighbor, numberCitiesInCluster) >
          getPathValue(bestNeighbor.neighbor, numberCitiesInCluster)) //COMPARE BEST LOCAL NEIGHBOR WITH BEST NEIGHBOR
      {
        free(best.neighbor);
        best.neighbor = copyArray(bestNeighbor.neighbor, numberCitiesInCluster);
      }
      if (tabuListCount == tabuListSize) //UPDATE TABU LIST
        tabuListCount = 0;
      tabuList[tabuListCount].i = neighbors[bestNeighborValue.id].m.i; // ADD MOVIMENTS
      tabuList[tabuListCount].j = neighbors[bestNeighborValue.id].m.j;
      tabuListCount++;
    }
    if (i != 0)
    {
      firstCityOfCluster = best.neighbor[0];
      totalPathValue += euclDist(cities[lastCityOfCluster].c,
                                 cities[firstCityOfCluster].c);
    }

    totalPathValue += getPathValue(best.neighbor, numberCitiesInCluster);
    lastCityOfCluster = best.neighbor[numberCitiesInCluster - 1];
  }
  printf("TOTAL: %d\n", totalPathValue);
}

int getPathValue(int *visitedCities, int len)
{
  int pathValue = 0;
  for (int i = 0; i < len - 1; i++)
    pathValue +=
        euclDist(cities[visitedCities[i]].c,
                 cities[visitedCities[i + 1]].c);

  return pathValue;
}
int getNumberCitiesInCluster(int clusterId)
{
  int numberCitiesInCluster = 0;
  for (int i = 0; i < numberCities; i++)
    if (mapCityCluster[i] == clusterId)
      numberCitiesInCluster++;

  return numberCitiesInCluster;
}

Neighbor *getNeighbors(int *bestNeighbor, int len)
{
  Neighbor *neighbors = (Neighbor *)calloc(neighborArrayLen(len), sizeof(Neighbor));
  int neighborCount = 0;
  int aux;
  for (int i = 0; i < len - 1; i++)
    for (int j = i + 1; j < len; j++)
    {
      neighbors[neighborCount].len = len;
      neighbors[neighborCount].neighbor = copyArray(bestNeighbor, len);
      aux = neighbors[neighborCount].neighbor[j];
      neighbors[neighborCount].neighbor[j] = neighbors[neighborCount].neighbor[i];
      neighbors[neighborCount].neighbor[i] = aux;
      neighbors[neighborCount].m.i = i;
      neighbors[neighborCount].m.j = j;
      neighborCount++;
    }

  // for (int i = 0; i < len - 1; i++)
  // {
  //   neighbors[i].len = len;
  //   neighbors[i].neighbor = copyArray(bestNeighbor, len);
  //   aux = neighbors[i].neighbor[i + 1];
  //   neighbors[i].neighbor[i + 1] = neighbors[i].neighbor[i];
  //   neighbors[i].neighbor[i] = aux;
  //   neighbors[i].m.i = i;
  //   neighbors[i].m.j = i + 1;
  // }

  return neighbors;
}

void shakePath(int *pathArray, int len)
{
  srand(time(NULL));
  int indx1, indx2, aux;
  for (int i = 0; i < len; i++)
  {
    indx1 = rand() % len;
    indx2 = rand() % len;

    aux = pathArray[indx1];
    pathArray[indx1] = pathArray[indx2];

    pathArray[indx2] = aux;
  }
}
int checkIfMovIsTabu(TabuMoviment tabuList[], int len, TabuMoviment m)
{
  if (len == 0)
    return 0;
  for (int i = 0; i < len; i++)
  {
    if (tabuList[i].i == m.i && tabuList[i].j == m.j)
      return 1;
  }

  return 0;
}

/*********************************************/

/*************** AUX FUNCTIONS ***************/
int euclDist(Coord c1, Coord c2)
{
  return round(sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2)));
}

void RemoveSpaces(string source)
{
  char *i = source;
  char *j = source;
  while (*j != '\0')
  {
    *i = *j++;
    if (*i != ' ')
      i++;
  }
  *i = 0;
}
int *copyArray(int *array, int len)
{
  int *copy = (int *)calloc(len, sizeof(int));
  for (int i = 0; i < len; i++)
    copy[i] = array[i];

  return copy;
}

void printArray(int *array, int len)
{
  for (int i = 0; i < len; i++)
    printf("%d ", array[i]);

  printf("\n");
}

void freeNeighborArray(Neighbor *array, int len)
{
  for (int i = 0; i < len; i++)
    free(array[i].neighbor);
}
int neighborArrayLen(int len)
{
  int sum = 0;
  for (int i = 0; i < len - 1; i++)
    sum += i + 1;
  return sum;
}

void swap(int *x, int *y)
{
  int temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

void permute(int *a, int len, int l)
{
  int i;
  int r = len - 1;
  if (l == r)
    printArray(a, len);
  else
  {
    for (i = l; i <= r; i++)
    {
      swap((a + l), (a + i));
      permute(a, len, l + 1);
      swap((a + l), (a + i)); //backtrack
    }
  }
}

void findNearestCity(int *cts, int len, int idCity)
{  
  for (int i = 0; i < len; i++)
    if (euclDist(cities[idCity].c, cities[cts[0]].c) > euclDist(cities[idCity].c, cities[cts[i]].c))
      swap(cts + i, cts);
}

/*********************************************/