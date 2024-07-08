#include <iostream>
#include <fstream>
#include <string>
#include "BST.cpp"
#include "Hash.cpp"
#include <vector>
#include <sstream>
#include <cctype>
#include <chrono>
#include <random>
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


void insertVec (vector<WordItem> & list, string Word, string filename){

	int wordCount = 0;
	int fileCount = 0;

	for(int i=0; i<list.size(); i++){
		if(list[i].word == Word){
			for(int k=0; k<list[i].file.size();k++){
				if(list[i].file[k]==filename){
					list[i].count[k] ++;
				}
				else{
					fileCount++;
				}
			}
			if(fileCount == list[i].file.size()){
				list[i].file.push_back(filename);
				list[i].count.push_back(1);
			}
		}
		else{
			wordCount++;
		}
	}

	if(wordCount == list.size()){
		WordItem temp;
		temp.word = Word;
		temp.file.push_back(filename);
		temp.count.push_back(1);
		list.push_back(temp);
	}
}

void insertionSort (vector <WordItem> & a)
{
	int j;
				// loop over the passes
	for (int p = 1;  p < a.size(); p++)
	{
		WordItem tmp = a[p];
							// loop over the elements
		for (j = p; j > 0 &&  tmp.word < a[j-1].word; j--)
			a[j] = a[j-1];
		a[j] = tmp;
	}
}

inline int leftChild( int i )
{
	return 2*i+1;
}


void percDown(vector<WordItem> & a, int i, int n)
{
	int child;
      WordItem tmp;

      for (tmp=a[i] ; leftChild(i) < n; i = child)
      {
            child = leftChild(i);
            if (child != n-1 && a[child].word < a[child+1].word)
                 child++;
                 if (a[child].word > tmp.word)
                      a[i] = a[child];
                 else
                      break;
       } 
       a[ i ] = tmp;
}


void heapsort(vector<WordItem> & a)
{
	// buildHeap
	for (int i = a.size()/2; i >=0; i--)
		percDown(a, i, a.size());

	// sort
	for (int j = a.size() -1; j >0; j--)
	{
  	     swap(a[0], a[j]);    // swap max to the last pos.
          percDown(a, 0, j); // re-form the heap
	}
}

void merge(vector<WordItem> & a, vector<WordItem> & tmpArray, int leftPos, int rightPos, int rightEnd)
{
        int leftEnd = rightPos - 1;
        int tmpPos = leftPos;
        int numElements = rightEnd - leftPos + 1;
    // Main loop
    while (leftPos <= leftEnd && rightPos <= rightEnd)
            if (a[leftPos].word <= a[rightPos].word)
                tmpArray[tmpPos++] = a[leftPos++];
            else
                tmpArray[tmpPos++] = a[rightPos++];

    while (leftPos <= leftEnd)    // Copy rest of first half
        tmpArray[tmpPos++] = a[leftPos++];

    while (rightPos <= rightEnd)  // Copy rest of right half
        tmpArray[tmpPos++] = a[rightPos++];

    // Copy tmpArray back
    for (int i = 0; i < numElements; i++, rightEnd--)
        a[rightEnd] = tmpArray[rightEnd];
}



void mergeSort(vector<WordItem> & a, vector<WordItem> & tmpArray, int left, int right )
{
        if ( left < right )
        {
            int center = ( left + right ) / 2;
            mergeSort( a, tmpArray, left, center );
            mergeSort( a, tmpArray, center + 1, right );
            merge( a, tmpArray, left, center + 1, right );
        }
}


void mergeSort(vector<WordItem> & a)
{
        vector<WordItem> tmpArray(a.size());

        mergeSort(a, tmpArray, 0, a.size() - 1);
}


inline void swap( WordItem & obj1, WordItem & obj2 )
{
		WordItem tmp = obj1;
        obj1 = obj2;
        obj2 = tmp;
}

const WordItem & median3( vector<WordItem> & a, int left, int right )
{
		int center = (left + right) / 2;
		if (a[center].word < a[left].word)
		swap(a[left], a[center]);
		if (a[right].word < a[left].word)
		swap(a[left], a[right]);
		if (a[right].word < a[center].word)
		swap(a[center], a[right]);

		// Place pivot at position right - 1
		swap(a[center], a[right - 1]);
		return a[right - 1];
}

void insertionSort(vector<WordItem> & a, int left, int right)
{
        for (int p = left + 1; p <= right; p++)
        {
            WordItem tmp = a[p];
            int j;

            for (j = p; j > left && tmp.word < a[j - 1].word; j--)
                a[j] = a[j - 1];
            a[j] = tmp;
    }
}


