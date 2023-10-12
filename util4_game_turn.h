// methods to initiate game turn and handle game turns for the duration of the game
#ifndef UTIL4_GAME_TURN_H
#define UTIL4_GAME_TURN_H
#endif

#include "util3_map.h"

//function to spawn pokemon in tall grass
void spawnPokemon(worldStruct *world){
    //Load random pokemon
    pokemon encounteredPokemon = pokemonArray[rand() % pokemonArray.size()];

    //get manhatten distance from origin
    float mapDistance = 0.0;
    mapDistance = sqrt(pow((curMap.row - 200), 2) + pow(((curMap.col - 200)), 2));

    int minLevel = 1;
    int maxLevel = 1;
    if(mapDistance > 201){ //avoid doing (201 - 200) / 2 which returns 0
        minLevel = (int)((mapDistance - 200) / 2);
        maxLevel = 100;
    }
    else if(mapDistance > 1){ //2 < distance < 200 , also to avoid doing 0 divided by 2 or 1 divided by 2
        maxLevel = mapDistance / 2;
    }


    int noMoves = rand() % 2 + 1; //number of moves the pokemon will learn ( 1 or 2 )
    for(int i = 0; i < noMoves; i++){ //learn either one or two moves
        while(true){
            pokeMoves tmp = pokemonMovesArray[rand() % pokemonMovesArray.size()];
            if((tmp.id == encounteredPokemon.species) && (tmp.pokemon_move_method_id == 1)){
                if(i == 1){ //learning 2nd move
                    //check duplicate as to not learn two same moves
                    if(tmp.move_id == encounteredPokemon.move_set[0].id){
                        continue;
                    }
                }
                (encounteredPokemon.move_set).push_back(movesArray[tmp.move_id - 1]);
                break;
            }
        }
    }

    for(int i = 0; i < (int)pokemonStatsArray.size(); i++){ //get base stats of pokemon
        if(pokemonStatsArray[i].pokemon_id == encounteredPokemon.id){
            encounteredPokemon.base_stats[pokemonStatsArray[i].stat_id - 1] = pokemonStatsArray[i].base_stat;
        }
    }
    //generate IV for pokemon stats (iterate through poke_stats enum)
    for(int i = statsMap.at("hp"); i <= statsMap.at("speed"); i++){
        encounteredPokemon.stats_iv[i - 1] = rand() % 16 + 1; //get IV between range 1 - 15
        encounteredPokemon.stats[i - 1] =  encounteredPokemon.stats_iv[i]; //initialize starting stats as IV
    }

    int pokeLevel = rand() % (maxLevel + 1 - minLevel) + minLevel; //generate random level for pokemon ranging from minLevel to maxLevel
    
    encounteredPokemon.level = 0; //init level as 0
    for(int i = 0; i < pokeLevel; i++){ //level pokemon n times(n = level)
        levelPokemon(&encounteredPokemon);
    }
    //generate gender for pokemon (0 - male, 1 - female)
    encounteredPokemon.gender = rand() % 2;

    //determine if pokemon is shiny or not (1/8192 chance)
    rand() % 8192 == 0 ? encounteredPokemon.is_shiny = 1 : encounteredPokemon.is_shiny = 0; // (0 - no, 1 - yes)

    //set current pokemon hp as hp stat
    encounteredPokemon.currHP = encounteredPokemon.stats[statsMap.at("hp") - 1];

    wildPokemon = encounteredPokemon;
    pokemonBattle(world, NULL);
}


// function to enter a building
int enterBuilding(worldStruct *world, pcStruct *pc)
{
    switch (world->map[pc->y][pc->x])
    {
    case 'C': // poke center
        return 1;
    case 'M': // poke mart
        return 2;
    default: // no building
        return 0;
    }
}

void initialize_gameTurn(pqGameTurn **pqGT, worldStruct *world, int i, int j)
{
    // push all characters into a game turn priority queue
    point neighbors[8];
    point npcSource;
    npcSource.element = world->npcMap[i][j]->c;
    npcSource.col = world->npcMap[i][j]->x;
    npcSource.row = world->npcMap[i][j]->y;
    getNeighbors(npcSource, neighbors, world->map);
    long nextMovement = INT_MAX;
    long tmp;
    int type = 1;

    if (world->npcMap[i][j]->c == 'h')
        type = 0;
    if (world->npcMap[i][j]->c == 'm')
        type = 2;

    if (world->npcMap[i][j]->c == 'h' || world->npcMap[i][j]->c == 'r' || world->npcMap[i][j]->c == 'm')
    { // set nextX and nextY values for hiker/rivals/swimmer
        for (int k = 0; k < 8; k++)
        {
            tmp = getDistanceMap(world, type, neighbors[k].row, neighbors[k].col);
            if (tmp < nextMovement)
            {   // move to the cell with least value
                nextMovement = getDistanceMap(world, type, neighbors[k].row, neighbors[k].col);
                world->npcMap[i][j]->nextX = neighbors[k].col; // update next X pos for pc
                world->npcMap[i][j]->nextY = neighbors[k].row; // update next Y pos for pc
            }
        }
    } // other NPC types would have their own nextX and nextY values already set by other functions on generate NPC

    // push character into queue
    if (!gt_isEmpty(pqGT))
    {
        gt_push(pqGT, world->npcMap[i][j], 0, 0);
    }
    else
    {
        *pqGT = gt_newNode(world->npcMap[i][j], 0);
    }
}

