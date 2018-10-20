#include "headers.h"
#include "functions.h"
#include "structs.h"
#include "globals.h"

int main(int argc, string argv[])
{
  int minArgs = 2;
  if (argc != minArgs)
  {
    printf("ERROR: INVALID NUMBER OF ARGUMENTS\nEXPECTED %d\nOBTAINED: %d\n",minArgs,argc);    
    exit(1);
  }
  readCities();
  numberClusters = atoi(argv[1]);
  createClusters();
  kMeans();  
  tabuSearch(20,1000,20); 

  // int a[4] = { 1,2,3,4};
  // int *b = a + 1;

  // printf("%d\n",b[1]);
  free(mapCityCluster);
  free(cities);
  free(clusters);


  return EXIT_SUCCESS;
}