g++ -g -c Order.cpp -o Order.o
g++ -g -c OpenOrder.cpp -o OpenOrder.o
g++ -g -c ClosedOrder.cpp -o ClosedOrder.o
g++ -g -c VectorOrder.cpp -o VectorOrder.o

g++ -g -c main.cpp -o main.o
g++ -g Order.o OpenOrder.o ClosedOrder.o VectorOrder.o main.o -o my_program
