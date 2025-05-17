/**
 
receiver.c - Program to receive messages from the message queue created by sender.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

// Define message structure - must match the sender's structure
struct message {
    long msg_type;  // Message type (used as priority)
    char msg_text[100]; // Message content
};

int main() {
    key_t key;
    int msgid;
    struct message msg;

    // Generate the same key used by the sender
    if ((key = ftok(".", 'A')) == -1) {
        perror("ftok error");
        exit(1);
    }

    // Access the existing message queue
    if ((msgid = msgget(key, 0666)) == -1) {
        perror("msgget error");
        exit(1);
    }

    printf("Accessing message queue with ID: %d\n", msgid);
    printf("Receiving messages...\n\n");

    // Receive and display each message from the queue
    // Use 0 as msg_type to receive messages in FIFO order
    for (int i = 1; i <= 3; i++) {
        // Receive any message (0 means any type)
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), i, 0) == -1) {
            perror("msgrcv error");
            exit(1);
        }

        printf("Received message: %s (Priority: %ld)\n", msg.msg_text, msg.msg_type);
    }

    // Optional: Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl error");
        exit(1);
    }

    printf("\nAll messages received. Message queue removed.\n");

    return 0;
}