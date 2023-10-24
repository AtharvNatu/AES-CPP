clear

cd ./bin

g++ -std=c++20 -o AES ../test/*.cpp ../src/CPU/*.cpp 

cp AES ../

cd ..

./AES data/Test.txt data/ENC_FILE data/DEC_FILE

# ./AES data/win.pdf data/ENC_FILE data/DEC_FILE

# ./AES data/Ventura.bmp data/ENC_FILE data/DEC_FILE

# ./AES data/Floor.png data/ENC_FILE data/DEC_FILE

# valgrind --tool=massif --pages-as-heap=yes ./AES data/5gb-file.dat data/ENC_FILE data/DEC_FILE