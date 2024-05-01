#!/bin/sh
if [[ -z "$1" || -z "$2" ]]; then
    echo "Error 1: Please enter the first argument as a valid file directory"
    echo "Error 2: Please enter the second argument as the string you want to find"
    exit 1
else  
    if [ ! -d "$1" ]; then
        echo "Error: '$1' this directory does not exist"
        exit 1
    fi
fi
echo "The number of files are $(find "$1" -type f | wc -l) and the number of matching lines are $(grep -rn "$1" -e "$2"* | wc -l)"
echo -e $(grep -rn "$1" -e "$2"*)