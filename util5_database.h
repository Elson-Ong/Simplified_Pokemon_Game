/*
Header file to store csv data structs
*/
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <map>

#ifndef UTIL5_DATABASE_H
# define UTIL5_DATABASE_H
# endif

using namespace std;

#define NUM_STATS 8

//******************** MOVES.CSV ********************//
class moves{
    public:
        int id;
        std::string name;
        int generation_id;
        int type_id;
        int power;
        int pp;
        int accuracy;
        int priority;
        int target_id;
        int damage_class_id;
        int effect_id;
        int effect_chance;
        int contest_type_id;
        int contest_effect_id;
        int super_contest_effect_id;
        friend std::ostream &operator<<(std::ostream& o, const moves &m);
};

moves constructMoves(std::vector<std::string> values){ //moves constructor
    moves move;

    move.id = std::stoi(values[0]);
    move.name = values[1];
    move.generation_id = std::stoi(values[2]);
    move.type_id = std::stoi(values[3]);
    move.power = std::stoi(values[4]);
    move.pp = std::stoi(values[5]);
    move.accuracy = std::stoi(values[6]);
    move.priority = std::stoi(values[7]);
    move.target_id = std::stoi(values[8]);
    move.damage_class_id = std::stoi(values[9]);
    move.effect_id = std::stoi(values[10]);
    move.effect_chance = std::stoi(values[11]);
    move.contest_type_id = std::stoi(values[12]);
    move.contest_effect_id = std::stoi(values[13]);
    move.super_contest_effect_id = std::stoi(values[14]);

    return move;
}

std::ostream &operator<<(std::ostream &o, const moves &m){ //print function for moves.csv
    o << ((m.id != -1) ? std::to_string(m.id) : "") << ",";
    o << ((m.name != "-1") ? m.name : "") << ",";
    o << ((m.generation_id != -1) ? std::to_string(m.generation_id) : "") << ",";
    o << ((m.type_id != -1) ? std::to_string(m.type_id) : "") << ",";
    o << ((m.power != -1) ? std::to_string(m.power) : "") << ",";
    o << ((m.pp != -1) ? std::to_string(m.pp) : "") << ",";
    o << ((m.accuracy != -1) ? std::to_string(m.accuracy) : "") << ",";
    o << ((m.priority != -1) ? std::to_string(m.priority) : "") << ",";
    o << ((m.target_id != -1) ? std::to_string(m.target_id) : "") << ",";
    o << ((m.damage_class_id != -1) ? std::to_string(m.damage_class_id) : "") << ",";
    o << ((m.effect_id != -1) ? std::to_string(m.effect_id) : "") << ",";
    o << ((m.effect_chance != -1) ? std::to_string(m.effect_chance) : "") << ",";
    o << ((m.contest_type_id != -1) ? std::to_string(m.contest_type_id) : "") << ",";
    o << ((m.contest_effect_id != -1) ? std::to_string(m.contest_effect_id) : "") << ",";
    o << ((m.super_contest_effect_id != -1) ? std::to_string(m.super_contest_effect_id) : "");
    return o;
}

//******************** POKEMON_MOVES.CSV ********************//
class pokeMoves{
    public:
        int id;
        int version_group_id;
        int move_id;
        int pokemon_move_method_id;
        int level;
        int order;
        friend std::ostream &operator<<(std::ostream& o, const pokeMoves &pm);
};

pokeMoves constructPokeMoves(std::vector<std::string> values){ //pokemon_moves constructor
    pokeMoves pokeMove;

    pokeMove.id = std::stoi(values[0]);
    pokeMove.version_group_id = std::stoi(values[1]);
    pokeMove.move_id = std::stoi(values[2]);
    pokeMove.pokemon_move_method_id = std::stoi(values[3]);
    pokeMove.level = std::stoi(values[4]);
    pokeMove.order = std::stoi(values[5]);

    return pokeMove;
}

