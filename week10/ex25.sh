#!/bin/bash


touch ex5.txt


chmod a-w ex5.txt
echo "Checking permissions of ex5.txt:"
ls -l ex5.txt

chmod ugo=rwx,g= ex5.txt
echo "Checking permissions of ex5.txt:"
ls -l ex5.txt

chmod g=u ex5.txt
echo "Checking permissions of ex5.txt:"
ls -l ex5.txt

echo "660 - Read and write for owner and group, no permissions for others."
echo "775 - All permissions for owner and group, read and execute for others."
echo "777 - All permissions for everyone."