char getTerrainChar(worldStruct *world, int row, int col)
{
    return world->map[row][col];
}

// check whether the adjacent map has been generated before
void checkAdjacentMap(point curMap, worldStruct *world[401][401], int north, int south, int west, int east)
{
    int i;
    if (curMap.row > 0 && world[curMap.row - 1][curMap.col] != NULL)
    {
        for (i = 0; i < COL_COUNT; i++)
        {
            if (getTerrainChar(world[curMap.row - 1][curMap.col], ROW_COUNT - 1, i) == '#')
                north = i; // set the north exit of the map as the south exit of the map to the north
        }
    }
    if (curMap.row < 400 && world[curMap.row + 1][curMap.col] != NULL)
    {
        for (i = 0; i < COL_COUNT; i++)
        {
            if (getTerrainChar(world[curMap.row + 1][curMap.col], 0, i) == '#')
                south = i; // set the north exit of the map as the south exit of the map to the north
        }
    }
    if (curMap.col < 400 && world[curMap.row][curMap.col + 1] != NULL)
    {
        for (i = 0; i < ROW_COUNT; i++)
        {
            if (getTerrainChar(world[curMap.row][curMap.col + 1], i, 0) == '#')
                east = i; // set the north exit of the map as the south exit of the map to the north
        }
    }
    if (curMap.col > 0 && world[curMap.row][curMap.col - 1] != NULL)
    {
        for (i = 0; i < ROW_COUNT; i++)
        {
            if (getTerrainChar(world[curMap.row][curMap.col - 1], i, COL_COUNT - 1) == '#')
                west = i; // set the north exit of the map as the south exit of the map to the north
        }
    }
    world[curMap.row][curMap.col] = (worldStruct *)malloc(sizeof(worldStruct));
    generateTerrain(world[curMap.row][curMap.col], north, south, west, east, curMap.row, curMap.col); // generate new map
}

int movePlayer(int x, int y, worldStruct *world[401][401])
{
    char obstacles[] = "%%^~"; // obstacles not allowed to move on
    int moveType = 0;          // 0 - unsuccessful, 1 - successful, 2 - map change
    int exit, north, south, east, west;

    if (!spotTaken(world[curMap.row][curMap.col]->map[pc->y + y][pc->x + x], obstacles, sizeof(obstacles) / sizeof(obstacles[0])) &&
        world[curMap.row][curMap.col]->npcMap[pc->y + y][pc->x + x] == NULL)
    { // successfully moved to a new spot
        pc->y = pc->y + y;
        pc->x = pc->x + x;
        north = (rand() % (COL_COUNT - 2)) + 1;
        south = (rand() % (COL_COUNT - 2)) + 1;
        west = (rand() % (ROW_COUNT - 2)) + 1;
        east = (rand() % (ROW_COUNT - 2)) + 1;
        
        int min_trainers = 1;
        int max_trainers = 2;
        int mapDistance = getDistance(curMap.col - 200, (curMap.row - 200), 0, 0); //get manhattan distance of map from origin

        if(mapDistance > 0){
            min_trainers += rand() % (mapDistance % 10);
            max_trainers += rand() % (mapDistance % 10); //increase min/max trainers the further the PC is from origin
        }

        num_trainers = rand() % max_trainers + min_trainers; //set number of trainers from min_trainers to max_trainers
        
        moveType = 2;                   // default return type as map change

        if (pc->y == 0)
        {                                                // north exit
            exit = world[curMap.row][curMap.col]->north; // col for south exit of north map
            curMap.row--;
            if (world[curMap.row][curMap.col] == NULL)
            {                 // undiscovered map
                south = exit; // use current map's north exit as new map's south exit
                checkAdjacentMap(curMap, world, north, south, west, east);
            }
            pc->x = exit;
            pc->y = ROW_COUNT - 2;
        }
        else if (pc->y == ROW_COUNT - 1)
        {                                                // south exit
            exit = world[curMap.row][curMap.col]->south; // col for north exit of south map
            curMap.row++;
            if (world[curMap.row][curMap.col] == NULL)
            {                 // undiscovered map
                north = exit; // use current map's south exit as new map's north exit
                checkAdjacentMap(curMap, world, north, south, west, east);
            }
            pc->x = exit;
            pc->y = 1;
        }
        else if (pc->x == 0)
        {                                               // west exit
            exit = world[curMap.row][curMap.col]->west; // col for east exit of west map
            curMap.col--;
            if (world[curMap.row][curMap.col] == NULL)
            {                // undiscovered map
                east = exit; // use current map's west exit as new map's east exit
                checkAdjacentMap(curMap, world, north, south, west, east);
            }
            pc->x = COL_COUNT - 2;
            pc->y = exit;
        }
        else if (pc->x == COL_COUNT - 1)
        {                                               // east exit
            exit = world[curMap.row][curMap.col]->east; // col for west exit of east map
            curMap.col++;
            if (world[curMap.row][curMap.col] == NULL)
            {                // undiscovered map
                west = exit; // use current map's east exit as new map's west exit
                checkAdjacentMap(curMap, world, north, south, west, east);
            }
            pc->x = 1;
            pc->y = exit;
        }
        else
        { // normal movement
            // update NPC distance maps
            dijkstra(world[curMap.row][curMap.col]->map, 0, world[curMap.row][curMap.col]->hikerDistanceMap);
            dijkstra(world[curMap.row][curMap.col]->map, 1, world[curMap.row][curMap.col]->rivalDistanceMap);
            moveType = 1; // set return type as a normal successful movement

            if(world[curMap.row][curMap.col]->map[pc->y][pc->x] == ':'){ //tall grass
                //chance for pokemon spawn
                if(rand() % 10 == 0){ // 1 in 10 chance to spawn pokemon
                    spawnPokemon(world[curMap.row][curMap.col]);
                }
            }
        }
        return moveType; // successfully moved
    }

    if (world[curMap.row][curMap.col]->npcMap[pc->y + y][pc->x + x] != NULL)
    { // moving on an NPC cell
        if (!world[curMap.row][curMap.col]->npcMap[pc->y + y][pc->x + x]->isDefeated)
        { // NPC has not been defeated yet
            // commence Pokemon Battle
            pokemonBattle(world[curMap.row][curMap.col], world[curMap.row][curMap.col]->npcMap[pc->y + y][pc->x + x]);
        }
    }
    return moveType; // unsuccessful move(blocked)
}

