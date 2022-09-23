rm test.txt;
for file in *.diff
do
echo $file >> test.txt;
cat $file >>test.txt;
echo "************">>test.txt
done;
