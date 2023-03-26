#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "Map.h"

void printProvince(Province *pro)
{
    printf("name:  %s\narmies: %d\nprovince=%s\n", pro->name, pro->armies, pro->continent->name);
    for(int i=0; i<8; ++i){
        if (pro->borders[i]!=NULL){
            printf("border[%d]=%s\n", i, pro->borders[i]);
        }
    }
}

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

//helper fucntion for make map
Province *makeProvince(char *nameStr, Continent *con)
{
    Province *myProvince= (Province *)malloc(sizeof(Province));
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

//read file that specifies 
//TODO file assume all data is valid, add checks for invalid data
void populateMapVertices(Map *myMap, char *fileName)
{
    char buffer[256];
    char pro0Name[128];
    char pro1Name[128];
    int startPos;
    int endPos;

    FILE *f = fopen(fileName, "r");        
    while(fgets(buffer, 256, f)!=NULL){
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
    printf("%s, %s\n",pro1Name, pro1Name);
    }
    fclose(f);
    return;
}
