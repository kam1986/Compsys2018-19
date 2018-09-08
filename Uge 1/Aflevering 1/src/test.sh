#!/usr/bin/env bash

# Exit immediately if any command below fails.
#set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -r test_files/*


echo "Generating test files.."
printf "Hello, World!\n" > test_files/ascii.input
printf "Hello, World!" > test_files/ascii2.input
printf "Hello,\x00World!\n" > test_files/data.input
printf "" > test_files/empty.input
### new tests ###
printf "s" > test_files/shortfile.input             # testing 1-byte long ascii file 
printf "\x00" > test_files/shortdatafile.input      # testing 1-byte lone data file
printf "a\x0d" > test_files/withCRnoLF.input        # testing carraige return
printf "a\x07" > test_files/testlowerboarder.input  # testing lowerboarder
printf "aa\x08" > test_files/within8to13.input      # testing lowerboarder
printf "aa\x1b" > test_files/withX1b.input          # testing escape sequence
printf "\x20A" > test_files/0X20.input              # testing boarder
printf "\x1Fa" > test_files/0X1F.input              # testing boarder
printf "\x7Es" > test_files/0X7E.input              # testing boarder
printf "\x7Fe" > test_files/0X7F.input              # testing boarder
printf "hemmelighed" > test_files/hemmelig.input    # testing none-readable files.
chmod -r test_files/hemmelig




echo "Running the tests.."
exitcode=0
for f in test_files/*.input
do
  echo ">>> Testing ${f}.."
  file    "${f}" > "${f}.expected"
  ./file  "${f}" > "${f}.actual"

  if ! diff -u "${f}.expected" "${f}.actual"
  then
    echo ">>> Failed :-("
    exitcode=1
  else
    echo ">>> Success :-)"
  fi
done
 
echo "Testing none-exiting file"

./file No_file > test_files/No_file.actual
file No_file > test_files/No_file.expected

if ! diff -u test_files/No_file.expected test_files/No_file.actual
then
  echo ">> Failed :-("
  exitcode=1
else
  echo ">> Success :-)"
fi

exit $exitcode