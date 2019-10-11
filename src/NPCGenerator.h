/*
 * NPCGenerator.h
 *
 *  Created on: Feb 25, 2019
 *      Author: Eric
 */
#ifndef NPCGENERATOR_H_
#define NPCGENERATOR_H_
#include <string>
#include <fstream>
using namespace std;

//=====================USER INPUT FUNCTIONS===============

void command(string input, string arg1); //Function takes input from parse and runs correlating functions
void parse(string input); //Takes in user input and parses it into tokens before sending it to command

//=====================NPC CLASS FUNCTIONS================

class NPC {

	//Attributes of characters
	string race;
	int age;
	string firstName;
	bool hasLastName;
	string lastName;
	string name;
	string profession;
	string description;
	string feature;

	//TODO
	//General backstory

public:
	NPC(); //Constructor to build a brand new character
	NPC(string filePath); //Constructor to load a character from a file
	bool decideLastName(string race); //Function to decide if the character has a last name
	void display(); //Function to print the characters information to the screen
	void write(string fileName); //Function to save a character to the characters directory with a given file name
	void write(); //Function to save a character to the characters directory with the characters first name as a fileName

	string getName(); //Not really used

};

//=====================READ FROM FILE FUNCTIONS===============

string getNameFromFile(string fileName); //Gets the characters name from a file
string getRaceFromFile(string fileName); //Gets the characters race from a file
int getAgeFromFile(string fileName); //Gets the characters age from a file
string getProfFromFile(string fileName); //Gets the characters profession from a file
string getDescFromFile(string fileName); //Gets the characters description from a file
string getFeatureFromFile(string fileName); //Gets the characters feature from a file

//=====================RANDOM SELECTION FUNCTIONS=============

int randomNum(int max); //Returns a random number between 1 and a given max

string randomRace(); //Returns a random race

int randomAge(string race); //Returns a random age based on the characters race

string randomFirstName(string race); //Returns a random first name based on the characters race

string randomLastName(string race); //Returns a random last name based on the characters race

string randomProfession(); //Returns a random profession

string randomFeature(string race); //Returns a random feature to help shape the character

//=====================RANDOM DESCRIPTION FUNCTIONS===========

//Random description uses lots of functions so I gave it its
//own section to reflect that

string randomDescription(string race); //Returns a description about the character based on their race

int randomHeight(string race); //Returns the height in ft based on a given race

int randomWeight(string race); //Returns the weight in lbs based on a given race

string randomHairColor(); //Returns a random hair color

string randomHair(); //Returns a random hair type

string randomEyeColor(string race); //Returns a random Eye color based on race

#endif /* NPCGENERATOR_H_ */
