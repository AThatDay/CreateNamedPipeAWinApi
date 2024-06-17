#include <windows.h>
#include <stdio.h>

#define PIPE_NAME1 "\\\\.\\pipe\\MyNamedPipe1"
#define PIPE_NAME2 "\\\\.\\pipe\\MyNamedPipe2"
#define BUFFER_SIZE 512

void ConnectAndSendMessage(LPCSTR pipeName, const char* message);

int main() {
    // Connect to the first named pipe and send a message
    ConnectAndSendMessage(PIPE_NAME1, "Hello from Client to Pipe1!");

    // Connect to the second named pipe and send a message
    ConnectAndSendMessage(PIPE_NAME2, "Hello from Client to Pipe2!");

    return 0;
}

void ConnectAndSendMessage(LPCSTR pipeName, const char* message) {
    HANDLE hPipe;
    char buffer[BUFFER_SIZE];
    DWORD bytesRead, bytesWritten;

    // Connect to the named pipe
    hPipe = CreateFileA(
        pipeName,              // pipe name
        GENERIC_READ |         // read and write access
        GENERIC_WRITE,
        0,                     // no sharing
        NULL,                  // default security attributes
        OPEN_EXISTING,         // opens existing pipe
        0,                     // default attributes
        NULL);                 // no template file

    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Failed to connect to pipe %s, GLE=%d.\n", pipeName, GetLastError());
        return;
    }

    printf("Connected to pipe %s.\n", pipeName);

    // Write a message to the pipe
    BOOL result = WriteFile(
        hPipe,                 // handle to pipe
        message,               // buffer to write from
        strlen(message) + 1,   // number of bytes to write
        &bytesWritten,         // number of bytes written
        NULL);                 // not overlapped I/O
    printf("endwrite file\n");

    if (!result) {
        printf("WriteFile to pipe %s failed, GLE=%d.\n", pipeName, GetLastError());
        CloseHandle(hPipe);
        return;
    }

    // Read the server's response
    result = ReadFile(
        hPipe,                 // handle to pipe
        buffer,                // buffer to receive data
        BUFFER_SIZE,           // size of buffer
        &bytesRead,            // number of bytes read
        NULL);                 // not overlapped I/O

    if (!result || bytesRead == 0) {
        printf("ReadFile from pipe %s failed, GLE=%d.\n", pipeName, GetLastError());
    }
    else {
        printf("Received response: %s\n", buffer);
    }

    // Close the pipe
    CloseHandle(hPipe);
}
