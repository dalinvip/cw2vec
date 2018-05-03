/**
* @Author: bamtercelboo
* @Date: 2018/03/15
* @File: dictionary.h
* @Contact: bamtercelboo@{gmail.com, 163.com}
* @Function: None
*/

#pragma once

#include "args.h"
#include "real.h"
#include "alphabet.h"
#include "Utf.h"

#include <random>
#include <memory>
#include <assert.h>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <map>


struct entry {
	std::string word;
	int64_t count;
	std::vector<int32_t> subwords;
};

//readfeature
struct  feature{
	std::string word;
	int32_t count;
	std::string subfeature;
};

class Dictionary {
  protected:
	  //static const int32_t MAX_VOCAB_SIZE = 100000000;
	  static const int32_t MAX_VOCAB_SIZE = 30000000;
	static const int32_t MAX_LINE_SIZE = 1024;

	int32_t findWord(const std::string&) const;
	void addWord(const std::string&);
	int32_t findTarget(const std::string&) const;
	void addTarget(const std::string&, int64_t);
	int32_t findFeature(const std::string&) const;
	void addFeature(const std::string&, int64_t);

	void initFeature();
	void initTargets();
	void initNgrams();

	void reset(std::istream&) const;

	std::shared_ptr<Args> args_;
	alphabet words_;
	//std::vector<entry> wordprops_;
	std::map<std::string, std::string> featuremap;
	std::map<std::string, std::string> ::iterator featpos;
	alphabet features_;
	alphabet targets_;
	std::vector<real> pdiscard_;
	int64_t ntokens_;

public:
	static const std::string EOS;
	static const std::string BOW;
	static const std::string EOW;

	explicit Dictionary(std::shared_ptr<Args>);

	std::vector<entry> wordprops_;

	int32_t nwords() const;
	int32_t ntargets() const;
	int32_t nfeatures() const;
	int64_t ntokens() const;
	int32_t getWordId(const std::string&) const;
	int32_t getTargetId(const std::string&) const;
	int32_t getFeatureId(const std::string&) const;
	std::string getWord(int32_t) const;
	std::string getTarget(int32_t) const;
	std::string getFeature(int32_t) const;
	std::string getFeat(std::string);
	void trim(std::string&);

	std::vector<int64_t> getCounts() const;
	void computeSubwords(const std::string&, std::vector<std::string>&) const;
	void computeSubwords(const std::string&, std::vector<int32_t>&) const;


	//subfeature
	void computerSubfeat(const std::string&, std::vector<std::string>&) const;
	void computerSubfeat(const std::string&, std::vector<int32_t>&) const;

	void initTableDiscard();
	bool discard(int32_t, real) const;

	bool readWord(std::istream&, std::string&) const;
	void readFeature(std::istream&);
	void readFromFile(std::istream&);
	void readFromFile(std::istream&, std::istream&);
	//int32_t getLine(std::istream&, std::vector<int32_t>&, std::minstd_rand&) const;
	int32_t getLine(std::istream&, std::vector<std::vector<int32_t> >&, std::vector<std::vector<int32_t> >&, std::vector<int32_t>&, std::minstd_rand&) const;
	int32_t getLine_zh(std::istream&, std::vector<std::vector<int32_t> >&, std::vector<std::vector<int32_t> >&, std::vector<int32_t>&, std::minstd_rand&) const;
};

const std::string Dictionary::EOS = "</s>";
const std::string Dictionary::BOW = "<";
const std::string Dictionary::EOW = ">";

/**
* @Function: initial Dictionary class argument.
*/
Dictionary::Dictionary(std::shared_ptr<Args> args) : args_(args) {
	words_.setCapacity(MAX_VOCAB_SIZE - 1);
	features_.setCapacity(MAX_VOCAB_SIZE - 1);
	targets_.setCapacity(MAX_VOCAB_SIZE - 1);
}

/**
* @Function: find word Id.
*/
int32_t Dictionary::findWord(const std::string& w) const {
	return words_.from_string(w);
}


/**
* @Function: find word Id.
*/
int32_t Dictionary::getWordId(const std::string& w) const {
	return findWord(w);
}

/**
* @Function: find word from Id.
*/
std::string Dictionary::getWord(int32_t id) const {
	assert(id >= 0);
	assert(id <= words_.m_size);
	return words_.from_id(id);
}

