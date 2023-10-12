// Utilities folder
#ifndef UTIL1_H
#define UTIL1_H
#endif

#include "util5_database.h"
#include <ncurses.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define COL_COUNT 80 // number of column of terrain
#define ROW_COUNT 21 // number of row of terrain

//colors for ncurses
#define TALL_GRASS 1
#define SHORT_GRASS 2
#define FOREST 3
#define MOUNTAIN 4
#define WATER 5
#define PATH 6
#define POKEMART 7
#define POKECENTER 8
#define DEFAULT 9

//variables to hold the data parsed from csv files
std::vector<pokemon> pokemonArray;

std::vector<moves> movesArray;

std::vector<pokeMoves> pokemonMovesArray;

std::vector<pokemon_species> pokemonSpeciesArray;

std::vector<experience> expArray;

std::vector<type_names> typeNamesArray;

std::vector<pokemon_stats> pokemonStatsArray;

std::vector<stats> statsArray;
std::map<std::string, int> statsMap;

std::vector<pokemon_types> pokemonTypesArray;

/*************function stubs***************************/
void dijkstra(char terrain[ROW_COUNT][COL_COUNT], int npcType, long distanceMap[ROW_COUNT][COL_COUNT]);
 
/******************************************************/

// return the distance between 2 points
typedef struct point {
  int row;
  int col;
  char element;
} point;

typedef struct pathStruct{
    point *pathing; //array containing each point on the NPC pathing route
    int onPath; //curent spot on path
    int pathLength; //length of path
    int direction; //direction of pathing (to or from)
} pathStruct;

class itemStruct{
    public:
        int quantity;
        std::string name;
};

class pcStruct{
    public:
        int x;
        int y;
        char c;
        std::vector<itemStruct> bag; //vector of usable items
        std::vector<pokemon> pokemons; //vector of owned pokemons
        pcStruct(){ //class constructor
            bag.clear();
            pokemons.clear();
        }
};

class npcStruct: public pcStruct{
    public:
        int nextX; //next x pos
        int nextY; //next y pos
        bool isDefeated; //is NPC defeated
        char terrainType;
        pathStruct path; //path
};

//Function to check if spot is occupied by a certain object(building, character, terrain, etc.)
bool spotTaken(char current, char destination[], int size){
    for(int i = 0; i < size; i++){
        if(current == destination[i]){
            return true;
        }
    }
    return false;
}

