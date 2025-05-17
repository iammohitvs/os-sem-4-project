/**
 * sender.c - Program to create a message queue and send messages with different priorities
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

// Define message structure
struct message {
    long msg_type;  // Message type (used as priority)
    char msg_text[100]; // Message content
};

int main() {
    key_t key;
    int msgid;
    struct message msg;
    char str[100];

    // Generate a key for the message queue
    if ((key = ftok(".", 'A')) == -1) {
        perror("ftok error");
        exit(1);
    }

    // Create message queue with read and write permissions for all users
    if ((msgid = msgget(key, IPC_CREAT | 0666)) == -1) {
        perror("msgget error");
        exit(1);
    }

    printf("Message queue created with ID: %d\n", msgid);


    for(int i = 1; i <=3; i++) {
        printf("Enter a message:\n");
        scanf(" %[^\n]", str);
        strcpy(msg.msg_text, str);
        printf("Enter priority, between 1 and 3, do not repeat: ");
        scanf("%ld", &msg.msg_type);
        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd error for message 1");
            exit(1);
        }
        printf("Sent message: %s (Priority: %ld)\n", msg.msg_text, msg.msg_type);
    }

    printf("All messages sent successfully!\n");

    return 0;
}
