#!/bin/bash


echo "Checking file types in /dev and /etc:"
file /dev/* /etc/* # Lists file types in /dev and /etc

echo "Counting number of directories in /etc:"
ls -l /etc | grep ^d | wc -l 

mkdir test
cd test

# Write a hello world program in C (English)
echo 'Writing a hello world program (ex1-1.c):'
echo '#include <stdio.h>' > ex1-1.c
echo 'int main() {' >> ex1-1.c
echo '    printf("Hello, World!\\n");' >> ex1-1.c
echo '    return 0;' >> ex1-1.c
echo '}' >> ex1-1.c



# Write a hello world program in C (Russian)
echo 'Writing a hello world program (ex1.c):'
echo '#include <stdio.h>' > ex1.c
echo 'int main() {' >> ex1.c
echo '    printf("Привет, мир!\\n");' >> ex1.c
echo '    return 0;' >> ex1.c
echo '}' >> ex1.c

echo "File type of ex1-1.c before compilation: (English)"
file ex1-1.c 

echo "Compiling ex1.c: (English)"
gcc ex1-1.c -o ex1-1 

# Check file type of ex1.c before and after compilation
echo "File type of ex1.c before compilation: (Russian)"
file ex1.c 

echo "Compiling ex1.c: (Russian)"
gcc ex1.c -o ex1 # Compiles ex1.c

echo "File type of ex1.c after compilation: (Russian)"
file ex1 


echo "\nWhat is the difference?"
echo "ex1-1.c: (English) -> Encoding is ASCII"
echo "ex1.c: (Russian) -> Encoding is Unicode"

cd ..