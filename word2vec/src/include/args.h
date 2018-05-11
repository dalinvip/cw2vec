/**
* @Author: bamtercelboo
* @Date： 2018/03/15
* @File: main.cpp
* @Contact： bamtercelboo@{gmail.com, 163.com}
* @Function：None
*/

#pragma once

#include<iostream>
#include<vector>
#include<string>


enum class model_name : int { skipgram = 1, cbow, subword, substoke};
enum class loss_name : int {ns = 1};

class Args {
	protected:
		std::string lossToString(loss_name) const;
		std::string boolToString(bool) const;
		std::string modelToString(model_name) const;

	public:
		Args();
		std::string input;
		std::string infeature;
		std::string output;
		double lr;
		int lrUpdateRate;
		int dim;
		int ws; 
		int epoch;
		int minCount;
		int minCountLabel; 
		int neg;
		loss_name loss;
		model_name model;
		int bucket;
		int minn;
		int maxn;
		int thread;
		double t; 
		std::string label;
		int verbose;
		std::string pretrainedVectors;
		std::string featurepad;
		bool saveOutput;

		size_t cutoff;
		void parseArgs(const std::vector<std::string>& args);
		void printHelp();
		void printBasicHelp();
		void printDictionaryHelp();
		void printTrainingHelp();
		void save(std::ostream&);
		void load(std::istream&);
};

/**
 * @Function: initial Args class argument.
*/
Args::Args() {
	lr = 0.05;
	dim = 100;
	ws = 5;
	epoch = 5;
	minCount = 10;
	neg = 5;
	loss = loss_name::ns;
	model = model_name::skipgram;
	bucket = 2000000;
	minn = 3;
	maxn = 6;
	thread = 1;
	lrUpdateRate = 100;
	t = 1e-3;
	label = "__label__";
	verbose = 2;
	pretrainedVectors = "";
	featurepad = 'N';
	saveOutput = false;
}

/**
 * @Function: parseArgs.
*/
void Args::parseArgs(const std::vector<std::string>& args) {
	std::string command(args[1]);
	if (command == "cbow") {
		model = model_name::cbow;
	} else if (command == "subword") {
		model = model_name::subword;
	} else if ( command == "substoke") {
		model = model_name::substoke;
	}
	for (int ai = 2; ai < args.size(); ai += 2) {
		if (args[ai][0] != '-') {
			std::cerr << "Provided argument without a dash! Usage:" << std::endl;
			printHelp();
			std::getchar();
			exit(EXIT_FAILURE);
		}
		try {
			if (args[ai] == "-h" || args[ai] == "--help") {
				std::cerr << "Here is the help infprmation! Usage:" << std::endl;
				printHelp();
				//std::getchar();
				exit(EXIT_FAILURE);
			} else if (args[ai] == "-input") {
				input = std::string(args.at(ai + 1));
			} else if (args[ai] == "-infeature") {
				infeature = std::string(args.at(ai + 1));
			} else if (args[ai] == "-output") {
				output = std::string(args.at(ai + 1));
			} else if (args[ai] == "-lr") {
				lr = std::stof(args.at(ai + 1));
			} else if (args[ai] == "-lrUpdateRate") {
				lrUpdateRate = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-dim") {
				dim = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-ws") {
				ws = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-epoch") {
				epoch = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-minCount") {
				minCount = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-minCountLabel") {
				minCountLabel = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-neg") {
				neg = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-loss") {
				if (args.at(ai + 1) == "ns") {
					loss = loss_name::ns;
				} else {
					std::cerr << "Unknown loss: " << args.at(ai + 1) << std::endl;
					printHelp();
					exit(EXIT_FAILURE);
				}
			} else if (args[ai] == "-bucket") {
				bucket = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-minn") {
				minn = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-maxn") {
				maxn = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-thread") {
				thread = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-t") {
				t = std::stof(args.at(ai + 1));
			} else if (args[ai] == "-label") {
				label = std::string(args.at(ai + 1));
			} else if (args[ai] == "-verbose") {
				verbose = std::stoi(args.at(ai + 1));
			} else if (args[ai] == "-pretrainedVectors") {
				pretrainedVectors = std::string(args.at(ai + 1));
			} else if (args[ai] == "-saveOutput") {
				saveOutput = true;
				ai--;
			} else if (args[ai] == "-cutoff") {
				cutoff = std::stoi(args.at(ai + 1));
			} else {
				std::cerr << "Unknown argument: " << args[ai] << std::endl;
				printHelp();
				exit(EXIT_FAILURE);
			}
		}
		catch (std::out_of_range){
			std::cerr << args[ai] << "is missing an argument" << std::endl;
			printHelp();
			exit(EXIT_FAILURE);
		}
	}
	
	if (model == model_name::substoke && infeature == "") {
		std::cerr << "substoke need infeature file, [-infeature] is empty." << std::endl;
		std::getchar();
		printHelp();
		exit(EXIT_FAILURE);
	}
}

