
gcc -c rbtree.c -o rbtree.o -lm
gcc -c  main.c -o  main.o -lm
gcc -Wall main.o rbtree.o -o main -lm

rm main.o
rm rbtree.o 