/**
* @Function: add word to alphabet.
*/
void Dictionary::addWord(const std::string& w) {
	words_.add_string(w);
}

/**
* @Function: word count in alphabet.
*/
int32_t Dictionary::nwords() const {
	return words_.m_size;
}

/**
* @Function: find target Id.
*/
int32_t Dictionary::findTarget(const std::string& w) const {
	return targets_.from_string(w);
}

/**
* @Function: find target Id.
*/
int32_t Dictionary::getTargetId(const std::string& w) const {
	return findTarget(w);
}

/**
* @Function: find target from Id.
*/
std::string Dictionary::getTarget(int32_t id) const {
	assert(id >= 0);
	assert(id < targets_.m_size);
	return targets_.from_id(id);
}

/**
* @Function: add target to alphabet.
*/
void Dictionary::addTarget(const std::string& w, int64_t freq) {
	targets_.add_string(w);
}

/**
* @Function: target count in alphabet.
*/
int32_t Dictionary::ntargets() const {
	return targets_.m_size;
}

/**
* @Function: find feature Id.
*/
int32_t Dictionary::findFeature(const std::string& w) const {
	return features_.from_string(w);
}

/**
* @Function: find feature Id.
*/
int32_t Dictionary::getFeatureId(const std::string& w) const {
	return findFeature(w);
}

/**
* @Function: find feature from Id.
*/
std::string Dictionary::getFeature(int32_t id) const {
	assert(id >= 0);
	assert(id < features_.m_size);
	return features_.from_id(id);
}

/**
* @Function: add feature to alphabet.
*/
void Dictionary::addFeature(const std::string& w, int64_t freq) {
	features_.add_string(w);
}

/**
* @Function: feature count in alphabet.
*/
int32_t Dictionary::nfeatures() const {
	return features_.m_size;
}

/**
* @Function: target initial.
*/
void Dictionary::initTargets() {
	//same as source
	for (size_t i = 0; i < words_.m_size; i++) {
		addTarget(words_.from_id(i), words_.m_id_to_freq[i]);
	}
}

/**
* @Function: feature initial.
*/
void Dictionary::initFeature() {
	// skipgram and cbow model don't need feature
	if ((args_->model == model_name::skipgram) || (args_->model == model_name::cbow))
		return;
	std::cout << "initail feature, maybe take a while...... " << std::endl;
	//subword for english
	if (args_->model == model_name::subword) {
		for (size_t i = 0; i < words_.m_size; i++) {
			std::string word = BOW + words_.from_id(i) + EOW;
			if (word != EOS) {
				vector<string> ngrams;
				computeSubwords(word, ngrams);
				for (size_t j = 0; j < ngrams.size(); j++) {
					addFeature(ngrams[j], words_.m_id_to_freq[i]);
				}
			}
		}
	}

	//substoke for chinese word stoke feature
	if (args_->model == model_name::substoke) {
		//std::cerr << "initial word stoke feature" << std::endl;
		//std::cerr << "substoke model" << std::endl;
		std::string word;
		std::string feat;
		for (size_t i = 0; i < words_.m_size; i++) {
			word = words_.from_id(i);
			feat = getFeat(word);
			std::string featBE = (BOW + feat + EOW);
			if (word != EOS) {
				vector<string> ngrams;
				computerSubfeat(featBE, ngrams);
				//computeSubwords(featBE, ngrams);
				for (size_t j = 0; j < ngrams.size(); j++) {
					addFeature(ngrams[j], words_.m_id_to_freq[i]);
				}
			}
		}
	}
	std::cout << "initail feature finished. " << std::endl;
}

/**
* @Function: erase the empty space
*/
void Dictionary::trim(std::string& s) {
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}
}

/**
* @Function: get feature from feature map in dictionary.
*/
std::string Dictionary::getFeat(std::string word) {
	std::vector<string> char_vec;
	getCharactersFromUTF8String(word, char_vec);
	std::string feat;
	for (int i = 0; i < char_vec.size(); i++) {
		std::string char_str = char_vec[i];
		featpos = featuremap.find(char_str);
		if (featpos != featuremap.end()) {
			//std::cout << (*featpos).first << "	" << (*featpos).second << std::endl;
			feat += (*featpos).second;
		}
	}
	trim(feat);
	//if (feat == "") {
	//	//feat = args_->featurepad;
	//	feat = word;
	//}
	return feat;
}

