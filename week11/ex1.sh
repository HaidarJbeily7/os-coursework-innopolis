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


#Compiling ex1.c and copying it to the LOFS
gcc ex1.c -o ex1
cp ex1 ./lofsdisk

# Add files to the LOFS
echo "Haidar" > ./lofsdisk/file1 
echo "Jbeily" > ./lofsdisk/file2 

# Function to retrieve shared libraries
get_libs() {
    ldd $1 | tr -s '[:blank:]' | cut -d ' ' -f3 | grep '/'
}


# Preaparing by creating the directories for binaries and libraries
sudo mkdir -p ./lofsdisk/bin ./lofsdisk/lib ./lofsdisk/lib64
sudo cp /lib64/ld-linux-x86-64.so.2 ./lofsdisk/lib64/


# Copy binaries and their libraries to the LOFS
for BIN in bash cat echo ls; do
    # Find the full path of the binary
    BIN_PATH=$(which $BIN)

    # Copy the binary to the LOFS /bin directory
    sudo cp $BIN_PATH ./lofsdisk/bin/

    # Get the shared libraries using the get_libs function
    LIBS=$(get_libs $BIN_PATH)

    # Copy each shared library
    for LIB in $LIBS; do
        # Ensure the directory structure for the library exists in LOFS
        LIB_DIR=$(dirname $LIB)
        sudo mkdir -p ./lofsdisk$LIB_DIR

        # Copy the shared library
        sudo cp $LIB ./lofsdisk$LIB_DIR
    done
done


# Change root and run the C program
sudo chroot ./lofsdisk /bin/bash -c './ex1 > /ex1.txt'

# Run the same program without chroot
./ex1 >> ./ex1.txt

# Append the analysis of outputs to ex1.txt
echo "Analysis:" >> ./ex1.txt
echo "In the chroot environment, the program sees the root as the LOFS. Outside, it sees the system's root directory." >> ./ex1.txt