// function to display trainers on a new ncurses window
void displayTrainers(worldStruct *world)
{
    int offset = 0;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // get window size and properties of terminal and assign to w
    int listSize = w.ws_row;              // height of current terminal
    char input;
    do
    {
        int count = 0;
        clear();
        int curRow = pc->y;
        int rowUp = -1;
        int rowDown = 1;
        for (int i = 0; i < COL_COUNT; i++)
        {
            if (world->npcMap[curRow][i])
            {
                count++;
                if (count > offset && count <= listSize + offset)
                {
                    printw("%c, ", world->npcMap[curRow][i]->c);
                    if (pc->x - i > 0)
                    { // pc westward
                        printw("%d west\n", pc->x - i);
                    }
                    else
                    { // pc eastward
                        printw("%d east\n", (pc->x - i) * (-1));
                    }
                }
            }
        }
        while (curRow + rowUp > 0 || curRow + rowDown < 20)
        {
            if (curRow + rowUp >= 1)
            { // row above
                for (int i = 0; i < COL_COUNT; i++)
                {
                    if (world->npcMap[curRow + rowUp][i])
                    {
                        count++;
                        if (count > offset && count <= listSize + offset)
                        {
                            printw("%c, ", world->npcMap[curRow + rowUp][i]->c);
                            printw("%d north", rowUp * -1);
                            if (pc->x - i > 0)
                            { // pc westward
                                printw(" and %d west\n", pc->x - i);
                            }
                            else if (pc->x - i < 0)
                            { // pc eastward
                                printw(" and %d east\n", (pc->x - i) * (-1));
                            }
                            else
                            { // same x axis
                                printw("\n");
                            }
                        }
                    }
                }
            }
            if (curRow + rowDown < 20)
            { // row below
                for (int i = 0; i < COL_COUNT; i++)
                {
                    if (world->npcMap[curRow + rowDown][i])
                    {
                        count++;
                        if (count > offset && count <= listSize + offset)
                        {
                            printw("%c, ", world->npcMap[curRow + rowDown][i]->c);
                            printw("%d south", rowDown);
                            if (pc->x - i > 0)
                            { // pc westward
                                printw(" and %d west\n", pc->x - i);
                            }
                            else if (pc->x - i < 0)
                            { // pc eastward
                                printw(" and %d east\n", (pc->x - i) * (-1));
                            }
                            else
                            { // same x axis
                                printw("\n");
                            }
                        }
                    }
                }
            }
            rowUp--;
            rowDown++;
        }
        input = getch();

        if (input == 2)
        { // scroll down (DOWN ARROW KEY)
            if (count - listSize - (offset + 1) >= 0)
            { // not out of list range
                offset++;
            }
            continue;
        }
        else if (input == 3)
        { // scroll up (UP ARROW KEY)
            if (offset - 1 >= 0)
            { // not out of list range
                offset--;
            }
            continue;
        }
        else if (input == 27)
        { // ESC key
            break;
        }
        else
        {
            continue;
        }
    } while (input != 27); // not escape key
}

