#include <iostream>
#include <fstream>
#include <string>
#include "BST.cpp"
#include "Hash.cpp"
#include <vector>
#include <sstream>
#include <cctype>
#include <chrono>
using namespace std;


struct WordItem {  //a struct that will store all informations about the word 
	string word; // the word
	vector<string> file;  //a vector to store all file names that have the word
	vector<int> count; //a vector to store number of occurens of the word in each file
	WordItem()              // default constructor
    : word(""), file(), count() {}  // Initialize word to an empty string, file and count to empty vectors
	WordItem(string w){ //parameterized constracter
		word = w;
	}
};



string makeLower (string word){ //a  function that will lower all letters in a word
	for(unsigned int k = 0; k<word.size();k++)
	if(word[k]>='A' && word[k]<='Z'){
		word[k] = word[k] - ('A' - 'a');
	}
	return word;
}

void insertData (string word, BSTree<string,WordItem> & tree,  HashTable<string,WordItem> & map, string fileName){
	WordItem tempItem; //intializing a word item

	WordItem item = tree.find(word); //looking for the word in tree

	if(item.word =="-1"){ //item is not found in tree that means also not in table
		tempItem.word = word;
		tempItem.file.push_back(fileName);
		tempItem.count.push_back(1);
		tree.insert(word, tempItem);
		map.insert(word, tempItem);
	}
	else{ //item exist in tree and table
		bool stop = false;
		for(unsigned int i=0; i<item.file.size(); i++){
			if(item.file[i] == fileName){ //increasing count
				item.count[i]++;
				tree.remove(word);
				map.remove(word);
				tree.insert(item.word, item);
				map.insert(item.word, item);
				stop = true;
				break;
			}
		}
		if(stop == false){//adding new file name
			item.file.push_back(fileName);
			item.count.push_back(1);
			tree.remove(word);
			map.remove(word);
			tree.insert(word, item);
			map.insert(word, item);
		}
	}
	word = "";
}
int main(){
	
	//getting number of files as input
	int numberOfFiles; 
	cout<<"Enter number of input files: ";
	cin>>numberOfFiles;
	cout<<endl;


	// creating AVL Tree
	WordItem notFound;
	notFound.word = "-1";
	BSTree<string,WordItem> tree(notFound);
	HashTable<string,WordItem> map(notFound);

	//the loop that will get names of files and process them
	vector<string> files;
	for(int i=1;i<=numberOfFiles;i++){

		//getting name of file and opening them
		string fileName;
		cout<<"Enter "<< i <<". file name: ";
		cin>> fileName;
		files.push_back(fileName);
	}
	for(int i=0;i<files.size();i++){
		//processing the files
		ifstream file;
		file.open(files[i]);
		if(!file.fail()){
			string line; //reading files line by line
			while(getline(file,line)){
				line = makeLower(line);
				stringstream ss(line);
				
				string w;
				//reading line word by word
				while(ss >> w){
					string word = "";
					//readinng words character by character
					for(char &c : w){
						if(c >= 'a' && c <= 'z'){
							word += c;
						}
						else{
							if(word != "")
							{		
								//inserting into tree and table
								insertData(word, tree, map, files[i]);
								word = "";
							}
						}
					}
					if(word != "")
					{			
						//inserting into tree and table
						insertData(word, tree, map, files[i]);
						word = "";
					}
				}
			}
		}
	}

	//giving final results
	cout<<endl;
	cout<<"After preprocessing, the unique word count is "<< map.sizeofTable() << " Current load ratio is "<< map.loadofTable() << endl;
	
	//clear cin	
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clearing the cin
		
	string line;
	cout<<endl;//getting input
	cout<<"Enter queried words in one line: ";
	getline(cin, line);

	vector<string> words; //a vector to store words
	istringstream iss(line);

	string word; //reading line word by word
	while(iss >> word){
		string sepWord = "";
		word = makeLower(word);
		//reading word character by character
		for(char &c : word){
			if(c >= 'a' && c <= 'z'){
				sepWord += c;
			}
			else{
				if(sepWord != "")
				{			
					//recording the word
					words.push_back(sepWord);
					sepWord = "";
				}
			}
		}
		if(sepWord != "")
		{						
			//recording the word
			words.push_back(sepWord);
			sepWord = "";
		}
	}



	vector<string> filesContainQuery;

	//finding files which contains all the words given
	WordItem first = tree.find(words[0]);
	int count;
	for(unsigned int i=0; i<first.file.size();i++){			//for all files of first word given 
		count =0;									//looking files of other words 
		for(unsigned int k=0;k<words.size();k++){			//if all of them contains that file 
			WordItem temp = tree.find(words[k]);	//adding it to a vector
			for(unsigned int j=0;j<temp.file.size();j++){
				string tempFile =temp.file[j];
				string firstFile = first.file[i];
				if(tempFile == firstFile){
					count++;
				}
			}
		}
		if(count == words.size()){
			filesContainQuery.push_back(first.file[i]);
		}
	}

	//tree output part
	if(filesContainQuery.size()!=0){ //there is at least one file contains all words
		for(unsigned int k = 0; k<filesContainQuery.size(); k++){
			for(unsigned int i =0; i<words.size();i++){
				WordItem result = tree.find(words[i]);
				for(unsigned int j = 0; j<result.file.size(); j++){
					if(result.file[j]==filesContainQuery[k]){
						if(i == 0){
							cout<<"in Document "<<filesContainQuery[k];
						}
						cout<<", "<<words[i]<<" found "<<result.count[j]<<" times";
					}
				
				}
			}
			cout<<"."<<endl;
		}	
	}
	else{ //there is no file which contains all words
		cout<<"No document contains the given query"<<endl;
	}

	//table output part
	if(filesContainQuery.size()!=0){ //there is at least one file contains all words
		for(unsigned int k = 0; k<filesContainQuery.size(); k++){
			for(unsigned int i =0; i<words.size();i++){
				WordItem result = map.find(words[i]);
				for(unsigned int j = 0; j<result.file.size(); j++){
					if(result.file[j]==filesContainQuery[k]){
						if(i == 0){
							cout<<"in Document "<<filesContainQuery[k];
						}
						cout<<", "<<words[i]<<" found "<<result.count[j]<<" times";
					}
				
				}
			}
			cout<<"."<<endl;
		}	
	}
	else{ //there is no file which contains all words
		cout<<"No document contains the given query"<<endl;
	}

	//measuring search time
	int k = 100;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++)
	{
		string word; //reading line word by word
		while(iss >> word){
			string sepWord = "";
			word = makeLower(word);
			//reading word character by character
			for(char &c : word){
				if(c >= 'a' && c <= 'z'){
					sepWord += c;
				}
				else{
					if(sepWord != "")
					{			
						//recording the word
						words.push_back(sepWord);
						sepWord = "";
					}
				}
			}
			if(sepWord != "")
			{						
				//recording the word
				words.push_back(sepWord);
				sepWord = "";
			}
		}

		vector<string> filesContainQuery;

		//finding files which contains all the words given
		WordItem first = tree.find(words[0]);
		int count;
		for(unsigned int i=0; i<first.file.size();i++){			//for all files of first word given 
			count =0;									//looking files of other words 
			for(unsigned int k=0;k<words.size();k++){			//if all of them contains that file 
				WordItem temp = tree.find(words[k]);	//adding it to a vector
				for(unsigned int j=0;j<temp.file.size();j++){
					string tempFile =temp.file[j];
					string firstFile = first.file[i];
					if(tempFile == firstFile){
						count++;
					}
				}
			}
			if(count == words.size()){
				filesContainQuery.push_back(first.file[i]);
			}
		}
		//tree output part
		if(filesContainQuery.size()!=0){ //there is at least one file contains all words
			for(unsigned int k = 0; k<filesContainQuery.size(); k++){
				for(unsigned int i =0; i<words.size();i++){
					WordItem result = tree.find(words[i]);
					for(unsigned int j = 0; j<result.file.size(); j++){
					}
				}
			}	
		}
		else{ //there is no file which contains all words
			;
		}

	}
	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
	cout << "\nTime: " << BSTTime.count() / k << "\n";
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++)
	{
		string word; //reading line word by word
		while(iss >> word){
			string sepWord = "";
			word = makeLower(word);
			//reading word character by character
			for(char &c : word){
				if(c >= 'a' && c <= 'z'){
					sepWord += c;
				}
				else{
					if(sepWord != "")
					{			
						//recording the word
						words.push_back(sepWord);
						sepWord = "";
					}
				}
			}
			if(sepWord != "")
			{						
				//recording the word
				words.push_back(sepWord);
				sepWord = "";
			}
		}
		vector<string> filesContainQuery;

		//finding files which contains all the words given
		WordItem first = tree.find(words[0]);
		int count;
		for(unsigned int i=0; i<first.file.size();i++){			//for all files of first word given 
			count =0;									//looking files of other words 
			for(unsigned int k=0;k<words.size();k++){			//if all of them contains that file 
				WordItem temp = tree.find(words[k]);	//adding it to a vector
				for(unsigned int j=0;j<temp.file.size();j++){
					string tempFile =temp.file[j];
					string firstFile = first.file[i];
					if(tempFile == firstFile){
						count++;
					}
				}
			}
			if(count == words.size()){
				filesContainQuery.push_back(first.file[i]);
			}
		}
			//table output part
		if(filesContainQuery.size()!=0){ //there is at least one file contains all words
			for(unsigned int k = 0; k<filesContainQuery.size(); k++){
				for(unsigned int i =0; i<words.size();i++){
					WordItem result = map.find(words[i]);
					for(unsigned int j = 0; j<result.file.size(); j++){		
					}
				}
			}	
		}
	}
	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
	cout << "\nTime: " << HTTime.count() / k << "\n";


	float treeTime = BSTTime.count() / k;
	float hashTime = HTTime.count() / k;

	float speedUp = treeTime/ hashTime;
	cout<< "Spedd up: " << speedUp <<endl;
	

	return 0;
}