void quicksort(vector<WordItem> & a, int left, int right)
{
        if (left + 10 <= right)
        {
            WordItem pivot = median3(a, left, right);
			// Begin partitioning
        int i = left, j = right - 1;
        for (;;)
        {
            while (a[++i].word < pivot.word) {}

            while (pivot.word < a[--j].word) {}

            if (i < j)
                    swap(a[i], a[j]);
            else
                    break;
        }
				            swap(a[i], a[right - 1]);   // Restore pivot

            quicksort(a, left, i - 1);       // Sort small elements
            quicksort(a, i + 1, right);    // Sort large elements
    }
    else  // Do an insertion sort on the subarray
            insertionSort(a, left, right);
}


void quicksort(vector<WordItem> & a)
 {
       quicksort(a, 0, a.size() - 1);
 }

void quicksortFirst(vector<WordItem> & a, int left, int right)
{
        if (left + 10 <= right)
        {
            WordItem pivot = a[left];
			// Begin partitioning
        int i = left, j = right - 1;
        for (;;)
        {
            while (a[++i].word < pivot.word) {}

            while (pivot.word < a[--j].word) {}

            if (i < j)
                    swap(a[i], a[j]);
            else
                    break;
        }
				            swap(a[i], a[right - 1]);   // Restore pivot

            quicksort(a, left, i - 1);       // Sort small elements
            quicksort(a, i + 1, right);    // Sort large elements
    }
    else  // Do an insertion sort on the subarray
            insertionSort(a, left, right);
}


void quicksortFirst(vector<WordItem> & a)
 {
       quicksortFirst(a, 0, a.size() - 1);
 }


void quicksortRandom(vector<WordItem> & a, int left, int right)
{
        if (left + 10 <= right)
        {

			random_device rand;
			mt19937 gen(rand());
			uniform_int_distribution<> dis(left, right);
			int randomPivot = dis(gen);

			swap(a[randomPivot],a[left]);

            WordItem pivot = a[left];
			// Begin partitioning
        int i = left, j = right - 1;
        for (;;)
        {
            while (a[++i].word < pivot.word) {}

            while (pivot.word < a[--j].word) {}

            if (i < j)
                    swap(a[i], a[j]);
            else
                    break;
        }
				            swap(a[i], a[right - 1]);   // Restore pivot

            quicksort(a, left, i - 1);       // Sort small elements
            quicksort(a, i + 1, right);    // Sort large elements
    }
    else  // Do an insertion sort on the subarray
            insertionSort(a, left, right);
}


void quicksortRandom(vector<WordItem> & a)
 {
       quicksortFirst(a, 0, a.size() - 1);
 }



// Recursive binary search function to find a word in a sorted vector of strings
int binarySearch(const vector<WordItem>& list, const string& target, int left, int right) {
    if (left > right) {
        return -1; // Base case: Element not found
    }

    int mid = left + (right - left) / 2;

    if (list[mid].word == target) {
        return mid; // Element found at index mid
	} else if (list[mid].word > target) {
        return binarySearch(list, target, left, mid - 1); // Search the left half
    } else{
        return binarySearch(list, target, mid + 1, right); // Search the right half
    }
}



