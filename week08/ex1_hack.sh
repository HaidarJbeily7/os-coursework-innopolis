#!/bin/bash

PID=$(cat /tmp/ex1.pid)
PASSWORD_PREFIX="pass:"

grep rw-p /proc/$PID/maps | while read m; do
    start=$(echo $m | awk '{print $1}' | awk -F"-" '{print $1}')
    end=$(echo $m | awk '{print $1}' | awk -F"-" '{print $2}')
    
    mem_contents=$(sudo dd if=/proc/$PID/mem bs=1 skip=$((0x$start)) count=$((0x$end-0x$start)) 2>/dev/null)

    echo "$mem_contents" | grep -aob "$PASSWORD_PREFIX" | while read match; do
        offset=$(echo $match | awk -F":" '{print $1}')
        password_address=$((0x$start + $offset))
        
        # Extracting the password after the "pass:" prefix
        password=$(echo "$mem_contents" | dd bs=1 skip=$offset count=13 2>/dev/null | tail -c 8)

        echo "Found password: $password at address: $password_address"
        kill -9 $PID
    done
done