#!/bin/bash
 
for i in $(find "$1" -type f)
do
        ls -lh "$i" | awk '{print $1, $5, $9}' 
done
