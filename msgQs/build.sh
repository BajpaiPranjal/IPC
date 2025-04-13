#!/bin/bash

rm client server 

g++ aSyncClient.cpp -o client
g++ server.cpp -o server

echo "Build complete!"
