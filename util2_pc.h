//methods for pc and npc spawning and movement
#ifndef UTIL2_PC_H
#define UTIL2_PC_H
#endif

#include "util1.h"

int num_trainers;// initialize variable to store number of trainers per map
int num_hiker, num_rival, num_pacer, num_wanderer, num_sentry, num_explorer, num_swimmer; // initialize global variable to store number each NPCs

const char *pokemon_gender_header[2] = {"Male", "Female"};
enum bag_items{potion, revive, pokeball, swap_poke}; //enum to hold bag items or actions to use on pokemon

pcStruct *pc;
point curMap;
pokemon wildPokemon;//keep track of wild pokemon that spawns in tall grass

std::vector<std::string> trainerPrefix {"Artist", "Athlete", "Cyclist", "Dancer", "Biker", "Bird Keeper", "Baker", "Bug Catcher", "Chef", "Coach"};
std::vector<std::string> trainerName {"Aaron", "Agatha", "Alder", "Bertha", "Brendan", "Brock", "Bruno", "Byron", "Caitlin", "Cheren", "Chuck", "Cress", "Cynthia", "Erika"};


float getDistance(int x, int y, int x1, int y1)
{
    float distance = 0.00;
    distance = sqrt(pow(x - 0, 2) + pow(y - 0, 2));
    return distance;
}


void printMessage(std::string msg){
    int index = 0;
    
    if(msg.compare("\n\n") != 0){ //text animation
            while(msg[index]){
                clear();
                for(int i = 0; i <= index; i++){
                    string s(1, msg.at(i));
                    printw(s.c_str());
                }
                refresh();
                index++;
                usleep(35000);
                flushinp(); //flush user input while message is printing
            }
        }
        else{
            printw("\n\n");
        }
}

//function to print pokemon encounter details
void pokemonEncounter(pokemon encounteredPoke, int numMoves, std::string msg){
    //char input;
        clear();
        msg += "\nLevel " + std::to_string(encounteredPoke.level) + " " + encounteredPoke.name + "\n\n";
        msg += "Moveset: \n";
        for(int i = 0; i < numMoves; i++){ //print moves
            msg += "\t> " + encounteredPoke.move_set[i].name + "\n";
        }

        msg += "\t\t";
        for(int i = statsMap.at("hp"); i <= statsMap.at("speed"); i++){ //print pokemon base stats header
            msg += "\t";
            msg += statsArray.at(i-1).name;
        }

        msg += "\nBase Stats:\t"; 
        for(int i = statsMap.at("hp"); i <= statsMap.at("speed"); i++){ //print pokemon base stats value
            msg += "\t";
            if(i == 5 || i == 6)
                msg += "\t";
            msg += std::to_string(encounteredPoke.base_stats[i-1]);
        }

        msg += "\nIndividual Value(IV):"; //print IV for pokemon stats
        for(int i = statsMap.at("hp"); i <= statsMap.at("speed"); i++){
            msg += "\t";
            if(i == 5 || i == 6)
                msg += "\t";
            msg += std::to_string(encounteredPoke.stats_iv[i-1]);
        }

        msg += "\nActual Stats:\t"; 
        for(int i = statsMap.at("hp"); i <= statsMap.at("speed"); i++){ //print pokemon stats value
            msg += "\t";
            if(i == 5 || i == 6)
                msg += "\t";
            msg += std::to_string(encounteredPoke.stats[i-1]);
        }

        msg += "\n\n";

        msg += "Gender: "; //print pokemon gender
        msg += pokemon_gender_header[encounteredPoke.gender];
        msg += "\n\n";

        msg += "Shiny: "; //print pokemon is shiny or not
        encounteredPoke.is_shiny ? msg += "Yes" : msg += "No";

        printw(msg.c_str());
        printw("\n");
        getch();
}
void screenTransition(){
    
    initscr();
    noecho();
    curs_set(FALSE);

    int x = 0;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); //get window size and properties of terminal and assign to w

    switch(rand()%2){ //random pick between 2 different transitions
        case 0:
            for(int i = 0; i < w.ws_col; i++){
                
                clear(); 

                for(int j = 0; j < w.ws_row; j++){
                    mvprintw(j, x, "|"); 
                }

                refresh();

                usleep(7500); 
                x++;
            }
            break;
        case 1:
            x = w.ws_col - 1;
            for(int i = 0; i < w.ws_col; i++){
                
                clear(); 

                for(int j = 0; j < w.ws_row; j++){
                    mvprintw(j, x, "|"); 
                }

                refresh();

                usleep(7500); 
                x--;
            }
            break;
    }

    endwin();
}
//function to initialize bag of character
void initialize_bag(pcStruct *trainer){
    itemStruct potion;
    itemStruct revive;
    itemStruct pokeball;

    potion.name = "potion";
    revive.name = "revive";
    pokeball.name = "pokeball";

    potion.quantity = revive.quantity = pokeball.quantity = 0;

    //push back according to bag_items Enum
    trainer->bag.push_back(potion);
    trainer->bag.push_back(revive);
    trainer->bag.push_back(pokeball);
}

//function to calculate pokemon move damage
int getMoveDamage(pokemon poke,moves move){
    float damage = ((float)(2 * poke.level) / 5) + 2;
    float critical = 1, stab = 1, type_modifier = 1;
    float rand_modifier = ((rand() % 16) + 85) / (float)100; //0.85 to 1.0 modifier
    if(rand() % 256 < poke.base_stats[statsMap.at("speed")-1] / 2)
        critical = 1.5;

    for(int i = 0; i < (int)pokemonTypesArray.size(); i++){
        if(pokemonTypesArray[i].pokemon_id == poke.id && pokemonTypesArray[i].type_id == move.type_id){ //Same Type Attack Bonus (STAB)
            stab = 1.5;
        }

        if(pokemonTypesArray[i].pokemon_id > poke.id){ //id is sorted in ascending order
            //can stop searching if searched passed the id it is looking for
            break;
        }
    }

    // if() //TODO: Implement type_modifier of 0, .25, .5, 1, 2, 4 in asmt1-09

    damage = damage * (move.power <= 0 ? 1 : move.power) * ((float)poke.stats[statsMap.at("attack")-1] / (float)poke.stats[statsMap.at("defense")-1]);
    damage = (damage / 50) + 2;
    damage = damage * critical * rand_modifier * stab * type_modifier;

    return (int)damage;
}

//function to throw pokeball and catch pokemon
int throw_pokeball(pokemon poke){
    int return_status = 0;
    pc->bag[pokeball].quantity--;

    if(pc->pokemons.size() >= 6){ //full pokemon slots

    }
    else{
        pc->pokemons.push_back(poke); //add pokemon to collection
        // pc->pokemons.
        return_status = 1;
    }

    return return_status; 
}

