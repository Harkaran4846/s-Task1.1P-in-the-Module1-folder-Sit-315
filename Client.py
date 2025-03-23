import socket

# Create a TCP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the server
client_socket.connect(('127.0.0.1', 12345))
print("Connected to the server. Type 'exit' to quit.")

while True:
    # Get user input
    message = input("Enter message: ")
    
    # Send message to the server
    client_socket.send(message.encode())

    # Check for exit condition
    if message.lower() == "exit":
        break
    
    # Receive response from the server
    response = client_socket.recv(1024).decode()
    print(f"Received from server: {response}")

# Close the connection
client_socket.close()
