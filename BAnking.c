#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define MAX_ACCOUNTS 115
#define TRANSACTION_HISTORY_FILE "transactions.txt"
#define ACCOUNTS_FILE "accounts.txt"


// Function prototypes for different modules
void viewAllStocks();
void sortStocksByPrice();
void searchForStock();
void convertStockPrice();
void accountCreation();
void loanManagement();
void onlineBanking();
void marketExploration();
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


struct Transaction {
    float amount;
    char type[10];
    char dateTime[20];
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

    srand(time(0));

    // Load accounts and transactions from files
    loadAccountsFromFile();
    loadTransactionsFromFile();

    // int cho;
    // unsigned long long accountNumber;
    // unsigned long long fromAccount, toAccount;
    // float amount;

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
                scanf("%f", &amount1);
                transferFunds(fromAccount, toAccount, amount1);
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

int main() {
    int ch;

    while (1) {
        printf("==========================================================\n");
        printf("|                     Main Menu                          |\n");
        printf("==========================================================\n");
        printf("1) Account Creation\n");
        printf("2) Loan Management\n");
        printf("3) Online Banking\n");
        printf("4) Market Exploration\n");
        printf("5) Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                accountCreation();
                break;
            case 2:
                loanManagement();
                break;
            case 3:
                onlineBanking();
                break;
            case 4:
                marketExploration();
                break;
            case 5:
                printf("Exiting the program. Thank you!\n");
                return 0;
            default:
                printf("Invalid option! Please enter a valid choice.\n");
        }

        printf("\nPress Enter to return to the main menu...");
        getchar(); // To capture the newline after choice input
        getchar(); // To wait for user to press Enter
        printf("\n\n\n");
    }
}

// Placeholder for Account Creation module
void accountCreation() 
{
    struct BankAccount {
        char name[50];             
        char accountnumber[20];
        char email[50];            
        float balance;             
    };
    
    struct BankAccount account; 
    account.balance = 10000.0;  
    printf("We are excited to have you onboard! To get started, please create an account.\n\n");

    for (int i = 0; i < 120; i++) printf("-");
    printf("\n|                                           Account Creation Menu                                                      |\n");
    for (int i = 0; i < 120; i++) printf("-");
    printf("\n\n");

    printf("\nEnter your full name: ");
    scanf("%49s", account.name); 

    printf("\nEnter your account number: ");
    scanf("%19s", account.accountnumber);

    int ValidEmail(char email[]) {
        int atSymbol= 0, dot = 0;
        for (int i = 0; i < strlen(email); i++) {
            if (email[i] == '@') {
                atSymbol= 1;
            }
            if (atSymbol && email[i] == '.') {
                dot= 1;
                break;
            }
        }
        return atSymbol && dot;
    }

    int emailValid = 0;
    while (!emailValid) {
        printf("\nEnter your email: ");
        scanf("%49s", account.email); 
        
        if (ValidEmail(account.email)) {
            emailValid = 1;
        } else {
            printf("Invalid email format. Please include '@' and '.' symbols in the correct order.\n");
        }
    }

    printf("\nAccount created successfully. Here are your details:\n");
    printf("Name: %s\n", account.name);
    printf("Account Number: %s\n", account.accountnumber);
    printf("Email: %s\n", account.email);

    int choice;
    float amount;
    do {
        printf("\n--- Transaction Menu ---\n");
        printf("1. Deposit Amount\n");
        printf("2. Withdraw Amount\n");
        printf("3. Check Balance\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nEnter the amount you want to deposit: ");
                scanf("%f", &amount);

                if (amount > 0 && amount <= 100000) {
                    account.balance += amount;
                    printf("Amount successfully deposited!\n");
                } else {
                    printf("Invalid deposit amount. Please enter a value between 0 and 100,000.\n");
                }
                break;

            case 2:
                printf("\nEnter the amount you want to withdraw: ");
                scanf("%f", &amount);

                if (amount > 0 && amount <= account.balance) {
                    account.balance -= amount;
                    printf("Amount successfully withdrawn!\n");
                } else if (amount > account.balance) {
                    printf("Insufficient funds.\n");
                } else {
                    printf("Invalid withdrawal amount.\n");
                }
                break;

            case 3:
                printf("Current Balance: $%.2f\n", account.balance);
                break;

            case 4:
                printf("Thank you for using our services!\n");
                break;

            default:
                printf("Invalid choice. Please enter a number from 1 to 4.\n");
        }
    } while (choice != 4);
}


