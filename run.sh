
path_input=./sample
path_out=.

rm -rf ./bin
cp -rf ./word2vec/bin .

./bin/word2vec skipgram -input ${path_input}/zhwiki.txt -output ${path_out}/zhwiki_skipgram.300d -lr 0.025 -dim 300 -ws 5 -epoch 5 -minCount 10 -neg 5 -loss ns -thread 8 -t 1e-4 -lrUpdateRate 100


