import socket

# Create a TCP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to an address and port
server_socket.bind(('127.0.0.1', 12345))

# Listen for incoming connections (max 1 client)
server_socket.listen(1)
print("TCP Server is waiting for connections...")

# Accept a client connection
client_socket, client_address = server_socket.accept()
print(f"Connected to {client_address}")

while True:
    # Receive data from the client
    data = client_socket.recv(1024).decode()
    
    # Check for exit condition
    if data.lower() == "exit":
        print("Client disconnected.")
        break
    
    # Process the received message
    char_count = len(data)
    print(f"Received from client: {data} (Characters: {char_count})")
    
    # Send response to the client
    response = f"{char_count} - {data.upper()}"
    client_socket.send(response.encode())

# Close the connection
client_socket.close()
server_socket.close()