//function to get list of pokemons
int open_pokemons(pcStruct *trainer, int poke_action, bool in_battle){
    int return_status = 0;

    clear();
    initialize_terminal();

    int active = 0;//active selection
    char input;
    bool action_status = false;
    std::string bagMsg = "My Pokemons, 'esc' to return.\n\n";

    if(poke_action == swap_poke){
        bagMsg = "Change to which pokemon?\n\n";
    }

    do{
        clear();
        printw(bagMsg.c_str());
        bagMsg = "My Pokemons, 'esc' to return.\n\n";
        if(poke_action == swap_poke){
            bagMsg = "Change to which pokemon?\n";
        }

        if(return_status && in_battle){ //used item on pokemon / swap pokemon during battle
            printw("\nPress any key to return.\n");
            input = getch(); //get any user input
            input = 27;
            continue;
            // break; //exit loop
        }
        
        for(int i = 0; i < (int)trainer->pokemons.size(); i++){
            if(i == active)
                printw("-> ");
            printw(trainer->pokemons[i].name.c_str());
            printw("\tLevel ");
            printw(std::to_string(trainer->pokemons[i].level).c_str());
            printw(" ");
            printw(trainer->pokemons[i].gender ? "M" : "F");
            printw("\n");
            if(i == active)
                printw("   ");
            printw("HP ");
            printw(std::to_string(trainer->pokemons[i].currHP).c_str());
            printw("/");
            printw(std::to_string(trainer->pokemons[i].stats[statsMap.at("hp")-1]).c_str());
            printw("\n\n");
        }

        input = getch();

        if(input == 27 && in_battle && pc->pokemons[0].currHP <= 0){ //trying to esc when in_battle
            //prevent player from returning to fight screen if pokemon had fainted
            bagMsg += "\n";
            input = -1;
            continue; 
        }
        switch(input){
            case 2: // (DOWN ARROW KEY)
                if(active == (int)trainer->pokemons.size() - 1){ //last pokemon
                    bagMsg += "\n";
                    continue;
                }
                else
                    active++;
                action_status = false;
                break; 
            case 3: // (UP ARROW KEY)
                if(active == 0){ //first pokemon
                    bagMsg += "\n";
                    continue;
                }
                else
                    active--;
                action_status = false;
                break; 
            case 10: //ENTER KEY
                if(trainer->pokemons[active].currHP < trainer->pokemons[active].stats[statsMap.at("hp")-1] && poke_action == potion && trainer->pokemons[active].currHP != 0){ //use potion
                    trainer->pokemons[active].stats[statsMap.at("hp")-1] < 20 ? trainer->pokemons[active].currHP = trainer->pokemons[active].stats[statsMap.at("hp")-1] : trainer->pokemons[active].currHP += 20;
                    std::string message;
                    message = "Successfully recovered " + trainer->pokemons[active].name + " by 20 HP.\n";
                    getch();
                    printMessage(message);
                    
                    if(trainer->pokemons[active].currHP > trainer->pokemons[active].stats[statsMap.at("hp")-1])
                    {
                        trainer->pokemons[active].currHP = trainer->pokemons[active].stats[statsMap.at("hp")-1];
                    }
                    
                    trainer->bag[potion].quantity--;
                    return_status = 1;
                }
                else if(poke_action == potion){ //failed to use potion
                    std::string message;
                    message = "Failed to use potion on " + trainer->pokemons[active].name + "\n";
                    printMessage(message);
                    getch();
                }
                else if(poke_action == swap_poke){ //swap pokemons (during battle)
                    if(active != 0 && trainer->pokemons[active].currHP > 0){ //swapping to a different pokemon that still has HP
                        //swap pokemons
                        pokemon tmp = trainer->pokemons[0];
                        trainer->pokemons[0] = trainer->pokemons[active];
                        trainer->pokemons[active] = tmp;
                        return_status = 1;
                        std::string message;
                        message = "Successfully swapped pokemons!\n";
                        printMessage(message);
                        getch();
                        break;
                    }
                    else{ //swapping to a fainted pokemon
                        bagMsg += "You can't swap to that pokemon.\n";
                        action_status = false;
                    }
                }
                else if(poke_action == revive && trainer->pokemons[active].currHP == 0){ //revive on fainted pokemon
                    trainer->pokemons[active].currHP = trainer->pokemons[active].stats[statsMap.at("hp")-1];
                    std::string message;
                    message = "Successfully revived " + trainer->pokemons[active].name + ".\n";
                    printMessage(message);
                    getch();
                    trainer->bag[revive].quantity--;
                    return_status = 1;
                }
                else if(poke_action == revive){ //failed to use revive
                    std::string message;
                    message = "Failed to use revive on " + trainer->pokemons[active].name + ".\n";
                    printMessage(message);
                    getch();
                }
                action_status = true;
                break;
            default:
                action_status = false;
                break;
        }

        action_status ? bagMsg += "" : bagMsg += "\n";
        action_status = false;

    }while(input != 27); //esc key to escape
    clear();
    endwin();

    return return_status;
}

