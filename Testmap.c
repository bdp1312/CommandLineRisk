#include <stdio.h>
#include "Map.h"

int main(void) {
/*
    Province eastUS;
    eastUS.name="Eastern United States";
    eastUS.armies=0;
    eastUS.continent=NULL;
    for(int i=0;i<8;++i){
        eastUS.borders[i]=NULL;
    }

    Province westUS;
    eastUS.borders[0]=&westUS;

    printProvince(&eastUS);
*/
    Map* testMap = readMapFromTxt("MapFiles/Map0Node.txt");
    for(int i =0;i<testMap->numPros; ++i){printProvince(testMap->provinces[i]);}
    populateMapVertices(testMap, "MapFiles/Map0Vertex.txt");
    return 0;
}
