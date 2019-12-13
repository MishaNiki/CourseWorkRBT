
gcc -c rbtree.c -o rbtree.o -lm -O2
gcc -c  main.c -o  main.o -lm -O2
gcc -Wall main.o rbtree.o -o main.exe -lm -O2

rm main.o
rm rbtree.o 
