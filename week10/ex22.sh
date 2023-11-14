#!/bin/bash

cd test

echo "Checking inode, blocks, size, and permissions of ex1:"
stat ex1 


echo "Copying ex1 to ex2:"
cp ex1 ex2

echo "Checking link count and inode numbers for ex2:"
ls -li ex2 

echo " ex1, ex2 don't have the same inode number!!!\n\n"

stat -c "%h - %n" /etc/* | grep ^3