// Placeholder for Loan Management module
void loanManagement()
{
     int option;
    float principal, tenure, rate, emi;

mainMenu:
    printf("Hello and Welcome to the Loan Management Section of our Bank");
    printf("\nTo continue press:-\n 1. Explore EMI Options \n 2. Loan Eligibility \n 3. Exit\n");
    scanf("%d", &option);

    switch (option) {
    case 1:
        printf("Hey, Thanks for exploring the EMI Option\n");

    l2:
        printf("Enter the amount you want in EMI\nNOTE!:-We offer EMI on amounts starting from INR 50,000 and up to INR 10,00,000.\n");
        scanf("%f", &principal);
        printf("Enter Tenure of your EMI\nNOTE!:-We offer EMI only on 12 and 24 months Tenure\n");
        scanf("%f", &tenure);

        if (principal >= 50000 && principal <= 100000) {
            if (tenure == 12) {
                rate = 8;
            } else if (tenure == 24) {
                rate = 10;
            } else {
                printf("Please enter correct data\n");
                goto l2;
            }
        } else if (principal > 100000 && principal <= 500000) {
            if (tenure == 12) {
                rate = 10;
            } else if (tenure == 24) {
                rate = 12;
            } else {
                printf("Please enter correct data\n");
                goto l2;
            }
        } else if (principal > 500000 && principal <= 1000000) {
            if (tenure == 12) {
                rate = 9;
            } else if (tenure == 24) {
                rate = 10;
            } else {
                printf("Please enter correct data\n");
                goto l2;
            }
        } else {
            printf("Please enter a valid amount, your asking price is beyond our policy\n");
            goto l2;
        }

        float monthlyRate = rate / (12 * 100);
        emi = (principal * monthlyRate * pow(1 + monthlyRate, tenure)) / (pow(1 + monthlyRate, tenure) - 1);
        printf("Your Calculated EMI is: INR%.2f\n", emi);
        goto mainMenu;

    case 2: {
        float exemi, income, expenses, eligibility;
        printf("Enter the amount you want as Loan: ");
        scanf("%f", &principal);
        printf("Enter your monthly income: ");
        scanf("%f", &income);
        printf("Enter your monthly expenses: ");
        scanf("%f", &expenses);
        printf("Enter your current EMI expenses (if any): ");
        scanf("%f", &exemi);

        eligibility = income - (expenses + exemi);
        if (eligibility > principal) {
            printf("You are eligible for the loan.\n");
        } else {
            printf("You are ineligible for the loan.\n");
        }
        goto mainMenu;
    }

    case 3:
        printf("Thank you for using the Loan Management System. Goodbye!\n");
        break;

    default:
        printf("Invalid option. Please try again.\n");
        goto mainMenu;
    }
}