//function to open bag
int open_bag(bool in_battle){

    int return_status = 0;
    clear();
    initialize_terminal();
    int active = 0; //active selection
    char input;
    do{
        clear();
        printw("Personal Bag, 'esc' to return.\n\n");
        for(int i = 0; i < (int)pc->bag.size(); i++){
            if(i == active)
                printw("-> ");
            printw(pc->bag[i].name.c_str());
            printw(" x");
            printw(std::to_string(pc->bag[i].quantity).c_str());
            printw("\n");
        }
        input = getch();
        switch(input){
            case 2: // (DOWN ARROW KEY)
                if(active == (int)pc->bag.size() - 1) //last item
                    continue;
                else
                    active++;
                break; 
            case 3: // (UP ARROW KEY)
                if(active == 0) //first item
                    continue;
                else
                    active--;
                break; 
            case 10: //ENTER KEY
                if(pc->bag[active].quantity > 0){ 
                    switch(active){
                        case potion: //potion
                            return_status = open_pokemons(pc, potion, in_battle);
                            break;
                        case revive: //revive
                            return_status = open_pokemons(pc, revive, in_battle);
                            break;
                        case 2: //pokeball
                            if(pc->bag[pokeball].quantity > 0)
                                return_status = 2; //throw pokeball
                            break;
                    }
                }
                break;
        }

        if(return_status && in_battle){ //if used item on pokemon during battle
            break; //return to battle
        }
    }while(input != 27); //esc to escape

    clear();
    endwin();

    return return_status;
}
// function to commence a pokemon battle interface
void pokemonBattle(worldStruct *world, npcStruct *enemyTrainer){

    screenTransition();
    clear();
    initialize_terminal();
    
    enum actions {fight, pkmn, pack, run};
    // char buttons[] = {'a','b','c','d'};

    int escapeAttempts = 0;
    bool escaped = false;
    int attempt_catch = 0; //flag to check if successfully caught pokemon
    pokemon *currPokemon = &pc->pokemons[0];
    pokemon *enemyPokemon;

    int percentageHP;
    char input;

    std::string battleMsg;
    std::string playerBattleMsg;
    std::string enemyTrainerName;

    if(!enemyTrainer){ //wild Pokemon
        enemyPokemon = &wildPokemon;
        battleMsg = "A wild " + enemyPokemon->name + " has appeared!\n\n";
    }
    else{ //trainer Battle
        enemyPokemon = &enemyTrainer->pokemons[0];
        battleMsg = "Enemy trainer wants to battle!\n\n";
        enemyTrainerName = trainerPrefix[rand() % (int) trainerPrefix.size()];
        enemyTrainerName += " " + trainerName[rand() % (int) trainerName.size()];
    }

    int active = 0;

    do{ 
        clear();

        printMessage(battleMsg);

        battleMsg = "";
        int attempt_swap = 0, attempt_item = 0; //flag to check if trainer swapped poke or used item during battle
        int enemyTrainerSwap = false;
        if(enemyPokemon->currHP == 0 || escaped || attempt_catch){ //enemy pokemon fainted / successfully escaped / successfully caught
            if(enemyTrainer && enemyTrainer->pokemons.size() > 1){ //if enemy trainer's current pokemon has fainted
                for(int i = 1; i < (int)enemyTrainer->pokemons.size(); i++){
                    if(enemyTrainer->pokemons[i].currHP > 0){ //swap active pokemon with another one that has not yet fainted
                        pokemon tmp = enemyTrainer->pokemons[0];
                        enemyTrainer->pokemons[0] = enemyTrainer->pokemons[i];
                        enemyTrainer->pokemons[i] = tmp;
                        enemyPokemon = &enemyTrainer->pokemons[0];
                        enemyTrainerSwap = true;
                        break;
                    }
                }

                if(!enemyTrainerSwap){ //enemy trainer no remaining pokemon alive
                    std::string message;
                    message = "\nEnemy trainer ran out of pokemons and is defeated!";
                    printMessage(message);
                    enemyTrainer->isDefeated = true; //set trainer is defeated
                }
                else{ //enemy trainer swapped pokemons
                    battleMsg += "Enemy trainer swapped pokemons!\n\n"; 
                    input = getch();
                    continue;
                }
            }
            else if(enemyTrainer){
                std::string message;
                message = "\nEnemy trainer ran out of pokemons and is defeated!";
                printMessage(message);
                enemyTrainer->isDefeated = true; //set trainer is defeated
            }
            printw("\nPress any key to exit");
            input = getch();
            break; //end battle
        }

        if(currPokemon->currHP == 0){ //current PC pokemon fainted
            input = getch(); //pause for user input
            bool allFainted = true;
            //check if there are any other ones to swap to
            for(int i = 0; i < (int)pc->pokemons.size(); i++){
                if(pc->pokemons[i].currHP > 0){
                    allFainted = false;
                    break;
                }
            }

            if(allFainted){ //no pokemons left
                std::string message;
                message = "\nYou have ran out of pokemons";
                message += "\nPress any key to exit";
                printMessage(message);
                input = getch();
                break;
            }
            else{ //swap pokemon
                attempt_swap = open_pokemons(pc, swap_poke, true);
                battleMsg += "Successfully swapped pokemons!\n\n";
                currPokemon = &pc->pokemons[0]; //update curr pokemon to latest pokemon
                continue;
            }
        }

        //print opponent pokemon details
        if(enemyTrainer){ //trainer battle
            //print enemy trainer name and number of pokemons
            // printw("\n");
            printw(enemyTrainerName.c_str());
            int pokemonsAlive = 0;
            int pokemonsFainted = 0;
            for(int i = 0; i < (int)enemyTrainer->pokemons.size(); i++){
                if(enemyTrainer->pokemons[i].currHP == 0)
                    pokemonsFainted++;
                else
                    pokemonsAlive++;
            }
            printw("\n");
            for(int i = 0; i < (int)enemyTrainer->pokemons.size(); i++){
                printw("  _   ");
            }
            printw("\n");
            for(int i = 0; i < pokemonsAlive; i++){
                printw("(-o-) ");
            }
            for(int i = 0; i < pokemonsFainted; i++){
                printw("(-x-) ");
            }
            printw("\n\n");
        }
        printw(enemyPokemon->name.c_str());
        printw("\n\t:Level ");
        printw(std::to_string(enemyPokemon->level).c_str());
        printw(enemyPokemon->gender ? "M" : "F");
        printw("\n");

        percentageHP = std::ceil((float) enemyPokemon->currHP / float(enemyPokemon->stats[statsMap.at("hp") -1]) * 100 / 10);

        printw("|");
        for(int i = 0; i < percentageHP; i++ ){
            printw("-");
        }
        for(int i = 0; i < 10 - percentageHP; i++){
            printw(" ");//pad missing hp
        }
        printw("|\n");
        printw(std::to_string(enemyPokemon->currHP).c_str());
        printw("/");
        printw(std::to_string(enemyPokemon->stats[statsMap.at("hp") - 1]).c_str());
        printw("\n\n");

        //print player's pokemon details
        printw(currPokemon->name.c_str());
        printw("\n\t:Level ");
        printw(std::to_string(currPokemon->level).c_str());
        printw(currPokemon->gender ? "M" : "F");
        printw("\n");

        percentageHP = std::ceil((float) currPokemon->currHP / float(currPokemon->stats[statsMap.at("hp") - 1]) * 100 / 10);

        printw("|");
        for(int i = 0; i < percentageHP; i++ ){
            printw("-");
        }
        for(int i = 0; i < 10 - percentageHP; i++){
            printw(" ");//pad missing hp
        }
        printw("|\n");
        printw(std::to_string(currPokemon->currHP).c_str());
        printw("/");
        printw(std::to_string(currPokemon->stats[statsMap.at("hp") - 1]).c_str());
        printw("\n\n");

        switch(active){
            case 0:
                printw("-> Fight\tPkmn\n");
                printw("Pack\t\tRun\n");
                break;
            case 1:
                printw("Fight\t\tPkmn\n");
                printw("-> Pack\t\tRun\n");
                break;
            case 2:
                printw("Fight\t\t-> Pkmn\n");
                printw("Pack\t\tRun\n");
                break;
            case 3:
                printw("Fight\t\tPkmn\n");
                printw("Pack\t\t-> Run\n");
                break;
        }
        // printw("-> Fight (A)\t-> Pkmn (B)\n");
        // printw("-> Pack  (C)\t-> Run  (D)\n");
        
        input = getch();

        // testing - end battle with esc key
        // if(input == 27){
        //     break;
        // }
        
        moves currPokeMove;
        int currPokeMoveDmg = 0;

        int fight_active = 0;
        switch(input){
            case 4: //Arrow KEY_RIGHT
                battleMsg = "\n\n";
                if(active >= 2)
                    active -= 2;
                continue;
                // break;
            case 5: //Arrow KEY_LEFT
                battleMsg = "\n\n";
                if(active < 2)
                    active += 2;
                continue;
                // break;
            case 3: //Arrow KEY_UP
                battleMsg = "\n\n";
                if(active == 1 || active == 3)
                    active--;
                continue;
                // break;
            case 2: //Arrow KEY_DOWN
                battleMsg = "\n\n";
                if(active == 0 || active == 2)
                    active++;
                continue;
                // break;
            case 10: //ENTER key
                switch(active){
                    case 0: //FIGHT
                        enum button_fight {A, B, C, D};
                        do{
                            WINDOW *fight_window = newwin(6,20,10,32);
                            refresh();
                            mvwprintw(stdscr, 9, 32, "Choose a move");
                            box(fight_window,0,0);
                            for(int i = 0; i < (int) currPokemon->move_set.size(); i++){
                                if(i == fight_active)
                                    mvwprintw(fight_window, 1 + i, 1, "-> ");
                                if(i == fight_active)
                                    mvwprintw(fight_window, 1 + i, 1 + 3, currPokemon->move_set[i].name.c_str());
                                else
                                    mvwprintw(fight_window, 1 + i, 1, currPokemon->move_set[i].name.c_str());
                            }
                            refresh();
                            wrefresh(fight_window);
                            input = getch();
                            switch(input){
                                case 2: // (DOWN ARROW KEY)
                                    if(fight_active == (int) currPokemon->move_set.size() - 1){ //last item
                                    refresh();
                                    wrefresh(fight_window);
                                        delwin(fight_window);
                                        endwin();
                                        continue;
                                    }
                                    else
                                        fight_active++;
                                    break; 
                                case 3: // (UP ARROW KEY)
                                    if(fight_active == 0){ //first item
                                    refresh();
                                    wrefresh(fight_window);
                                        delwin(fight_window);
                                        endwin();
                                        continue;
                                    }
                                    else
                                        fight_active--;
                                    break; 
                                case 10: //ENTER KEY
                                    escapeAttempts = 0; //reset escape attempt if a pokemon makes a move
                                    currPokeMove = currPokemon->move_set[fight_active];
                                    playerBattleMsg = currPokemon->name + " used " + currPokemon->move_set[fight_active].name;
                                    
                                    int moveAcc;
                                    
                                    currPokemon->move_set[fight_active].accuracy <= 0 ? moveAcc = 100 : moveAcc = currPokemon->move_set[fight_active].accuracy;
                                    if(rand() % 100 > moveAcc){ //if random number generated more than move accuracy
                                        //miss
                                        playerBattleMsg += ", it missed\n";
                                    }
                                    else{ //hit
                                        currPokeMoveDmg = getMoveDamage(*currPokemon, currPokemon->move_set[fight_active]);

                                        playerBattleMsg += ", it did " + std::to_string(currPokeMoveDmg) + " damage\n";
                                    }
                                    refresh();
                                    wrefresh(fight_window);
                                    delwin(fight_window);
                                    endwin();
                                    break;
                            }
                            delwin(fight_window);

                            endwin();
                        }while(input != 27 && input != 10); //exit when esc/enter is pressed

                        if(input == 27){
                            battleMsg = "\n\n";
                            continue;
                        }
                        break;
                    case 1://Back Pack
                        attempt_item = open_bag(true);
                    
                        if(attempt_item == 2 && enemyTrainer){ //trainer battle
                            battleMsg = "You can't use that in a trainer battle\n\n";
                            continue;
                        }
                        else if(attempt_item == 2){ //threw a pokeball
                            attempt_catch = throw_pokeball(*enemyPokemon);
                            if(attempt_catch)
                                playerBattleMsg += "Successfully caught wild " + enemyPokemon->name;
                            else{
                                playerBattleMsg += "Oh no, wild " + enemyPokemon->name + " broke free!\n";
                                attempt_catch = 2; //broke free
                            }
                        }
                        else if(attempt_item){
                            playerBattleMsg = "Successfully used item!\n";
                        }
                        else{
                            battleMsg = "\n\n";
                            continue;
                        }
                        break;
                    case 2: //Pokemon List
                        attempt_swap = open_pokemons(pc, swap_poke, true);
                        if(attempt_swap){
                            playerBattleMsg += "Successfully swapped pokemons!\n";
                            currPokemon = &pc->pokemons[0]; //update curr pokemon to latest pokemon
                        }
                        else{
                            battleMsg = "\n\n";
                            continue;
                        }
                        break;
                    case 3://Run
                        if(enemyTrainer){ //trainer battle
                            battleMsg = "You can't run from a trainer battle\n\n";
                            continue;
                        }
                        else{ //wild pokemon
                            escapeAttempts++;
                            float oddEscape = pc->pokemons[0].stats[statsMap.at("speed") - 1] * 32; //speed of trainer's active pokemon multiplied by 32
                            oddEscape = oddEscape / ((enemyPokemon->stats[statsMap.at("speed") - 1] / 4) % 256);
                            oddEscape = oddEscape + 30;
                            oddEscape = oddEscape * escapeAttempts;

                            if((int) oddEscape > 255){
                                battleMsg = "You have successfully escaped\n\n";
                                escaped = true; //automatically escapes if odds > 255
                                continue;
                            }
                            else{
                                int tryEscape = rand() % 256; //generate number between 0 - 255
                                if(tryEscape < oddEscape){ //if random number generated is lower than the odds of escape
                                    escaped = true; //escape
                                    battleMsg = "You have successfully escaped\n\n";
                                    continue;
                                }
                                else{
                                    playerBattleMsg = "You failed to escape\n";
                                    break;
                                }
                            }
                            battleMsg = "\n\n";
                            continue;
                        }
                        break;
                }
            break;
            default:
                battleMsg = "\n\n";
                continue;
        }

        moves enemyPokemonMove = enemyPokemon->move_set[rand() % (int)enemyPokemon->move_set.size()];
        int enemyPokemonMoveDmg = getMoveDamage(*enemyPokemon, enemyPokemonMove);
        
        std::vector<int> attack_turn; //0 - pc turn, 1 - opponent's turn
        if(currPokeMove.priority > enemyPokemonMove.priority || 
            ((currPokeMove.priority == enemyPokemonMove.priority) && currPokemon->stats[statsMap.at("speed") - 1] > enemyPokemon->stats[statsMap.at("speed") - 1]) ||
            attempt_swap || attempt_item || attempt_catch){
            //PC goes first
            attack_turn.push_back(0); 
            attack_turn.push_back(1); 
        }
        else{ //opponent goes first
            attack_turn.push_back(1);
            attack_turn.push_back(0);
        }

        bool pokemonDown = false;
        int pokemonCaught = attempt_catch;
        for(int i = 0; i < 2; i++){
            
            if(pokemonDown){ //if pokemon has fainted
                break; //exit for loop
            }
            
            if(pokemonCaught == 2){ //pokemon broke free
                battleMsg += playerBattleMsg;
                battleMsg += "Wild " + enemyPokemon->name + " has escaped!\n";
                break;
            }
            else if(pokemonCaught){ //caught pokemon
                battleMsg += playerBattleMsg;
                playerBattleMsg = "";
                break; //exit loop
            }

            switch(attack_turn[i]){
                case 0: //PC Turn 
                    battleMsg += playerBattleMsg;
                    playerBattleMsg = "";

                    enemyPokemon->currHP -= currPokeMoveDmg;
                    if(enemyPokemon->currHP <= 0){ //killed pokemon
                        battleMsg += "Enemy " + enemyPokemon->name + " has fainted\n";
                        enemyPokemon->currHP = 0;
                        pokemonDown = true;
                    }

                    break;
                case 1: //Opponent Turn
                    battleMsg += "Enemy " + enemyPokemon->name + " used " + enemyPokemonMove.name;

                    int moveAcc;
                    enemyPokemonMove.accuracy <= 0 ? moveAcc = 100 : moveAcc = enemyPokemonMove.accuracy;

                    if(rand() % 100 > moveAcc){ //if random number generated more than move accuracy
                        //miss
                        battleMsg += ", it missed\n";
                    }
                    else{ //hit
                        currPokemon->currHP -= enemyPokemonMoveDmg;

                        battleMsg += ", it did " + std::to_string(enemyPokemonMoveDmg) + " damage";

                        if(currPokemon->currHP <= 0){ //PC pokemon died
                            battleMsg += ", " + currPokemon->name + " has fainted";
                            currPokemon->currHP = 0;
                        pokemonDown = true;
                        }
                        battleMsg += "\n";
                    }
                    break;
            }

        }

    }while(true);

    // printw("You are in a Pokemon Battle, 'esc' to exit\n");
    // while(getch() != 27){ //leave battle when escape key is pressed
    //     clear();
    //     printw("You are in a Pokemon Battle, 'esc' to exit\n");
    // }
    // world->npcMap[row][col]->isDefeated = true; //defeat NPC
    clear();
    endwin();
}


