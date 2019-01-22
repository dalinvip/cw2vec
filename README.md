
## Introduction ##

Paper Link: [cw2vec: Learning Chinese Word Embeddings with Stroke n-gram Information](http://www.statnlp.org/wp-content/uploads/papers/2018/cw2vec/cw2vec.pdf)  

Paper Detail Summary: [cw2vec理论及其实现](https://bamtercelboo.github.io/2018/05/11/cw2vec/)

## Requirements ##

>cmake version 3.10.0-rc5  
>make  GNU Make 4.1  
>gcc  version 5.4.0

## Run Demo ##

- I have uploaded  `word2vec` binary executable file in `cw2vec/word2vec/bin` and rewrite `run.sh` for simple test, you can run `run.sh` directly for simple test.

- According to the *[Building cw2vec using cmake](https://github.com/bamtercelboo/cw2vec#building-cw2vec-using-cmake)*  to recompile and run other model with the *[Example use cases](https://github.com/bamtercelboo/cw2vec#example-use-cases)*.


## Building cw2vec using cmake ##

	git clone git@github.com:bamtercelboo/cw2vec.git
	cd cw2vec && cd word2vec && cd build
	cmake ..
	make
	cd ../bin

This will create the word2vec binary and also all relevant libraries.

## Example use cases ##
the repo not only implement cw2vec(named **substoke**), but also the **skipgram**, **cbow** of word2vec, furthermore, fasttext skipgram is implemented(named **subword**).  

	Please modify train.txt and feature.txt into your own train document.

	skipgram: ./word2vec skipgram -input train.txt -output skipgram_out -lr 0.025 -dim 100 -ws 5 -epoch 5 -minCount 10 -neg 5 -loss ns -thread 8 -t 1e-4 -lrUpdateRate 100  

	cbow:     ./word2vec cbow -input train.txt -output cbow_out -lr 0.05 -dim 100 -ws 5 -epoch 5 -minCount 10 -neg 5 -loss ns -thread 8 -t 1e-4 -lrUpdateRate 100

	subword:  ./word2vec subword -input train.txt -output subword_out -lr 0.025 -dim 100 -ws 5 -epoch 5 -minCount 10 -neg 5 -loss ns -minn 3 -maxn 6 -thread 8 -t 1e-4 -lrUpdateRate 100

	substoke: ./word2vec substoke -input train.txt -infeature feature.txt -output substoke_out -lr 0.025 -dim 100 -ws 5 -epoch 5 -minCount 10 -neg 5 -loss ns -minn 3 -maxn 18 -thread 8 -t 1e-4 -lrUpdateRate 100




## Get chinese stoke feature ##
substoke model need chinese stoke feature(`-infeature`)，I have written a script to acquire the Chinese character of stroke information from [handian](http://www.zdic.net/). here is the script [extract_zh_char_stoke](https://github.com/bamtercelboo/corpus_process_script/tree/master/extract_zh_char_stoke),  see the readme for details.  

Now, I have uploaded a file of stroke features in simplified Chinese, which contains a total of 20901 Chinese characters for use. The file in the [Simplified_Chinese_Feature](https://github.com/bamtercelboo/cw2vec/blob/master/Simplified_Chinese_Feature/sin_chinese_feature.txt) folder.  Or you can use the above script to get it yourself.



**feature file(feature.txt) like this**:

	中 丨フ一丨
	国 丨フ一一丨一丶一
	庆 丶一ノ一ノ丶
	假 ノ丨フ一丨一一フ一フ丶
	期 一丨丨一一一ノ丶ノフ一一
	香 ノ一丨ノ丶丨フ一一
	江 丶丶一一丨一
	将 丶一丨ノフ丶一丨丶
	涌 丶丶一フ丶丨フ一一丨
	入 ノ丶
	人 ノ丶
	潮 丶丶一一丨丨フ一一一丨ノフ一一
	......

I provided a feature file for the test，path is `sample/substoke_feature.txt`.


## Substoke model output embeddings ##

- In this paper, the context word embeddings is used directly as the final word vector. However, according to the idea of fasttext, I also take into account the n-gram feature vector of the stroke information, the n-gram feature vector of the stroke information is taken as an average substitute for the word vector. 

-  There are two outputs in substoke model:
	-  output ends with vec is the context word vector.
	-  output ends with avg is the n-gram feature vector average.


## Word similarity evaluation ##

#### 1. Evaluation script ####
I have already written a Chinese word similarity evaluation script. [Chinese-Word-Similarity-and-Word-Analogy](https://github.com/bamtercelboo/Chinese_Word_Similarity_and_Word_Analogy), see the readme for details.

#### 2. Parameter Settings ####
The parameters are set as follows:  

	dim  100
	window sizes  5
	negative  5
	epoch  5
	minCount  10
	lr  skipgram(0.025)，cbow(0.05)，substoke(0.025)
	n-gram  minn=3, maxn=18

#### 3. result ####
Experimental results show follows  

![](https://i.imgur.com/u0O6RoE.jpg)
  
![](https://i.imgur.com/p4gjsaD.jpg)


## Full documentation ##
Invoke a command without arguments to list available arguments and their default values:

	./word2vec 
	usage: word2vec <command> <args>
	The commands supported by word2vec are:

	skipgram  ------ train word embedding by use skipgram model
	cbow      ------ train word embedding by use cbow model
	subword   ------ train word embedding by use subword(fasttext skipgram)  model
	substoke  ------ train chinses character embedding by use substoke(cw2vec) model

	./word2vec substoke -h
	Train Embedding By Using [substoke] model
	Here is the help information! Usage:

	The Following arguments are mandatory:
		-input              training file path
		-infeature          substoke feature file path
		-output             output file path
	
	The Following arguments are optional:
		-verbose            verbosity level[2]

	The following arguments for the dictionary are optional:
		-minCount           minimal number of word occurences default:[10]
		-bucket             number of buckets default:[2000000]
		-minn               min length of char ngram default:[3]
		-maxn               max length of char ngram default:[6]
		-t                  sampling threshold default:[0.001]

	The following arguments for training are optional:
		-lr                 learning rate default:[0.05]
		-lrUpdateRate       change the rate of updates for the learning rate default:[100]
		-dim                size of word vectors default:[100]
		-ws                 size of the context window default:[5]
		-epoch              number of epochs default:[5]
		-neg                number of negatives sampled default:[5]
		-loss               loss function {ns} default:[ns]
		-thread             number of threads default:[1]
		-pretrainedVectors  pretrained word vectors for supervised learning default:[]
		-saveOutput         whether output params should be saved default:[false]

## References ##
[1] [Cao, Shaosheng, et al. "cw2vec: Learning Chinese Word Embeddings with Stroke n-gram Information." (2018). ](http://www.statnlp.org/wp-content/uploads/papers/2018/cw2vec/cw2vec.pdf)   
[2][ Bojanowski, Piotr, et al. "Enriching word vectors with subword information." arXiv preprint arXiv:1607.04606 (2016).](https://arxiv.org/pdf/1607.04606.pdf)  
[3] [fastText-github](https://github.com/facebookresearch/fastText)  
[4] [cw2vec理论及其实现](https://bamtercelboo.github.io/2018/05/11/cw2vec/)

## Question ##

- if you have any question, you can open a issue or email bamtercelboo@{gmail.com, 163.com}.

- if you have any good suggestions, you can PR or email me.






	