/**
* @Function: computer subfeature for chinese character feature, like radaical/stoke.
*/
void Dictionary::computerSubfeat(const std::string& featbe_s, std::vector<std::string>& substrings) const {
	std::vector<string> featbe;
	getCharactersFromUTF8String(featbe_s, featbe);
	std::string ngram = "";
	for (size_t i = 0; i < featbe.size(); i++) {
		for (size_t j = args_->minn; j <= args_->maxn; j++) {
			if ((i + j) > featbe.size()) 
				continue;
			
			ngram.clear();
			for (size_t k = i; k <(i + j); k++) {
				ngram += featbe[k];
			}
			substrings.push_back(ngram);
		}
	}

	/*for (size_t i = 0; i < featbe.size(); i++) {
		std::string ngram;
		if ((featbe[i] & 0xC0) == 0x80) continue;
		for (size_t j = i, n = 1; j < featbe.size() && n <= args_->maxn; n++) {
			ngram.push_back(featbe[j++]);
			while (j < featbe.size() && (featbe[j] & 0xC0) == 0x80) {
				ngram.push_back(featbe[j++]);
			}
			if (n >= args_->minn && !(n == 1 && (i == 0 || j == featbe.size()))) {
				substrings.push_back(ngram);
			}
		}
	}*/
}

/**
* @Function: computer subfeature for chinese character feature, like radaical/stoke.
*/
void Dictionary::computerSubfeat(const std::string& word_s, std::vector<int32_t>& ngrams) const {
	std::vector<string> word;
	getCharactersFromUTF8String(word_s, word);
	std::string ngram = "";
	for (size_t i = 0; i < word.size(); i++) {
		for (size_t j = args_->minn; j <= args_->maxn; j++) {
			if ((i + j) > word.size())
				continue;

			ngram.clear();
			for (size_t k = i; k <(i + j); k++) {
				ngram += word[k];
			}
			int32_t h = findFeature(ngram);
			if (h >= 0)
				//ngrams.push_back(words_.m_size + h);
				ngrams.push_back(h);
		}
	}
	
	/*for (size_t i = 0; i < word.size(); i++) {
		std::string ngram;
		if ((word[i] & 0xC0) == 0x80) continue;
		for (size_t j = i, n = 1; j < word.size() && n <= args_->maxn; n++) {
			ngram.push_back(word[j++]);
			while (j < word.size() && (word[j] & 0xC0) == 0x80) {
				ngram.push_back(word[j++]);
			}
			if (n >= args_->minn && !(n == 1 && (i == 0 || j == word.size()))) {
				int32_t h = findFeature(ngram);
				if (h >= 0)
					ngrams.push_back(words_.m_size + h);
			}
		}
	}*/
}

/**
* @Function: computer subword for english.
*/
void Dictionary::computeSubwords(const std::string& word, std::vector<std::string>& substrings) const {
	for (size_t i = 0; i < word.size(); i++) {
		std::string ngram;
		if ((word[i] & 0xC0) == 0x80) continue;
		for (size_t j = i, n = 1; j < word.size() && n <= args_->maxn; n++) {
			ngram.push_back(word[j++]);
			while (j < word.size() && (word[j] & 0xC0) == 0x80) {
				ngram.push_back(word[j++]);
			}
			if (n >= args_->minn && !(n == 1 && (i == 0 || j == word.size()))) {
				substrings.push_back(ngram);
			}
		}
	}
}

/**
* @Function: computer subword for english.
*/
void Dictionary::computeSubwords(const std::string& word, std::vector<int32_t>& ngrams) const {
	for (size_t i = 0; i < word.size(); i++) {
		std::string ngram;
		if ((word[i] & 0xC0) == 0x80) continue;
		for (size_t j = i, n = 1; j < word.size() && n <= args_->maxn; n++) {
			ngram.push_back(word[j++]);
			while (j < word.size() && (word[j] & 0xC0) == 0x80) {
				ngram.push_back(word[j++]);
			}
			if (n >= args_->minn && !(n == 1 && (i == 0 || j == word.size()))) {
				int32_t h = findFeature(ngram);
				if (h >= 0)ngrams.push_back(words_.m_size + h);
			}
		}
	}
}

/**
* @Function: ntokens count.
*/
int64_t Dictionary::ntokens() const {
	return ntokens_;
}