// Placeholder for Online Banking module
void onlineBanking() 
{   

    
    srand(time(0));

    // Load accounts and transactions from files
    loadAccountsFromFile();
    loadTransactionsFromFile();

    int choice;
    unsigned long long accountNumber;
    unsigned long long fromAccount, toAccount;
    float amount1;

    while (1) 
    {
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
                scanf("%f", &amount1);
                transferFunds(fromAccount, toAccount, amount1);
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
}
    


// Placeholder for Market Exploration module
void marketExploration()
 {
  
    
void marketExploration() {
    int i;
    while (1) {
        for (i = 1; i <= 120; i++) printf("-");
        printf("\n|");
        printf("                                          Market Exploration Menu                                                     |");
        printf("\n");
        for (i = 1; i <= 120; i++) printf("-");
        printf("\n");

        printf("1) View All Stocks \n2) Sort stocks By Price \n3) Search For Stocks\n4) Convert Stock prices to another country \n5) Exit This menu \n ");
        
        printf("\nKindly Enter the option to continue with: ");
    
        int choice;
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
            case 1:
                viewAllStocks();
                break;
            case 2:
                sortStocksByPrice();
                break;
            case 3:
                searchForStock();
                break;
            case 4:
                convertStockPrice();
                break;
            case 5:
                printf("\nReturning to the main menu...\n");
                return;
            default:
                printf("\nInvalid option! Please try again.\n");
        }

        printf("Press Enter to return to the main menu: ");
        getchar();
        getchar();
        printf("\n \n \n\n\n\n");
    }
}

void viewAllStocks() {
    int i;
    for (i = 1; i <= 120; i++) printf("-");
    printf("\n|");
    printf("                                          Viewing All Stocks                                                          |");
    printf("\n");
    for (i = 1; i <= 120; i++) printf("-");
    printf("\n");

    printf("| %-10s | %-25s | %-10s | %-18s |\n", "Symbol", "Stock Name", "Price(USD)", "Market Cap (M USD)");

    for (i = 0; i < 76; i++) printf("-");
    printf("\n");

    printf("| %-10s | %-25s | %-10s | %-18s |\n", "AAPL", "Apple Inc.", "$145.32", "2410000");
    printf("| %-10s | %-25s | %-10s | %-18s |\n", "TSLA", "Tesla Inc.", "$720.50", "720000");
    printf("| %-10s | %-25s | %-10s | %-18s |\n", "MSFT", "Microsoft Corp.", "$299.72", "2250000");
    printf("| %-10s | %-25s | %-10s | %-18s |\n", "AMZN", "Amazon.com Inc.", "$3415.25", "1700000");
    printf("| %-10s | %-25s | %-10s | %-18s |\n", "GOOGL", "Google LLC", "$2729.34", "1800000");

    for (i = 0; i < 76; i++) printf("-");
    printf("\n");
}

void sortStocksByPrice() {
    // Placeholder: Sorting logic can be implemented here
    printf("Sort function is currently a placeholder.\n");
}

void searchForStock() {
    char stockSymbol[10];
    printf("Enter the stock symbol to search (e.g., TSLA, AAPL, MSFT, AMZN): ");
    scanf("%s", stockSymbol);

    if (strcmp(stockSymbol, "TSLA") == 0) {
        printf("\n|                Tesla, Inc. (TSLA)               |\n");
        printf("Tesla, Inc. (TSLA) Performance Overview: ...");
        // Additional details for Tesla
    } else if (strcmp(stockSymbol, "AAPL") == 0) {
        printf("\n|                Apple Inc. (AAPL)               |\n");
        printf("Apple Inc. (AAPL) Performance Overview: ...");
        // Additional details for Apple
    } else if (strcmp(stockSymbol, "MSFT") == 0) {
        printf("\n|           Microsoft Corporation (MSFT)         |\n");
        printf("Microsoft Corporation (MSFT) Performance Overview: ...");
        // Additional details for Microsoft
    } else if (strcmp(stockSymbol, "AMZN") == 0) {
        printf("\n|              Amazon.com, Inc. (AMZN)           |\n");
        printf("Amazon.com, Inc. (AMZN) Performance Overview: ...");
        // Additional details for Amazon
    } else {
        printf("Stock symbol '%s' not recognized. Please try again.\n", stockSymbol);
    }
}

void convertStockPrice() {
    float priceInUSD, convertedPrice;
    char currency[4];
    int i, found = 0;

    const char *countries[25] = {"United States", "Eurozone", "Japan", "China", "India",
                                 "United Kingdom", "Brazil", "Canada", "Russia", "South Korea",
                                 "Australia", "Mexico", "India", "Netherlands", "Saudi Arabia",
                                 "Turkey", "Switzerland", "Taiwan", "Sweden", "Poland",
                                 "Thailand", "Argentina", "Nigeria", "South Africa", "Philippines"};

    const char *currencyCodes[25] = {"USD", "EUR", "JPY", "CNY", "INR",
                                     "GBP", "BRL", "CAD", "RUB", "KRW",
                                     "AUD", "MXN", "INR", "EUR", "SAR",
                                     "TRY", "CHF", "TWD", "SEK", "PLN",
                                     "THB", "ARS", "NGN", "ZAR", "PHP"};

    float exchangeRates[25] = {1.0, 0.92, 145.0, 7.1, 83.5,
                               0.81, 5.3, 1.36, 65.0, 1200.0,
                               1.5, 20.5, 83.5, 0.92, 3.75,
                               27.0, 0.92, 31.0, 11.0, 4.5,
                               35.0, 350.0, 770.0, 18.5, 56.5};

    printf("Enter the stock price in USD: ");
    scanf("%f", &priceInUSD);

    printf("\nAvailable currencies for conversion:\n");
    printf("------------------------------------------------------------\n");
    printf("| %-20s | %-10s |\n", "Country", "Currency Code");
    printf("------------------------------------------------------------\n");

    for (i = 0; i < 25; i++) {
        printf("| %-20s | %-10s |\n", countries[i], currencyCodes[i]);
    }
    printf("------------------------------------------------------------\n");

    printf("\nEnter the target currency code (e.g., EUR, INR, GBP): ");
    scanf("%s", currency);

    for (i = 0; i < 25; i++) {
        if (strcmp(currency, currencyCodes[i]) == 0) {
            convertedPrice = priceInUSD * exchangeRates[i];
            printf("\nStock price: %.2f USD\n", priceInUSD);
            printf("Converted price in %s (%s): %.2f\n", currency, countries[i], convertedPrice);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Currency code '%s' not recognized. Please try again.\n", currency);
    }
}
  
}
