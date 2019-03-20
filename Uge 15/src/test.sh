
echo "This is a test!"

echo "running make"
make


for test in ./Tests/*.test
do
    echo "running $test: "
    if ./$test; then
        echo "$test succeded"
    else
        echo "$test failed"
    fi
done


