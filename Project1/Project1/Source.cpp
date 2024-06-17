#include <windows.h>
#include <stdio.h>

#define PIPE_NAME1 "\\\\.\\pipe\\MyNamedPipe1"
#define PIPE_NAME2 "\\\\.\\pipe\\MyNamedPipe2"
#define BUFFER_SIZE 512

void CreateAndHandlePipe(LPCSTR pipeName);

int main() {
    HANDLE hPipe1, hPipe2;

    // Create two named pipes
    CreateAndHandlePipe(PIPE_NAME1);
    CreateAndHandlePipe(PIPE_NAME2);

    return 0;
}

void CreateAndHandlePipe(LPCSTR pipeName) {
    HANDLE hPipe;
    char buffer[BUFFER_SIZE];
    DWORD bytesRead, bytesWritten;

    // Create a named pipe
    hPipe = CreateNamedPipeA(
        pipeName,               // pipe name
        PIPE_ACCESS_DUPLEX,     // read/write access
        PIPE_TYPE_MESSAGE |     // message type pipe
        PIPE_READMODE_MESSAGE | // message-read mode
        PIPE_WAIT,              // blocking mode
        1,                      // max. instances
        BUFFER_SIZE,            // output buffer size
        BUFFER_SIZE,            // input buffer size
        0,                      // client time-out
        NULL);                  // default security attribute

    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("CreateNamedPipe failed for %s, GLE=%d.\n", pipeName, GetLastError());
        return;
    }

    printf("Named pipe %s created successfully.\n", pipeName);

    // Wait for the client to connect
    BOOL connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
    printf("Client connected to .\n");

    if (connected) {
        printf("Client connected to %s.\n", pipeName);

        // Read and write messages to and from the client
        while (1) {
            BOOL result = ReadFile(
                hPipe,          // handle to pipe
                buffer,         // buffer to receive data
                BUFFER_SIZE,    // size of buffer
                &bytesRead,     // number of bytes read
                NULL);          // not overlapped I/O

            if (!result || bytesRead == 0) {
                if (GetLastError() == ERROR_BROKEN_PIPE) {
                    printf("Client disconnected from %s.\n", pipeName);
                }
                else {
                    printf("ReadFile failed, GLE=%d.\n", GetLastError());
                }
                break;
            }

            // Process the received message (here we simply echo it back)
            printf("Received message: %s\n", buffer);

            // Write the response to the pipe
            result = WriteFile(
                hPipe,          // handle to pipe
                buffer,         // buffer to write from
                bytesRead,      // number of bytes to write
                &bytesWritten,  // number of bytes written
                NULL);          // not overlapped I/O

            if (!result || bytesRead != bytesWritten) {
                printf("WriteFile failed, GLE=%d.\n", GetLastError());
                break;
            }
        }
    }
    else {
        printf("ConnectNamedPipe failed, GLE=%d.\n", GetLastError());
    }

    // Close the pipe
    CloseHandle(hPipe);
}
