//main method
#include "util4_game_turn.h"

#define mapxy(x, y) (world->map[y][x])
#define heightxy(x, y) (world->height[y][x])
#define heightpair(pair) (world->height[pair[dim_y]][pair[dim_x]])

int main(int argc, char *argv[])
{
    int i, j, north, south, west, east;
    char msg[80] = "";
    srand(time(NULL));

    num_trainers = rand() % 4 + 1;
    
    // select a random point at the 4 cardinal direction of the terrain for the exit
    north = rand() % (COL_COUNT - 2) + 1;
    south = rand() % (COL_COUNT - 2) + 1;
    west = rand() % (ROW_COUNT - 2) + 1;
    east = rand() % (ROW_COUNT - 2) + 1;

    //generate player
    pc = (pcStruct *)malloc(sizeof(pcStruct)); //initialize PC
    pc->c = '@';
    initialize_bag(pc);
    pc->bag[potion].quantity = 50; //set starting potions to 50
    pc->bag[revive].quantity = 50; //set starting revives to 50
    pc->bag[pokeball].quantity = 50; //set starting pokeballs to 50

    worldStruct *world[401][401];
    for (int i = 0; i < 401; i++)
    {
        for (int j = 0; j < 401; j++)
        {
            world[i][j] = NULL;
        }
    }
    curMap.row = 200;
    curMap.col = 200;
    world[curMap.row][curMap.col] = (worldStruct *)malloc(sizeof(worldStruct));
    world[curMap.row][curMap.col]->num_trainers = num_trainers;
    getDatabase(); 
    generateTerrain(world[curMap.row][curMap.col], north, south, west, east, curMap.row, curMap.col); 
    pqGameTurn *pqGT;
    pqGT = gt_newNode(pc,0); //add pc to priority queue
    for (int i = 0; i < ROW_COUNT; i++)
    {
        for (int j = 0; j < COL_COUNT; j++)
        {
            if (world[curMap.row][curMap.col]->npcMap[i][j])
            {
                initialize_gameTurn(&pqGT, world[curMap.row][curMap.col], i, j);
            }
        }
    }
    setStartingPokemon(pc ,curMap);
    initialize_terminal();
    printTerrain(world[curMap.row][curMap.col], msg);
    set_escdelay(0); //set delay of esc key to 0
    execute_gameTurn(pqGT, world[curMap.row][curMap.col], world);
    
    cout << "GAME END" << endl;

    for (i = 0; i < 401; i++)
    {
        for (j = 0; j < 401; j++)
        {
            if (world[i][j] == NULL)
            {
                continue;
            }
            else
            {
                if (world[i][j]->num_trainers > 0)
                { // if map has trainers, free them
                    for (int k = 0; k < ROW_COUNT; k++)
                    {
                        for (int l = 0; l < COL_COUNT; l++)
                        {
                            if (world[i][j]->npcMap[k][l] != NULL)
                            {
                                free(world[i][j]->npcMap[k][l]);
                                world[i][j]->npcMap[k][l] = NULL;
                            }
                        }
                    }
                }
                free(world[i][j]);
                world[i][j] = NULL;
            }
        }
    }
    free(pc);
    return 0;
}