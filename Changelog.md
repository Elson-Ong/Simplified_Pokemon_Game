Jan 29
Added printMap(), generateMap(), generateBorder() and initializeMap()
Added generateTallGrass(), generateWater(), generateShortGrass(), generateTrees(), populateArea(), generateBoulders(),
isValidPosition(), selectPointL(), and selectPointR()
Added fillEmpty();
Added generateExit(), generateNSPath() and generateWEPath(), also added color to output
Added generateBuildings()
Moved the order of map generation so that it generates borders -> exits -> paths -> buildings -> various regions

Feb 5
Created the function to allow the keeping track of the generated maps and the moving between maps and fly function. Removed the scattering of trees and boulders during map generation due to segmentation error. Fixed some minor bugs. Also moved some of the functions from TerrainGenerator.c to utilities.h. Created a function to calculate distance of map from origin point and use the distance to determine the probability of buildings spawning.

Feb 6
Added the handling of edge maps where if it is at the edge, no exit will be generated in that direction

Feb 12
Added method getNodeCost(), getNeighbor(), dijkstra() method with reference to https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
Also added a priority queue in utilities.h from https://www.geeksforgeeks.org/priority-queue-using-linked-list/

Feb 24
Changed terrainGeneration order to generate path and buildings after generating terrains.
Changed path to be generated as bridge '=' when it is over water. 
Added another priority queue for game turn.

Feb 25
Added methods:
    generateNPC(), getNumTrainers(), setWanderer(), setExplorer(), setSwimmer(), gameTurn_init(), gameTurn_exec(), getPath();

Feb 26
Debugged the various functions for npc movement. 

Mar 12 
Added ncurses as a include in utilities.h. Added movePlayer() to allow the movement of player based on input key. Removed the use of usleep for delay. 

Mar 13
Added color to terrain. Added the function to view the npcs in the current map with the distance to them.

Mar 14
Renamed generatePlayer() to spawnPlayer(). Added the function for pokeBattle() and added the boolean isDefeated for pcStruct. npcs stop moving once they are defeated.

Mar 15 
Allow the movement of player to other maps through the exit gate.

Mar 25
Changed all files to c++
Refactored the code into multiple files to make it easier to read.

Mar 26
Added Fly function

April 1
Added classes for the different files to parse

April 2
Added the method to parse the CSV file

April 8
Added function to spawn wild pokemon in tall grass ":" with a 10% chance
Added function to display encounter with wild pokemon by displaying stats of the wild pokemon

April 9
Added function to start the game by allowing user to select their starting pokemon
Added function to assign 1-6 pokemon(s) to all trainers when generating npc
Changed pokemonBattle to call the pokemonEncounter function as a placeholder to display the stats of the first pokemon of the trainer.

April 10
Fixed a bug where Floating point error will happen when loading a new map

April 16
Implemented PokemonBattle function.

April 17
Make PokeMart and PokeCenter actually restore your item/pokemon respectively
Added Screen transition for pokemon battle
Added text animation when displaying messages to the user