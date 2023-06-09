#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "Map.h"

//takes Province referance, prints name, armies, continent, and borders
void printProvince(Province *pro)
{
    printf("name:  %s\narmies: %d\ncontinent=%s\n", pro->name, pro->armies, pro->continent->name);
    for(int i=0; i<8; ++i){
        if (pro->borders[i]!=NULL){
            printf("border[%d]=%s\n", i, pro->borders[i]->name);
        }
    }
}

//takes raw string, parese into name and bonus, creates & returns Continent struct
//helper fuction for makeMap
Continent *makeContinent(char *inStr)
{
    Continent *myCont=(Continent *) malloc(sizeof(Continent));
    //parse string by ','
    int i=0;
    while(inStr[i]!=','){
        ++i;
    }
    myCont->name=(char *)malloc(sizeof(char)*i);
    for(int k=0;k<i;k++){
        myCont->name[k]=inStr[k];
    }
    myCont->name[i]='\0';
    ++i;
    int j=0;
    while(inStr[j+i]!='\n'){++j;}
    char numStr[j];
    for(int k = 0; k<j; k++){numStr[k]=inStr[i+k];}
    myCont->bonus=atoi(numStr);
    return myCont;
}
//returns a Province struct, takes name string and a reference to a continent
//helper fucntion for make map
Province *makeProvince(char *nameStr, Continent *con)
{
    Province *myProvince= (Province *)malloc(sizeof(Province));
    //set numBorders to 0
    myProvince->numBorders=0;
    //initialize nameLen, set to 0
    int nameLen=0;
    for(int i=0;nameStr[i]!='\n';++i)
    {
        if(nameStr[i]!='\n'){++nameLen;}
    }
    myProvince->name=(char*)malloc(sizeof(char)*nameLen);
    myProvince->name[nameLen]='\0';
    nameLen=0;
    for(int i=0;nameStr[i]!='\n';++i)
    {
        if(nameStr[i]!='\n'){
            myProvince->name[nameLen]=nameStr[i];
            ++nameLen;
        } 
    }
    //myProvince->name=nameStr;
    //myProvince->name=(char*)malloc(sizeof(*nameStr)-sizeof(char));
    //strcpy(myProvince->name, nameStr);
    //TODO remove errant '\n' from name
    myProvince->continent=con;
    myProvince->armies=0;
    return(myProvince);
}

//Create a valid Map structure by reading from text file
//TODO This fucntion only creates nodes of map, would be faster to combine txt files and create nodes and edges in one function
Map *readMapFromTxt(char *fName){
    //read in maps from file
    FILE * f = (FILE *)fopen(fName, "r");
    //get number of continets, provinces
    
    //tally continents, provincees
    int numCon=0;
    int numPro=0;
    char hasComma;
    //buffer to read file
    char buffer[256];

    //while not EOF
    while(fgets(buffer, 256, f)!=NULL){
        //if buffer is not empty line
        if(buffer[0]!='\n'){
            //check if buffer contains comma
            hasComma='f';
            for(int j=0; buffer[j]!='\0'; j++){
                if(buffer[j]==','){
                    hasComma='t';
                }
            }
            //buffer is comma line if it contains comma
            if(hasComma=='t'){numCon++;}
            //else province
            else{numPro++;}
        }
    }
    //initialize fixed size arrays for map, provinces, continents
    Map *myMap = (Map*) malloc(sizeof(Map));
    myMap->numPros=numPro;
    myMap->numCons=numCon;
    myMap->provinces=malloc(sizeof(Province *)*numPro);
    numPro=0;
    myMap->continents=malloc(sizeof(Continent *)*numCon);
    numCon=0;
    //return to file start
    fseek(f, 0, SEEK_SET);
    //read in conintents, provinces
    //while not eof 
    while(fgets(buffer, 256, f)!=NULL){
        //if buffer is not empty line
        if(buffer[0]!='\n'){
            hasComma='f';
            //check if buffer contains comma
            for(int j=0; buffer[j]!='\0'; j++){
                if(buffer[j]==','){
                    hasComma='t';
                }
            }
            //buffer is comma line if it contains comma
            if(hasComma=='t'){
                //comma read as continent 
                myMap->continents[numCon]=makeContinent(buffer);
                numCon++;
            }
            //else province
            else{
                //else read as province
                myMap->provinces[numPro]=makeProvince(buffer, myMap->continents[numCon-1]); 
                numPro++;
            }
        }
    }
    fclose(f);
    //return return map
    return myMap; 
}

//function takes Map referance and name of a text file, reads file and parses edge information, connects all verticies
//TODO file assume all data is valid, add checks for invalid data
void populateMapEdges(Map *myMap, char *fileName)
{
    //buffer for reading file
    char buffer[256];
    //buffers for province names
    char pro0Name[128];
    char pro1Name[128];
    //startPos & endPos track positions in the file
    int startPos;
    int endPos;
    //referance varriables for the two provinces that are too be connected
    Province *pro1;
    Province *pro0;

    FILE *f = fopen(fileName, "r");        
    //iterate over file by line
    while(fgets(buffer, 256, f)!=NULL){
        pro0=NULL;
        pro1=NULL;
        //can be refactored into function???
        //tokenize stirng by comma
        endPos=0;
        while(buffer[endPos]!=','){++endPos;}
        pro0Name[endPos]='\0';
        for(int i=0;i<endPos;++i){pro0Name[i]=buffer[i];}
        startPos=endPos+1;
        endPos=startPos;
        while(buffer[endPos]!='\n'){++endPos;}
        pro1Name[endPos-startPos]='\0';
        for(int i=0;i<endPos-startPos;++i){
            pro1Name[i]=buffer[startPos+i];    
        }
//    printf("%s, %s\n",pro1Name, pro1Name);
        //find names in myMap->provinces
        //TODO currently uses linear search, faster algorithm could be used
        for(int i=0; i<myMap->numPros;++i){
            if(strcmp(myMap->provinces[i]->name, pro0Name)==0){
                pro0=myMap->provinces[i]; 
            }
            if(strcmp(myMap->provinces[i]->name, pro1Name)==0){
                pro1=myMap->provinces[i];  
            }
            //if both provinces found add each other to thier borders
            if(pro1!=NULL && pro0!=NULL){
               pro0->borders[pro0->numBorders]=pro1;
               ++(pro0->numBorders);
               pro1->borders[pro1->numBorders]=pro0;
               ++(pro1->numBorders);
               break;
            }
        }
    }
    fclose(f);
    return;
}

//frees all memory dynamically alocated by Map
void clearMap(Map *myMap){
    //free continents
    for(int i=0;i<myMap->numCons;++i){
        //free province name, (Not sure why I dynamically allocated it in the first place)
        free(myMap->continents[i]->name);
        free(myMap->continents[i]);
    }
    //free provinces
    for(int i=0;i<myMap->numPros;++i){
        //free province name, still not sure why i didn't pre-alocate
        free(myMap->provinces[i]->name);
        free(myMap->provinces[i]);
    }
    //free map
    free(myMap->continents);
    free(myMap->provinces);
    free(myMap);
    myMap=NULL;
    return;
}
