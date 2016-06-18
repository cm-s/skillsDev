/*
 * adv_game (v6.7.4)
 * Adventuring game in which a player tarverses a retro-style board or grid, solving pizzles and avoiding enmeies
 * in order to get to the next level and ultimately win the game.
 *
 * CMS
 * Date created: 4-26-16, 10:41am
 */
#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <stdlib.h>
#include "external/objs.h"
#include "external/console_reader.h"
using namespace std;

/*Random functions*/
/**
 * Function returning either zero or a number higher than zero and no higher than one with a fifty percent
 * chance of either outcome.
 *
 * @return the random number.
 */
int flip(); int flip() { return rand() %2; };
/**
 * Function for generating a random number between thirteen and four.
 *
 * @return the random number.
 */
int randomCell(); int randomCell() { srand(time(NULL));
                                     return rand() % 13 + 4; };

 /**
  * Method handling the printing and basic control of the main menu.
  *
  * @param game level variable.
  * @param input reading object.
  */
void welcomeSequence(short int game_level, InputReader* cinreader);
/**
 * Logic for handling the picking up of items within the game.
 *
 * @param player picking up "objects".
 * @param grid variable.
 * @parma variable for saving the game state.
 * @param array containing all possible key combos in the game.
 * @param number representing the current active keyset.
 * @param conditional determining if random key is set for key pickup.
 */
void item_pickup(player& character, char current_grid[20][20], bool& gameOver, int keyring[10][2], int keySet, bool do_rand);
/**
 * Function generating a giving structure within the main grid.
 *
 * @param grid variable.
 * @param structure to be generated within the grid.
 */
void gen_struct(char current_grid[20][20], const char templateStruct[5][5]);
/**
 * Function for printing the grid to the console two dimentionally.
 *
 * @param grid variable.
 * @parm size of grid.
 */
void display_grid(char current_grid[20][20], int grid_size);
/**
 * Method for overwriting the entire grid with a level template.
 *
 * @param level to overwrite grid.
 * @param grid variable.
 */
void levelConstruct(const char level[20][20], char current_grid[20][20]);
/**
 * Simple method printing enpty lines to the console. Used to make space for another frame of the game.
 *
 */
void make_space();
/**
 * Most basic method of getting and handling user directional input.
 *
 * @param character to be moved.
 * @param variable storing direction to move the player in.
 */
void action_input(player& character, char move_direction);
/**
 * Advanced method for getting and handling user direcitonal and command input.
 *
 * @param grid variable.
 * @param player to be moved.
 * @param first gate object.
 * @param second gate object.
 * @param variable storing directinal input.
 * @param object instantination for input methods(console_reader)
 */
void action_input(char current_grid[20][20], player& character, lock* gatekeeper1, lock* gatekeeper2, char move_direction, InputReader* cinreader);
/**
 * Client side methodology for checking if the player is attempting to move inside a gate.
 *
 * @param grid variable.
 * @param character to be checked against gate's coords.
 * @param fist gate object (in lock class)
 * @param second gate object (in lock class)
 */
void gate_check(char current_grid[20][20], player& character, lock* gatekeeper1, lock* gatekeeper2);
/**
 * Method for executing display function and for resetting the last place the player appeared on the grid.
 *
 * @param grid variable.
 * @param subjected player.
 * @param size of the grid.
 * @param direction the palyer has moved.
 */
void step(char current_grid[20][20], player& character, int grid_size, char& move_direction);
/**
 * Logic for player and grid cell and object reset. Resets the player to the last oocupiable space
 * if the player has moved into an object.
 *
 * @param grid variable.
 * @param character being reset.
 * @param size of the grid.
 * @param direction last moved.
 * @param variabel representing the object that has been moved into.
 */
