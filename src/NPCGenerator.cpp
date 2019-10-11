//============================================================================
// Name        : NPCGenerator.cpp
// Author      : Eric Mock
// Description :
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> //For rand()
#include <time.h> //Grabs time for the random functions
#include "NPCGenerator.h"
using namespace std;

//======================FIELDS============================

bool hasCharacter = false; //Whether or not an NPC object is in existence
NPC *npc; //Global level npc object to make sure we're not using to many resources

//======================MAIN==============================


//Currently just waits for input and terminates if the user enters "quit".
int main() {
	string input;
	cout << "Welcome to NPC Generator!" << '\n';
	cout << "Type \"help\" for help!" << '\n';
	while (true) {
		getline(cin, input);
		if (input == "quit") { //Terminate
			break;
		}
		parse(input);
	}

	return 0;
}

//=====================USER INPUT FUNCTIONS===============

//Function to take user input and arguments and perform functions based on those
void command(string input = "", string arg1 = "") {
	if (input == "new" && !hasCharacter) { //Creates a new character
		hasCharacter = true;
		npc = new NPC();
		cout << "New character made!" << '\n';
	}
	else if(input == "write" && !(arg1 == "") && hasCharacter) { //Saves a character with a given filename
		npc->write(arg1);
		cout << "Character saved as " << arg1 << ".txt!" << '\n';
	}
	else if(input == "write" && hasCharacter) { //Saves a character with its default filename
		npc->write();
		cout << "Character saved!" << '\n';
	}
	else if(input == "load" && !hasCharacter && !(arg1 == "")) { //Loads a saved character
		npc = new NPC(arg1);
		hasCharacter = true;
		cout << npc->getName() << " has been loaded!" << '\n';
	}
	else if(input == "load" && hasCharacter && !(arg1 == "")) { //Loads a saved character
		delete npc;
		npc = new NPC(arg1);
		hasCharacter = true;
		cout << npc->getName() << " has been loaded!" << '\n';
	}
	else if(input == "new" && hasCharacter) { //Creates a new character
		delete npc;
		npc = new NPC();
		cout << "New character made!" << '\n';
	}
	else if(input == "display" && hasCharacter) { //Displays the characters information
		npc->display();
	}
	else if(input == "help") { //Tells the user what functions are available
		cout << "new: Creates a new character." << '\n';
		cout << "display: Prints the current character's attributes to the screen." << '\n';
		cout << "write -fileName: Saves the current character." << '\n';
		cout << "load -fileName: Loads a character from a file." << '\n';
		cout << "help: This command." << '\n';
		cout << "quit: Terminates the program." << '\n';
	}
	else {
		cout << "Unrecognized command." << '\n';
	}
}

//Function to go through user input to separate the commands and arguments

void parse(string input) {
	//Currently only breaks input into two tokens, no need to have more
	string i1 = input; //If there's no arguments, just send the whole thing
	string i2 = "";
	for (int i = 0; i < (int)input.length(); i++) { //Probably a better way to do this, probably just a bool function that can check if the delimiter is in the input
		if (input.at(i) == '-') {
			string delimiter = " -";
			int pos = input.find(delimiter);
			i1 = input.substr(0,pos);
			input.erase(0,pos + delimiter.length());
			i2 = input;
		}
	}
	command(i1,i2);
}

//=====================NPC CLASS FUNCTIONS================

//Constructor to make a brand new character
NPC::NPC() {
	race = randomRace();
	age = randomAge(race);
	firstName = randomFirstName(race);
	hasLastName = decideLastName(race);
	if (hasLastName) {
		lastName = randomLastName(race);
		name = firstName + " " + lastName;
	} else {
		lastName = "";
		name = firstName;
	}
	profession = randomProfession();
	description = randomDescription(race);
	feature = randomFeature(race);
}

//Constructor to load a character from a file

NPC::NPC(string filePath) {
	name = getNameFromFile(filePath);
	race = getRaceFromFile(filePath);
	age = getAgeFromFile(filePath);
	profession = getProfFromFile(filePath);
	description = getDescFromFile(filePath);
	feature = "";
	hasLastName = false; //default as no last name unless we find a last name

	//Split the name into a first and last name if there is a last name
	int n;
	for (int i = 0; i < (int)name.length(); i++) {
		if (name.at(i) == ' ') {
			n = i;
			hasLastName = true;
			for (int j = 0; j < n; j++) {
				firstName = firstName + name.at(j);
			}
			for (int j = n+1; j < (int)name.length(); j++) {
				lastName = lastName + name.at(j);
			}
		}
	}
	if (!hasLastName) {
		firstName = name;
		lastName = "";
	}
}

