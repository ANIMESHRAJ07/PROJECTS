#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define MAX_ACCOUNTS 115
#define TRANSACTION_HISTORY_FILE "transactions.txt"
#define ACCOUNTS_FILE "accounts.txt"

// Function prototypes
void createAccount();
void viewAccount(unsigned long long accountNumber);
void transferFunds(unsigned long long fromAccount, unsigned long long toAccount, float amount);
unsigned long long generate_random_16_digit_number();
void generatePassword(char *password, size_t length);
void calculateInterest(int index, int years);
void logTransaction(int index, float amount, const char *type);
void viewTransactionHistory(int index);
int verifyPassword(int index);
void loadAccountsFromFile();
void saveAccountsToFile();
void loadTransactionsFromFile();
void saveTransactionsToFile();
int count = 15;
float interestRate = 0.05;

struct Transaction {
    float amount;
    char type[10];
    char dateTime[20];
};

struct fastpay {
    unsigned long long accountNumber;
    char UserName[50];
    char Name[100];
    char password[20];
    float balance;
    struct Transaction history[100];
    int transactionCount;
} on[MAX_ACCOUNTS] = {
    {1001, "user1", "Alice Johnson", "password1", 1000.0, {}, 0},
    {1002, "user2", "Bob Smith", "password2", 1500.0, {}, 0},
    {1003, "user3", "Charlie Brown", "password3", 2000.0, {}, 0},
    {1004, "user4", "Diana Prince", "password4", 2500.0, {}, 0},
    {1005, "user5", "Evan Davis", "password5", 3000.0, {}, 0},
    {1006, "user6", "Fiona Lewis", "password6", 3500.0, {}, 0},
    {1007, "user7", "George Miller", "password7", 4000.0, {}, 0},
    {1008, "user8", "Hannah White", "password8", 4500.0, {}, 0},
    {1009, "user9", "Ian Clark", "password9", 5000.0, {}, 0},
    {1010, "user10", "Jack Wilson", "password10", 5500.0, {}, 0},
    {1011, "user11", "Karen Green", "password11", 6000.0, {}, 0},
    {1012, "user12", "Liam King", "password12", 6500.0, {}, 0},
    {1013, "user13", "Mia Adams", "password13", 7000.0, {}, 0},
    {1014, "user14", "Noah Thompson", "password14", 7500.0, {}, 0},
    {1015, "user15", "Olivia Hall", "password15", 8000.0, {}, 0}
};

// Function to log a transaction with date and time
void logTransaction(int index, float amount, const char *type) {
    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);

    struct Transaction *trans = &on[index].history[on[index].transactionCount++];
    trans->amount = amount;
    strcpy(trans->type, type);
    snprintf(trans->dateTime, sizeof(trans->dateTime), "%02d-%02d-%d %02d:%02d:%02d",
             localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900,
             localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
    
    // Save transaction to file
    saveTransactionsToFile();
}

// Function to view transaction history
void viewTransactionHistory(int index) {
    printf("\nTransaction History for Account: %llu\n", on[index].accountNumber);
    for (int i = 0; i < on[index].transactionCount; i++) {
        printf("%s - %s: $%.2f\n", 
               on[index].history[i].dateTime, 
               on[index].history[i].type, 
               on[index].history[i].amount);
    }
}

// Function to verify password
int verifyPassword(int index) {
    char enteredPassword[20];
    printf("Enter password: ");
    scanf("%s", enteredPassword);

    if (strcmp(on[index].password, enteredPassword) == 0) {
        return 1;
    } else {
        printf("Incorrect password.\n");
        return 0;
    }
}

// Function to calculate interest
void calculateInterest(int index, int years) {
    float newBalance = on[index].balance * pow(1 + interestRate, years);
    printf("Balance after %d years at %.2f%% interest: $%.2f\n", years, interestRate * 100, newBalance);
}

// Generate a random 16-digit account number
unsigned long long generate_random_16_digit_number() {
    unsigned long long number = 1000000000000000 + rand() % 9000000000000000;
    return number;
}