void obj_reset(char current_grid[20][20], player& character, int grid_size, char& move_direction, char client);
/**
 * Funciton to indicate wether or not the player has moved into a designeted space within the grid.
 *
 * @param grid variable.
 * @param player to be scanned for.
 * @param left-top grid cell coordinate to define starting point for scanning. Single intager representing x and y.
 * @param defining size of the area that will be scanned for the player's prescense.
 * @return wether or not the player has been detected in the scanning area.
 */
bool detected_in(char current_grid[20][20], player character, int LTstart, int area);
/**
 * Logic for checking wether the player has died, signaling the user and ending the game.
 *
 * @param main character.
 * @param global game status variable
 */
void is_alive(player character, bool& gameOver);
/**
 * Funciton handling the visiblity of the enemy and making the enemy's movement visible.
 *
 * @param grid variable.
 * @param enemy to be visualized.
 * @param player being passed into the enemy's interactive functionallity.
 */
void enemy_action(char current_grid[20][20], enemy* MonsterOne, player& character);
/**
 * Indicator for what object an entity is moving into, if any.
 *
 * @param grid variable.
 * @param enemy subject to indication.
 * @return the cell type of any object that may be returned.
 */
char justify_move(char current_grid[20][20], player subject);
/**
 * Simple method indicating which direction a given entity is approaching anoter from.
 *
 * @param entity being checked for approach to subject.
 * @param subject being checked for approach from approachingEnt.
 * @return direction the approaching entity is approaching the subject from.
 */
char deduct_approach(player approachingEnt, box subject);

/*BEGIN MAIN*/

int main()
{
/*INITIALIZING BEGIN*/

//grid initializations

const char cave[5][5] = {
    {'#','#','#','#','#'},
    {'#','^',' ',' ','#'},
    {'#','#','#',' ','#'},
    {'#','^',' ',' ','#'},
    {'#','#','#','_','#'},
};
const char boulder[5][5] = {
    {' ','#','#','#',' '},
    {'#','#','#','#','#'},
    {'#','#','^','W','W'},
    {'#','#','^','#','#'},
    {' ','#','#','#',' '},
};
const char initial_grid[20][20] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
};
const char levelTwo[20][20] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', 'T', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', '@', '=', '=', '@', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '+', ' ', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', 'X', ' ', ' ', '#', ' ', ' ', ' ', ' ', '+', '+', '+', ' ', ' ', ' ', '#', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
};
const char levelThree[20][20] = {           /*fist key here*/
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', '#', '^', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '/', 'H', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X', '#'},
    {'#', ' ', ' ', ' ', '#', '#', ' ', ' ', 'H', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '/', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
};
char current_grid[20][20] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', 'X', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
};

//non-grid variables
string input; //for the str_to_int() fxn
const int grid_size = 20;
char move_direction;
bool gameOver = false;
short int game_level = 0;
int keyring[10][2] = {
    {323, 232}, {890, 208},
    {157, 176}, {225, 755},
    {114, 495}, {611, 815},
    {212, 311}, {732, 326},
    {985, 860}, {615, 524}
};
/*INITIALIZING END*/

InputReader *cinreader;
cinreader = new InputReader();
welcomeSequence(game_level, cinreader);

gen_struct(current_grid, cave);//generating structure
player character;//init player

cout << "\nUse the WASD keys to move your character.";
cout << "\nUse the C key to enter a command." << endl;
game_level = 1;
move_direction = 'w';
enemy *MonsterOne;
MonsterOne = new enemy(25, 100);