void printTerrain(worldStruct *world, char msg[80])
{
    clear();
    printw("%s\n", msg);
    int i, j;
    for (i = 0; i < ROW_COUNT; i++)
    {
        for (j = 0; j < COL_COUNT; j++)
        {
            switch (world->map[i][j])
            {
            case ':': // tall grass
                attron(COLOR_PAIR(TALL_GRASS));
                break;
            case '.': // short grass
                attron(COLOR_PAIR(SHORT_GRASS));
                break;
            case '^': // forest
                attron(COLOR_PAIR(FOREST));
                break;
            case '%': // mountain
                attron(COLOR_PAIR(MOUNTAIN));
                break;
            case '~': // water
                attron(COLOR_PAIR(WATER));
                break;
            case '#':
                attron(COLOR_PAIR(PATH));
                break;
            case '=':
                attron(COLOR_PAIR(PATH));
                break;
            case 'M':
                attron(COLOR_PAIR(POKEMART));
                break;
            case 'C':
                attron(COLOR_PAIR(POKECENTER));
                break;
            default: // white color for the rest
                attron(COLOR_PAIR(DEFAULT));
                break;
            }
            if (i == pc->y && j == pc->x)
            { // pc location
                attron(COLOR_PAIR(DEFAULT));
                printw("@");
                continue;
            }
            if (world->npcMap[i][j])
            {
                attron(COLOR_PAIR(DEFAULT));
                printw("%c", world->npcMap[i][j]->c);
                continue;
            }
            printw("%c", world->map[i][j]);

            switch (world->map[i][j])
            {
            case ':': // tall grass
                attroff(COLOR_PAIR(TALL_GRASS));
                break;
            case '.': // short grass
                attroff(COLOR_PAIR(SHORT_GRASS));
                break;
            case '^': // forest
                attroff(COLOR_PAIR(FOREST));
                break;
            case '%': // mountain
                attroff(COLOR_PAIR(MOUNTAIN));
                break;
            case '~': // water
                attroff(COLOR_PAIR(WATER));
                break;
            case '#':
                attroff(COLOR_PAIR(PATH));
                break;
            case '=':
                attroff(COLOR_PAIR(PATH));
                break;
            case 'M':
                attroff(COLOR_PAIR(POKEMART));
                break;
            case 'C':
                attroff(COLOR_PAIR(POKECENTER));
                break;
            default: // white color for the rest
                attroff(COLOR_PAIR(DEFAULT));
                break;
            }
        }
        printw("\n");
    }
}

