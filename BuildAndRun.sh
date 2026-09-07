mkdir "Builds"

g++ main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o ./Builds/OrbSim.out &&
./Builds/OrbSim.out