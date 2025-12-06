#include <stdio.h>
#include <string.h>

#define MAX 50

struct User {
    int id;
    char name[50];
    int type;      // 1 = Premium, 2 = Normal
    int tickets;
};

// Queues
struct User premiumQ[MAX], normalQ[MAX];
int fp = 0, rp = 0;  // premium queue
int fn = 0, rn = 0;  // normal queue

int totalTickets = 20;
int receiptNo = 1;

// Add user to queues
void addPremium(struct User u) {
    premiumQ[rp++] = u;
}
void addNormal(struct User u) {
    normalQ[rn++] = u;
}

// Remove user from queues
struct User getPremium() { return premiumQ[fp++]; }
struct User getNormal() { return normalQ[fn++]; }

int premiumEmpty() { return fp == rp; }
int normalEmpty() { return fn == rn; }

// ------------------ RECEIPT FUNCTION ------------------
void saveReceipt(struct User u, int remaining) {
    FILE *fp = fopen("receipt.txt", "a");

    if (!fp) {
        printf("Error opening receipt file!\n");
        return;
    }

    fprintf(fp, "============================\n");
    fprintf(fp, "       RAILWAY E-TICKET     \n");
    fprintf(fp, "============================\n");
    fprintf(fp, "Receipt No   : %d\n", receiptNo++);
    fprintf(fp, "Name         : %s\n", u.name);
    fprintf(fp, "User ID      : %d\n", u.id);
    fprintf(fp, "Type         : %s\n", u.type == 1 ? "Premium" : "Normal");
    fprintf(fp, "Tickets Booked: %d\n", u.tickets);
    fprintf(fp, "Remaining    : %d\n", remaining);
    fprintf(fp, "============================\n\n");

    fclose(fp);
}

// ------------------ BOOKING PROCESS ------------------
void startBooking() {
    printf("\n--- BOOKING STARTED ---\n");

    while (!premiumEmpty() || !normalEmpty()) {
        struct User u;

        if (!premiumEmpty())
            u = getPremium();
        else
            u = getNormal();

        printf("\nUser %s (ID %d, %s) requests %d tickets\n",
               u.name, u.id, u.type == 1 ? "Premium" : "Normal", u.tickets);

        if (u.tickets <= totalTickets) {
            totalTickets -= u.tickets;
            printf("? BOOKED! Tickets left: %d\n", totalTickets);

            // Save receipt
            saveReceipt(u, totalTickets);
        }
        else {
            printf("? REJECTED! Not enough tickets.\n");
        }
    }

    printf("\n--- BOOKING COMPLETE ---\n");
}

int main() {
    int n, i;

    printf("Enter number of users: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        struct User u;

        printf("\nUser %d\n", i + 1);
        printf("Enter ID: ");
        scanf("%d", &u.id);

        printf("Enter Name: ");
        scanf("%s", u.name);

        printf("Type (1 Premium / 2 Normal): ");
        scanf("%d", &u.type);

        printf("Tickets required: ");
        scanf("%d", &u.tickets);

        if (u.type == 1)
            addPremium(u);
        else
            addNormal(u);
    }

    startBooking();
    return 0;
}