/**
* @Function: Ngrams initial.
*/
void Dictionary::initNgrams() {
	wordprops_.resize(words_.m_size);

	std::cout << "initail Ngrams feature, maybe take a while...... " << std::endl;

	if ((args_->model == model_name::skipgram) || (args_->model == model_name::cbow) || (args_->model == model_name::subword)) {
		for (size_t i = 0; i < words_.m_size; i++) {
			wordprops_[i].word = words_.from_id(i);
			wordprops_[i].count = words_.m_id_to_freq[i];

			std::string word = (BOW + wordprops_[i].word + EOW);
			wordprops_[i].subwords.clear();
			//wordprops_[i].subwords.push_back(i);
			if (wordprops_[i].word != EOS) {
				computeSubwords(word, wordprops_[i].subwords);
			}
		}
	}

	// substoke ngram
	if (args_->model == model_name::substoke) {
		for (size_t i = 0; i < words_.m_size; i++) {
			wordprops_[i].word = words_.from_id(i);
			wordprops_[i].count = words_.m_id_to_freq[i];
			std::string feat = getFeat(wordprops_[i].word);
			std::string featBE = BOW + feat + EOW;
			wordprops_[i].subwords.clear();
			//wordprops_[i].subwords.push_back(i);
			if (wordprops_[i].word != EOS) {
				computerSubfeat(featBE, wordprops_[i].subwords);
			}
		}
	}
	std::cout << "initail Ngrams feature finished. " << std::endl;
}

/**
* @Function: TableDiscard initial.
*/
void Dictionary::initTableDiscard() {
	pdiscard_.resize(words_.m_size);
	for (size_t i = 0; i < words_.m_size; i++) {
		//real f = real(wordprops_[i].count) / real(ntokens_);
		real f = real(words_.m_id_to_freq[i]) / real(ntokens_);
		pdiscard_[i] = std::sqrt(args_->t / f) + args_->t / f;
	}
}

/**
* @Function: None.
*/
bool Dictionary::discard(int32_t id, real rand) const {
	assert(id >= 0);
	assert(id < words_.m_size);

	return rand > pdiscard_[id];
}

/**
* @Function: read word.
*/
bool Dictionary::readWord(std::istream& in, std::string& word) const {
	char c;
	std::streambuf& sb = *in.rdbuf();
	word.clear();
	while ((c = sb.sbumpc()) != EOF) {
		//std::cout <<"char " << c << std::endl;
		if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f' || c == '\0') {
			if (word.empty()) {
				if (c == '\n') {
					word += EOS;
					return true;
				}
				continue;
			}
			else {
				if (c == '\n')
					sb.sungetc();
				return true;
			}
		}
		word.push_back(c);
	}
	// trigger eofbit
	in.get();
	return !word.empty();
}

/**
* @Function: read file.
*/
void Dictionary::readFromFile(std::istream& in) {
	std::string word;
	ntokens_ = 0;
	int64_t	minThreshold = 1;
	while (readWord(in, word)) {
		addWord(word);
		ntokens_++;
		if (ntokens_ % 1000000 == 0 && args_->verbose > 1) {
			std::cerr << "\rRead " << ntokens_ / 1000000 << "M words" << std::flush;
		}
		if (words_.m_size > 0.75 * MAX_VOCAB_SIZE) {
			minThreshold++;
			words_.prune(minThreshold);
		}
	}
	int64_t words = words_.m_size;
	words_.prune(args_->minCount);

	initFeature();
	initTargets();
	initNgrams();
	initTableDiscard();

	if (args_->verbose > 0) {
		std::cerr << "\rRead " << ntokens_ / 1000000 << "M words" << std::endl;
		std::cerr << "Number of all words:  " << ntokens_ << std::endl;
		std::cerr << "Number of all words:  " << words << std::endl;
		std::cerr << "Number of words:  " << words_.m_size << std::endl;
		std::cerr << "Number of features: " << features_.m_size << std::endl;
		std::cerr << "Number of targets: " << targets_.m_size << std::endl;
	}
	if (words_.m_size == 0) {
		throw std::invalid_argument("Empty vocabulary. Check the input file Or Try a smaller -minCount value.");
	}
}

