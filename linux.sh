clear

cd ./bin

echo "Compiling Source Files ... "
g++ -Wall -std=c++20 -o AES ../test/*.cpp ../src/Common/*.cpp ../src/CPU/*.cpp 

cp AES ../

cd ..

./AES data/Novel.txt Novel.txt.enc

# ./AES data/win.pdf win.pdf.enc 

# ./AES data/Sample.txt Sample.txt.enc
                                                                  
# ./AES data/win.pdf data/ENC_FILE data/DEC_FILE

# ./AES data/Ventura.bmp Ventura.bmp.enc decVentura.bmp

# ./AES data/Floor.png Floor.png.enc decFloor.png

# valgrind --tool=massif --pages-as-heap=yes ./AES data/Test.txt Test.txt.enc