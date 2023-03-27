#ifndef MAP_H
#define MAP_H

//define continent struct
//stores continent name, army bonus associated with each continent
typedef struct Continent {
    char *name;
    int bonus;
}Continent;

//define Province struct
//stores name, number of armies, associated continent, number of connected provinces (up to 8), and pointers to these provinces
typedef struct Province {
    char* name;
    int armies;
    Continent *continent;
    int numBorders;
    struct Province *borders [8]; 
} Province;

//define map struct
//stores pointers to all provinces, continents, and thier respective quantities
typedef struct Map {
    int numCons;
    int numPros;
    Continent **continents;
    Province **provinces;
}Map;

//takes Province referance, prints name, armies, continent, and borders
void printProvince(Province *);

//takes raw string, parese into name and bonus, creates & returns Continent struct
Continent *makeContinent(char *);

//takes name string and a reference to a continent, returns a Province struct 
Province *makeProvince(char *, Continent *);

//Create a valid Map structure by reading from text files
Map *readMapFromTxt(char *);

//function takes Map referance and name of a text file, reads file and parses edge information, connects all verticies
void populateMapEdges(Map *, char *);

//frees all memory dynamically alocated by Map
void clearMap(Map *);
#endif