//Determines whether or not someone has a last name based on their race
bool NPC::decideLastName(string race) {
	if (race == "Altmer") { //Only a small number of Altmer have Last Names
		int n = randomNum(100);
		if (n > 80) {return true;}
	}
	if (race == "Bosmer") { //Only a small number of Bosmer have Last Names
		int n = randomNum(100);
		if (n > 80) {return true;}
	}
	if (race == "Breton") {return true;} //Always have a last name
	if (race == "Dunmer") {return true;} //Always have a last name
	if (race == "Imperial") {return true;} //Always have a last name
	if (race == "Nord") { //Some Nords have clan names
		int n = randomNum(100);
		if (n > 60) {return true;}
	}
	if (race == "Orc") {return true;} //Always have a last name
	if (race == "Redguard") { //Very little Redguards have last names
		int n = randomNum(100);
		if (n > 90) {return true;}
	}
	return false; //Khajiit and Argonians never have last names.
}

//Prints the characters information to the console
void NPC::display() {
	cout << name << '\n';
	cout << race << '\n';
	cout << profession << '\n';
	cout << "They are " << age << " years old." << '\n';
	cout << description << '\n';
}

//Saves the character to a txt file with a given filename
void NPC::write(string fileName) {
	string filePath = "characters\\" + fileName + ".txt";
	ofstream file;
	file.open(filePath);
	file << "NAME" << '\n';
	file << name << '\n';
	file << "RACE" << '\n';
	file << race << '\n';
	file << "AGE" << '\n';
	file << age << '\n';
	file << "PROFESSION" << '\n';
	file << profession << '\n';
	file << "DESCRIPTION" << '\n';
	file << description << '\n';
	file << "ENDDESCRIPTION" << '\n';
	file << "FEATURE" << '\n';
	file << feature << '\n';
	file.close();
}

//Saves the character to a txt file with the default filename
void NPC::write() {
	string filePath = "characters\\" + firstName + ".txt";
	ofstream file;
	file.open(filePath);
	file << "NAME" << '\n';
	file << name << '\n';
	file << "RACE" << '\n';
	file << race << '\n';
	file << "AGE" << '\n';
	file << age << '\n';
	file << "PROFESSION" << '\n';
	file << profession << '\n';
	file << "DESCRIPTION" << '\n';
	file << description << '\n';
	file << "ENDDESCRIPTION" << '\n';
	file.close();
}


string NPC::getName() {
	return name;
}

//=====================READ FROM FILE FUNCTIONS===========

//Get the name from a saved character
string getNameFromFile(string fileName){
	string filePath = "characters\\" + fileName + ".txt";
	ifstream file;
	file.open(filePath);
	string name;
	while (!(name == "NAME") && file.is_open()) { //Essentially go through the txt until you find the NAME field
		getline(file,name);
	}
	if (name == "NAME") { //If we found a name field, return the next line
		getline(file,name);
		return name;
	}
	return "NONAME"; //If no name was found, return this

}

//Get the race from a saved character, same premise as above
string getRaceFromFile(string fileName) {
	string filePath = "characters\\" + fileName + ".txt";
	ifstream file;
	file.open(filePath);
	string race;
	while (!(race == "RACE") && file.is_open()) {
		getline(file,race);
	}
	if (race == "RACE") {
		getline(file,race);
		return race;
	}
	return "NORACE";
}

//Get the age from a saved character, same premise as above.
int getAgeFromFile(string fileName) {
	string filePath = "characters\\" + fileName + ".txt";
	ifstream file;
	file.open(filePath);
	string tempAge;
	while (!(tempAge == "AGE") && file.is_open()) {
		getline(file,tempAge);
	}
	if (tempAge == "AGE") {
		getline(file,tempAge);
		return stoi(tempAge); //Convert the string to an int
	}
	return -1; //Return this if no age is found.
}

//Get the profession from a saved character, same premise as above
string getProfFromFile(string fileName) {
	string filePath = "characters\\" + fileName + ".txt";
	ifstream file;
	file.open(filePath);
	string tempProf;
	while (!(tempProf == "PROFESSION") && file.is_open()) {
		getline(file,tempProf);
	}
	if (tempProf == "PROFESSION") {
		getline(file,tempProf);
		return tempProf;
	}
	return "NO PROFESSION FOUND";
}

//Get the description from a saved character
string getDescFromFile(string fileName) {
	string filePath = "characters\\" + fileName + ".txt";
	ifstream file;
	file.open(filePath);
	string tempDesc = "";
	while (!(tempDesc == "DESCRIPTION") && file.is_open()) {
		getline(file,tempDesc);
	}
	if (tempDesc == "DESCRIPTION") {
		string temp;
		getline(file,temp);
		while (!(temp == "ENDDESCRIPTION")) {
			tempDesc = tempDesc + '\n' + temp;
			getline(file,temp);
		}
		return tempDesc;
	}
	file.close();
	return "NO DESCRIPTION FOUND";
}