void initialize_terminal(){
    initscr();
    start_color();
    init_pair(TALL_GRASS, COLOR_GREEN, COLOR_BLACK);
    init_pair(SHORT_GRASS, COLOR_GREEN, COLOR_BLACK);
    init_pair(FOREST, COLOR_GREEN, COLOR_BLACK);
    init_pair(MOUNTAIN, COLOR_YELLOW, COLOR_BLACK);
    init_pair(WATER, COLOR_CYAN, COLOR_BLACK);
    init_pair(PATH, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(POKEMART, COLOR_RED, COLOR_BLACK);
    init_pair(POKECENTER, COLOR_BLUE, COLOR_BLACK);
    init_pair(DEFAULT, COLOR_WHITE, COLOR_BLACK);
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
}

//Priority Queue for npc movement 
typedef struct pqNode
{
    point data;

    int priority; // lower values = higher priority

    struct pqNode *next;
} pqNode;

// create a new node
pqNode *newNode(point d, int p)
{
    pqNode *temp = (pqNode *)malloc(sizeof(pqNode));
    temp->data = d;
    temp->priority = p;
    temp->next = NULL;

    return temp;
}
// return value at head
point peek(pqNode **head)
{
    return (*head)->data;
}
// removes element with highest priority from list
void pop(pqNode **head)
{
    pqNode *temp = *head;
    (*head) = (*head)->next;
    free(temp);
}
// push according to priority
void push(pqNode **head, point d, int p)
{
    pqNode *start = *head;

    pqNode *temp = newNode(d, p); // create new node

    if ((*head)->priority > p)
    { // new node higher priority than head
        // insert new node before head
        temp->next = *head;
        (*head) = temp;
    }
    else
    {
        while (start->next != NULL && start->next->priority < p)
        {
            start = start->next;
        }

        temp->next = start->next;
        start->next = temp;
    }
}
// check if list empty
int isEmpty(pqNode **head)
{
    return (*head) == NULL;
}


//priority queue for game turn
typedef struct pqGameTurn{
    pcStruct* data;

    int priority; //lower values = higher priority
    int currentTries; //current amount of tries to move to a spot
    struct pqGameTurn* next;
} pqGameTurn;

//Function to create a new node
pqGameTurn* gt_newNode(pcStruct* d, int p){
    pqGameTurn* temp = (pqGameTurn*)malloc(sizeof(pqGameTurn));
    temp->data = d;
    temp->priority = p;
    temp->next = NULL;
    temp->currentTries = 0;
    return temp;
}

pcStruct* gt_peek(pqGameTurn** head){//return value at head
    return(*head)->data;
}

void gt_pop(pqGameTurn** head){//removes element with highest priority from list
    pqGameTurn* temp = *head;
    (*head) = (*head)->next;
    free(temp);
}

void gt_push(pqGameTurn** head, pcStruct* d, int numTries, int p){ //push according to priority
    pqGameTurn* start = *head;

    pqGameTurn* temp = gt_newNode(d, p); //create new node
    temp->currentTries = numTries;

    if((*head)->priority > p){//new node higher priority than head
        //insert new node before head
        temp->next = *head;
        (*head) = temp;
    }
    else{
        while(start->next != NULL && start->next->priority < p){
            start = start->next;
        }

        temp->next = start->next;
        start->next = temp;
    }
}

int gt_isEmpty(pqGameTurn** head){ //check if list empty
    return (*head) == NULL;
}

int gt_getCurrentPriority(pqGameTurn** head){
    return (*head)->priority;
}

int gt_getCurrentTries(pqGameTurn** head){ //return current tries
    return (*head)->currentTries;
}

void gt_setCurrentTries(pqGameTurn** head, int n){ //set current try equals to n
    (*head)->currentTries = n;
}

class worldStruct{
    public:
        char map[ROW_COUNT][COL_COUNT];
        uint8_t height[ROW_COUNT][COL_COUNT];
        int north, south, west, east;
        npcStruct *npcMap[ROW_COUNT][COL_COUNT];
        int num_trainers;
        long hikerDistanceMap[ROW_COUNT][COL_COUNT];
        long rivalDistanceMap[ROW_COUNT][COL_COUNT];
        long swimmerDistanceMap[ROW_COUNT][COL_COUNT];
        pqGameTurn *pqGT;
};

//function to parse csv files
void parseCSV(std::string file){
    std::string path = "";
    char path1[] = "/share/cs327/pokedex";
    char *path2 = getenv("HOME");
    strcat(path2,"/poke327/pokedex");
    char path3[] = "../../../pokedex";

    //Get the path of the database location (1 out of 3 possibilities)
    {
        std::ifstream ifs(path1, std::ifstream::in);
        if(ifs.good()){
            path = path1;
        }
    }

    {
        std::ifstream ifs(path2, std::ifstream::in);
        if(ifs.good()){
            path = path2;
        }
    }

    {
        std::ifstream ifs(path3, std::ifstream::in);
        if(ifs.good()){
            path = path3;
        }
    }

    // cout << path << endl;
    if(path.empty()){ //return with error message if database not found
        cout << "Database not found" << endl;
        return;
    }
    
    path.append("/pokedex/data/csv/");
    path.append(file);
    path.append(".csv");
    std::string line; 
    std::ifstream ifs(path, std::ifstream::in);

    //get number of lines in file
    int lineCount = 0;
    while (std::getline(ifs, line)){
        lineCount++;
    }

    lineCount--; //do not count first row of csv files
    //reset file cursor to start of file
    ifs.clear(); 
    ifs.seekg(0);

    int i = 0;
    bool firstLine = true;
    if(!file.compare("pokemon")){ //case pokemon
        pokemon data[lineCount];
        while (std::getline(ifs, line)){
            if(firstLine){
                firstLine = false;
                continue;
            }

            std::vector<string> values;

            // Create a stringstream from line
            std::stringstream ss(line);
            std::string col;

            // Extract each column data
            while(std::getline(ss, col, ',')){
                if(col.empty()){ //push INT_MAX if empty cell
                    col = "-1";
                }
                values.push_back(col);
            }
            if(ss.eof() && (values.size() == 7)){ //if statement to check if last value is empty
                    col = "-1";
            }

            data[i] = constructPokemon(values);
            i++;
        }

        for(i = 0; i < lineCount; i++){ //add data to vector
            pokemonArray.push_back(data[i]);
        }
    }
    else if(!file.compare("moves")) {//case moves.csv
        moves data[lineCount];
        while (std::getline(ifs, line)){
            if(firstLine){
                firstLine = false;
                continue;
            }

            std::vector<string> values;

            // Create a stringstream from line
            std::stringstream ss(line);
            std::string col;

            // Extract each column data
            while(std::getline(ss, col, ',')){
                if(col.empty()){ //push -1 if empty cell
                    col = "-1";
                }
                values.push_back(col);
            }
            if(ss.eof() && (values.size() == 14)){ //if statement to check if last value is empty
                values.push_back("-1");
            }
            data[i] = constructMoves(values);
            i++;
        }

        for(i = 0; i < lineCount; i++){ //add data to vector
            movesArray.push_back(data[i]);
        }
    }
    else if(!file.compare("pokemon_moves")) {//case pokemon_moves.csv
        std::vector<pokeMoves> data; //use vector for this csv file due to the large size
        while (std::getline(ifs, line)){
            if(firstLine){
                firstLine = false;
                continue;
            }

            std::vector<string> values;

            // Create a stringstream from line
            std::stringstream ss(line);
            std::string col;

            // Extract each column data
            while(std::getline(ss, col, ',')){
                if(col.empty()){ //push -1 if empty cell
                    col = "-1";
                }
                values.push_back(col);
            }
            if(ss.eof() && (values.size() == 5)){ //if statement to check if last value is empty
                values.push_back("-1");
            }
            data.push_back(constructPokeMoves(values));
            i++;
        }

        for(i = 0; i < lineCount; i++){ //copy over data to array
            pokemonMovesArray.push_back(data[i]);
        }
    }
    else if(!file.compare("pokemon_species")) {//case pokemon_species.csv
        pokemon_species data[lineCount];
        while (std::getline(ifs, line)){
            if(firstLine){
                firstLine = false;
                continue;
            }

            std::vector<string> values;

            // Create a stringstream from line
            std::stringstream ss(line);
            std::string col;

            // Extract each column data
            while(std::getline(ss, col, ',')){
                if(col.empty()){ //push -1 if empty cell
                   col = "-1";
                }
                values.push_back(col);
            }
            if(ss.eof() && (values.size() == 19)){ //if statement to check if last value is empty
                values.push_back("-1");
            }
            data[i] = constructSpecies(values);
            i++;
        }

        for(i = 0; i < lineCount; i++){ //copy over data to array
            pokemonSpeciesArray.push_back(data[i]);
        }
    }
    else if(!file.compare("experience")) {//case experience.csv
        experience data[lineCount];
        while (std::getline(ifs, line)){
            if(firstLine){
                firstLine = false;
                continue;
            }

            std::vector<string> values;

            // Create a stringstream from line
            std::stringstream ss(line);
            std::string col;

            // Extract each column data
            while(std::getline(ss, col, ',')){
                if(col.empty()){ //push -1 if empty cell
                    col = "-1";
                }
                values.push_back(col);
            }
            if(ss.eof() && (values.size() == 2)){ //if statement to check if last value is empty
                values.push_back("-1");
            }
            data[i] = constructExperience(values);
            i++;
        }

        for(i = 0; i < lineCount; i++){ //add data to vector
            expArray.push_back(data[i]);
        }
    }
    else if(!file.compare("type_names")) {//case type_names.csv
        type_names data[lineCount];
        while (std::getline(ifs, line)){
            if(firstLine){
                firstLine = false;
                continue;
            }

            std::vector<string> values;

            // Create a stringstream from line
            std::stringstream ss(line);
            std::string col;

            // Extract each column data
            while(std::getline(ss, col, ',')){
                if(col.empty()){ //push -1 if empty cell
                    col = "-1";
                }
                values.push_back(col);
            }
            if(ss.eof() && (values.size() == 2)){ //if statement to check if last value is empty
                values.push_back("-1");
            }
            data[i] = constructTypeNames(values);
            i++;
        }

        for(i = 0; i < lineCount; i++){ //copy over data to array
            typeNamesArray.push_back(data[i]);
        }
    }

    else if(!file.compare("pokemon_stats")) {//case type_names.csv
        pokemon_stats data[lineCount];
        while (std::getline(ifs, line)){
            if(firstLine){
                firstLine = false;
                continue;
            }

            std::vector<string> values;

            // Create a stringstream from line
            std::stringstream ss(line);
            std::string col;

            // Extract each column data
            while(std::getline(ss, col, ',')){
                if(col.empty()){ //push -1 if empty cell
                    col = "-1";
                }
                values.push_back(col);
            }
            if(ss.eof() && (values.size() == 2)){ //if statement to check if last value is empty
                values.push_back("-1");
            }
            data[i] = constructPokeStats(values);
            i++;
        }

        for(i = 0; i < lineCount; i++){ //copy over data to array
            pokemonStatsArray.push_back(data[i]);
        }
    }

    else if(!file.compare("stats")) {//case stats.csv
        stats data[lineCount];
        while (std::getline(ifs, line)){
            if(firstLine){
                firstLine = false;
                continue;
            }

            std::vector<string> values;

            // Create a stringstream from line
            std::stringstream ss(line);
            std::string col;

            // Extract each column data
            while(std::getline(ss, col, ',')){
                if(col.empty()){ //push INT_MAX if empty cell
                    col = "-1";
                }
                values.push_back(col);
            }
            if(ss.eof() && (values.size() == 4)){ //if statement to check if last value is empty
                values.push_back("-1");
            }
            data[i] = constructStat(values);
            i++;
        }

        for(i = 0; i < lineCount; i++){//copy data to array
            statsArray.push_back(data[i]);
        }

        for(i = 0; i < lineCount; i++){//copy data to array
            statsMap.insert(std::pair<std::string, int>(statsArray.at(i).name, statsArray.at(i).id));
        }
    }
    
    else if(!file.compare("pokemon_types")) {//case pokemon_types.csv
        pokemon_types data[lineCount];
        while (std::getline(ifs, line)){
            if(firstLine){
                firstLine = false;
                continue;
            }

            std::vector<string> values;

            // Create a stringstream from line
            std::stringstream ss(line);
            std::string col;

            // Extract each column data
            while(std::getline(ss, col, ',')){
                if(col.empty()){ //push INT_MAX if empty cell
                    col = "-1";
                }
                values.push_back(col);
            }
            if(ss.eof() && (values.size() == 2)){ //if statement to check if last value is empty
                values.push_back("-1");
            }
            data[i] = constructPokeTypes(values);
            i++;
        }

        for(i = 0; i < lineCount; i++){
            pokemonTypesArray.push_back(data[i]);
        }
    }
}

//function to parse specific files in database and store into classes
void getDatabase(){
    std::vector<std::string> files{"pokemon","moves","pokemon_moves","pokemon_species","experience","type_names","pokemon_stats","stats","pokemon_types"};
    
    cout << "Parsing CSV Files to Retrieve Game Data..." << endl;
    int fileSize = files.size();
    for(int i = 0; i < fileSize; i++){
        parseCSV(files[i]);
    }
    cout << "Game Data Retrieved Successfully!" << endl;
    usleep(1250000);
}