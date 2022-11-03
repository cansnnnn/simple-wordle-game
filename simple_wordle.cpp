// Cansýn Narþahin

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct cell
{
	char letter; // letter inside the cell
	char color; // color of the cell
};

bool lowercaseCheck(const string & guessWord)
{ //checks if the given word is all lowercase
	int count=0;
	for ( int i=0; i<guessWord.length();i++)
	{
		if (guessWord.at(i)<='z' && guessWord.at(i)>='a')
		{count ++;}
	}
	if ( count==guessWord.length())
	{return true;}
	else
	{return false;}
}
bool repeatedLetterCheck(const string & guessWord)
{ //checks for repeating letters
	for(int i=0;i<guessWord.length();i++)
	{
		char letter=guessWord.at(i);
		if(guessWord.find(letter)!= guessWord.rfind(letter))
		{return false;}
	}
	return true;
}
bool triedPrevCheck(const string & guessWord,const vector<vector<cell>> & record) 
{ //checks if the word has been tried before
	for(int i=0; i< record.size(); i++)
	{
		int count=0;
		for(int x=0; x< record[0].size(); x++)
		{ if(guessWord.at(x)==record[i][x].letter) {count ++;} 	}
		if ( count==guessWord.length())
		{return false;}
	}
	return true;
}
bool greenCheck(const string & guessWord,const vector<vector<cell>> & record, const int & attemptMade)
{// checks if a green letter is used again
	for(int i = 0;i<guessWord.length();i++)
	{
		if (attemptMade>0)
		{
			if((record[attemptMade-1][i].color=='G') && record[attemptMade-1][i].letter!=guessWord.at(i))
			{return false;}
		}
	}
	return true;
}

bool attemptCheck( const string & guessWord, const string & word, const vector<vector<cell>> & record, 
	const int & attemptMade)
{//true if the entered word fits the criteria, false otherwise
	bool check= false;

	if ( guessWord.length()> word.length())
	{cout<<"The word is too long!"<<endl;}

	else if ( guessWord.length()< word.length())
	{cout<<"The word is too short!"<<endl;}

	else if (!lowercaseCheck(guessWord))
	{cout<<"Your input has illegal letters!"<<endl;}

	else if (!repeatedLetterCheck(guessWord))
	{cout<<"Your input has a duplicate letter!"<<endl;}

	else if (!triedPrevCheck(guessWord, record))
	{cout<<"You've already tried this word!"<<endl;}

	else if (!greenCheck(guessWord,record, attemptMade))
	{cout<<"You did not use the green letter in your word!"<<endl;}

	else {check=true;}

	return check;
}


char colorCheck4add2record(const string & word, const string & guessWord, const int & index)
{// checks the position of letters and returns the appropriate colors
	if(guessWord.at(index)==word.at(index))
	{return 'G';}

	else
	{
		for(int i=0; i< word.length();i++)
		{
			if(guessWord.at(index)==word.at(i))
			{return 'Y';}
		}
		return 'B';
	}
}
void wordAdd2record (vector<vector<cell>> & record,const string & guessWord, const int & attemptMade, const string & word)
{ // adds the words to the matrix
	for(int i=0; i< record[0].size(); i++)
	{
		record[attemptMade][i].letter=guessWord.at(i);
		record[attemptMade][i].color=colorCheck4add2record(word, guessWord, i);
	}
}

void print(const vector<vector<cell>> & record, const int & attempts, const int & wordLen )
{ //prints the matrix record

	for(int i=0; i< attempts; i++)
	{
		cout<<"|";
		for(int x=0; x< wordLen; x++)
		{
			cout<<"| "<<record[i][x].letter<<" , "<<record[i][x].color<<" |";
		}
		cout<<"|"<<endl;
	}
}

bool win_lose(const int & attemptMade, const string & guessWord, const string & word, const int & attempts)
{ // returns true if the game is won or lost, otherwise false so that the game can continue
	if(guessWord==word)
	{
		cout<<"You win!"<<endl;
		return true;
	}
	else if( (attempts==attemptMade) && (guessWord!=word) ) 
	{
		cout<<"You lose!"<<endl;
		return true;
	}
	else {return false;}
}

void toLower(string & filename)
{
	for(int i=0; i<filename.length();i++)
	{
		if (filename.at(i)<='Z' && filename.at(i)>='A')
		{
			char ch= tolower(filename.at(i));
			filename.at(i)=ch;
		}
	}
}


int main()
{
	cout<< "Welcome to SWordle!" <<endl; 

	string filename;
	ifstream input;

	do // gives an error message if the filename is wrong and asks again until it's correct
	{
		cout<< "Please enter the file name: ";
		cin>>filename;
		toLower(filename);
		input.open(filename.c_str());
		if(input.fail())
		{cout<< "Couldn't find the file!"<<endl;}
	}while (input.fail());

	int attempts;
	string word;

	while (!input.eof()) // reads word from file
	{
		input>> attempts;
		input >> word;
	}

	int wordLen= word.length();
	cout<<"The word that you will guess has "<<wordLen<<" letters and you have "<<
		attempts<<" attempts."<<endl;

	input.close();

	// creates an empty matrix based on word length and number of attempts
	cell empty;
	empty.letter='-'; empty.color='-';
	vector<vector<cell>> record( attempts, vector<cell>(wordLen, empty));

	int attemptMade=0;
	string guessWord;

	do
	{
		do // continues to take the prediction until the appropriate input is entered
		{
			cout<<"What's your guess? ";
			cin>>guessWord;
		} while(!attemptCheck(guessWord, word, record,attemptMade));

		wordAdd2record (record,guessWord,attemptMade, word);
		print(record, attempts, wordLen );
		attemptMade++;

	}while(!win_lose(attemptMade, guessWord, word, attempts)  );

	return 0;
}
