#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef char* string;

typedef struct coord {
    int x, y;
}Coord;

typedef struct city {
    int id;
    Coord c;
}City;


typedef struct cluster{
    int id;
    Coord c;
    int radius;
}Cluster;

typedef struct smallerDist{
    int clusterId;
    int dist;
}SmallerDist;

typedef struct tabuMoviment {
    int i,j;    
}TabuMoviment;

typedef struct neighbor {
    int *neighbor;
    int len;
    TabuMoviment m;
}Neighbor;

typedef struct valueId{
    int id;
    int value;
}ValueId;

#endif