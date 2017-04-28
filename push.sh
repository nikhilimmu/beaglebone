#!/bin/sh
if [ -z "$1" ]
  then
    echo "no comment supplied use: ./push comment"
    exit 1
fi
echo "Adding.."
git add .
echo "comment "$1
git commit -m $1
echo "set repo"
git remote add origin https://github.com/nikhilimmu/beaglebone.git
echo "push"
git push -u origin master
echo "completed..."
