#!/bin/bash

ln ex1.txt ex13.txt
ln ex1.txt ex12.txt

diff ex1.txt ex13.txt
diff ex1.txt ex12.txt
echo "There is no difference because all those files represent the hardlink to the same inode"


ls -i ex1.txt
ls -i ex13.txt
ls -i ex12.txt
echo "as we can see, all those files have the same inode"


echo "Checking Disk Usage of ex1.txt using du"
du ex1.txt


mv ex13.txt /tmp

echo "Tracing all links to ex1.txt in current path:"
find . -samefile ex1.txt

echo "Tracing all links to ex1.txt in root path:"
find / -samefile ex1.txt

echo "Checking number of hard links for ex1.txt:"
stat ex1.txt

find . -samefile ex1.txt -exec rm {} \;