//function to level up pokemon
void levelPokemon(pokemon *pokemon){
    int newStats[NUM_STATS]; //array to hold new stat values
    int pokemonLevel = pokemon->level; //current pokemon level

    //increase HP stat
    float newPokemonHP = pokemon->base_stats[statsMap.at("hp") - 1] + pokemon->stats_iv[statsMap.at("hp") - 1];
    newPokemonHP = newPokemonHP * 2 * pokemonLevel;
    newPokemonHP = newPokemonHP / 100;
    newPokemonHP = newPokemonHP + 10 + pokemonLevel;
    newStats[statsMap.at("hp") - 1] = (int)newPokemonHP;

    //increase other stats
    for(int i = statsMap.at("attack"); i <= statsMap.at("speed"); i++){ //iterate through poke_stats Enum
        float newOtherStat = pokemon->base_stats[i - 1] + pokemon->stats_iv[i - 1];
        newOtherStat = newOtherStat * 2 * pokemonLevel;
        newOtherStat = newOtherStat/100;
        newOtherStat = newOtherStat + 5;
        newStats[i - 1] = (int)newOtherStat;
    }

    //update pokemon stats and level
    pokemon->level = pokemon->level + 1;
    for(int i = 0; i < NUM_STATS; i++){
        pokemon->stats[i] = newStats[i];
    }
}