do {
    character.prev_x_coord = character.x;
    character.prev_y_coord = character.y;
    if (character.firstPlay == false) {
        make_space();
    };
    character.firstPlay = false;
    action_input(character, move_direction);
    if (detected_in(current_grid, character, 3, 2) == true) {//this block needs to be a function
        if (MonsterOne -> entry != true) {
            MonsterOne -> x = 4; MonsterOne -> y = 4;
            current_grid[MonsterOne -> x][MonsterOne -> y] = MonsterOne -> skin;
            MonsterOne -> entry = true;
        };
    };
    if (justify_move(current_grid, character) == 'M') {
        MonsterOne -> looseLife(current_grid, character);
        cout << "\nYou've hit the enemy!" << endl;
        obj_reset(current_grid, character, grid_size, move_direction, 'M');
    };
    enemy_action(current_grid, MonsterOne, character);
    is_alive(character, gameOver);
    if (justify_move(current_grid, character) == '#') {
        obj_reset(current_grid, character, grid_size, move_direction, '#');
    } else {
        item_pickup(character, current_grid, gameOver, keyring, 0, true);
        step(current_grid, character, grid_size, move_direction);
    };
} while((gameOver == false) && (game_level == 1));

    //setup level two
    delete MonsterOne; MonsterOne = NULL;
    move_direction = 's';
    gameOver = false;
    character.firstPlay = true;
    levelConstruct(levelTwo, current_grid);
    game_level = 2;

    lock *gatekeeper1; gatekeeper1 = new lock(); gatekeeper1 -> x = 11; gatekeeper1 -> y = 16; gatekeeper1 -> extX = 11; gatekeeper1 -> extY = 15;
    gatekeeper1 -> key = keyring[0][0]; gatekeeper1 -> gateSignature = std::to_string(keyring[0][0]).erase(0, 1);
    lock *gatekeeper2; gatekeeper2 = new lock(); gatekeeper2 -> x = 11; gatekeeper2 -> y = 13; gatekeeper2 -> extX = 11; gatekeeper2 -> extY = 14;
    gatekeeper2 -> key = keyring[0][1]; gatekeeper2 -> gateSignature = std::to_string(keyring[0][1]).erase(0, 1);
    current_grid[gatekeeper1 -> x][gatekeeper1 -> y] = gatekeeper1 -> skin;
    current_grid[gatekeeper2 -> x][gatekeeper2 -> y] = gatekeeper2 -> skin;

    do {
        character.prev_x_coord = character.x;
        character.prev_y_coord = character.y;
        make_space();
        if (character.firstPlay == true) { cout << "\nLevel Two" << endl; };
        character.firstPlay = false;
        action_input(current_grid, character, gatekeeper1, gatekeeper2, move_direction, cinreader);
        gate_check(current_grid, character, gatekeeper1, gatekeeper2);
        if (justify_move(current_grid, character) == '#') {
            obj_reset(current_grid, character, grid_size, move_direction, '#');
        } else if (justify_move(current_grid, character) == '=') {
            obj_reset(current_grid, character, grid_size, move_direction, '=');
        } else {
            item_pickup(character, current_grid, gameOver, keyring, 1, true);
            step(current_grid, character, grid_size, move_direction);
        };
    } while((gameOver == false) && (game_level == 2));
        //cleaning up for level three
        delete character.keeper1key; delete character.keeper2key; character.keeper1key = NULL; character.keeper2key = NULL;
        move_direction = 'w';
        gameOver = false;
        character.firstPlay = true;
        delete character.keeper1key; delete character.keeper2key;
        levelConstruct(levelThree, current_grid);
        game_level = 3;

        character.keeper1key = new item(1); character.keeper2key = new item(2);
        gatekeeper1 -> x = 9; gatekeeper1 -> y = 4;
        gatekeeper1 -> key = keyring[1][0]; gatekeeper1 -> gateSignature = std::to_string(keyring[1][0]).erase(0, 1);
        current_grid[gatekeeper1 -> x][gatekeeper1 -> y] = gatekeeper1 -> skin;

        box box1; box1.x = 2; box1.y = 5; current_grid[box1.x][box1.y] = 'H';
        box box2; box2.x = 3; box2.y = 8; current_grid[box2.x][box2.y] = 'H';

        do {
            character.prev_x_coord = character.x;
            character.prev_y_coord = character.y;
            make_space();
            if (character.firstPlay == true) { cout << "\nLevel three" << endl; };
            character.firstPlay = false;
            action_input(current_grid, character, gatekeeper1, gatekeeper2, move_direction, cinreader);
            gate_check(current_grid, character, gatekeeper1, gatekeeper2);
            if (justify_move(current_grid, character) == '/') {
                character.gateSignature = "dr";
                character.gateSignature += (char) character.x;
                character.gateSignature += (char) character.y;
                obj_reset(current_grid, character, grid_size, move_direction, '/');
            };
            if (justify_move(current_grid, character) == 'H') {
                if (character.x == box1.x) {
                    box1.approach = deduct_approach(character, box1);
                    box1.move();
                    current_grid[box1.x][box1.y] = 'H';//you aren't erasing the previous 'H'
                };
                if (character.x == box2.x) {
                    box2.approach = deduct_approach(character, box2);
                    box2.move();
                    current_grid[box2.x][box2.y] = 'H';
                };
            };
            if (justify_move(current_grid, character) == '#') {
                obj_reset(current_grid, character, grid_size, move_direction, '#');
            } else {
                item_pickup(character, current_grid, gameOver, keyring, 1, false);
                step(current_grid, character, grid_size, move_direction);
            };
        } while((gameOver == false) && (game_level == 3));

            //setting up level four
            game_level = 4;
        std::exit(EXIT_FAILURE);

