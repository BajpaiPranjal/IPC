rm server 
rm client

# g++ aSyncClient.cpp -o client
g++ server.cpp -o server
g++ client.cpp -o client
# echo "Build complete!"

# ./server
