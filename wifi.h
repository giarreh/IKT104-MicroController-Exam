#include "mbed.h"

nsapi_size_or_error_t send_request(TLSSocket *socket, const char *request) {
    nsapi_size_t bytes_to_send = strlen(request);
    nsapi_size_or_error_t bytes_sent = 0;

    printf("Sending message: \n%s", request);

    while (bytes_to_send) {
        bytes_sent = socket->send(request + bytes_sent, bytes_to_send);

        if (bytes_sent < 0) {
            return bytes_sent;
        } else {
            printf("sent %d bytes\n", bytes_sent);
        }

        bytes_to_send -= bytes_sent;
    }

    printf("Complete message sent\n");

    return 0;
}

nsapi_size_or_error_t read_response(TLSSocket *socket, char buffer[], int buffer_length) {
    memset(buffer, 0, buffer_length);

    int remaining_bytes = 4000;
    int received_bytes = 0;

    while (remaining_bytes > 0) {
        nsapi_size_or_error_t result = socket->recv(buffer + received_bytes, remaining_bytes);

        if (result == 0) {
            break;
        }

        if (result < 0) {
            return result;
        }

        received_bytes += result;
        remaining_bytes -= result;
    }

    printf("received %d bytes:\n%.*s\n", received_bytes,
                 strstr(buffer, "\n") - buffer, buffer);

        
        
    return 0;
}


nsapi_size_or_error_t send_request_tcp(TCPSocket *socket, const char *request) {
    nsapi_size_t bytes_to_send = strlen(request);
    nsapi_size_or_error_t bytes_sent = 0;

    printf("Sending message: \n%s", request);

    while (bytes_to_send) {
        bytes_sent = socket->send(request + bytes_sent, bytes_to_send);

        if (bytes_sent < 0) {
            return bytes_sent;
        } else {
            printf("sent %d bytes\n", bytes_sent);
        }

        bytes_to_send -= bytes_sent;
    }

    printf("Complete message sent\n");

    return 0;
}

nsapi_size_or_error_t read_response_tcp(TCPSocket *socket, char buffer[], int buffer_length) {
    memset(buffer, 0, buffer_length);

    int remaining_bytes = 4000;
    int received_bytes = 0;

    while (remaining_bytes > 0) {
        nsapi_size_or_error_t result = socket->recv(buffer + received_bytes, remaining_bytes);

        if (result == 0) {
            break;
        }

        if (result < 0) {
            return result;
        }

        received_bytes += result;
        remaining_bytes -= result;
    }

    printf("received %d bytes:\n%.*s\n", received_bytes,
                 strstr(buffer, "\n") - buffer, buffer);



    return 0;
}