return 0;
};

/*END OF MAIN*/

void welcomeSequence(short int game_level, InputReader* cinreader) {
    string command;
    cout << "#           Main Menu          #" << endl;//all lines allign with this one
    cout << "# Type \"options\" for options   #" << endl;
    cout << "# Type \"commands\" for commands #" << endl;
    cout << "# Type \"go\" to start the game  #" << endl;
    cinreader -> input_constructor("string", "options commands go");
    command = cinreader -> insert_string();
    if (command == "commands") {
        string consent;
        make_space();
        cout << "\nLevel one has no commands." << endl;
        cout << "\nList of commands for level two is as follows:" << endl;
            cout << "\n    \"unlock\": unlocks a gate.";
            cout << "\n    \"keys\": tells you what keys you have.";
            cout << endl;
        cout << "\nList of commands for level three is as follows:" << endl;
            cout << "\n    \"unlock\": unlocks a gate after you've tried moving through it.";
            cout << "\n    \"keys\": tells you what keys you have.";
            cout << "\n    \"open\": opens a door after you've tried to move though it.";
            cout << endl;

        cout << "\nOkay? (y/n)" << endl;
        cinreader -> input_constructor("bool");
        make_space();
        welcomeSequence(game_level, cinreader);
    } else if (command == "go") {
        make_space();
    } else if (command == "options") {
        int input;
        make_space();
        cout << "\nType the level you would like to go to: ";
        cinreader -> input_constructor("int");
        input = cinreader -> insert_intager();
        game_level = input;
    } else {
        make_space();
        cout << "\nTry Typing something else." << endl;;
        welcomeSequence(game_level, cinreader);
    };
};

void display_grid(char current_grid[20][20], int grid_size) {
    for (size_t row =0; row <= (grid_size -1); row++) {
        for (size_t col =0; col <= (grid_size -1); col++) {
            cout << current_grid[row][col] << ' ';
        };
        cout << endl;
    };
};

void gen_struct(char current_grid[20][20], const char templateStruct[5][5]) {
    int posNum = randomCell();
    if (posNum > 13) {
        int minus = posNum % 13;
        posNum -= minus;
    };
    //looping though the template and setting values to corresponding grid cells (plus random)
    for (size_t col = 0; col < 5; col++) {
        for (size_t row = 0; row < 5; row++) {
            current_grid[posNum +col][posNum +row] = templateStruct[col][row];
        };
    };
};

