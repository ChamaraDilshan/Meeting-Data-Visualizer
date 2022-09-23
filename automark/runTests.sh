rm *.diff
empty="";

inputFiles=./inputs/*.inp;

for prog in *.out;
do
directory="${prog/.out/$empty}";
rm -rf $directory;
mkdir $directory;
echo $directory;
#run all test cases
for file in $inputFiles;
do
outputFile="${file/.inp/.output}";
outFinal="${outputFile/inputs/$directory}";
cat $file;
cp $prog freq;
./freq $(cat $file) >$outFinal;
#printing expected files
outExpected="${outFinal/.output/.exp}";
rm ./freq;
cp sampleBinary/samplev1 freq;
./freq $(cat $file) >$outExpected;
#getting the difference
differenceFile="${prog/.out/.diff}";
touch $differenceFile;
#diff $outFinal $outExpected >> $differenceFile;
DIFFVAL=$(diff -w $outFinal $outExpected);
if [ "$DIFFVAL" != "" ] 
then
    echo "1">>$differenceFile;
fi

done;

done;
