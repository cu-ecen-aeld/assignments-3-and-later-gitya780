#!/bin/bash
if [[ -z "$1" || -z "$2" ]]; then
    echo "Error 1: Please enter the first argument as a valid file directory"
    echo "Error 2: Please enter the second argument as the string you want to find"
    exit 1
else  

echo $(install -Dv /dev/null "$1")
if [ $? -ne 0 ]; then
    echo "Error: Failed to create the file."
    exit 1
fi
echo $(echo "$2" > "$1")
fi