void item_pickup(player& character, char current_grid[20][20], bool& gameOver, int keyring[10][2], int keySet, bool do_rand) {
    int printk;
    switch(current_grid[character.x][character.y]) {
        case '^':
            if (do_rand == true) {
                if (flip() == 0 && character.keeper1key -> id != std::to_string(keyring[keySet][0]).erase(0, 1)) {
                    character.keeper1key -> setKey(keyring[keySet][0]);
                    character.keeper1key -> id = std::to_string(keyring[keySet][0]).erase(0, 1);
                    printk = character.keeper1key -> getKey();
                } else {
                    character.keeper2key -> setKey(keyring[keySet][1]);
                    character.keeper2key -> id = std::to_string(keyring[keySet][1]).erase(0, 1);
                    printk = character.keeper2key -> getKey();
                };
            } else {
                character.keeper1key -> setKey(keyring[1][0]);
                character.keeper1key -> id = std::to_string(keyring[1][0]).erase(0, 1);
                printk = character.keeper1key -> getKey();
            };
            cout << "\nYou found a key. The number " << printk << " is on it." << endl;
            break;
        case 'X':
            cout << "\nLevel Completed" << endl;
            gameOver = true;
            break;
        case '+':
            character.gainLife(10);
            cout << "\nYou have found a health charm-shard." << endl;
            cout << "\nYour health is now at " << character.getLife() << endl;
            break;
        case 'T':
            character.setDamage(50);
            cout << "\nYou've found a sword! Your damage has increased to " << character.getDamage() << "." << endl;
            break;
    };
};

bool detected_in(char current_grid[20][20], player character, int LTstart, int area) {
    for (size_t col = 0;col < area; col++) {
    for (size_t row = 0;row < area; row++) {
        if (current_grid[LTstart +col][LTstart +row] == character.skin) {return true; }
        else if ((col == area) && (row == area)) { return false; };
    };
    };
};

void levelConstruct(const char level[20][20], char current_grid[20][20]) {
    for (size_t col = 0; col <= 20; col++) {
        for (size_t row = 0; row < 20; row++) {
            current_grid[col][row] = level[col][row];
        };
    };
};

void make_space() {
    for (size_t space = 0; space <= 15; space++) {
        cout << endl;
    };
};

void action_input(char current_grid[20][20], player& character, lock* gatekeeper1, lock* gatekeeper2, char move_direction, InputReader* cinreader) {
    string command;
    switch (toupper(move_direction)) {
        case 'W':
            character.x -= 1;
            break;
        case 'A':
            character.y -= 1;
            break;
        case 'S':
            character.x += 1;
            break;
        case 'D':
            character.y += 1;
            break;
        case 'C':
            cout << endl; cout << endl; cout << endl;
            cout << "\nEnter an action: " << endl;
            //cinreader -> input_constructor("string", "unlock keys open");
            //command = cinreader -> insert_string();
            cin >> command;
            if (command == "unlock") {
                if (character.gateSignature == gatekeeper1 -> gateSignature) {
                    gatekeeper1 -> unlock(current_grid, character);
                    if ((gatekeeper2 -> unlocked == true) && (gatekeeper1 -> unlocked == true) && (character.keeper2key -> id != "00")) {
                        current_grid[gatekeeper1 -> extX][gatekeeper1 -> extY] = '|';
                        current_grid[gatekeeper2 -> extX][gatekeeper2 -> extY] = '|';
                    };
                } else if (character.gateSignature == gatekeeper2 -> gateSignature) {
                    gatekeeper2 -> unlock(current_grid, character);
                    if ((gatekeeper2 -> unlocked == true) && (gatekeeper1 -> unlocked == true)) {
                        current_grid[gatekeeper1 -> extX][gatekeeper1 -> extY] = '|';
                        current_grid[gatekeeper2 -> extX][gatekeeper2 -> extY] = '|';
                    };
                } else {
                    cout << "\nThere isn't a gate." << endl;
                };
            } else if (command == "keys") {
                if ((character.keeper1key -> id != "00") || (character.keeper2key -> id != "00")) {
                    cout << "\nYou have key ";
                } else { cout << "\nYou have no keys."; };
                if (character.keeper1key -> id != "00") {
                    cout << character.keeper1key -> getKey();
                    if (character.keeper2key -> id != "00") {
                        cout << " and key " << character.keeper2key -> getKey() << ".";
                    } else { cout << "."; };
                } else {
                    if (character.keeper2key -> id != "00") {
                        cout << character.keeper2key -> getKey() << ".";
                    };
                };
                cout << endl;
            } else if ((command == "open") && character.gateSignature[0] == 'd') {
                current_grid[(int) character.gateSignature[2]][(int) character.gateSignature[3]] = '_';
                cout << "\nDoor opened." << endl;
            } else {
                cout << "\nThat is not a command." << endl;
            };
            break;
        default:
            cout << "\nThat is not a move." << endl;
            break;
    };
};

