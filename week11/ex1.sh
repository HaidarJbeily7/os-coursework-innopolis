#!/bin/bash

# Create a file lofs.img of size at least 50 MiB
sudo dd if=/dev/zero of=lofs.img bs=1M count=50 # Create a blank file of 50MB

# Setup a loop device on the created file
sudo losetup -fP lofs.img # Associate a loop device with our file

# Create an ext4 filesystem on the loop device
sudo mkfs -t ext4 lofs.img # Format the loop device as ext4

# Create a mount point directory
mkdir -p ./lofsdisk

# Mount the loop device
sudo mount -o loop lofs.img ./lofsdisk # Mount the filesystem

gcc ex1.c -o ./lofsdisk/ex1

# Add files to the LOFS
echo "Haidar" > ./lofsdisk/file1 
echo "Jbeily" > ./lofsdisk/file2 

# Function to retrieve shared libraries
get_libs() {
    ldd $1 | tr -s '[:blank:]' | cut -d ' ' -f3 | grep '/'
}

# Copy binaries and their libraries to the LOFS
for BIN in bash cat echo ls; do
    BIN_PATH=$(which $BIN)
    sudo cp $BIN_PATH ./lofsdisk
    LIBS=$(get_libs $BIN_PATH)
    for LIB in $LIBS; do
        LIB_DIR=$(dirname $LIB)
        sudo mkdir -p ./lofsdisk$LIB_DIR
        sudo cp $LIB ./lofsdisk$LIB_DIR
    done
done

# Change root and run the C program
sudo chroot ./lofsdisk /bin/bash -c './ex1 > /ex1.txt'

# Run the same program without chroot
./ex1 >> ./lofsdisk/ex1.txt

# Append the analysis of outputs to ex1.txt
echo "Analysis:" >> ./lofsdisk/ex1.txt
echo "In the chroot environment, the program sees the root as the LOFS. Outside, it sees the system's root directory." >> ./lofsdisk/ex1.txt