//Get the feature from a saved character
string getFeatureFromFile(string fileName) {
	string filePath = "characters\\" + fileName + ".txt";
	ifstream file;
	file.open(filePath);
	string tempFeat = "";
	while (!(tempFeat == "FEATURE") && file.is_open()) {
		getline(file,tempFeat);
	}
	if (tempFeat == "FEATURE") {
		getline(file,tempFeat);
	}
	return tempFeat;
}

//=====================RANDOM SELECTION FUNCTIONS=========


//Randomly returns a number between 1 and the given max
int randomNum(int max = 1) {
	srand(time(NULL)); //This stops rand from constantly outputting 42
	return (rand() % max)+1;
}

//Returns one of the 10 most common civilized races of Tamriel
string randomRace() {
	int n = randomNum(10);
	string race;
	switch(n)
	{
	case 1:
		race = "Altmer";
		break;
	case 2:
		race = "Argonian";
		break;
	case 3:
		race = "Bosmer";
		break;
	case 4:
		race = "Breton";
		break;
	case 5:
		race = "Dunmer";
		break;
	case 6:
		race = "Imperial";
		break;
	case 7:
		race = "Khajiit";
		break;
	case 8:
		race = "Nord";
		break;
	case 9:
		race = "Orc";
		break;
	case 10:
		race = "Redguard";
		break;
	}
	return race;
}

//Returns a random age for the character, the mer races lifespan maxes around 300 while the rest 100
int randomAge(string race) {
	if (race == "Altmer" || race == "Bosmer" || race == "Dunmer") {
		return randomNum(285)+15;
	}else {
		return randomNum(85)+15;
	}
}

//Returns a random name based on the given race
string randomFirstName(string race){
	string filePath = "data\\";
	string name = "BADRACE"; //Just an output if something breaks
	int n;
	ifstream file;

	//Check which file to open, depending on Race
	if (race == "Altmer") {
		n = randomNum(867);
		file.open(filePath + "altmerFirstNames.txt");
	}
	if (race == "Argonian") {
		n = randomNum(639);
		file.open(filePath + "argonianFirstNames.txt");
	}
	if (race == "Bosmer") {
		n = randomNum(670);
		file.open(filePath + "bosmerFirstNames.txt");
	}
	if (race == "Breton") {
		n = randomNum(722);
		file.open(filePath + "bretonFirstNames.txt");
	}
	if (race == "Dunmer") {
		n = randomNum(852);
		file.open(filePath + "dunmerFirstNames.txt");
	}
	if (race == "Imperial") {
		n = randomNum(525);
		file.open(filePath + "imperialFirstNames.txt");
	}
	if (race == "Khajiit") {
		n = randomNum(648);
		file.open(filePath + "khajiitFirstNames.txt");
	}
	if (race == "Nord") {
		n = randomNum(707);
		file.open(filePath + "nordFirstNames.txt");
	}
	if (race == "Orc") {
		n = randomNum(645);
		file.open(filePath + "orcFirstNames.txt");
	}
	if (race == "Redguard") {
		n = randomNum(819);
		file.open(filePath + "redguardFirstNames.txt");
	}

	//Based on the random number generated, find the name that corresponds to that number in the file
	for (int i = 0; i < n; i++) {
		getline(file,name);
	}

	file.close(); //CLOSE IT!
	return name;
}

//Returns a random last name based on the given race.
string randomLastName(string race) {
	string filePath = "data\\";
	string name = "BADRACE"; //Just an output if something breaks
	int n;
	ifstream file;

	if (race == "Altmer") {
		n = randomNum(144);
		file.open(filePath + "altmerLastNames.txt");
	}
	if (race == "Bosmer") {
		n = randomNum(144);
		file.open(filePath + "bosmerLastNames.txt");
	}
	if (race == "Breton") {
		n = randomNum(353);
		file.open(filePath + "bretonLastNames.txt");
	}
	if (race == "Dunmer") {
		n = randomNum(393);
		file.open(filePath + "dunmerLastNames.txt");
	}
	if (race == "Imperial") {
		n = randomNum(247);
		file.open(filePath + "imperialLastNames.txt");
	}
	if (race == "Nord") {
		n = randomNum(297);
		file.open(filePath + "nordLastNames.txt");
	}
	if (race == "Orc") {
		n = randomNum(113);
		file.open(filePath + "orcLastNames.txt");
	}
	if (race == "Redguard") {
		n = randomNum(122);
		file.open(filePath + "redguardLastNames.txt");
	}

	//Based on the random number generated, find the name that corresponds to that number in the file
	for (int i = 0; i < n; i++) {
		getline(file,name);
	}

	file.close(); //CLOSE IT!
	return name;
}

//Returns a random profession
string randomProfession() {
	string filePath = "data\\";
	string prof = "whoops";
	int n = randomNum(91);
	ifstream file;
	file.open(filePath + "occupations.txt");
	for (int i = 0; i < n; i++) {
		getline(file,prof);
	}
	return prof;
}