std::ostream &operator<<(std::ostream &o, const pokeMoves &pm){ //print function for pokemon_moves.csv
    o << ((pm.id != -1) ? std::to_string(pm.id) : "") << ",";
    o << ((pm.version_group_id != -1) ? std::to_string(pm.version_group_id) : "") << ",";
    o << ((pm.move_id != -1) ? std::to_string(pm.move_id) : "") << ",";
    o << ((pm.pokemon_move_method_id != -1) ? std::to_string(pm.pokemon_move_method_id) : "") << ",";
    o << ((pm.level != -1) ? std::to_string(pm.level) : "") << ",";
    o << ((pm.order != -1) ? std::to_string(pm.order) : "");
    
    return o;
}

//******************** POKEMON_SPECIES.CSV ********************//
class pokemon_species{
    public:
        int id;
        std::string name;
        int generation_id;
        int evolves_from_species_id;
        int evolution_chain_id;
        int color_id;
        int shape_id;
        int habitat_id;
        int gender_rate;
        int capture_rate;
        int base_happiness;
        int is_baby;
        int hatch_counter;
        int has_gender_differences;
        int growth_rate_id;
        int forms_switchable;
        int is_legendary;
        int is_mythical;
        int order;
        int conquest_order;
        friend std::ostream &operator<<(std::ostream& o, const pokemon_species &ps);
};

pokemon_species constructSpecies(std::vector<std::string> values){ //pokemon_species constructor
    pokemon_species species;

    species.id = stoi(values[0]);
    species.name = values[1];
    species.generation_id = stoi(values[2]);
    species.evolves_from_species_id = stoi(values[3]);
    species.evolution_chain_id = stoi(values[4]);
    species.color_id = stoi(values[5]);
    species.shape_id = stoi(values[6]);
    species.habitat_id = stoi(values[7]);
    species.gender_rate = stoi(values[8]);
    species.capture_rate = stoi(values[9]);
    species.base_happiness = stoi(values[10]);
    species.is_baby = stoi(values[11]);
    species.hatch_counter = stoi(values[12]);
    species.has_gender_differences = stoi(values[13]);
    species.growth_rate_id = stoi(values[14]);
    species.forms_switchable = stoi(values[15]);
    species.is_legendary = stoi(values[16]);
    species.is_mythical = stoi(values[17]);
    species.order = stoi(values[18]);
    species.conquest_order = stoi(values[19]);

    return species;
}

std::ostream &operator<<(std::ostream &o, const pokemon_species &ps){ //print function for pokemon_species.csv
    o << ((ps.id != -1) ? std::to_string(ps.id) : "") << ",";
    o << ((ps.name != "-1") ? ps.name : "") << ",";
    o << ((ps.generation_id != -1) ? std::to_string(ps.generation_id) : "") << ",";
    o << ((ps.evolves_from_species_id != -1) ? std::to_string(ps.evolves_from_species_id) : "") << ",";
    o << ((ps.evolution_chain_id != -1) ? std::to_string(ps.evolution_chain_id) : "") << ",";
    o << ((ps.color_id != -1) ? std::to_string(ps.color_id) : "") << ",";
    o << ((ps.shape_id != -1) ? std::to_string(ps.shape_id) : "") << ",";
    o << ((ps.habitat_id != -1) ? std::to_string(ps.habitat_id) : "") << ",";
    o << ((ps.gender_rate != -1) ? std::to_string(ps.gender_rate) : "") << ",";
    o << ((ps.capture_rate != -1) ? std::to_string(ps.capture_rate) : "") << ",";
    o << ((ps.base_happiness != -1) ? std::to_string(ps.base_happiness) : "") << ",";
    o << ((ps.is_baby != -1) ? std::to_string(ps.is_baby) : "") << ",";
    o << ((ps.hatch_counter != -1) ? std::to_string(ps.hatch_counter) : "") << ",";
    o << ((ps.has_gender_differences != -1) ? std::to_string(ps.has_gender_differences) : "") << ",";
    o << ((ps.growth_rate_id != -1) ? std::to_string(ps.growth_rate_id) : "") << ",";
    o << ((ps.forms_switchable != -1) ? std::to_string(ps.forms_switchable) : "") << ",";
    o << ((ps.is_legendary != -1) ? std::to_string(ps.is_legendary) : "") << ",";
    o << ((ps.is_mythical != -1) ? std::to_string(ps.is_mythical) : "") << ",";
    o << ((ps.order != -1) ? std::to_string(ps.order) : "") << ",";
    o << ((ps.conquest_order != -1) ? std::to_string(ps.conquest_order) : "");
    return o;
}

