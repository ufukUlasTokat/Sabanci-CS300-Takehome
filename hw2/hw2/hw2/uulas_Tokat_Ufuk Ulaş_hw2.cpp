#include <iostream>
#include <fstream>
#include <string>
#include "AVL.cpp"
#include <vector>
#include <sstream>
using namespace std;


struct WordItem {  //a struct that will store all informations about the word 
	string word; // the word
	vector<string> file;  //a vector to store all file names that have the word
	vector<int> count; //a vector to store number of occurens of the word in each file
	WordItem(){ //default constructer
		word = "";
	}
	WordItem(string w){ //parameterized constracter
		word = w;
	}
};


string makeClean(string word){ //a function deletes empty spaces in the word
	while(word.substr(0,1)==" " || word.substr(0,1)==" "){
		word = word.substr(1);
	}
	return word;
}

string makeLower (string word){ //a  function that will lower all letters in a word
	for(unsigned int k = 0; k<word.size();k++)
	if(word[k]>='A' && word[k]<='Z'){
		word[k] = word[k] - ('A' - 'a');
	}
	return word;
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
	AvlTree<string,WordItem> tree(notFound);

	//the loop that will get names of files and process them
	vector<string> files;
	for(int i=1;i<=numberOfFiles;i++){

		//getting name of file and opening them
		string fileName;
		ifstream file;
		cout<<"Enter "<< i <<". file name: ";
		cin>> fileName;
		files.push_back(fileName);
		file.open(fileName);

		//processing the files
		string line;
		if(!file.fail()){
			while(getline(file,line)){
				line = makeLower(line);
				string word;
				for(unsigned int i = 0; i<line.length();i++){ //a loop to get words one by one
					if(!(line.substr(i,1) >= "a" && line.substr(i,1) <="z")){
						word = line.substr(0,i);
						line = line.substr(i+1);
						i=0;
						
						WordItem tempItem; //intializing a word item
						word = makeClean(word);

						WordItem item = tree.find(word); //looking for the word in tree
						if(item.word =="-1"){ //item is not found in tree
							tempItem.word = word;
							tempItem.file.push_back(fileName);
							tempItem.count.push_back(1);
							tree.insert(word, tempItem);
						}
						else{ //item exist in tree
							bool stop = false;
							for(unsigned int i=0; i<item.file.size(); i++){
								if(item.file[i] == fileName){ //increasing count
									item.count[i]++;
									tree.remove(word);
									tree.insert(item.word, item);
									stop = true;
									break;
								}
							}
							if(stop == false){//adding new file name
								item.file.push_back(fileName);
								item.count.push_back(1);
								tree.remove(word);
								tree.insert(word, item);
							}
						}
					}
				}
			}
		}
	}

	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clearing the cin
	bool end = false;

	while(end == false){ // a loop until input says to stop
		
		vector<string> words; //a vector to store words
		string querys;
		vector<string> filesContainQuery;

		//getting input
		cout<<endl;
		cout<<"Enter queried words in one line: ";
		getline(std::cin, querys);
		stringstream ss(querys);
		string newWord;
		string query;

		//seperating the input into words
		bool remove = false;
		bool notfound = false;
		while(ss >> query){
			newWord = makeLower(query);
			if(tree.find(newWord).word != "-1"){//word is exist in tree 
				words.push_back(newWord);
			}
			else{//word is not found
				notfound = true;
			}
			if(query=="REMOVE"){//words will be removed
				remove = true;
			}
		}



		if(querys == "ENDOFINPUT"){//code will stop
			return 0;
		}
		

		else if(remove == true){//removing the words
			for(unsigned int i=0;i<words.size();i++){
				if(tree.find(words[i]).word!="-1"){
					tree.remove(words[i]);
					cout<<words[i] << " has been REMOVED"<<endl;
				}
			}
		}


		else if(notfound == true){//a word is not found in tree 
			cout<<"No document contains the given query"<<endl;
		}
		
		

		if(remove == false && notfound == false){//no word will be removed and all words are exist in tree

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
	

			//output part
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
		}
	}

	return 0;
}