#!/bin/bash
if cmp -s text1.txt res11.txt; then
    if cmp -s text2.txt res12.txt; then
        echo Good
    else 
        echo Fail
    fi
else 
    echo Fail
fi