/**
* @Function: print Help information.
*/
void Args::printHelp() {
	printBasicHelp();
	printDictionaryHelp();
	printTrainingHelp();
}

/**
* @Function: print Help information.
*/
void Args::printBasicHelp() {
	std::cerr
		<< "\n The Following arguments are mandatory:\n"
		<< "  -input						     training file path\n"
		<< "  -infeature				 substoke feature file path\n"
		<< "  -output							   output file path\n"
		<< "\n The Following arguments are optional:\n"
		<< "  -verbose   verbosity level[" << verbose << "]\n"
		<< std::endl;
}

/**
* @Function: print Help information.
*/
void Args::printDictionaryHelp() {
	std::cerr
		<< "\nThe following arguments for the dictionary are optional:\n"
		<< "  -minCount           minimal number of word occurences default:[" << minCount << "]\n"
		<< "  -bucket             number of buckets default:[" << bucket << "]\n"
		<< "  -minn               min length of char ngram default:[" << minn << "]\n"
		<< "  -maxn               max length of char ngram default:[" << maxn << "]\n"
		<< "  -t                  sampling threshold default:[" << t << "]\n";
}

/**
* @Function: print Help information.
*/
void Args::printTrainingHelp() {
	std::cerr
		<< "\nThe following arguments for training are optional:\n"
		<< "  -lr                 learning rate default:[" << lr << "]\n"
		<< "  -lrUpdateRate       change the rate of updates for the learning rate default:[" << lrUpdateRate << "]\n"
		<< "  -dim                size of word vectors default:[" << dim << "]\n"
		<< "  -ws                 size of the context window default:[" << ws << "]\n"
		<< "  -epoch              number of epochs default:[" << epoch << "]\n"
		<< "  -neg                number of negatives sampled default:[" << neg << "]\n"
		<< "  -loss               loss function {ns} default:[" << lossToString(loss) << "]\n"
		<< "  -thread             number of threads default:[" << thread << "]\n"
		<< "  -pretrainedVectors  pretrained word vectors for supervised learning default:[" << pretrainedVectors << "]\n"
		<< "  -saveOutput         whether output params should be saved default:[" << boolToString(saveOutput) << "]\n";
}

/**
* @Function: convert type to string type;
*/
std::string Args::lossToString(loss_name ln) const {
	switch (ln) {
	case loss_name::ns:
		return "ns";
	}
	return "Unknow loss!";
}

/**
* @Function: convert type to string type;
*/
std::string Args::boolToString(bool b) const {
	if (b) {
		return "true";
	} else {
		return "false";
	}
}

/**
* @Function: convert type to string type;
*/
std::string Args::modelToString(model_name mn) const {
	switch (mn)
	{
	case model_name::skipgram:
		return "skipgram";
	case model_name::cbow:
		return "cbow";
	case model_name::subword:
		return "subword";
	case model_name::substoke:
		return "substoke";
	default:
		return "Unknow model name!";
	}
}