// Generate a random password of given length
void generatePassword(char *password, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length - 1; i++) {
        password[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    password[length - 1] = '\0';
}

// Modify viewAccount to include viewing transaction history
void viewAccount(unsigned long long accountNumber) {
    for (int i = 0; i < count; i++) {
        if (on[i].accountNumber == accountNumber) {
            if (verifyPassword(i)) {
                printf("\nAccount Details:\n");
                printf("Account Number: %llu\n", on[i].accountNumber);
                printf("Account Holder Name: %s\n", on[i].Name);
                printf("Username: %s\n", on[i].UserName);
                printf("Balance: $%.2f\n", on[i].balance);

                int years;
                printf("Enter the number of years to calculate interest: ");
                scanf("%d", &years);
                calculateInterest(i, years);

                printf("Would you like to view your transaction history? (y/n): ");
                char ch;
                scanf(" %c", &ch);
                if (ch == 'y' || ch == 'Y') {
                    viewTransactionHistory(i);
                }
            }
            return;
        }
    }
    printf("Account number %llu not found.\n", accountNumber);
}

// Function to create a new account
void createAccount() {
    if (count >= MAX_ACCOUNTS) {
        printf("Cannot create more accounts.\n");
        return;
    }

    printf("Enter account holder's name: ");
    getchar();
    fgets(on[count].Name, sizeof(on[count].Name), stdin);
    on[count].Name[strcspn(on[count].Name, "\n")] = '\0';

    on[count].accountNumber = generate_random_16_digit_number();
    snprintf(on[count].UserName, sizeof(on[count].UserName), "user%d", count + 1);
    generatePassword(on[count].password, 8);
    on[count].balance = 0.0;
    on[count].transactionCount = 0;

    // Ask for an initial deposit
    printf("Enter an initial deposit amount: $");
    float deposit;
    scanf("%f", &deposit);
    if (deposit < 0) {
        printf("Invalid deposit amount.\n");
        return;
    }
    on[count].balance = deposit;

    printf("Account created successfully!\n");
    printf("Account Number: %llu\n", on[count].accountNumber);
    printf("Generated Password: %s\n", on[count].password);
    printf("Initial Deposit: $%.2f\n", deposit);

    count++;

    // Save accounts to file
    saveAccountsToFile();
}

// Function to transfer funds with password verification
void transferFunds(unsigned long long fromAccount, unsigned long long toAccount, float amount) {
    int fromIndex = -1, toIndex = -1;

    for (int i = 0; i < count; i++) {
        if (on[i].accountNumber == fromAccount) fromIndex = i;
        if (on[i].accountNumber == toAccount) toIndex = i;
    }

    if (fromIndex == -1 || toIndex == -1) {
        printf("Account not found.\n");
        return;
    }

    if (!verifyPassword(fromIndex)) return;

    if (on[fromIndex].balance < amount) {
        printf("Insufficient balance in account number %llu.\n", fromAccount);
        return;
    }

    on[fromIndex].balance -= amount;
    on[toIndex].balance += amount;
    printf("Successfully transferred $%.2f from account %llu to account %llu.\n", amount, fromAccount, toAccount);

    logTransaction(fromIndex, amount, "Debit");
    logTransaction(toIndex, amount, "Credit");

    // Save updated accounts and transactions to file
    saveAccountsToFile();
    saveTransactionsToFile();
}

// Function to load accounts from the file
void loadAccountsFromFile() {
    FILE *file = fopen(ACCOUNTS_FILE, "r");
    if (file) {
        while (fscanf(file, "%llu %s %s %s %f %d", 
                      &on[count].accountNumber, 
                      on[count].UserName, 
                      on[count].Name, 
                      on[count].password, 
                      &on[count].balance, 
                      &on[count].transactionCount) == 6) {
            for (int i = 0; i < on[count].transactionCount; i++) {
                fscanf(file, "%s %f %s", 
                       on[count].history[i].dateTime, 
                       &on[count].history[i].amount, 
                       on[count].history[i].type);
            }
            count++;
        }
        fclose(file);
    }
}

// Function to save accounts to the file
void saveAccountsToFile() {
    FILE *file = fopen(ACCOUNTS_FILE, "w");
    if (!file) {
        printf("Error saving accounts to file.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%llu %s %s %s %.2f %d\n", 
                on[i].accountNumber, 
                on[i].UserName, 
                on[i].Name, 
                on[i].password, 
                on[i].balance, 
                on[i].transactionCount);
        for (int j = 0; j < on[i].transactionCount; j++) {
            fprintf(file, "%s %.2f %s\n", 
                    on[i].history[j].dateTime, 
                    on[i].history[j].amount, 
                    on[i].history[j].type);
        }
    }
    fclose(file);
}

// Function to load transactions from the file
void loadTransactionsFromFile() {
    FILE *file = fopen(TRANSACTION_HISTORY_FILE, "r");
    if (file) {
        while (fscanf(file, "%s %f %s", 
                      on[count].history[on[count].transactionCount].dateTime, 
                      &on[count].history[on[count].transactionCount].amount, 
                      on[count].history[on[count].transactionCount].type) == 3) {
            on[count].transactionCount++;
        }
        fclose(file);
    }
}

// Function to save transactions to the file
void saveTransactionsToFile() {
    FILE *file = fopen(TRANSACTION_HISTORY_FILE, "w");
    if (!file) {
        printf("Error saving transactions to file.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < on[i].transactionCount; j++) {
            fprintf(file, "%s %.2f %s\n", 
                    on[i].history[j].dateTime, 
                    on[i].history[j].amount, 
                    on[i].history[j].type);
        }
    }
    fclose(file);
}

// Main function
int main() {
    srand(time(0));

    // Load accounts and transactions from files
    loadAccountsFromFile();
    loadTransactionsFromFile();

    int choice;
    unsigned long long accountNumber;
    unsigned long long fromAccount, toAccount;
    float amount;

    while (1) {
        printf("\nOnline Banking System\n");
        printf("1. Create Account\n");
        printf("2. View Account\n");
        printf("3. Transfer Funds\n");
        printf("4. View Transaction History\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                printf("Enter account number to view: ");
                scanf("%llu", &accountNumber);
                viewAccount(accountNumber);
                break;
            case 3:
                printf("Enter account number to transfer from: ");
                scanf("%llu", &fromAccount);
                printf("Enter account number to transfer to: ");
                scanf("%llu", &toAccount);
                printf("Enter amount to transfer: ");
                scanf("%f", &amount);
                transferFunds(fromAccount, toAccount, amount);
                break;
            case 4:
                printf("Enter account number to view transaction history: ");
                scanf("%llu", &accountNumber);
                for (int i = 0; i < count; i++) {
                    if (on[i].accountNumber == accountNumber) {
                        if (verifyPassword(i)) {
                            viewTransactionHistory(i);
                        }
                        break;
                    }
                }
                break;
            case 5:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