//Returns a random feature
string randomFeature(string race) {
	return "";
}
//======================RANDOM DESCRIPTION FUNCTIONS======

//Returns a description of the character based on the given race
string randomDescription(string race) {
	string desc;
	int height = randomHeight(race);
	desc = "They are " + to_string(height/12) + "'" + to_string(height%12) + "\"" + '\n'
			+ "and weigh " + to_string(randomWeight(race)) + " lbs." + '\n'
			+ "Their hair is " + randomHairColor() + " and " + randomHair() + '\n'
			+ "Their Eyes are " + randomEyeColor(race) + ".";
	return desc;
}

//Returns a height in ft based on the given race
int randomHeight(string race) {
	if (race == "Altmer") {
		return 73 + randomNum(13);
	}else if(race == "Argonian") {
		return 68 + randomNum(8);
	}else if(race == "Bosmer") {
		return 47 + randomNum(22);
	}else if(race == "Breton") {
		return 63 + randomNum(14);
	}else if(race == "Dunmer") {
		return 62 + randomNum(14);
	}else if(race == "Imperial") {
		return 63 + randomNum(14);
	}else if(race == "Khajiit") {
		return 65 + randomNum(17);
	}else if(race == "Nord") {
		return 70 + randomNum(9);
	}else if(race == "Orc") {
		return 70 + randomNum(9);
	}else if(race == "Redguard") {
		return 65 + randomNum(12);
	}else {
		return -1; //Something went wrong
	}
}

//Returns a weight in lbs based on the given race
int randomWeight(string race) {
	if (race == "Altmer") {
		return 119 + randomNum(161);
	}else if(race == "Argonian") {
		return 134 + randomNum(91);
	}else if(race == "Bosmer") {
		return 99 + randomNum(91);
	}else if(race == "Breton") {
		return 104 + randomNum(146);
	}else if(race == "Dunmer") {
		return 104 + randomNum(146);
	}else if(race == "Imperial") {
		return 104 + randomNum(146);
	}else if(race == "Khajiit") {
		return 134 + randomNum(91);
	}else if(race == "Nord") {
		return 134 + randomNum(151);
	}else if(race == "Orc") {
		return 149 + randomNum(176);
	}else if(race == "Redguard") {
		return 114 + randomNum(146);
	}else {
		return -1;
	}
}

//Returns a random hair color
string randomHairColor() {
	string filePath = "data\\descriptions\\hairColors.txt";
	string color = "";
	ifstream file;
	file.open(filePath);
	int n = randomNum(11);
	for (int i = 0; i < n; i++) {
		getline(file,color);
	}
	return color;
}

//TODO make this based on race, argonians don't have hair
//Returns a random hair style
string randomHair() {
	string filePath = "data\\descriptions\\hairTypes.txt";
	string hair = "";
	ifstream file;
	file.open(filePath);
	int n = randomNum(33);
	for (int i = 0; i < n; i++) {
		getline(file,hair);
	}
	return hair;
}

//Returns the eye color
string randomEyeColor(string race) {
	string filePath = "data\\descriptions\\";
	ifstream file;
	int n;
	string color = "";
	if (race == "Altmer") {
		file.open(filePath + "altmerEyeColor.txt");
		n = randomNum(4);
		for (int i = 0; i < n; i++) {
			getline(file,color);
		}
		return color;
	}else if(race == "Imperial" || race == "Nord" || race == "Redguard" || race == "Breton") {
		file.open(filePath + "humanEyeColor.txt");
		n = randomNum(9);
		for (int i = 0; i < n; i++) {
			getline(file,color);
		}
		return color;
	}else if(race == "Argonian") {
		file.open(filePath + "argonianEyeColor.txt");
		n = randomNum(10);
		for(int i = 0; i < n; i++) {
			getline(file,color);
		}
		return color;
	}else if(race == "Dunmer") {
		file.open(filePath + "dunmerEyeColor.txt");
		n = randomNum(4);
		for(int i = 0; i < n; i++) {
			getline(file,color);
		}
		return color;
	}else if(race == "Khajiit") {
		file.open(filePath + "khajiitEyeColor.txt");
		n = randomNum(12);
		for(int i = 0; i < n; i++) {
			getline(file,color);
		}
		return color;
	}else if(race == "Orc") {
		file.open(filePath + "orcEyeColor.txt");
		n = randomNum(5);
		for(int i = 0; i < n; i++) {
			getline(file,color);
		}
		return color;
	}else if(race == "Bosmer") {
		file.open(filePath + "bosmerEyeColor.txt");
		n = randomNum(5);
		for(int i = 0; i < n; i++) {
			getline(file,color);
		}
		return color;
	}else {
		return "NO EYE COLOR";
	}
}
































