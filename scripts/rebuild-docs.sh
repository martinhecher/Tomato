#!/bin/sh
for i in `ls ../src/**/*.?pp`; do docco -o ../docs -s 'parallel' $i; done

