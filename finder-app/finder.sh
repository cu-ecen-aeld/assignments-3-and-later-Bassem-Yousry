if [ $# != 2 ]
then
echo "2 parameters required"
exit 1
elif [ ! -d $1 ]
then
echo "$1 this dir doesn't exist"
exit 1
else
a=$( ls -A $1 | wc -l )
b=$( grep --exclude-dir=* $2 ${1}/* | wc -l )
echo "The number of files are $a and the number of matching lines are $b"
fi