void initialize_pokemon(pokemon *pokemon){ //initialize pokemon
    int numMoves = rand() % 2 + 1; //number of moves the pokemon will learn ( 1 or 2 )
    for(int i = 0; i < numMoves; i++){ //learn either one or two moves
        while(true){
            pokeMoves tmp = pokemonMovesArray[rand() % pokemonMovesArray.size()];
            if((tmp.id == pokemon->species) && (tmp.pokemon_move_method_id == 1)){
                if(i == 1){ //learning 2nd move
                    //check duplicate as to not learn two same moves
                    if(tmp.move_id == pokemon->move_set[0].id){
                        continue;
                    }
                }
                (pokemon->move_set).push_back(movesArray[tmp.move_id - 1]);
                break;
            }
        }
    }

    for(int i = 0; i < (int)pokemonStatsArray.size(); i++){ //get base stats of pokemon
        if(pokemonStatsArray[i].pokemon_id == pokemon->id){
            pokemon->base_stats[pokemonStatsArray[i].stat_id - 1] = pokemonStatsArray[i].base_stat;
        }
    }
    //generate IV for pokemon stats (iterate through poke_stats enum)
    for(int i = statsMap.at("hp"); i <= statsMap.at("speed"); i++){
        pokemon->stats_iv[i - 1] = rand() % 16 + 1; //get IV between range 1 - 15
        pokemon->stats[i - 1] = pokemon->stats_iv[i - 1]; //initialize starting stats as IV values
    }

    //generate gender for pokemon (0 - male, 1 - female)
    pokemon->gender = rand() % 2;

    pokemon->is_shiny = 0; //default to not shiny
}

void setStartingPokemon(pcStruct *trainer, point curMap){
    std::vector<pokemon> startingPokemons;
    std::vector<int> basicStarterPack = {1,4,7}; //bulbasaur, charmander, squirtle
    std::string msg;
    
    if(trainer->c == '@'){ //PC
        // int randNum = rand() % (int)pokeArray.size(); //get random pokemon index num

        for(int i = 0; i < 3; i++){ //generate three starting pokemons to choose from
            // pokemon starterPokemon = pokeArray[randNum];
            pokemon starterPokemon = pokemonArray[basicStarterPack[i] - 1]; //subtract one to get index number (1 less than id number)
            initialize_pokemon(&starterPokemon);
            startingPokemons.push_back(starterPokemon);
            // randNum = rand() % (int)pokeArray.size(); //get random pokemon index num
        }

        initialize_terminal();

        char userInput = 0;
        bool chosen = false; //flag to check if pokemon has been successfully chosen
        //Print starter pokemon details
        int active = 0; //active selection
        do{
            clear();
            if(userInput == 0){
                msg = "Welcome to the world of pokemon!\n";
                msg += "Choose your starting pokemon\n\n";
                printMessage(msg);
            }
            else{
                printw("Welcome to the world of pokemon!\n");
                printw("Choose your starting pokemon\n\n");
            }
            std::string details;

            for(int i = 0; i < (int) startingPokemons.size(); i++){
                if(i == active)
                    details += "-> ";
                 details += startingPokemons[i].name + "\n";
            }

            printw(details.c_str());
            userInput = getch();

            switch(userInput){
                case 2: // (DOWN ARROW KEY)
                    if(active == (int)startingPokemons.size() - 1) //last item
                        continue;
                    else
                        active++;
                    break; 
                case 3: // (UP ARROW KEY)
                    if(active == 0) //first item
                        continue;
                    else
                        active--;
                    break; 
                case 10: //ENTER KEY
                    clear();
                    msg = "Confirm to pick ";
                    msg += startingPokemons[active].name.c_str();
                    msg += "? (y/n)\n";
                    printMessage(msg);
                    userInput = getch();
                    if(userInput == 'y' || userInput == 10){
                        (trainer->pokemons).push_back(startingPokemons[active]);
                        chosen = true;
                    }
                    else{
                        userInput = 0;
                    }
                break;
            }
        }
        while(!chosen);

        trainer->pokemons[0].level = 1; //init pokemon level to 0
        for(int i = 1; i <= 5; i++){ //level pokemon to 5
            levelPokemon(&(trainer->pokemons[0]));
        }
        trainer->pokemons[0].currHP = trainer->pokemons[0].stats[statsMap.at("hp") - 1]; //set current hp as max current hp stat
        clear();
        endwin();
    }
    else{ //NPC - follows the same rule as spawnPokemon()
        //get manhattan distance from origin
        float mapDistance = getDistance(curMap.col - 200, curMap.row - 200, 0, 0);

        int minLevel = 1;
        int maxLevel = 1;
        if(mapDistance > 201){ //avoid doing (201 - 200) / 2 which returns 0
            minLevel = (int)((mapDistance - 200) / 2);
            maxLevel = 100;
        }
        else if(mapDistance > 1){ //2 < distance < 200 , also to avoid doing 0 divided by 2 or 1 divided by 2
            maxLevel = int(mapDistance / 2);
        }
        do{
            int randNum = rand() % (int)pokemonArray.size(); //get random pokemon index num
            //Load random pokemon
            pokemon pokemon = pokemonArray[randNum];
            initialize_pokemon(&pokemon);

            int pokeLevel = rand() % (maxLevel + 1 - minLevel) + minLevel; //generate random level for pokemon ranging from minLevel to maxLevel

            pokemon.level = 1; //init level as 0
            for(int i = 1; i <= pokeLevel; i++){ //level pokemon n times(n = level)
                levelPokemon(&pokemon);
            }
            pokemon.currHP = pokemon.stats[statsMap.at("hp") - 1]; //set current hp as max current hp stat
            (trainer->pokemons).push_back(pokemon); //add pokemon to trainer's pokemon collection
            if(rand() % 10 < 4){ //60% to generate another pokemon (0 - 3 dont spawn, 4 - 9 spawn)
                break; //exit do...while loop
            }
        }while((int)(trainer->pokemons.size()) < 6); //maximum 6 pokemons per trainer
    }
}

