// methods for map generation
#ifndef UTIL3_MAP_H
#define UTIL3_MAP_H
#endif

#include "util2_pc.h"

char terrain[ROW_COUNT][COL_COUNT];// initialize a frame of terrain

bool onExitGate(worldStruct *world, int x, int y){
    if((x == world->north && y == 1) || (x == world->south && y == ROW_COUNT - 2) || (y == world->east && x == COL_COUNT - 2) || (y == world->west && x == 1)){
        return true;
    }
    return false;
}

// check if the position is an empty spot
bool isValidPosition(int x, int y, char terrain[ROW_COUNT][COL_COUNT])
{
    if ((x > 0 && x < COL_COUNT - 1) && (y > 0 && y < ROW_COUNT - 1) && terrain[y][x] == ' ')
        return true;

    return false;
}
// populate the area with the symbol of the region
void populateArea(char terrain[ROW_COUNT][COL_COUNT], int startX, int startY, char c)
{
    int i, j, length, widthL, widthR;
    length = rand() % (10 - 2) + 3; // get a random number to decide how long the region extends from the point in the y-axis
    for (j = startY; j < startY + length; j++)
    {                                   // recursion to randomize width for each points
        widthL = rand() % (10 - 2) + 3; // how far does the region extend to the left
        widthR = rand() % (10 - 2) + 3; // how far does the region extend to the right
        for (i = startX; i > startX - widthL; i--)
        {
            if (isValidPosition(i, j, terrain))
                terrain[j][i] = c;
        }
        for (i = startX; i < startX + widthR; i++)
        {
            if (isValidPosition(i, j, terrain))
                terrain[j][i] = c;
        }
    }
}
// to initialize the terrain with ' ' at all the spaces
void initialize_terrain(char terrain[ROW_COUNT][COL_COUNT])
{
    int i, j;
    for (i = 0; i < ROW_COUNT; i++)
    {
        for (j = 0; j < COL_COUNT; j++)
        {
            terrain[i][j] = ' ';
        }
    }
}
// to border the terrain with boulders '%'
void generateBorder(char terrain[ROW_COUNT][COL_COUNT])
{
    int i, j;
    for (i = 0; i < ROW_COUNT; i++)
    {
        for (j = 0; j < COL_COUNT; j++)
        {
            if (i == 0 || i == ROW_COUNT - 1 || j == 0 || j == COL_COUNT - 1)
                terrain[i][j] = '%';
        }
    }
}
// generate the path connecting the north and south exit
void generateNSPath(char terrain[ROW_COUNT][COL_COUNT], int start, int end, bool nEdge, bool sEdge, int stop)
{
    int i, count, difference;
    count = rand() % (13 - 6) + 7;
    difference = end - start;
    if (stop != -1)
    {
        if (nEdge)
        {
            for (i = stop; i < ROW_COUNT; i++) // extend from south exit
            {
                if (terrain[i][end] == '~')
                    terrain[i][end] = '=';
                else
                    terrain[i][end] = '#';
            }
        }
        if (sEdge)
        {
            for (i = 0; i <= stop; i++) // extend from north exit
            {
                if (terrain[i][start] == '~')
                    terrain[i][start] = '=';
                else
                    terrain[i][start] = '#';
            }
        }
    }
    else
    {
        if (nEdge)
        {
            for (i = ROW_COUNT - 2; i > 0; i--) // extend from south exit
            {
                if (terrain[i][end] != '#' && terrain[i][end] != '=')
                    if (terrain[i][end] == '~')
                        terrain[i][end] = '=';
                    else
                        terrain[i][end] = '#';
                else
                    break;
            }
        }
        else if (sEdge)
        {
            for (i = 1; i < ROW_COUNT; i++) // extend from north exit
            {
                if (terrain[i][start] != '#' && terrain[i][start] != '=')
                    if (terrain[i][start] == '~')
                        terrain[i][start] = '=';
                    else
                        terrain[i][start] = '#';
                else
                    break;
            }
        }
        else
        {
            for (i = 0; i < count; i++)
            { // extend from north exit
                if (terrain[i][start] == '~')
                    terrain[i][start] = '=';
                else
                    terrain[i][start] = '#';
            }
            for (i = count; i < ROW_COUNT; i++)
            { // extend from south exit
                if (terrain[i][end] == '~')
                    terrain[i][end] = '=';
                else
                    terrain[i][end] = '#';
            }
            // connect both path
            if (difference > 0)
            {
                for (i = 0; i < difference; i++)
                {
                    if (terrain[count][start + i] == '~')
                        terrain[count][start + i] = '=';
                    else
                        terrain[count][start + i] = '#';
                }
            }
            else
            {
                for (i = 0; i > difference; i--)
                {
                    if (terrain[count][start + i] == '~')
                        terrain[count][start + i] = '=';
                    else
                        terrain[count][start + i] = '#';
                }
            }
        }
    }
}
// generate the path connecting the east and west exit
void generateWEPath(char terrain[ROW_COUNT][COL_COUNT], int start, int end, bool wEdge, bool eEdge)
{
    int i, count, difference;
    count = rand() % (50 - 22) + 23;
    difference = end - start;
    if (wEdge)
    {
        for (i = COL_COUNT - 2; i > 0; i--) // extend from east exit
        {
            if (terrain[end][i] != '#' && terrain[end][i] != '=')
            {
                if (terrain[end][i] == '~')
                    terrain[end][i] = '=';
                else
                    terrain[end][i] = '#';
            }
            else
                break;
        }
    }
    else if (eEdge)
    {
        for (i = 1; i < COL_COUNT; i++) // extend from west exit
        {
            if (terrain[start][i] != '#' && terrain[start][i] != '=')
            {
                if (terrain[start][i] == '~')
                    terrain[start][i] = '=';
                else
                    terrain[start][i] = '#';
            }
            else
                break;
        }
    }
    else
    {
        for (i = 0; i < count; i++) // extend from west exit
        {
            if (terrain[start][i] == '~')
                terrain[start][i] = '=';
            else
                terrain[start][i] = '#';
        }
        for (i = count; i < COL_COUNT; i++) // extend to east exit
        {
            if (terrain[end][i] == '~')
                terrain[end][i] = '=';
            else
                terrain[end][i] = '#';
        }
        // connect both path
        if (difference > 0)
        {
            for (i = 0; i < difference; i++)
            {
                if (terrain[start + i][count] == '~')
                    terrain[start + i][count] = '=';
                else
                    terrain[start + i][count] = '#';
            }
        }
        else
        {
            for (i = 0; i > difference; i--)
            {
                if (terrain[start + i][count] == '~')
                    terrain[start + i][count] = '=';
                else
                    terrain[start + i][count] = '#';
            }
        }
    }
}
// generate exit from the terrain and paths connecting them
void generateExit(worldStruct *world, int north, int south, int west, int east, int curMapRow, int curMapCol)
{
    int stop = -1;
    bool nEdge = true;
    bool sEdge = true;
    bool eEdge = true;
    bool wEdge = true;
    if (curMapRow != 0)
    {
        terrain[0][north] = '#';
        world->north = north;
        nEdge = false;
    }
    if (curMapRow != 400)
    {
        terrain[ROW_COUNT - 1][south] = '#';
        world->south = south;
        sEdge = false;
    }
    if (curMapCol != 0)
    {
        terrain[west][0] = '#';
        world->west = west;
        wEdge = false;
    }
    if (curMapCol != 400)
    {
        terrain[east][COL_COUNT - 1] = '#';
        world->east = east;
        eEdge = false;
    }
    if ((nEdge || sEdge) && (wEdge || eEdge)) // maps at the corner
    {
        if (nEdge) // no north exit
        {
            if (wEdge) // no west exit
                stop = east;
            if (eEdge) // no east exit
                stop = west;
        }
        if (sEdge) // no south exit
        {
            if (wEdge) // no west exit
                stop = east;
            if (eEdge) // no east exit
                stop = west;
        }
        generateNSPath(terrain, north, south, nEdge, sEdge, stop);
        generateWEPath(terrain, west, east, wEdge, eEdge);
        return;
    }
    if (nEdge || sEdge) // if map does not have north or south exit, generate WE path first
    {
        generateWEPath(terrain, west, east, wEdge, eEdge);
        generateNSPath(terrain, north, south, nEdge, sEdge, stop);
        return;
    }
    generateNSPath(terrain, north, south, nEdge, sEdge, stop);
    generateWEPath(terrain, west, east, wEdge, eEdge);
}
// build the Pokemon Center or PokeMart
void generateBuildings(char terrain[ROW_COUNT][COL_COUNT], char building, int curMapRow, int curMapCol)
{
    int x, y, i, j, probability;
    float mapDistance = getDistance(curMapCol - 200, curMapRow - 200, 0, 0);
    float chance = ((-45 * mapDistance) / 200 + 50) / 100;
    bool done = false;
    probability = rand() % 101; // select 0 - 100
    if (mapDistance == 0)
        chance = 1;
    if (mapDistance > 200)
        chance = 0.05;

    if (probability < int(chance * 100))
    {
        while (!done)
        {
            x = rand() % (COL_COUNT - 6) + 3;
            y = rand() % (ROW_COUNT - 6) + 3;

            for (i = 3; i < ROW_COUNT - 3; i++)
            {
                if (terrain[i][x] == '#') // check for a path tile
                {
                    if (terrain[i + 1][x] == '.' && terrain[i + 2][x] == '.') // up
                    {
                        terrain[i + 1][x] = building;
                        terrain[i + 2][x] = building;
                        done = true;
                        return;
                    }
                    if (terrain[i - 1][x] == '.' && terrain[i - 2][x] == '.') // down
                    {
                        terrain[i - 1][x] = building;
                        terrain[i - 2][x] = building;
                        done = true;
                        return;
                    }
                    if (terrain[i][x + 1] == '.' && terrain[i][x + 2] == '.') // right
                    {
                        terrain[i][x + 1] = building;
                        terrain[i][x + 2] = building;
                        done = true;
                        return;
                    }
                    if (terrain[i][x - 1] == '.' && terrain[i][x - 2] == '.') // left
                    {
                        terrain[i][x - 1] = building;
                        terrain[i][x - 2] = building;
                        done = true;
                        return;
                    }
                }
            }
            for (j = 3; j < COL_COUNT - 3; j++)
            {
                if (terrain[y][j] == '#') // check for path
                {
                    if (terrain[y][j + 1] == '.' && terrain[y][j + 2] == '.') // up
                    {
                        terrain[y][j + 1] = building;
                        terrain[y][j + 2] = building;
                        done = true;
                        return;
                    }
                    if (terrain[y][j - 1] == '.' && terrain[y][j - 2] == '.') // down
                    {
                        terrain[y][j - 1] = building;
                        terrain[y][j - 2] = building;
                        done = true;
                        return;
                    }
                    if (terrain[y + 1][j] == '.' && terrain[y + 2][j] == '.') // right
                    {
                        terrain[y + 1][j] = building;
                        terrain[y + 2][j] = building;
                        done = true;
                        return;
                    }
                    if (terrain[y - 1][j] == '.' && terrain[y - 2][j] == '.') // left
                    {
                        terrain[y - 1][j] = building;
                        terrain[y - 2][j] = building;
                        done = true;
                        return;
                    }
                }
            }
        }
    }
}
// select random point at left half of terrain
int *selectPointsL(char terrain[ROW_COUNT][COL_COUNT])
{
    static int pos[2];
    pos[0] = rand() % ((COL_COUNT / 2)) + 1;
    pos[1] = rand() % (ROW_COUNT - 5) + 1;
    return pos;
}
// select random point at right half of terrain
int *selectPointsR(char terrain[ROW_COUNT][COL_COUNT])
{
    static int pos[2];
    pos[0] = rand() % (COL_COUNT - ((COL_COUNT / 2) + 1)) + (1 + (COL_COUNT / 2)); // set one at right half
    pos[1] = rand() % (ROW_COUNT - 5) + 1;
    return pos;
}
// generate 2 regions of tall grass
void generateTallGrass(char terrain[ROW_COUNT][COL_COUNT])
{
    int *pos;
    pos = selectPointsL(terrain);
    populateArea(terrain, pos[0], pos[1], ':');
    pos = selectPointsR(terrain);
    populateArea(terrain, pos[0], pos[1], ':');
}
// generate 2 regions of water
void generateWater(char terrain[ROW_COUNT][COL_COUNT])
{
    int *pos;
    pos = selectPointsL(terrain);
    populateArea(terrain, pos[0], pos[1], '~');
    pos = selectPointsR(terrain);
    populateArea(terrain, pos[0], pos[1], '~');
}
// generate 2 regions of short grass
void generateShortGrass(char terrain[ROW_COUNT][COL_COUNT])
{
    int *pos;
    pos = selectPointsL(terrain);
    populateArea(terrain, pos[0], pos[1], '.');
    pos = selectPointsR(terrain);
    populateArea(terrain, pos[0], pos[1], '.');
}
// generate 2 regions of boulder
void generateBoulders(char terrain[ROW_COUNT][COL_COUNT])
{
    int *pos;
    pos = selectPointsL(terrain);
    populateArea(terrain, pos[0], pos[1], '%');
    pos = selectPointsR(terrain);
    populateArea(terrain, pos[0], pos[1], '%');
}
// generate 2 regions of trees
void generateTrees(char terrain[ROW_COUNT][COL_COUNT])
{
    int *pos;
    pos = selectPointsL(terrain);
    populateArea(terrain, pos[0], pos[1], '^');
    pos = selectPointsR(terrain);
    populateArea(terrain, pos[0], pos[1], '^');
}
// fill empty spaces with short grass '.'
void fillEmpty(char terrain[ROW_COUNT][COL_COUNT])
{
    int i, j;
    for (i = 0; i < ROW_COUNT; i++)
    {
        for (j = 0; j < COL_COUNT; j++)
        {
            if (terrain[i][j] == ' ')
                terrain[i][j] = '.';
        }
    }
}

// generate a terrain
void generateTerrain(worldStruct *world, int north, int south, int west, int east, int curMapRow, int curMapCol)
{
    int i, j;
    initialize_terrain(terrain);
    generateBorder(terrain);
    generateTallGrass(terrain);
    generateWater(terrain);
    generateShortGrass(terrain);
    generateBoulders(terrain);
    generateTrees(terrain);
    generateExit(world, north, south, west, east, curMapRow, curMapCol);
    generateBuildings(terrain, 'M', curMapRow, curMapCol); // PokeMart
    generateBuildings(terrain, 'C', curMapRow, curMapCol); // Pokemon Center
    spawnPlayer(terrain);
    fillEmpty(terrain);

    for (i = 0; i < ROW_COUNT; i++)
    {
        for (j = 0; j < COL_COUNT; j++)
        {
            world->map[i][j] = terrain[i][j];
        }
    }
    generateNPC(world);
}