// Execute game
void execute_gameTurn(pqGameTurn *pqGT, worldStruct *world, worldStruct *world_master_struct[401][401])
{
    bool quitGame = false;
    npcStruct *u;
    char inputMapCoordinate[2][10];
    bool valid = true;
    bool hasSign;
    int x, y, north, south, east, west;

    north = rand() % (COL_COUNT - 2) + 1;
    south = rand() % (COL_COUNT - 2) + 1;
    west = rand() % (ROW_COUNT - 2) + 1;
    east = rand() % (ROW_COUNT - 2) + 1;

    while (!quitGame)
    {
        u = (npcStruct *)gt_peek(&pqGT); // get next character in queue
        int currentTime = gt_getCurrentPriority(&pqGT);
        int type;
        char msg[80] = "";
        int playerMove = 0;

        switch (u->c)
        {
        case 'h':
            type = 0; // 0 - hiker
            break;
        case 'm':
            type = 2; // 2 - swimmer
            break;
        default:
            type = 1; // 1 - rival and others
            break;
        }

        if (u->c == '@')
        { // PC
            char input = getch();
            switch (input)
            {
            case '7': // upper left
            case 'y':
                playerMove = movePlayer(-1, -1, world_master_struct);
                break;
            case '8': // up
            case 'k':
                playerMove = movePlayer(0, -1, world_master_struct);
                break;
            case '9': // upper right
            case 'u':
                playerMove = movePlayer(1, -1, world_master_struct);
                break;
            case '6': // right
            case 'l':
                playerMove = movePlayer(1, 0, world_master_struct);
                break;
            case '3': // lower right
            case 'n':
                playerMove = movePlayer(1, 1, world_master_struct);
                break;
            case '2': // down
            case 'j':
                playerMove = movePlayer(0, 1, world_master_struct);
                break;
            case '1': // lower left
            case 'b':
                playerMove = movePlayer(-1, 1, world_master_struct);
                break;
            case '4': // left
            case 'h':
                playerMove = movePlayer(-1, 0, world_master_struct);
                break;
            case '>': // attempt to enter pokemart/pokecenter
                clear();
                if (enterBuilding(world_master_struct[curMap.row][curMap.col], pc) == 1)
                {   
                    std::string message;
                    strcpy(msg, "Exited Pokemon Center!");
                    initialize_terminal();
                    message = "You are in a pokemon center!\n\n";
                    message += "Restoring your pokemon...\n";

                    for(int i =0; i < (int)pc->pokemons.size(); i++)
                    {
                        message = message + pc->pokemons[i].name + "HP: " + std::to_string(pc->pokemons[i].currHP) + " -> "+ std::to_string(pc->pokemons[i].stats[statsMap.at("hp")-1]) + "\n";
                        pc->pokemons[i].currHP = pc->pokemons[i].stats[statsMap.at("hp")-1];
                    }

                    message += "\nAll Pokemon restored. Press '<' to exit.";
                    printMessage(message);
                    
                    while (getch() != '<')
                    {
                        clear();
                        printMessage("You are in a pokemon center! Press '<' to exit.\n");
                    }
                    clear();
                    endwin();
                }
                else if (enterBuilding(world_master_struct[curMap.row][curMap.col], pc) == 2)
                {
                    
                    std::string message;
                    strcpy(msg, "Exited PokeMart!");
                    initialize_terminal();
                    message = "You are in a PokeMart!\n\n";
                    
                    message += "Restroring items in bags...\n";
                    message = message + "Number of potions in bag: " + std::to_string(pc->bag[potion].quantity) + " -> 50\n";
                    pc->bag[potion].quantity = 50; //set potions to 50
                    
                    message = message + "Number of revives in bag: " + std::to_string(pc->bag[revive].quantity) + " -> 50\n";
                    pc->bag[revive].quantity = 50; //set revives to 50
                    
                    message = message + "Number of pokeballs in bag: " + std::to_string(pc->bag[pokeball].quantity) + " -> 50\n";
                    pc->bag[pokeball].quantity = 50; //set pokeballs to 50
                    
                    message += "\nBag restored! Press '<' to exit.";
                    printMessage(message);
                    while (getch() != '<')
                    {
                        clear();
                        printMessage("You are in a PokeMart! Press '<' to exit.\n");
                    }
                    clear();
                    endwin();
                }
                else
                {
                    strcpy(msg, "No building in proximity!");
                    printTerrain(world_master_struct[curMap.row][curMap.col], msg);
                    continue;
                }
                break;
            case '5': // rest for a turn, NPCs still move
            case '.':
            case ' ':
                playerMove = movePlayer(0, 0, world_master_struct);
                break;
            case 'B': //open bag
                open_bag(false);
                playerMove = 0; //consider as unsuccessful move
                break;
            case 't': // display list of trainers
                clear();
                initialize_terminal();
                displayTrainers(world_master_struct[curMap.row][curMap.col]);
                clear();
                endwin();
                playerMove = 0; // considered as an unsuccessful move
                break;
            case 'f':
                while (true)
                { 
                    clear();
                    if (!valid)
                        printMessage("Invalid input. Coordindates shoule be between -200 to 200.\n");
                    initialize_terminal();
                    echo();
                    printMessage("Enter the x-coordinate of the map to fly to: \n");
                    getstr(inputMapCoordinate[0]);
                    printMessage("Enter the y-coordinate of the map to fly to: \n");
                    getstr(inputMapCoordinate[1]);
                    valid = true;
                    if (strlen(inputMapCoordinate[0]) < 1 || strlen(inputMapCoordinate[0]) > 4 || strlen(inputMapCoordinate[1]) < 1 || strlen(inputMapCoordinate[1]) > 4 ||
                        atoi(inputMapCoordinate[0]) < -200 || atoi(inputMapCoordinate[0]) > 200 || atoi(inputMapCoordinate[1]) < -200 || atoi(inputMapCoordinate[1]) > 200)
                    {
                        valid = false;
                        continue;
                    }
                    hasSign = false;
                    x = strlen(inputMapCoordinate[0]);
                    for (int i = 0; i < x; i++)
                    {
                        if (inputMapCoordinate[0][0] == '-' || inputMapCoordinate[0][0] == '+')
                        {
                            hasSign = true;
                        }
                        if (hasSign && strlen(inputMapCoordinate[0]) == 1)
                        {
                            valid = false;
                            break;
                        }
                        if (hasSign)
                        {
                            continue;
                        }
                        if (!isdigit(inputMapCoordinate[0][i]))
                        {
                            valid = false;
                            break;
                        }
                    }
                    hasSign = false;
                    y = strlen(inputMapCoordinate[1]);
                    for (int i = 0; i < y; i++)
                    {
                        if (inputMapCoordinate[1][0] == '-' || inputMapCoordinate[1][0] == '+')
                        {
                            hasSign = true;
                        }
                        if (hasSign && strlen(inputMapCoordinate[1]) == 1)
                        {
                            valid = false;
                            break;
                        }
                        if (hasSign)
                        {
                            continue;
                        }
                        if (!isdigit(inputMapCoordinate[1][i]))
                        {
                            valid = false;
                            break;
                        }
                    }
                    curMap.col = atoi(inputMapCoordinate[0]) + 200;
                    curMap.row = atoi(inputMapCoordinate[1]) + 200;
                    clear();
                    endwin();
                    if (world_master_struct[curMap.row][curMap.col] == NULL)
                    { // undiscovered map
                        checkAdjacentMap(curMap, world_master_struct, north, south, west, east);
                    }
                    playerMove = 2;
                    int col = curMap.col - 200;
                    int row = curMap.row - 200;
                    sprintf(msg, "Flying to map: (%d, %d)", col, row);
                    initialize_terminal();
                    printTerrain(world_master_struct[curMap.row][curMap.col], msg);
                    break;
                }
                break;
            case 'Q': // quit game
                endwin();
                quitGame = true;
                break;
            default:
                continue;
            }

            if (playerMove && !quitGame)
            { // sucessful move
                if (playerMove == 1)
                {                                                                                                                           // normal move
                    gt_push(&pqGT, pc, 0, getNodeCost(world_master_struct[curMap.row][curMap.col]->map[pc->y][pc->x], type) + currentTime); //re-add it into queue with new priority
                    gt_pop(&pqGT);
                }
                else if (playerMove == 2)
                {                                                        // map change
                    world = world_master_struct[curMap.row][curMap.col]; // update current map
                    while (!gt_isEmpty(&pqGT))
                    { // clear game turn queue
                        gt_pop(&pqGT);
                    }
                    pqGT = gt_newNode(pc, 0); // re-initialize priority queue
                                              // add each pc to priority queue
                    for (int i = 0; i < ROW_COUNT; i++)
                    {
                        for (int j = 0; j < COL_COUNT; j++)
                        {
                            if (world->npcMap[i][j])
                            {
                                initialize_gameTurn(&pqGT, world, i, j);
                            }
                        }
                    }
                }
            }

            if (!quitGame)
            {
                printTerrain(world_master_struct[curMap.row][curMap.col], msg);
            }
            continue;
        }

        if (u->isDefeated)
        {                  // npc is defeated
            gt_pop(&pqGT); // remove npc from queue
            printTerrain(world, msg);
            continue;
        }

        if (u->c == 's') // sentry
        {
            if (u->nextY == pc->y && u->nextX == pc->x) {// trying to move on pc cell
                pokemonBattle(world, u);          // poke-battle
            }
            gt_push(&pqGT, world->npcMap[u->y][u->x], 0, getNodeCost(world->map[u->nextY][u->nextX], type) + currentTime); // re-add it into queue with new priority
            gt_pop(&pqGT);
            printTerrain(world, msg);
            continue;
        }

        if (u->c == 'p') // pacer
        {
            if (world->npcMap[u->nextY][u->nextX] != NULL || (u->nextX == pc->x && u->nextY == pc->y) || onExitGate(world, u->nextX, u->nextY))
            {                                               // destination cell is occupied
                if (u->nextX == pc->x && u->nextY == pc->y){ // poke battle if attempt to move to PC cell
                    pokemonBattle(world, u);
                }
                gt_push(&pqGT, u, 0, getNodeCost(world->map[u->nextY][u->nextX], type) + currentTime); // re-add it into queue with new priority
                gt_pop(&pqGT);
                printTerrain(world, msg);
                continue;
            }
            else
            {                                                                               // move pacer to next spot (copy entire data to new location)
                world->npcMap[u->nextY][u->nextX] = new npcStruct;
                // define pc x,y,c values
                world->npcMap[u->nextY][u->nextX]->c = u->c;
                world->npcMap[u->nextY][u->nextX]->x = u->nextX;
                world->npcMap[u->nextY][u->nextX]->y = u->nextY;
                world->npcMap[u->nextY][u->nextX]->isDefeated = u->isDefeated;
                world->npcMap[u->nextY][u->nextX]->pokemons = u->pokemons;

                int originalDirection = u->path.direction; // store original direction into a variable
                int newSpotDirection;                      // variable to hold new location's direction

                // define the new spot's direction value
                if (world->npcMap[u->y][u->x]->path.onPath == (u->path.pathLength) - 2 && u->path.direction == 1) // was on second last spot of pathing
                    newSpotDirection = -1;                                                                        // means it is currently at the end of path, walk back to original spot (direction negative)

                else if (world->npcMap[u->y][u->x]->path.onPath == 1 && u->path.direction == -1) // was one spot from returning to original spot
                    newSpotDirection = 1;                                                        // means it is currently back at the start of the path, walk away from original spot (direction positive)

                else // continue along path, u->path.direction is unchanged
                    newSpotDirection = u->path.direction;

                // copy over path properties
                world->npcMap[u->nextY][u->nextX]->path.onPath = world->npcMap[u->y][u->x]->path.onPath + originalDirection; // set new spot's onPath value = current path + current direction

                // copy over path.direction and path.length properties
                world->npcMap[u->nextY][u->nextX]->path.direction = newSpotDirection;
                world->npcMap[u->nextY][u->nextX]->path.pathLength = u->path.pathLength;

                // malloc new memory for path array
                world->npcMap[u->nextY][u->nextX]->path.pathing = (point *)malloc(sizeof(point) * u->path.pathLength);
                for (int p = 0; p < u->path.pathLength; p++)
                { // deep copy over path array
                    world->npcMap[u->nextY][u->nextX]->path.pathing[p].col = u->path.pathing[p].col;
                    world->npcMap[u->nextY][u->nextX]->path.pathing[p].row = u->path.pathing[p].row;
                }

                // set nextX and nextY values for newly generated cell
                world->npcMap[u->nextY][u->nextX]->nextX = u->path.pathing[world->npcMap[u->nextY][u->nextX]->path.onPath + newSpotDirection].col;
                world->npcMap[u->nextY][u->nextX]->nextY = u->path.pathing[world->npcMap[u->nextY][u->nextX]->path.onPath + newSpotDirection].row;

                // set existing spot to null
                world->npcMap[u->y][u->x] = NULL;
            }

            // update move cost and push into priority queue
            int moveCost = getNodeCost(world->map[world->npcMap[u->nextY][u->nextX]->nextY][world->npcMap[u->nextY][u->nextX]->nextX], type) + currentTime;
            gt_push(&pqGT, world->npcMap[u->nextY][u->nextX], 0, moveCost);
            gt_pop(&pqGT);
            printTerrain(world, msg);
            continue;
        }

        if (u->c == 'w') // wanderer
        {
            char obstacles[] = "%%^~MCs"; // change directions if encounter any of these cells
            int targetX = u->x + u->nextX;
            int targetY = u->y + u->nextY;
            if (targetX == pc->x && targetY == pc->y)
            { // poke battle if moving to pc cell
                pokemonBattle(world, u);
                printTerrain(world, msg);
                continue;
            }
            else if (world->npcMap[targetY][targetX] != NULL || onExitGate(world, u->nextX, u->nextY))
            {
                setWanderer(world, u);
            }
            else if (spotTaken(world->map[targetY][targetX], obstacles, sizeof(obstacles) / sizeof(obstacles[0])) || world->map[targetY][targetX] != u->terrainType)
            { // wanderer hits a sentry or a different terrain
                // stop and walk in a new direction
                setWanderer(world, u); // find a direction with same terrain type
            }
            else
            {                                                                             // move wanderer to new pos
                world->npcMap[targetY][targetX] = new npcStruct;
                world->npcMap[targetY][targetX]->c = u->c;
                world->npcMap[targetY][targetX]->x = (targetX);
                world->npcMap[targetY][targetX]->y = (targetY);
                world->npcMap[targetY][targetX]->nextX = u->nextX;
                world->npcMap[targetY][targetX]->nextY = u->nextY;
                world->npcMap[targetY][targetX]->terrainType = u->terrainType;
                world->npcMap[targetY][targetX]->isDefeated = u->isDefeated;
                world->npcMap[u->nextY][u->nextX]->pokemons = u->pokemons;

                world->npcMap[u->y][u->x] = NULL; // null existing spot
                // push to priority queue
                gt_push(&pqGT, world->npcMap[targetY][targetX], 0, getNodeCost(world->map[targetY][targetX], 1) + currentTime);
                gt_pop(&pqGT);
                printTerrain(world, msg);
                continue;
            }

            // push to priority queue
            gt_push(&pqGT, u, 0, getNodeCost(world->map[targetY][targetX], 1) + currentTime);
            gt_pop(&pqGT);
            printTerrain(world, msg);
            continue;
        }

        if (u->c == 'e') // explorer
        {
            char obstacles[] = "MCs%%^~"; // change directions if encounter any of these cells
            int targetX = u->x + u->nextX;
            int targetY = u->y + u->nextY;

            if ((targetX > 0 && targetX < COL_COUNT - 1) && (targetY > 0 && targetY < ROW_COUNT - 1))
            { // not going into a border
                if (targetX == pc->x && targetY == pc->y)
                { // poke battle if moving to pc cell
                    pokemonBattle(world, u);
                    printTerrain(world, msg);
                    continue;
                }
                else if (world->npcMap[targetY][targetX] != NULL || onExitGate(world, u->nextX, u->nextY))
                { // colliding with another npc
                    setExplorer(world, u);
                }
                else if (spotTaken(world->map[targetY][targetX], obstacles, sizeof(obstacles) / sizeof(obstacles[0])))
                {
                    setExplorer(world, u);
                }
                else
                {                                                                             // move to new pos
                    world->npcMap[targetY][targetX] = new npcStruct;
                    world->npcMap[targetY][targetX]->c = u->c;
                    world->npcMap[targetY][targetX]->x = (targetX);
                    world->npcMap[targetY][targetX]->y = (targetY);
                    world->npcMap[targetY][targetX]->nextX = u->nextX;
                    world->npcMap[targetY][targetX]->nextY = u->nextY;
                    world->npcMap[targetY][targetX]->isDefeated = u->isDefeated;
                    world->npcMap[u->nextY][u->nextX]->pokemons = u->pokemons;

                    world->npcMap[u->y][u->x] = NULL; // null existing spot

                    // push to priority queue
                    gt_push(&pqGT, world->npcMap[targetY][targetX], 0, getNodeCost(world->map[targetY][targetX], 1) + currentTime);
                    gt_pop(&pqGT);
                    printTerrain(world, msg);
                    continue;
                }
            }
            else
            {
                setExplorer(world, u);
            }

            gt_push(&pqGT, u, 0, getNodeCost(world->map[targetY][targetX], 1) + currentTime); // re-add it into queue with new priority
            gt_pop(&pqGT);
            printTerrain(world, msg);
            continue;
        }

        if (u->c == 'm') // swimmer
        {
            char obstacles[] = "%%^:.MCs"; // change directions if encounter any of these cells
            int targetX = u->x + u->nextX;
            int targetY = u->y + u->nextY;

            if (targetX == pc->x && targetY == pc->y)
            { // poke battle if moving to pc cell
                pokemonBattle(world, u);
                printTerrain(world, msg);
                continue;
            }
            else if (world->npcMap[targetY][targetX] != NULL || onExitGate(world, u->nextX, u->nextY))
            {
                setSwimmer(world, u);
            }
            else if (spotTaken(world->map[targetY][targetX], obstacles, sizeof(obstacles) / sizeof(obstacles[0])) ||
                     (world->map[targetY][targetX] != u->terrainType && world->map[targetY][targetX] != '='))
            { // swimmer hits a sentry or a different terrain
                // stop and walk in a new direction
                setSwimmer(world, u); // find a direction with same terrain type
            }
            else
            {                                                                             // move swimmer to new pos
                world->npcMap[targetY][targetX] = new npcStruct;
                world->npcMap[targetY][targetX]->c = u->c;
                world->npcMap[targetY][targetX]->x = (targetX);
                world->npcMap[targetY][targetX]->y = (targetY);
                world->npcMap[targetY][targetX]->nextX = u->nextX;
                world->npcMap[targetY][targetX]->nextY = u->nextY;
                world->npcMap[targetY][targetX]->terrainType = u->terrainType;
                world->npcMap[targetY][targetX]->isDefeated = u->isDefeated;
                world->npcMap[u->nextY][u->nextX]->pokemons = u->pokemons;

                world->npcMap[u->y][u->x] = NULL; // null existing spot
                // gt_setCurrentTries(&pqGT, 0); //reset current tries
                // push to priority queue
                gt_push(&pqGT, world->npcMap[targetY][targetX], 0, getNodeCost(world->map[targetY][targetX], 2) + currentTime);
                gt_pop(&pqGT);
                printTerrain(world, msg);
                continue;
            }
            // push to priority queue
            gt_push(&pqGT, u, 0, getNodeCost(world->map[targetY][targetX], 2) + currentTime);
            gt_pop(&pqGT);
            printTerrain(world, msg);
            continue;
        }
        if (u->nextX == pc->x && u->nextY == pc->y)
        { // poke battle if attempt to move to PC cell
            pokemonBattle(world, u);
            // re-add character into queue
            if (!gt_isEmpty(&pqGT))
            {
                gt_push(&pqGT, u, 0, getNodeCost(world->map[u->nextY][u->nextX], type) + currentTime);
            }
            else
            {
                pqGT = gt_newNode(world->npcMap[u->nextY][u->nextX], getNodeCost(world->map[u->nextY][u->nextX], type) + currentTime);
            }
        }
        else if (world->npcMap[u->nextY][u->nextX] != NULL || onExitGate(world, u->nextX, u->nextY)) // destination cell is occupied
        {
            // re-add it to queue with new movement cost
            int k;
            int moveCost = getNodeCost(world->map[u->nextY][u->nextX], type) + currentTime;
            if (!(u->nextX == pc->x && u->nextY == pc->y))
            {                                                             // have not reached pc (Clashing with another NPC or object)
                gt_setCurrentTries(&pqGT, gt_getCurrentTries(&pqGT) + 1); // increase attempts to move by 1
                if (gt_getCurrentTries(&pqGT) > 3)
                { // more than 3 attempts to move to the same spot
                    // get cell value of neighbors again
                    point neighbors[8];
                    point npcSource;
                    npcSource.element = u->c;
                    npcSource.col = u->x;
                    npcSource.row = u->y;
                    getNeighbors(npcSource, neighbors, world->map);

                    // store occupied cell into new variable as u->nextX and u->nextY will be updated in upcoming for loop
                    point occupiedPos;
                    occupiedPos.col = u->nextX;
                    occupiedPos.row = u->nextY;

                    // find the lowest cell value, excluding the one that the NPC is currently trying to move into
                    long nextMovement = INT_MAX;
                    for (k = 0; k < 8; k++)
                    {
                        if (neighbors[k].row == occupiedPos.row && neighbors[k].col == occupiedPos.col)
                        { // avoid trying to move to the same position again
                            continue;
                        }
                        if (getDistanceMap(world, type, neighbors[k].row, neighbors[k].col) < nextMovement)
                        { // get next lowest movement cell
                            nextMovement = getDistanceMap(world, type, neighbors[k].row, neighbors[k].col);
                            world->npcMap[u->y][u->x]->nextX = neighbors[k].col;
                            world->npcMap[u->y][u->x]->nextY = neighbors[k].row;
                            moveCost = getNodeCost(world->map[neighbors[k].row][neighbors[k].col], type) + currentTime;
                        }
                    }

                    gt_setCurrentTries(&pqGT, 0); // reset current tries
                }
            }

            gt_push(&pqGT, world->npcMap[u->y][u->x], gt_getCurrentTries(&pqGT), moveCost);
        }
        else // move pc to new position
        {
            int k;
            gt_setCurrentTries(&pqGT, 0); // reset current tries on move
            world->npcMap[u->nextY][u->nextX] = new npcStruct;
            world->npcMap[u->nextY][u->nextX]->c = u->c;
            world->npcMap[u->nextY][u->nextX]->x = u->nextX;
            world->npcMap[u->nextY][u->nextX]->y = u->nextY;
            world->npcMap[u->nextY][u->nextX]->isDefeated = u->isDefeated;
            world->npcMap[u->nextY][u->nextX]->pokemons = u->pokemons;
            world->npcMap[u->y][u->x] = NULL;

            gt_setCurrentTries(&pqGT, 0); // reset current tries
            if (u->c == 'r' || u->c == 'h')
            { // if hiker or rival or swimmer
                point neighbors[8];
                point npcSource;
                npcSource.element = u->c;
                npcSource.col = u->nextX;
                npcSource.row = u->nextY;
                getNeighbors(npcSource, neighbors, world->map);
                long nextMovement = INT_MAX;
                point nextPos;
                for (k = 0; k < 8; k++)
                {
                    if (getDistanceMap(world, type, neighbors[k].row, neighbors[k].col) < nextMovement)
                    { // get next lowest movement cell
                        nextMovement = getDistanceMap(world, type, neighbors[k].row, neighbors[k].col);
                        world->npcMap[u->nextY][u->nextX]->nextX = neighbors[k].col;
                        world->npcMap[u->nextY][u->nextX]->nextY = neighbors[k].row;
                        nextPos.col = neighbors[k].col;
                        nextPos.row = neighbors[k].row;
                    }
                }

                // push character into queue
                int moveCost = getNodeCost(world->map[nextPos.row][nextPos.col], type) + currentTime;
                if (!gt_isEmpty(&pqGT))
                {
                    gt_push(&pqGT, world->npcMap[u->nextY][u->nextX], 0, moveCost);
                }
                else
                {
                    pqGT = gt_newNode(world->npcMap[u->nextY][u->nextX], moveCost);
                }
            }
        }
        gt_pop(&pqGT); // remove it from queue
        printTerrain(world, msg);
    }
}