long getDistanceMap(worldStruct *world, int type, int row, int col)
{
    switch (type)
    {
    case 0: // hiker
        return world->hikerDistanceMap[row][col];
    case 2: // swimmer
        return world->swimmerDistanceMap[row][col];
    default: // rival & others
        return world->rivalDistanceMap[row][col];
    }
}
// function to initialize trainers
void initialize_trainers()
{
    int i;

    if (num_trainers == 1)
    { // 1 trainer only
        switch (rand() % 2)
        { // assign it to either trainer or rival and quit function
        case 0:
            num_hiker++;
            return;
        case 1:
            num_rival++;
            return;
        }
    }

    while (true)
    {
        num_hiker = rand() % (num_trainers - 1) + 1; // at least 1 and at most less than 1 of maximum trainers
        if (num_trainers >= 8 && num_hiker >= (num_trainers / 2))
        {             // if hiker more than 50% of total trainers when total trainers is greater than 7
            continue; // re-assign a different number
        }
        else
        {
            break; // end loop
        }
    }

    if (num_trainers - num_hiker != 0)
    { // more than 1 total trainers
        while (true)
        {
            num_rival = rand() % (num_trainers - num_hiker) + 1; // at least 1 and at most 1 less than remaining trainers
            if (num_trainers >= 8 && num_rival >= (num_trainers / 2))
            {             // if rival more than 50% of total trainers when total trainers is greater than 7
                continue; // re-assign a different number
            }
            else
            {
                break; // end loop
            }
        }
    }
    for (i = (num_trainers - num_hiker - num_rival); i > 0; i--)
    {
        switch (rand() % 5)
        {
        case 0:
            num_pacer++;
            break;
        case 1:
            num_wanderer++;
            break;
        case 2:
            num_sentry++;
            break;
        case 3:
            num_explorer++;
            break;
        case 4:
            num_swimmer++;
            break;
        }
    }
}
// set wanderer pc
void setWanderer(worldStruct *world, npcStruct *wanderer)
{
    int directionX, directionY;

    while (true)
    {
        directionX = rand() % 3 - 1; // set random wanderer x direction movement (-1 0 1)
        directionY = rand() % 3 - 1; // set random wanderer y direction movement (-1 0 1)
        if (directionX + directionY == 0)
            continue; // retry

        if (world->map[wanderer->y + directionY][wanderer->x + directionX] == wanderer->terrainType)
        {
            // if position of next movement direction is same terrain type
            wanderer->nextX = directionX; // set x direction of next cell
            wanderer->nextY = directionY; // set y direction of next cell
            break;
        }
    }
}
// set explorer pc
void setExplorer(worldStruct *world, npcStruct *explorer)
{
    int directionX, directionY, targetX, targetY;
    char obstacles[] = "sMC^%%~"; // change directions if collide with these cells

    while (true)
    {
        directionX = rand() % 3 - 1; // set random explorer x direction movement (-1 0 1)
        directionY = rand() % 3 - 1; // set random explorer y direction movement (-1 0 1)

        if (directionX + directionY == 0)
            continue; // retry

        targetY = explorer->y + directionY;
        targetX = explorer->x + directionX;
        if (!spotTaken(world->map[targetY][targetX], obstacles, sizeof(obstacles) / sizeof(obstacles[0])) && (targetY > 0 && targetY < ROW_COUNT - 1) && (targetX > 0 && targetX < COL_COUNT - 1) && world->npcMap[targetY][targetX] == NULL)
        {                                 // if next movement direction is valid
            explorer->nextX = directionX; // set x direction of next cell
            explorer->nextY = directionY; // set y direction of next cell
            break;
        }
    }
}
// set swimmer pc
void setSwimmer(worldStruct *world, npcStruct *swimmer)
{
    int directionX, directionY;

    while (true)
    {
        directionX = rand() % 3 - 1; // set random swimmer x direction movement (-1 0 1)
        directionY = rand() % 3 - 1; // set random swimmer y direction movement (-1 0 1)
        if (directionX + directionY == 0)
            continue; // retry

        if (world->map[swimmer->y + directionY][swimmer->x + directionX] == swimmer->terrainType || world->map[swimmer->y + directionY][swimmer->x + directionX] == '=')
        {
            // if next movement direction is same terrain type
            swimmer->nextX = directionX; // set x direction of next cell
            swimmer->nextY = directionY; // set y direction of next cell
            break;
        }
    }
}

// function to get the path of pacer NPCs and initialize their values on npcMap
void getPath(worldStruct *world, point spawn)
{
    int length = 1;
    int direction = rand() % 8; // 7 random directions to start pacing to and fro
    char dest[] = "s@MC%%^~";   // blocks not allowed to pass
    point path[100];            // initialize a 100 length path
    int attempt = 0;
    point originalSpawn; // store original spawn location in a variable
    originalSpawn.col = spawn.col;
    originalSpawn.row = spawn.row;

    while (length < 3)
    { // repeat until path has at least 3 cells to move
        path[0].col = spawn.col;
        path[0].row = spawn.row;
        switch (direction)
        {
        case 0: // top left
            while (true)
            {
                if ((spawn.row - 1 == 0) || (spawn.col - 1 == 0) || spotTaken(world->map[spawn.row - 1][spawn.col - 1], dest, sizeof(dest) / sizeof(dest[0])) || (world->npcMap[spawn.row - 1][spawn.col - 1] != NULL && world->npcMap[spawn.row - 1][spawn.col - 1]->c == 's'))
                    break;

                spawn.row--; // update position y
                spawn.col--; // update position x
                length++;    // increase path length
                path[length - 1].col = spawn.col;
                path[length - 1].row = spawn.row;
            }
            break;
        case 1: // top
            while (true)
            {
                if ((spawn.row - 1 == 0) || spotTaken(world->map[spawn.row - 1][spawn.col], dest, sizeof(dest) / sizeof(dest[0])) || (world->npcMap[spawn.row - 1][spawn.col] != NULL && world->npcMap[spawn.row - 1][spawn.col]->c == 's'))
                    break;

                spawn.row--;
                length++;
                path[length - 1].col = spawn.col;
                path[length - 1].row = spawn.row;
            }
            break;
        case 2: // top right
            while (true)
            {
                if ((spawn.row - 1 == 0) || (spawn.col + 1 == (COL_COUNT - 1)) || spotTaken(world->map[spawn.row - 1][spawn.col + 1], dest, sizeof(dest) / sizeof(dest[0])) || (world->npcMap[spawn.row - 1][spawn.col + 1] != NULL && world->npcMap[spawn.row - 1][spawn.col + 1]->c == 's'))
                    break;

                spawn.row--;
                spawn.col++;
                length++;
                path[length - 1].col = spawn.col;
                path[length - 1].row = spawn.row;
            }
            break;
        case 3: // left
            while (true)
            {
                if ((spawn.col - 1 == 0) || spotTaken(world->map[spawn.row][spawn.col - 1], dest, sizeof(dest) / sizeof(dest[0])) || (world->npcMap[spawn.row][spawn.col - 1] != NULL && world->npcMap[spawn.row][spawn.col - 1]->c == 's'))
                    break;

                spawn.col--;
                length++;
                path[length - 1].col = spawn.col;
                path[length - 1].row = spawn.row;
            }
            break;
        case 4: // right
            while (true)
            {
                if ((spawn.col + 1 == (COL_COUNT - 1)) || spotTaken(world->map[spawn.row][spawn.col + 1], dest, sizeof(dest) / sizeof(dest[0])) || (world->npcMap[spawn.row][spawn.col + 1] != NULL && world->npcMap[spawn.row][spawn.col + 1]->c == 's'))
                    break;

                spawn.col++;
                length++;
                path[length - 1].col = spawn.col;
                path[length - 1].row = spawn.row;
            }
            break;
        case 5: // bottom left
            while (true)
            {
                if ((spawn.row + 1 == (ROW_COUNT - 1)) || (spawn.col - 1 == 0) || spotTaken(world->map[spawn.row + 1][spawn.col - 1], dest, sizeof(dest) / sizeof(dest[0])) || (world->npcMap[spawn.row + 1][spawn.col - 1] != NULL && world->npcMap[spawn.row + 1][spawn.col - 1]->c == 's'))
                    break;

                spawn.row++;
                spawn.col--;
                length++;
                path[length - 1].col = spawn.col;
                path[length - 1].row = spawn.row;
            }
            break;
        case 6: // bottom
            while (true)
            {
                if ((spawn.row + 1 == (ROW_COUNT - 1)) || spotTaken(world->map[spawn.row + 1][spawn.col], dest, sizeof(dest) / sizeof(dest[0])) || (world->npcMap[spawn.row + 1][spawn.col] != NULL && world->npcMap[spawn.row + 1][spawn.col]->c == 's'))
                    break;

                spawn.row++;
                length++;
                path[length - 1].col = spawn.col;
                path[length - 1].row = spawn.row;
            }
            break;
        case 7: // bottom right
            while (true)
            {
                if ((spawn.row + 1 == (ROW_COUNT - 1)) || (spawn.col + 1 == (COL_COUNT - 1)) || spotTaken(world->map[spawn.row + 1][spawn.col + 1], dest, sizeof(dest) / sizeof(dest[0])) || (world->npcMap[spawn.row + 1][spawn.col + 1] != NULL && world->npcMap[spawn.row + 1][spawn.col + 1]->c == 's'))
                    break;

                spawn.col++;
                spawn.row++;
                length++;
                path[length - 1].col = spawn.col;
                path[length - 1].row = spawn.row;
            }
            break;
        }

        if (length < 3)
        {
            if (attempt > 3)
            { // more than 5 attempts trying to generate path
                while (true)
                { // spawn pacer in a new place and try again
                    spawn.col = rand() % (COL_COUNT - 2) + 1;
                    spawn.row = rand() % (ROW_COUNT - 2) + 1;
                    if (!spotTaken(world->map[spawn.row + 1][spawn.col + 1], dest, sizeof(dest) / sizeof(dest[0])) ||
                        (world->npcMap[spawn.row + 1][spawn.col + 1] == NULL))
                        break;
                }

                originalSpawn.col = spawn.col; // update original spawn position
                originalSpawn.row = spawn.row;
                attempt = 0; // reset attempt count
            }
            else
            { // reset spawn location to original spot and try again
                spawn.col = originalSpawn.col;
                spawn.row = originalSpawn.row;
            }
            length = 1;
            direction = rand() % 8; // another direction
            attempt++;              // increase attempt count
        }
    }
    // after successful path generation
    world->npcMap[originalSpawn.row][originalSpawn.col] = new npcStruct;
    world->npcMap[originalSpawn.row][originalSpawn.col]->path.pathing = (point *)malloc(sizeof(point) * length);
    world->npcMap[originalSpawn.row][originalSpawn.col]->path.onPath = 0; // start on index 0 of path array
    world->npcMap[originalSpawn.row][originalSpawn.col]->path.pathLength = length;
    world->npcMap[originalSpawn.row][originalSpawn.col]->c = 'p';
    world->npcMap[originalSpawn.row][originalSpawn.col]->x = originalSpawn.col;
    world->npcMap[originalSpawn.row][originalSpawn.col]->y = originalSpawn.row;
    world->npcMap[originalSpawn.row][originalSpawn.col]->isDefeated = false;
    world->npcMap[originalSpawn.row][originalSpawn.col]->path.direction = 1; // moving away from original spot
    for (int i = 0; i < length; i++)
    { // deep copy path array into pc path
        world->npcMap[originalSpawn.row][originalSpawn.col]->path.pathing[i] = path[i];
    }

    // set next destination as index 1 on path array
    world->npcMap[originalSpawn.row][originalSpawn.col]->nextX = path[1].col;
    world->npcMap[originalSpawn.row][originalSpawn.col]->nextY = path[1].row;
}

