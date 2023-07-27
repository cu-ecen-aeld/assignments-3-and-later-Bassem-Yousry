if [ $# != 2 ]
then
echo "2 parameters required"
exit 1
else
mkdir -p $(dirname $1) &&  touch $1
if [ $? != 0 ] 
then
echo "File not created"
exit 1
fi
echo $2 > $1
fi
