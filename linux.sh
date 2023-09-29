clear

cd ./bin

g++ -std=c++20 -o AES ../src/CPU/*.cpp

cp AES ../

cd ..

./AES data/Test.txt data/Encrypted_Data data/Decrypted_Data

# ./AES Data/win.pdf Data/enc.pdf Data/dec.pdf

# ./AES Data/Ventura.bmp Data/encVentura.bmp Data/decVentura.bmp

# ./AES Data/Floor.png Data/EncFloor.png Data/DecFloor.png