//******************** EXPERIENCE.CSV ********************//
class experience{
    public:
        int growth_rate_id;
        int level;
        int exp;
        friend std::ostream &operator<<(std::ostream& o, const experience &e);
};

experience constructExperience(std::vector<std::string> values){ //experience constructor
    experience exp;

    exp.growth_rate_id = stoi(values[0]);
    exp.level = stoi(values[1]);
    exp.exp = stoi(values[2]);

    return exp;
}

std::ostream &operator<<(std::ostream &o, const experience &e){ //print function for exp.csv
    o << ((e.growth_rate_id != -1) ? std::to_string(e.growth_rate_id) : "") << ",";
    o << ((e.level != -1) ? std::to_string(e.level) : "") << ",";
    o << ((e.exp != -1) ? std::to_string(e.exp) : "");
    return o;
}

//******************** TYPE_NAMES.CSV ********************//
class type_names{
    public:
        int type_id;
        int local_language_id;
        std::string name;
        friend std::ostream &operator<<(std::ostream& o, const type_names &tn);
};

type_names constructTypeNames(std::vector<std::string> values){ //type name constructor
    type_names typeName;

    typeName.type_id = stoi(values[0]);
    typeName.local_language_id = stoi(values[1]);
    typeName.name = values[2];

    return typeName;
}

std::ostream &operator<<(std::ostream &o, const type_names &tn){ //print function for type_name.csv
    o << ((tn.type_id != -1) ? std::to_string(tn.type_id) : "") << ",";
    o << ((tn.local_language_id != -1) ? std::to_string(tn.local_language_id) : "") << ",";
    o << ((tn.name != "-1") ? tn.name : "");
    return o;
}

//******************** POKEMON_STATS.CSV ********************//
class pokemon_stats{
    public:
        int pokemon_id;
        int stat_id;
        int base_stat;
        int effort;
        friend std::ostream &operator<<(std::ostream& o, const pokemon_stats &pks);
};

pokemon_stats constructPokeStats(std::vector<std::string> values){ //pokemon stat constructor
    pokemon_stats pokeStat ;

    pokeStat.pokemon_id = stoi(values[0]);
    pokeStat.stat_id = stoi(values[1]);
    pokeStat.base_stat = stoi(values[2]);
    pokeStat.effort = stoi(values[3]);

    return pokeStat;
}

std::ostream &operator<<(std::ostream &o, const pokemon_stats &pks){ //print function for pokemon_stats.csv
    o << ((pks.pokemon_id != -1) ? std::to_string(pks.pokemon_id) : "") << ",";
    o << ((pks.stat_id != -1) ? std::to_string(pks.stat_id) : "") << ",";
    o << ((pks.base_stat != -1) ? std::to_string(pks.base_stat) : "") << ",";
    o << ((pks.effort != -1) ? std::to_string(pks.effort) : "") << ",";

    return o;
}

//******************** STATS.CSV ********************//
class stats{
    public:
        int id;
        int damage_class_id;
        std::string name;
        int is_battle_only;
        int game_index;
        friend std::ostream &operator<<(std::ostream& o, const stats &s);
};

