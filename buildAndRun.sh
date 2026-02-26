rm -rf build
mkdir build
cd build

cmake ..

make

echo ""
echo "Running exe"
./main

echo ""
echo "Running CTest"
ctest

cd ..
echo ""
echo "Done!"
