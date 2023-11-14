#!/bin/bash


[ -d "./tmp" ] && rm -r ./tmp
ln -s ./tmp tmp1

echo "Running ls -li before creating ./tmp:"
ls -li

mkdir ./tmp

echo "Running ls -li after creating ./tmp:"
ls -li

echo "\n\nWhat is the diffrence? The diiference that after we created the ./tmp directory, the symbolic link is now pointing to the new directory and usable but before that this was a broken link.\n\n"


./gen.sh 10 ./tmp/ex1.txt


ln -s ./tmp1/tmp2


./gen.sh 10 ./tmp1/tmp2/ex1.txt


echo "Checking the content of ./tmp1/tmp2:"
ls ./tmp1/tmp2

rm -r ./tmp

echo "Checking symbolic links after deleting ./tmp:"
ls -l tmp1
