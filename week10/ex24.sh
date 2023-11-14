#!/bin/bash

echo "Delete ./tmp if exists"
[ -d "./tmp" ] && rm -r ./tmp

echo "\n\nCreating symbolic link tmp1 to ./tmp:\n"
ln -s ./tmp tmp1

echo "\nRunning ls -li before creating ./tmp:"
ls -li

mkdir ./tmp

echo "\nRunning ls -li after creating ./tmp:"
ls -li


echo "\n\nWhat is the diffrence? The diiference that after we created the ./tmp directory, the symbolic link is now pointing to the new directory and usable but before that this was a broken link.\n\n"


echo "\n\nAdding ex1.txt file to ./tmp:"
chmod -x ./gen.sh
./gen.sh 10 ./tmp/ex1.txt

echo "\n\nChecking the content of ./tmp1:"
ls -li ./tmp1


echo "\n\nAdding symbolic link to ./tmp:"
ln -s "$(realpath .)/tmp" ./tmp1/tmp2

./gen.sh 10 ./tmp1/tmp2/ex1.txt


echo "\n\nChecking the content of ./tmp1/tmp2/tmp2/tmp2:"
ls ./tmp1/tmp2/tmp2/tmp2

rm -r ./tmp

echo "\n\nChecking symbolic links after deleting ./tmp:"
ls -l tmp1

rm -r ./tmp1