void action_input(player& character, char move_direction) {
    switch (toupper(move_direction)) {
        case 'W':
            character.x -= 1;
            break;
        case 'A':
            character.y -= 1;
            break;
        case 'S':
            character.x += 1;
            break;
        case 'D':
            character.y += 1;
            break;
        default:
            cout << "\nThat is not a move." << endl;
            break;
    };
};

void gate_check(char current_grid[20][20], player& character, lock* gatekeeper1, lock* gatekeeper2) {
    if (current_grid[character.x][character.y] == '@') {//maybe repalce the "if()" with a single function later
        if ((character.x == gatekeeper1 -> x) && (character.y == gatekeeper1 -> y)) {
            gatekeeper1 -> detect(current_grid, character);
        } else {
            gatekeeper2 -> detect(current_grid, character);
        };
    };
};

void step(char current_grid[20][20], player& character, int grid_size, char& move_direction) {
    current_grid[character.x][character.y] = character.skin;
    //printing grid again
    display_grid(current_grid, grid_size);
    //replacing last cell that the character was in
    current_grid[character.x][character.y] = ' ';
    cin >> move_direction;
};

void obj_reset(char current_grid[20][20], player& character, int grid_size, char& move_direction, char client) {
    if (client == '#') { cout << "\nYou cannot move here." << endl; };
    if (client == '/') { cout << "\nA door is blocking your path. You should be able to open it." << endl; };
    if (client == '=') { cout << "\nThere's a gate here. Maybe there's a lock nearby." << endl; };
    character.x = character.prev_x_coord;//setting player to previous coords (before moved into a wall)
    character.y = character.prev_y_coord;
    if (client != '/') { display_grid(current_grid, grid_size); };
    if (client != '/') { cin >> move_direction; };
};

void is_alive(player character, bool& gameOver) {
    if (character.getLife() == 0) {
        cout << "\nYou died." << endl;
        gameOver = true;
        delete &character;
    };
};

void enemy_action(char current_grid[20][20], enemy* MonsterOne, player& character) {
    if (MonsterOne -> entry == true) {
        MonsterOne -> prev_x_coord = MonsterOne -> x; MonsterOne -> prev_y_coord = MonsterOne -> y;
        MonsterOne -> auto_move(current_grid, character);
        current_grid[MonsterOne -> x][MonsterOne -> y] = MonsterOne -> skin;
        current_grid[MonsterOne -> prev_x_coord][MonsterOne -> prev_y_coord] = ' ';
    };
};

char justify_move(char current_grid[20][20], player subject) {
    switch (current_grid[subject.x][subject.y]) {
        case '#': return '#';
        case 'M': return 'M';
        case 'H': return 'H';
        case '/': return '/';
        case '_': return '_';
        case '=': return '=';
    };
};

char deduct_approach(player approachingEnt, box subject) {
    if (approachingEnt.prev_y_coord < subject.y) { return 'l'; };
    if (approachingEnt.prev_y_coord > subject.y) { return 'r'; };
    if (approachingEnt.prev_x_coord > subject.x) { return 'd'; };
    if (approachingEnt.prev_x_coord < subject.x) { return 'u'; };
};