stats constructStat(std::vector<std::string> values){ //stats constructor
    stats stat;

    stat.id = stoi(values[0]);
    stat.damage_class_id = stoi(values[1]);
    stat.name = values[2];
    stat.is_battle_only = stoi(values[3]);
    stat.game_index = stoi(values[4]);

    return stat;
}

std::ostream &operator<<(std::ostream &o, const stats &s){ //print function for stats.csv
    o << ((s.id != -1) ? std::to_string(s.id) : "") << ",";
    o << ((s.damage_class_id != -1) ? std::to_string(s.damage_class_id) : "") << ",";
    o << ((s.name != "-1") ? s.name : "") << ",";
    o << ((s.is_battle_only != -1) ? std::to_string(s.is_battle_only) : "") << ",";
    o << ((s.game_index != -1) ? std::to_string(s.game_index) : "") << ",";
    return o;
}

//******************** POKEMON_TYPE.CSV ********************//
class pokemon_types{
    public:
        int pokemon_id;
        int type_id;
        int slot;
        friend std::ostream &operator<<(std::ostream& o, const stats &p);
};

pokemon_types constructPokeTypes(std::vector<std::string> values){ //pokemon type constructor
    pokemon_types pokeType ;

    pokeType.pokemon_id = stoi(values[0]);
    pokeType.type_id = stoi(values[1]);
    pokeType.slot = stoi(values[2]);

    return pokeType;
}

std::ostream &operator<<(std::ostream &o, const pokemon_types &p){ //print function for pokemon_types.csv
    o << ((p.pokemon_id != -1) ? std::to_string(p.pokemon_id) : "") << ",";
    o << ((p.type_id != -1) ? std::to_string(p.type_id) : "") << ",";
    o << ((p.slot != -1) ? std::to_string(p.slot) : "") << ",";

    return o;
}
//******************** POKEMON.CSV ********************//
class pokemon{
    public:
        int id;
        std::string name;
        int species;
        int height;
        int weight;
        int base_experience;
        int order;
        int is_default;
        int level;
        int gender; // 0 - male, 1 - female
        int is_shiny; //0 - no, 1 - yes
        std::vector<moves> move_set;
        int currHP;
        int base_stats[NUM_STATS]; //HP, Attack, Defense, Special Attack, Special Defense, Speed, Accuracy, Evasion
        int stats[NUM_STATS]; //HP, Attack, Defense, Special Attack, Special Defense, Speed, Accuracy, Evasion (Current stats)
        int stats_iv[NUM_STATS]; //HP, Attack, Defense, Special Attack, Special Defense, Speed, Accuracy, Evasion (Individual Value)
        friend std::ostream &operator<<(std::ostream& o, const pokemon &p);
};

pokemon constructPokemon(std::vector<std::string> values){ //pokemon constructor
    pokemon poke;

    poke.id = stoi(values[0]);
    poke.name = values[1];
    poke.species = stoi(values[2]);
    poke.height = stoi(values[3]);
    poke.weight = stoi(values[4]);
    poke.base_experience = stoi(values[5]);
    poke.order = stoi(values[6]);
    poke.is_default = stoi(values[7]);

    return poke;
}

std::ostream &operator<<(std::ostream &o, const pokemon &p){ //print function for pokemon.csv
    o << ((p.id != -1) ? std::to_string(p.id) : "") << ",";
    o << ((p.name != "-1") ? p.name : "") << ",";
    o << ((p.species != -1) ? std::to_string(p.species) : "") << ",";
    o << ((p.height != -1) ? std::to_string(p.height) : "") << ",";
    o << ((p.weight != -1) ? std::to_string(p.weight) : "") << ",";
    o << ((p.base_experience != -1) ? std::to_string(p.base_experience) : "") << ",";
    o << ((p.order != -1) ? std::to_string(p.order) : "") << ",";
    o << ((p.is_default != -1) ? std::to_string(p.is_default) : "");
    return o;
}