/**
* @Function: read file.
*/
void Dictionary::readFromFile(std::istream& in, std::istream& infeature) {
	std::string word;
	ntokens_ = 0;
	int64_t	minThreshold = 1;
	while (readWord(in, word)) {
		addWord(word);
		ntokens_++;
		if (ntokens_ % 1000000 == 0 && args_->verbose > 1) {
			std::cerr << "\rRead " << ntokens_ / 1000000 << "M words" << std::flush;
		}
		if (words_.m_size > 0.75 * MAX_VOCAB_SIZE) {
			minThreshold++;
			words_.prune(minThreshold);
		}
	}

	int64_t words = words_.m_size;
	words_.prune(args_->minCount);

	//read feature file
	readFeature(infeature);

	// initial feature and ngram
	initFeature();
	initTargets();
	initNgrams();
	initTableDiscard();

	if (args_->verbose > 0) {
		std::cerr << "\rRead " << ntokens_ / 1000000 << "M words" << std::endl;
		std::cerr << "Number of all words:  " << ntokens_ << std::endl;
		std::cerr << "Number of all words:  " << words << std::endl;
		std::cerr << "Number of words:  " << words_.m_size << std::endl;
		std::cerr << "Number of features: " << features_.m_size << std::endl;
		std::cerr << "Number of targets: " << targets_.m_size << std::endl;
	}
	if (words_.m_size == 0) {
		throw std::invalid_argument("Empty vocabulary. Check the input file Or Try a smaller -minCount value.");
	}
}

/**
* @Function: read feature file.
*/
void Dictionary::readFeature(std::istream& infeature) {
	std::string line;
	std::string word;
	std::string feat;
	featuremap.clear();
	while (std::getline(infeature, line)){
		//std::cout << line << std::endl;
		int pos = line.find_first_of(' ');
		if (pos == -1) {
			std::cerr << "Warning " << line << std::endl;
			continue;
		}
		word = line.substr(0, pos);
		feat = line.substr(pos + 1);
		//std::cout << word << " && " << feat << std::endl;
		featuremap[word] = feat;
	}
	std::cerr << "\nfeaturemap size	" << featuremap.size() << std::endl;
}

/**
* @Function: reset input iostream.
*/
void Dictionary::reset(std::istream& in) const {
	if (in.eof()) {
		in.clear();
		in.seekg(std::streampos(0));
	}
}

/**
* @Function: getCounts.
*/
std::vector<int64_t> Dictionary::getCounts() const {
	std::vector<int64_t> counts;
	for (int i = 0; i < wordprops_.size(); i++) {
		counts.push_back(wordprops_[i].count);
	}
	return counts;
}

/**
* @Function: getLine.
*/
int32_t Dictionary::getLine(std::istream& in, std::vector<std::vector<int32_t> >& sourceTypes,
	std::vector<std::vector<int32_t> >& sources, std::vector<int32_t>& targets, std::minstd_rand& rng) const {
	std::uniform_real_distribution<> uniform(0, 1);
	std::string token;
	vector<string> words;
	int32_t ntokens = 0;

	reset(in);
	sourceTypes.clear();
	sources.clear();
	targets.clear();
	words.clear();
	while (readWord(in, token)) {
		if (token == EOS)
			break;
		words.push_back(token);
	}

	int word_num = words.size();
	int valid = 0;
	
	for (int i = 0; i < word_num; i++) {
		int32_t wid = findWord(words[i]);
		int32_t tid = findTarget(words[i]);
		ntokens++;
		//int a = discard(wid, uniform(rng));
		if (wid < 0 || tid < 0 || discard(wid, uniform(rng)))
			continue;
		valid++;
		sourceTypes.push_back(std::vector<int32_t>());
		sources.push_back(std::vector<int32_t>());
		sourceTypes[valid - 1].push_back(0);
		sources[valid - 1].push_back(wid);
		targets.push_back(tid);

		if (ntokens > MAX_LINE_SIZE) break;

		if ((args_->model == model_name::skipgram) || (args_->model == model_name::cbow))
			continue;

		sources[valid - 1].pop_back();
		
		int ngrams_count = wordprops_[wid].subwords.size();
		for (int j = 0; j < ngrams_count; j++) {
			sourceTypes[valid - 1].push_back(0);
			sources[valid - 1].push_back(wordprops_[wid].subwords[j]);
		}
		/*if (ntokens > MAX_LINE_SIZE)
			std::cout << "aaa" << std::endl;
			break;*/
	}
	return ntokens;
}

