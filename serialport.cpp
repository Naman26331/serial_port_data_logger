#include <iostream>
#include <fstream>
#include <string>
#include <windows.h> 

int main() {
    // Open the File for Logging
    std::ofstream logFile("sensor_data.csv", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
        return 1;
    }
    // Open the Serial Port 
    HANDLE hSerial = CreateFileA("COM4", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error connecting to port!" << std::endl;
        return 1;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    char buffer[256];
    DWORD bytesRead;
    std::cout << "Listening for data... Press Ctrl+C to stop." << std::endl;

    while (true) {
        // Read data from the serial port into the buffer
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
            buffer[bytesRead] = '\0'; 
            
            // Output to console and log to file
            std::cout << buffer; 
            logFile << buffer;
            logFile.flush(); 
        }
    }

    CloseHandle(hSerial);
    logFile.close();
    return 0;
}