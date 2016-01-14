
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>

using namespace std;

//functions used for separating input paramethers
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

//Class Node represents element of deep bounded tree that is built to reconstruct constrained supersequence 
class Node
{
public:int i, j, len; // i -current position in sequence 1; j - current position in sequence2; len - length of current solution
public:string middleSolution; // part of solution that is stored in current node
public:map<char, int> T_L; // Constraint dictionray that contains current minimum number of occurances for each existing sequence mark
public:Node* parent; // Pointer to parent node
// Simple object constructor
public: Node(int i, int j, int len, string middleSolution, map<char, int> T_L, Node* parent)
{
	this->i = i;
	this->j = j;
	this->len = len;
	this->middleSolution = middleSolution;
	this->T_L = T_L;
	this->parent = parent;
}
};

void k_C_SCS(string s1, string s2, map<char, int> T_L, int k, string path)
{	
	vector<Node*> tree; // main tree of partial solutions
	std::ofstream myfile; // output file
	myfile.open(path.c_str());

	int  j, len;
	map<char, int>  T_LInParent, T_LInNode; // T_LInParent - key-value pairs of constraints in parent node;  
											// T_LInNode - key-value pairs of constraints in current node
	string middleSolution;
	Node *parent, *finalNode;
	//tree.push_back(new Node(s1.length() - 1, s2.length() - 1, 0, "", T_L, NULL)); WinRootNode
	tree.push_back(new Node(s1.length() - 2, s2.length() - 2, 0, "", T_L, NULL)); //root Node 
	while (!tree.empty())
	{
		// Reading and removing last element of a Node vector 
		int i = tree.back()->i;
		j = tree.back()->j;
		len = tree.back()->len;
		T_LInParent = tree.back()->T_L;
		parent = tree.back();
		tree.pop_back();

		// Terminating conditions
		if (k >= len)
		{
			if (i < 0 || j < 0)
			{
				T_LInNode = T_LInParent;
				middleSolution = "";
				if (i < 0)
				{
					for (int y = j; y >= 0; y--)
					{  
						//concatenating remaining characters of second sequence into middleSolution
						middleSolution += s2[y];
						T_LInNode[s2[y]]--;
					}
					j = -1;
				}
				if (j < 0)
				{
					//concatenating remaining characters of first sequence into middleSolution
					for (int x = i; x >= 0; x--)
					{
						middleSolution += s1[x];
						T_LInNode[s1[x]]--;
					}
					i = -1;
				}

				//checking if current solution is feasible
				if (len + middleSolution.length() <= k)
				{
					bool feasible = true;
					for (map<char, int>::iterator it = T_LInNode.begin(); it != T_LInNode.end(); ++it)
					{
						// If any of constraints is not satisfied, solution is not feasible
						if (T_LInNode[it->first] > 0)
						{
							feasible = false; break;
						}
					}
					if (feasible)
					{
						// Adding final tree node and finishing with search
						finalNode = new Node(-1, -1, -1 , middleSolution, T_LInNode, parent);
						break;
					}
				}
				continue;
			}

			// Matching i-th position of first sequence with j-th position of second sequence
			if (s1[i] == s2[j])
			{
				// if positions are matched, character is added to current solution and stored at the and of the vector
				middleSolution = s1[i];

				T_LInNode = T_LInParent;
				for (int num = 0; num < middleSolution.length(); num++)
				{
					T_LInNode[middleSolution[num]]--;
				}
				//Continue branching with s1[1,i-1] and s2 [1,j-1]
				tree.push_back(new Node(i - 1, j - 1, len + middleSolution.length(), middleSolution, T_LInNode, parent));
			}
			// Branching
			else
			{
				// Case 1: s2[1,j] contains s1[i] 
				int index = -1;
				for (int num = j; num >= 0; num--)
				{
					if (s2[num] == s1[i])
					{
						index = num;
						break;
					}
				}
				if (index != -1)
				{
					middleSolution = "";
					// Adding s2[index+1,j] to current solution; index - index of matched character 
					for (int num = index  ; num <= j ; num++)
					{
						middleSolution += s2[num];
					}
					T_LInNode = T_LInParent;
					for (int num = 0; num < middleSolution.length(); num++)
					{
						T_LInNode[middleSolution[num]]--;
					}
					//Continue branching with s1[1,i-1] and s2 [1,index-1]
					tree.push_back(new Node(i - 1, index - 1, len + middleSolution.length(), middleSolution, T_LInNode, parent));
				}

				// Case 2: s1[1,i] contains s[j]
				index = -1;
				for (int num = i; num >= 0; num--)
				{
					if (s1[num] == s2[j])
					{
						index = num;
						break;
					}
				}
				if (index != -1) {
					middleSolution = "";
					// Adding s1[index+1,i] to current solution; index - index of matched character 
					for (int num = index ; num <= i; num++)
					{
						middleSolution += s1[num];
					}
					T_LInNode = T_LInParent;
					for (int num = 0; num < middleSolution.length(); num++)
					{
						T_LInNode[middleSolution[num]]--;
					}
					//Continue branching with s1[1,index-1] and s2 [1,j-1]
					tree.push_back(new Node(index - 1, j - 1, len + middleSolution.length(), middleSolution, T_LInNode, parent));
				}

				// Positions i and j do not match
				middleSolution = s2[j];
				middleSolution += s1[i];
				T_LInNode = T_LInParent;

				for (int num = 0; num < middleSolution.length(); num++)
				{
					T_LInNode[middleSolution[num]]--;
				}
				//Continue branching with s1[1,i-1] and s2 [1,j-1]
				tree.push_back(new Node(i - 1, j - 1, len + middleSolution.length(), middleSolution, T_LInNode, parent));


			}

		}

	}
	// Printing final solution to file
	while (finalNode)
	{
		myfile << finalNode->middleSolution;
		finalNode = finalNode->parent;
	}
	myfile.close();

}

