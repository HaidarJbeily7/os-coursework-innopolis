#!/bin/bash

./gen.sh 10 > test/ex1.txt

cd test

ln ex1.txt ex13.txt
ln ex1.txt ex12.txt

diff ex1.txt ex13.txt
diff ex1.txt ex12.txt
echo "There is no difference because all those files represent the hardlink to the same inode"


ls -i ex1.txt >> output.txt
ls -i ex13.txt >> output.txt
ls -i ex12.txt >> output.txt

echo "as we can see, all those files have the same inode"


echo "Checking Disk Usage of ex1.txt using du"
du ex1.txt


mv ex13.txt /tmp

echo "Tracing all links to ex1.txt in current path:"
find . -samefile ex1.txt 2> error.log

echo "Tracing all links to ex1.txt in root path:"
find / -samefile ex1.txt 2> error.log

echo "\n\nWhen you search in the current path (./), you are searching within a specific directory and its subdirectories. This is useful for finding files and directories within a limited scope, such as the contents of a specific folder.

On the other hand, searching in the root path (/) allows you to search the entire filesystem, including all directories, partitions, and mounted devices. This is necessary when you need to search for files or directories across the entire system, regardless of their location.

\n\n"

echo "Checking number of hard links for ex1.txt:"
stat ex1.txt

find . -samefile ex1.txt -exec rm {} \;

cd ..