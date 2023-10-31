clear

cd ./bin

echo "Compiling Source Files ... "
g++ -Wall -std=c++20 -o AES ../test/*.cpp ../src/CPU/*.cpp -fopenmp

cp AES ../

cd ..

./AES data/Novel.txt Novel.txt.enc

# ./AES data/Test.txt Test.txt.enc

# ./AES data/win.pdf data/ENC_FILE data/DEC_FILE

# ./AES data/Ventura.bmp data/ENC_FILE data/DEC_FILE

# ./AES data/Floor.png data/ENC_FILE data/DEC_FILE

# valgrind --tool=massif --pages-as-heap=yes ./AES data/5gb-file.dat data/ENC_FILE data/DEC_FILE