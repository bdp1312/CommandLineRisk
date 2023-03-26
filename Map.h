#ifndef MAP_H
#define MAP_H

typedef struct Continent {
    char *name;
    int bonus;
}Continent;

typedef struct Province {
    char* name;
    int armies;
    Continent *continent;
    int numBorders;
    struct Province *borders [8]; 
} Province;

typedef struct Map {
    int numCons;
    int numPros;
    Continent **continents;
    Province **provinces;
}Map;

void printProvince(Province *);
Continent *makeContinent(char *);
Province *makeProvince(char *, Continent *);
Map *readMapFromTxt(char *);
void populateMapVertices(Map *, char *);
void clearMap(Map *);
#endif