// function to generate NPCs and their locations
void generateNPC(worldStruct *world)
{
    int i, randX, randY;
    num_hiker = num_rival = num_pacer = num_wanderer = num_sentry = num_explorer = num_swimmer = 0; // default number of each type of trainer to 0
    if (num_trainers > 0)
    { // initialize trainers
        initialize_trainers();
        char dest[] = "@MC#=%%^hrpwsn~"; // array to check invalid cells to occupy
        // num_hiker = 1;
        if (num_hiker > 0)
        {
            dijkstra(world->map, 0, world->hikerDistanceMap); // generate distance map for hiker
            for (i = 0; i < num_hiker; i++)
            { // hiker
                while (true)
                {
                    randX = rand() % (COL_COUNT - 2) + 1;
                    randY = rand() % (ROW_COUNT - 2) + 1;

                    if (!spotTaken(world->map[randY][randX], dest, sizeof(dest) / sizeof(dest[0])) && world->npcMap[randY][randX] == NULL)
                    {
                        world->npcMap[randY][randX] = new npcStruct;
                        world->npcMap[randY][randX]->c = 'h';
                        world->npcMap[randY][randX]->x = randX;
                        world->npcMap[randY][randX]->y = randY;
                        world->npcMap[randY][randX]->isDefeated = false;
                        break;
                    }
                }

                pcStruct *tmp = (world->npcMap[randY][randX]);
                setStartingPokemon(tmp, curMap);
            }
        }
        if (num_rival > 0 || num_pacer > 0 || num_wanderer > 0 || num_sentry > 0 || num_explorer > 0)
        {
            dijkstra(world->map, 1, world->rivalDistanceMap); // generate distance map for rival

            for (i = 0; i < num_rival; i++)
            { // rival
                while (true)
                {
                    randX = rand() % (COL_COUNT - 2) + 1;
                    randY = rand() % (ROW_COUNT - 2) + 1;

                    if (!spotTaken(world->map[randY][randX], dest, sizeof(dest) / sizeof(dest[0])) && world->npcMap[randY][randX] == NULL)
                    {
                        world->npcMap[randY][randX] = new npcStruct;
                        world->npcMap[randY][randX]->c = 'r';
                        world->npcMap[randY][randX]->x = randX;
                        world->npcMap[randY][randX]->y = randY;
                        world->npcMap[randY][randX]->isDefeated = false;
                        break;
                    }
                }

                pcStruct *tmp = (world->npcMap[randY][randX]);
                setStartingPokemon(tmp, curMap);
            }

            for (i = 0; i < num_sentry; i++)
            { // sentry
                while (true)
                {
                    randX = rand() % (COL_COUNT - 2) + 1;
                    randY = rand() % (ROW_COUNT - 2) + 1;

                    if (!spotTaken(world->map[randY][randX], dest, sizeof(dest) / sizeof(dest[0])) && world->npcMap[randY][randX] == NULL)
                    {
                        world->npcMap[randY][randX] = new npcStruct;
                        world->npcMap[randY][randX]->c = 's';
                        world->npcMap[randY][randX]->x = randX;
                        world->npcMap[randY][randX]->y = randY;
                        world->npcMap[randY][randX]->nextX = randX;
                        world->npcMap[randY][randX]->nextY = randY;
                        world->npcMap[randY][randX]->isDefeated = false;
                        break;
                    }
                }

                pcStruct *tmp = (world->npcMap[randY][randX]);
                setStartingPokemon(tmp, curMap);
            }

            for (i = 0; i < num_pacer; i++)
            { // pacer
                while (true)
                {
                    randX = rand() % (COL_COUNT - 2) + 1;
                    randY = rand() % (ROW_COUNT - 2) + 1;

                    if (!spotTaken(world->map[randY][randX], dest, sizeof(dest) / sizeof(dest[0])) && world->npcMap[randY][randX] == NULL)
                    {
                        // generate pathing
                        point spawn;
                        spawn.col = randX;
                        spawn.row = randY;
                        getPath(world, spawn);
                        break;
                    }
                }
                pcStruct *tmp = (world->npcMap[randY][randX]);
                setStartingPokemon(tmp, curMap);
            }

            for (i = 0; i < num_wanderer; i++)
            { // wanderer
                while (true)
                {
                    randX = rand() % (COL_COUNT - 2) + 1;
                    randY = rand() % (ROW_COUNT - 2) + 1;

                    if (!spotTaken(world->map[randY][randX], dest, sizeof(dest) / sizeof(dest[0])) && world->npcMap[randY][randX] == NULL)
                    {
                        // define wanderer properties
                        world->npcMap[randY][randX] = new npcStruct;
                        world->npcMap[randY][randX]->c = 'w';
                        world->npcMap[randY][randX]->x = randX;
                        world->npcMap[randY][randX]->y = randY;
                        world->npcMap[randY][randX]->terrainType = world->map[randY][randX]; // set terrain type for wanderer pc
                        world->npcMap[randY][randX]->isDefeated = false;

                        // set wanderer directions for movements
                        setWanderer(world, world->npcMap[randY][randX]);
                        break;
                    }
                }

                pcStruct *tmp = (world->npcMap[randY][randX]);
                setStartingPokemon(tmp, curMap);
            }

            for (i = 0; i < num_explorer; i++)
            { // explorer
                while (true)
                {
                    randX = rand() % (COL_COUNT - 2) + 1;
                    randY = rand() % (ROW_COUNT - 2) + 1;

                    if (!spotTaken(world->map[randY][randX], dest, sizeof(dest) / sizeof(dest[0])) && world->npcMap[randY][randX] == NULL)
                    {
                        // define explorer properties
                        world->npcMap[randY][randX] = new npcStruct;
                        world->npcMap[randY][randX]->c = 'e';
                        world->npcMap[randY][randX]->x = randX;
                        world->npcMap[randY][randX]->y = randY;
                        world->npcMap[randY][randX]->isDefeated = false;

                        // set explorer and its directions
                        setExplorer(world, world->npcMap[randY][randX]);
                        break;
                    }
                }

                pcStruct *tmp = (world->npcMap[randY][randX]);
                setStartingPokemon(tmp, curMap);
            }
        }
        // num_swimmer = 1;
        if (num_swimmer > 0)
        {
            char dest[] = "@MC=#%%^hrpwsn:.";
            dijkstra(world->map, 2, world->swimmerDistanceMap); // generate distance map for swimmers

            for (i = 0; i < num_swimmer; i++)
            {
                while (true)
                {
                    randX = rand() % (COL_COUNT - 2) + 1;
                    randY = rand() % (ROW_COUNT - 2) + 1;

                    if (!spotTaken(world->map[randY][randX], dest, sizeof(dest) / sizeof(dest[0])) && world->npcMap[randY][randX] == NULL && world->map[randY][randX] == '~')
                    {
                        // define explorer properties
                        world->npcMap[randY][randX] = new npcStruct;
                        world->npcMap[randY][randX]->c = 'm';
                        world->npcMap[randY][randX]->x = randX;
                        world->npcMap[randY][randX]->y = randY;
                        world->npcMap[randY][randX]->terrainType = world->map[randY][randX];
                        world->npcMap[randY][randX]->isDefeated = false;

                        // set swimmer and its directions
                        setSwimmer(world, world->npcMap[randY][randX]);
                        break;
                    }
                }

                pcStruct *tmp = (world->npcMap[randY][randX]);
                setStartingPokemon(tmp, curMap);
            }
        }
    }
    else
    {
        return;
    }
}

