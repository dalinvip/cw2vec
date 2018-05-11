/**
* @Author: bamtercelboo
* @Date: 2018/03/15
* @File: main.cpp
* @Contact: bamtercelboo@{gmail.com, 163.com}
* @Function: None
*/
 
#include<iostream>
#include<vector>
#include<string>

#include "args.h"
#include "fasttext.h"


void printUsage() {
	std::cerr
		<< "usage: word2vec <command> <args>\n\n"
		<< "The commands supported by word2vec are:\n\n"
		<< "  skipgram  ------ train word embedding by use skipgram model\n"
		<< "  cbow  ------ train word embedding by use cbow model\n"
		<< "  subword   ------ train word embedding by use subword(fastetxt skipgram)  model\n"
		<< "  substoke   ------ train chinses character embedding by use substoke(cw2vec) model\n"
		<< std::endl;
}
 
void train(const std::vector<std::string> args) {
	std::cout << "Train Embedding By Using [" + args[1] + "] model" << std::endl;
	Args a = Args();
	a.parseArgs(args);
	FastText fasttext;
	fasttext.train(a);
	fasttext.saveVectors();
	std::cout << "Train Embedding By Using [" + args[1] + "] model have Finished" << std::endl;
}

int main(int argc, char** argv){
	std::vector<std::string> args(argv, argv + argc);
	if (args.size() < 2) {
		printUsage();
		std::getchar();
		exit(EXIT_FAILURE);
	}
	std::string command(args[1]);
	if (command != "skipgram" && command != "cbow" && command != "subword" && command != "substoke") {
		std::cerr << "\nError command: " + command << std::endl;
		printUsage();
		std::getchar();
		exit(EXIT_FAILURE);
	}
	// train start
	train(args);
	std::getchar();
	return 0;
}