#!/bin/bash

# Exit immediately if any command below fails.
set -e

algorithm=yoursort
# Make sure that the latest version of the implementation
make ${algorithm}
# Quicksort is needed for reference.
make quicksort

# The command with which you run x86prime: You should likely change this variable
X86PRIME=x86prime

echo "Generating a test_runs directory.."
mkdir -p test_runs
rm -f test_runs/*

echo "Running the tests.."
exitcode=0
for f in tests/*; do
  fname=${f#"tests/"}
  # Number of lines
  lines=$(wc -l < ${f})
  echo ">>> Testing ${fname}.."
  # Generate test-file for input
  cont=$(< ${f})
  # Generate test data
  printf "3\n${lines}\n${cont}\n" > test_runs/${fname}.input

  ${X86PRIME} -f ${algorithm}.s_prime -asm -run run < test_runs/${fname}.input > test_runs/${fname}.output
  cat test_runs/${fname}.output | head -n1 > test_runs/${fname}.result
  ${X86PRIME} -f quicksort.s_prime -asm -run run < test_runs/${fname}.input > test_runs/${fname}.qs_output
  cat test_runs/${fname}.qs_output | head -n1 > test_runs/${fname}.expected

  if ! diff -u test_runs/${fname}.expected test_runs/${fname}.result
  then
    echo ">>> Failed :-("
    exitcode=1
  else
    echo ">>> Success :-)"
  fi
done

exit $exitcode