int main(int argc, char *argv[]){
	if (argc < 5){
		std::cerr << "Usage: " << argv[0] << " [infile]" << std::endl;
		return -1;
	}
	std::ifstream input1(argv[1]); //subsequence 1
	std::ifstream input2(argv[2]); //subsequence 2
	std::ifstream input3(argv[3]); //constraints - T
	int k = (int)strtol(argv[5], (char **)NULL, 10); //k - max supersequence length

	// parameter validation
	if (!input1.good()){
		std::cerr << "Error opening '" << argv[1] << "'. Bailing out." << std::endl;
		return -1;
	}
	if (!input2.good()){
		std::cerr << "Error opening '" << argv[2] << "'. Bailing out." << std::endl;
		return -1;
	}
	if (!input3.good()){
		std::cerr << "Error opening '" << argv[3] << "'. Bailing out." << std::endl;
		return -1;
	}
	stringstream buffer1;
	buffer1 << input1.rdbuf();
	stringstream buffer2;
	buffer2 << input2.rdbuf();
	stringstream buffer3;
	buffer3 << input3.rdbuf();
	string seq1, seq2, constraints;
	seq1 = buffer1.str();
	seq2 = buffer2.str();
	constraints = buffer3.str();

	// checking basic constraint on parameter k..
	if (k < max(seq1.length(), seq2.length()))
	{
		std::cerr << "Error. Parameter k must be greater than max(seq1.length, seq2.length) " << std::endl;
		return -1;
	}

	// Spliting and maping constraint vector
	map<char, int> T_L;
	vector<string> constraintVector = split(constraints, ',');
	for (vector<string>::size_type i = 0; i != constraintVector.size(); i++)
	{
		if (i % 2 != 0)
		{
			T_L[constraintVector[i - 1].c_str()[0]] = atoi(constraintVector[i].c_str());
		}
	}

	// Runing algorithm
	k_C_SCS(seq1, seq2, T_L, k, argv[4]);

	return 0;
}