int main(){
	

	//
	// File Process Part
	//

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
	vector<WordItem> list;

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
								insertVec(list,word,files[i]);
								word = "";
							}
						}
					}
					if(word != "")
					{			
						//inserting into tree and table
						insertData(word, tree, map, files[i]);
						insertVec(list,word,files[i]);
						word = "";
					}
				}
			}
		}
	}

	//giving final results
	cout<<endl;
	cout<<"After preprocessing, the unique word count is "<< map.sizeofTable() << " Current load ratio is "<< map.loadofTable() << endl;

	//
	//   Query Process Part
	//


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



	//
	//   Sorting and time
	//
	

	vector<WordItem> inserVec = list;
	vector<WordItem> heapVec = list;
	vector<WordItem> mergeVec = list;
	vector<WordItem> medianVec = list;
	vector<WordItem> firstVec = list;
	vector<WordItem> randomVec = list;


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
		WordItem first = map.find(words[0]);
		int count;
		for(unsigned int i=0; i<first.file.size();i++){			//for all files of first word given 
			count =0;									//looking files of other words 
			for(unsigned int k=0;k<words.size();k++){			//if all of them contains that file 
				WordItem temp = map.find(words[k]);	//adding it to a vector
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
	start = std::chrono::high_resolution_clock::now();
		quicksort(medianVec);
	auto QuickTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
	start = std::chrono::high_resolution_clock::now();
		quicksortFirst(firstVec);
	auto firstTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
	start = std::chrono::high_resolution_clock::now();
		quicksortRandom(randomVec);
	auto randomTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
	start = std::chrono::high_resolution_clock::now();
		mergeSort(mergeVec);
	auto MergeTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
	start = std::chrono::high_resolution_clock::now();
		heapsort(heapVec);
	auto HeapTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
	start = std::chrono::high_resolution_clock::now();
		insertionSort(inserVec);
	auto InsertionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
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
		WordItem first;
		int index = binarySearch(mergeVec, words[0], 0, mergeVec.size());
		if(index != -1){
			first = mergeVec[index];
			int count;
			for(unsigned int i=0; i<first.file.size();i++){			//for all files of first word given 
				count =0;									//looking files of other words 
				for(unsigned int k=0;k<words.size();k++){			//if all of them contains that file 
					index = binarySearch(mergeVec, words[k], 0, mergeVec.size());
					if(index != -1){
						WordItem temp = mergeVec[index];
						for(unsigned int j=0;j<temp.file.size();j++){
							string tempFile =temp.file[j];
							string firstFile = first.file[i];
							if(tempFile == firstFile){
								count++;
							}
						}
					}
				}
				if(count == words.size()){
					filesContainQuery.push_back(first.file[i]);
				}
			}
		}

		//vector output part
		if(filesContainQuery.size()!=0){ //there is at least one file contains all words
			for(unsigned int k = 0; k<filesContainQuery.size(); k++){
				for(unsigned int i =0; i<words.size();i++){
				
					WordItem result;
					int index = binarySearch(mergeVec, words[i], 0, mergeVec.size());
					if(index != -1){
						result = mergeVec[index];
					

						for(unsigned int j = 0; j<result.file.size(); j++){		
						}
					}
				}
			}	
		}
		else{ //there is no file which contains all words
		}


		
	}
	auto BSTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);

	
	
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

	//vector output part
	if(filesContainQuery.size()!=0){ //there is at least one file contains all words
		for(unsigned int k = 0; k<filesContainQuery.size(); k++){
			for(unsigned int i =0; i<words.size();i++){
				
				WordItem result;
				int index = binarySearch(mergeVec, words[i], 0, mergeVec.size()-1);
				result = mergeVec[index];

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

	
	cout<<endl;


	cout << "Binary Search Tree Time: " << BSTTime.count()/100<<endl;
	cout << "Hash Tabe Time: "<< HTTime.count()/100<<endl;
	cout << "Binary Search Time: "<<BSTime.count()/100<<endl;
	cout<<endl;
	cout<< "Quick Sort(Median) Time: "<<QuickTime.count()<<endl;
	cout<< "Quick Sort(Random) Time: "<<randomTime.count()<<endl;
	cout<< "Quick Sort(First) Time: "<<firstTime.count()<<endl;
	cout<< "Merge Sort Time: "<<MergeTime.count()<<endl;
	cout<< "Heap Sort Time: "<<HeapTime.count()<<endl;
	cout<< "Insertion Sort Time: "<< InsertionTime.count()<<endl;
	cout<<endl;


	float treeTime = BSTTime.count() / 100;
	float hashTime = HTTime.count() / 100;
	float binaryTime = BSTime.count() / 100;
	float medTime = QuickTime.count();
	float merTime = MergeTime.count();
	float heTime = HeapTime.count();
	float insTime = InsertionTime.count();

	float speedUp = treeTime/ hashTime;
	cout<< "Speed Up BST/HST: "<<speedUp<<endl;
	speedUp = merTime/medTime;
	cout<< "Speed Up Merge Sort/Quick Sort(Median): "<< speedUp<<endl;
	speedUp = heTime/medTime;
	cout<< "Speed Up Heap Sort/Quick Sort(Median): "<< speedUp<<endl;
	speedUp = insTime/medTime;
	cout<< "Speed Up Insertion Sort/Quick Sort(Median): "<< speedUp<<endl;
	cout<<endl;

	speedUp = treeTime/binaryTime;
	cout<< "Speed Up Binary Search / Binary Search Tree Time: "<<speedUp<<endl;
	speedUp = hashTime/binaryTime;
	cout<< "Speed Up Binary Search / Hash Table Time: "<<speedUp<<endl;

	

	return 0;
}