// function to get the cost of node based on the type of terrain
int getNodeCost(char c, int type)
{
    // hiker = 0, rival & other = 1, swimmer = 2
    switch (c)
    {
    case '#': // path
        if (type == 2)
            return INT_MAX;
        return 10;

    case '=': // bridge
        if (type == 2)
            return 7;
        return 10;

    case 'M':
        if (type == 2)
            return INT_MAX;
        return 50; // 50 for pokemart

    case 'C':
        if (type == 2)
            return INT_MAX;
        return 50; // 50 for pokecenter

    case ':': // tall grass
        if (type == 0)
            return 15; // 15 for hiker
        if (type == 2)
            return INT_MAX;
        return 20; // 20 for rival

    case '.': // short grass
        if (type == 2)
            return INT_MAX;
        return 10;

    case '%': // mountain
        if (type == 0)
            return 15; // 15 for hiker
        if (type == 2)
            return INT_MAX;
        return INT_MAX; // infinity for rival

    case '^': // forest
        if (type == 0)
            return 15; // 15 for hiker
        if (type == 2)
            return INT_MAX;
        return INT_MAX; // infinity for rival

    case '~': // water
        if (type == 2)
            return 7;
        return INT_MAX;

    case 'b': // border
        return INT_MAX;

    default:
        return 0;
    }
}
// function to get cells in all 8 directions from the source
void getNeighbors(point pos, point neighbors[], char terrain[ROW_COUNT][COL_COUNT])
{
    bool northBorder = ((pos.row - 1) <= 0);
    bool southBorder = ((pos.row + 1) >= ROW_COUNT - 1);
    bool westBorder = ((pos.col - 1) <= 0);
    bool eastBorder = ((pos.col + 1) >= (COL_COUNT - 1));

    // top left
    neighbors[0].col = pos.col - 1;
    neighbors[0].row = pos.row - 1;
    if (northBorder || westBorder)
        neighbors[0].element = 'b';
    else
        neighbors[0].element = terrain[neighbors[0].row][neighbors[0].col];

    // top
    neighbors[1].col = pos.col;
    neighbors[1].row = pos.row - 1;
    if (northBorder)
        neighbors[1].element = 'b';
    else
        neighbors[1].element = terrain[neighbors[1].row][neighbors[1].col];

    // top right
    neighbors[2].col = pos.col + 1;
    neighbors[2].row = pos.row - 1;
    if (northBorder || eastBorder)
        neighbors[2].element = 'b';
    else
        neighbors[2].element = terrain[neighbors[2].row][neighbors[2].col];

    // left
    neighbors[3].col = pos.col - 1;
    neighbors[3].row = pos.row;
    if (westBorder)
        neighbors[3].element = 'b';
    else
        neighbors[3].element = terrain[neighbors[3].row][neighbors[3].col];

    // right
    neighbors[4].col = pos.col + 1;
    neighbors[4].row = pos.row;
    if (eastBorder)
        neighbors[4].element = 'b';
    else
        neighbors[4].element = terrain[neighbors[4].row][neighbors[4].col];

    // bottom left
    neighbors[5].col = pos.col - 1;
    neighbors[5].row = pos.row + 1;
    if (southBorder || westBorder)
        neighbors[5].element = 'b';
    else
        neighbors[5].element = terrain[neighbors[5].row][neighbors[5].col];

    // bottom
    neighbors[6].col = pos.col;
    neighbors[6].row = pos.row + 1;
    if (southBorder)
        neighbors[6].element = 'b';
    else
        neighbors[6].element = terrain[neighbors[6].row][neighbors[6].col];

    // bottom right
    neighbors[7].col = pos.col + 1;
    neighbors[7].row = pos.row + 1;
    if (southBorder || eastBorder)
        neighbors[7].element = 'b';
    else
        neighbors[7].element = terrain[neighbors[7].row][neighbors[7].col];
}
// function to calculate and print distance maps for hiker and rival NPCs
void dijkstra(char terrain[ROW_COUNT][COL_COUNT], int npcType, long distanceMap[ROW_COUNT][COL_COUNT])
{
    // npcType 0 - hiker, 1 - rival, 2- swimmer
    for (int i = 0; i < ROW_COUNT; i++)
    {
        for (int j = 0; j < COL_COUNT; j++)
        {
            distanceMap[i][j] = INT_MAX; // initialize each point as infinity
        }
    }
    distanceMap[pc->y][pc->x] = 0; // source point dist = 0
    point source;
    source.col = pc->x;
    source.row = pc->y;
    pqNode *pq = newNode(source, 0);
    while (!isEmpty(&pq))
    {
        point u = peek(&pq);
        pop(&pq);
        point neighbor[8];
        getNeighbors(u, neighbor, terrain);
        for (int i = 0; i < 8; i++)
        {
            long newDist = distanceMap[u.row][u.col] + getNodeCost(neighbor[i].element, npcType);
            if (newDist < distanceMap[neighbor[i].row][neighbor[i].col])
            {
                distanceMap[neighbor[i].row][neighbor[i].col] = newDist;
                source.col = neighbor[i].col;
                source.row = neighbor[i].row;
                if (isEmpty(&pq))
                    pq = newNode(source, newDist);
                else
                    push(&pq, source, newDist);
            }
        }
    }
}

// function to spawn pc on a random path cell
void spawnPlayer(char terrain[ROW_COUNT][COL_COUNT])
{
    // generate random pos for pc on path
    int randX, randY;
    bool done = false;
    while (!done)
    {
        randX = (rand() % (COL_COUNT - 2)) + 1;
        randY = (rand() % (ROW_COUNT - 2)) + 1;
        if (terrain[randY][randX] == '#' || terrain[randY][randX] == '=')
        {
            pc->x = randX; // update pc x-coordinates
            pc->y = randY; // update pc y-coordinates
            done = true;
        }
    }
}
