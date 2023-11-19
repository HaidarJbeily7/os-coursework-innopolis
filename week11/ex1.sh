#!/bin/bash

get_libs() {
    binary_path="$1"
    ldd "$binary_path" | awk '{print $3}'
}


file_size="50M"
file_name="lofs.img"
mount_point="./lofsdisk"

fallocate -l "$file_size" "$file_name"

loop_device=$(losetup -f)
losetup "$loop_device" "$file_name"

mkfs -t ext4 "$loop_device"

mkdir -p "$mount_point"

mount -t ext4 "$loop_device" "$mount_point"

echo "Haidar" > "$mount_point/file1"
echo "Jbeily" > "$mount_point/file2"

required_binaries=("bash" "cat" "echo" "ls")

for binary in "${required_binaries[@]}"; do
    cp "$(which $binary)" "$mount_point/"
    libs=($(get_libs "$(which $binary)"))
    for lib in "${libs[@]}"; do
        cp "$lib" "$mount_point/"
    done
done


chroot "$mount_point"

gcc -o ex1 ex1.c
./ex1 >> ex1.txt

exit


./ex1 >> ex1.txt

echo "Filesystem mounted at $mount_point"
