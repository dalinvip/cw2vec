
path_input=./sample
path_out=./sample

nohup ./word2vec skipgram -input ${path_input}/zhwiki_char_1b.txt -output ${path_out}/zhwiki_skipgram.300d -lr 0.025 -dim 300 -ws 5 -epoch 5 -minCount 10 -neg 5 -loss ns -thread 8 -t 1e-4 -lrUpdateRate 100 > log_skipgram_300d 2>&1 &


