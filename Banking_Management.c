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

struct BankAccount {
    char name[50];             
    char accountnumber[20];
    char email[50];            
    float balance;             
};

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

//animesh
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
//animesh

//shashwat 

int ValidEmail(char email[]) 
 {
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
//shashwat 

//yuvraj

void Loan_Management() {
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
//yuvraj

//aaditya

void viewAllStocks()
{
    int i;
    for(i=1; i<=120; i++) printf("-");
    printf("\n|");
    printf("                                          Viewing All Stocks                                                          |");
    printf("\n");
    for(i=1; i<=120; i++) printf("-");
    printf("\n");

    printf("| %-10s | %-25s | %-10s | %-18s |\n", "Symbol", "Stock Name", "Price(USD)", "Market Cap (M USD)");

    for(i = 0; i < 76; i++) printf("-");
    printf("\n");

    printf("| %-10s | %-25s | %-10s | %-18s |\n", "AAPL", "Apple Inc.", "$145.32", "2410000");
    printf("| %-10s | %-25s | %-10s | %-18s |\n", "TSLA", "Tesla Inc.", "$720.50", "720000");
    printf("| %-10s | %-25s | %-10s | %-18s |\n", "MSFT", "Microsoft Corp.", "$299.72", "2250000");
    printf("| %-10s | %-25s | %-10s | %-18s |\n", "AMZN", "Amazon.com Inc.", "$3415.25", "1700000");
    printf("| %-10s | %-25s | %-10s | %-18s |\n", "GOOGL", "Google LLC", "$2729.34", "1800000");

    for(i = 0; i < 76; i++) printf("-");
    printf("\n");
}

void sortStocksByPrice()
{
    // Additional code for sorting stocks can go here
}
// edit here bhai !!
void searchForStock()
{
    char stockSymbol[10];
    printf("Enter the stock symbol to search (e.g., TSLA, AAPL, MSFT, AMZN): ");
    scanf("%s", stockSymbol);

    if (strcmp(stockSymbol, "TSLA") == 0) {
        int i;
        for(i = 0; i < 40; i++) {
            printf(" ");
        }
        for(i = 0; i < 40; i++) {
            printf("-");
        }
       
		printf("\n");// TRYING BOX
    for(i=1; i<=50; i++) printf("-");
    printf("\n");
    printf("|                Tesla, Inc. (TSLA)               |");
    printf("\n");
    for(i=1; i<=50; i++) printf("-");
    printf("\n");
        // till here 
        printf("\nTesla, Inc. (TSLA) Performance Overview: \n");
        for(i = 0; i < 50; i++) {
            printf("-");
        }
        printf("  \n Tesla, Inc. (TSLA) leads the EV market and focuses on sustainable energy. Founded in 2003 by Elon Musk, it has\n pioneered electric cars, battery innovation, and renewable energy solutions.\n\n");

        printf("Key Metrics (Q2 2024):\n");
        for(i = 0; i < 50; i++) {
            printf("-");
        }
        printf(" \n- Revenue: $24.9 billion (+22%% YoY)\n- Net Income: $2.1 billion (+30%% YoY)\n");
        printf("- Deliveries: 400,000 vehicles (+17%% YoY)\n- Gross Margin: 23.5%%\n\n");

        printf("Recent Highlights:\n");
        for(i = 0; i < 50; i++) {
            printf("-");
        }
        printf(" \n - Expansion in Europe and Asia with new facilities.\n");
        printf("- Self-driving progress with Tesla Autopilot and FSD.\n- Battery R&D aiming at cost efficiency and range.\n \n");

        printf("Future Goals:\n");
        for(i = 0; i < 50; i++) {
            printf("-");
        }
        printf(" \n- Continue EV market expansion and reduce carbon footprint globally.\n\n");

    } else if (strcmp(stockSymbol, "AAPL") == 0) {
        int i;
        for(i = 0; i < 40; i++) {
            printf(" ");
        }
        for(i = 0; i < 40; i++) {
            printf("-");
        }
       
		printf("\n");// TRYING BOX
		for(i=1; i<=50; i++) printf("-");
    printf("\n");
    printf("|                Apple Inc. (AAPL)               |");
    printf("\n");
    for(i=1; i<=50; i++) printf("-");
    printf("\n");
		
        printf("\n Apple Inc. (AAPL) Performance Overview: \n");// HERE BHAI HERE
        for(i = 0; i < 50; i++) {
            printf("-");
        }
        printf(" \nAAPL, a technology giant founded in 1976, is known for pioneering consumer electronics and services.\n Major products include iPhone, iPad, and Mac.\n\n");

        printf("Key Metrics (Q3 2024):\n");
        for(i = 0; i < 50; i++) {
            printf("-");
        }
        printf("\n- Revenue: $94 billion (+10%% YoY)\n- Net Income: $23 billion (+15%% YoY)\n");
        printf("- iPhone Sales: 60 million units (+5%% YoY)\n- Services: $21 billion (+18%% YoY)\n\n");

        printf("Product Lines:\n");
		for(i = 0; i < 50; i++) {
            printf("-");
        }
		printf(" \n- iPhone 15 with enhanced camera tech and M2 MacBook.\n- Focus on growth in services like iCloud, Apple TV+.\n");

        printf("Strategic Focus:\n");
		for(i = 0; i < 50; i++) {
            printf("-");
        }
		printf("\n- Expanding augmented reality solutions, focusing on India for growth.\n\n");

    } else if (strcmp(stockSymbol, "MSFT") == 0) {
           int i;
        for(i = 0; i < 40; i++) {
            printf(" ");
        }
        for(i = 0; i < 40; i++) {
            printf("-");
        }
       
		printf("\n");// TRYING BOX
		for(i=1; i<=50; i++) printf("-");
    printf("\n");
    printf("|           Microsoft Corporation (MSFT)         |");
    printf("\n");
    for(i=1; i<=50; i++) printf("-");
    printf("\n");//HERE BHAI
        printf("\n Microsoft Corporation (MSFT) Performance Overview: \n");
        for(i = 0; i < 50; i++) {
            printf("-");
        }
        printf(" \nA leader in software, cloud computing, and enterprise solutions since 1975, MSFT drives innovation with Windows, Office, and Azure.\n\n");

        printf("Key Metrics (Q1 2024):\n");
        for(i = 0; i < 50; i++) {
            printf("-");
        }
        printf("\n- Revenue: $55.5 billion (+16%% YoY)\n- Net Income: $16 billion (+12%% YoY)\n");
        printf("- Azure Revenue: $12 billion (+30%% YoY)\n\n");

        printf("Focus Areas:\n");
		for(i = 0; i < 50; i++) {
            printf("-");
        }
		printf("\n- Cloud computing with Azure gaining share.\n- Growth in LinkedIn and Xbox gaming.\n");

        printf("Future Focus:\n");
		for(i = 0; i < 50; i++) {
            printf("-");
        }
		printf("\n- Strengthening AI integrations, remote work tools, and expanding hybrid workplace solutions.\n\n");

    } else if (strcmp(stockSymbol, "AMZN") == 0) {
          int i;
        for(i = 0; i < 40; i++) {
            printf(" ");
        }
        for(i = 0; i < 40; i++) {
            printf("-");
        }
       
		printf("\n");// TRYING BOX
		for(i=1; i<=50; i++) printf("-");
    printf("\n");
    printf("|              Amazon.com, Inc. (AMZN)           |");
    printf("\n");
    for(i=1; i<=50; i++) printf("-");
    printf("\n");//HERE BHAI
        printf("\n Amazon.com, Inc. (AMZN) Performance Overview: \n");
        for(i = 0; i < 50; i++) {
            printf("-");
        }
        printf(" \nFounded in 1994, Amazon has become a leader in e-commerce and cloud computing with its AWS platform.\n\n");

        printf("Key Metrics (Q2 2024):\n");
        for(i = 0; i < 50; i++) {
            printf("-");
        }
        printf("\n- Revenue: $138 billion (+15%% YoY)\n- Net Income: $4.7 billion (+18%% YoY)\n");
        printf("- AWS Revenue: $25 billion (+12%% YoY)\n\n");

        printf("Growth Areas:\n");
		for(i = 0; i < 50; i++) {
            printf("-");
        }
		printf("\n- Strong cloud presence with AWS.\n- Expanded grocery delivery with Amazon Fresh.\n");

        printf("Strategic Initiatives:\n ");
		for(i = 0; i < 50; i++) {
            printf("-");
        }
		printf("\n- Focus on logistics automation and innovation in AI and robotics.\n\n");

    } else {
        printf("Stock symbol '%s' not recognized. Please try again.\n", stockSymbol);
    }
}


void convertStockPrice()
{
    float priceInUSD, convertedPrice;
    char currency[4];
    int  i,found = 0;

    // List of 25 top economies with country names, currency codes, and sample exchange rates
    const char *countries[25] = {
        "United States", "Eurozone", "Japan", "China", "India",
        "United Kingdom", "Brazil", "Canada", "Russia", "South Korea",
        "Australia", "Mexico", "India", "Netherlands", "Saudi Arabia",
        "Turkey", "Switzerland", "Taiwan", "Sweden", "Poland",
        "Thailand", "Argentina", "Nigeria", "South Africa", "Philippines"
    };

    const char *currencyCodes[25] = {
        "USD", "EUR", "JPY", "CNY", "INR",
        "GBP", "BRL", "CAD", "RUB", "KRW",
        "AUD", "MXN", "INR", "EUR", "SAR",
        "TRY", "CHF", "TWD", "SEK", "PLN",
        "THB", "ARS", "NGN", "ZAR", "PHP"
    };

    // Example exchange rates for each currency relative to USD
    float exchangeRates[25] = {
        1.0, 0.92, 145.0, 7.1, 83.5,
        0.81, 5.3, 1.36, 65.0, 1200.0,
        1.5, 20.5, 83.5, 0.92, 3.75,
        27.0, 0.92, 31.0, 11.0, 4.5,
        35.0, 350.0, 770.0, 18.5, 56.5
    };

    // Ask the user to enter a stock price in USD
    printf("Enter the stock price in USD: ");
    scanf("%f", &priceInUSD);

    // Display all available countries with currencies in a neat table format
    printf("\nAvailable currencies for conversion:\n");
    printf("------------------------------------------------------------\n");
    printf("| %-20s | %-10s |\n", "Country", "Currency Code");
    printf("------------------------------------------------------------\n");

    for (i = 0; i < 25; i++) {
        printf("| %-20s | %-10s |\n", countries[i], currencyCodes[i]);
    }

    printf("------------------------------------------------------------\n");

    // Ask the user to enter the target currency
    printf("\nEnter the target currency code (e.g., EUR, INR, GBP): ");
    scanf("%s", currency);

    // Search for the currency code in the array and calculate the conversion
    for ( i = 0; i < 25; i++) {
        if (strcmp(currency, currencyCodes[i]) == 0) {
            convertedPrice = priceInUSD * exchangeRates[i];
            printf("\nStock price: %.2f USD\n", priceInUSD);
            printf("Converted price in %s (%s): %.2f\n", currency, countries[i], convertedPrice);
            found = 1;
            break;
        }
    }

    // If currency is not found
    if (!found) {
        printf("Currency code '%s' not recognized. Please try again.\n", currency);
    }
}


//aaditya

// Main function
int main()
{
    srand(time(0));

    // Load accounts and transactions from files
    loadAccountsFromFile();
    loadTransactionsFromFile();

    int choice;
    unsigned long long accountNumber;
    unsigned long long fromAccount, toAccount;
    float amount;
    float amount1;

    struct BankAccount account; 
    account.balance = 10000.0;  

    int emailValid = 0;

    while (1) {
        //printf("\nOnline Banking System\n");
        menu:
        printf("1.Create Account\n");
        printf("2. Loan Management\n");
        printf("3. Fast Pay Online Banking\n");
        printf("4. Market Exploration\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            l1:
            case 1:
            int c;
            printf("10. Account Creation Offline \n");
            printf("20. Offline Money Deposit\n");
            printf("30. Offline Withdraw\n");
            printf("40. Offline Balance\n");
            printf("50. Back To the Main Menu\n");
            printf("Enter your choice: \n");
            scanf("%d",&c); 
            switch(c)
            {
                case 10:
               printf("We are excited to have you onboard! To get started, please create an account.\n\n");
                for (int i = 0; i < 120; i++) printf("-");
                printf("\n|                                           Account Creation Menu                                                      |\n");
                for (int i = 0; i < 120; i++) printf("-");
                printf("\n\n");

                printf("\nEnter your full name: ");
                scanf("%49s", account.name); 

                printf("\nEnter your account number: ");
                scanf("%19s", account.accountnumber);

              while (!emailValid)
             {
                printf("\nEnter your email: ");
                scanf("%49s", account.email); 
                
                if (ValidEmail(account.email)) {
                    emailValid = 1;
                } else 
                {
                    printf("Invalid email format. Please include '@' and '.' symbols in the correct order.\n");
                }   
             }

              break;

             case 20:
                printf("\nEnter the amount you want to deposit: ");
                scanf("%f", &amount1);

                if (amount1 > 0 && amount1 <= 100000) {
                    account.balance += amount1;
                    printf("Amount successfully deposited!\n");
                } else {
                    printf("Invalid deposit amount. Please enter a value between 0 and 100,000.\n");
                }
                break;

             case 30:
                printf("\nEnter the amount you want to withdraw: ");
                scanf("%f", &amount1);

                if (amount1 > 0 && amount1 <= account.balance) {
                    account.balance -= amount1;
                    printf("Amount successfully withdrawn!\n");
                } else if (amount1 > account.balance) {
                    printf("Insufficient funds.\n");
                } else {
                    printf("Invalid withdrawal amount.\n");
                }
                break;

                 case 40:
                printf("Current Balance: $%.2f\n", account.balance);
                break;
                case 50:
                printf("Going Back To the Main Menu\n");
             goto menu;

                default:
                printf("Invalid Entry\nPlease Try Again\n");
                goto l1;
            }

            case 2:
            Loan_Management();
            goto menu;

            l3:
            case 3: 
         while(1)
        {
            int p1;
            printf("11. Create Account\n");
            printf("22. View Account\n");
            printf("33. Transfer Funds\n");
            printf("44. View Transaction History\n");
            printf("55. Exit\n");
            printf("66. Back To Menu\n");
            printf("Enter your choice: ");
            scanf("%d",&p1);
            switch(p1)
           {
                case 11:
                createAccount();
                break;
                case 22:
                printf("Enter account number to view: ");
                scanf("%llu", &accountNumber);
                viewAccount(accountNumber);
                break;
                case 33:
                printf("Enter account number to transfer from: ");
                scanf("%llu", &fromAccount);
                printf("Enter account number to transfer to: ");
                scanf("%llu", &toAccount);
                printf("Enter amount to transfer: ");
                scanf("%f", &amount);
                transferFunds(fromAccount, toAccount, amount);
                break;
                case 44:
                printf("Enter account number to view transaction history: ");
                scanf("%llu", &accountNumber);
                for (int i = 0; i < count; i++) {
                    if (on[i].accountNumber == accountNumber) 
                    {
                        if (verifyPassword(i)) 
                        {
                            viewTransactionHistory(i);
                        }
                        //goto l3;
                    }
                }
                break;
                case 55:
                printf("Exiting...\n");
                exit(0);
                case 66:
                printf("Going Back To Main Menu..\n\n\n\n");
                goto menu;
                default:
                printf("Invalid choice. Please try again.\n");
                goto l3; 
            }
            printf("Press Enter to return to the menu: ");
            getchar();
            getchar();
            printf("\n \n \n\n\n\n");
        }
            case 4:

                int i;
    l4:
    while(1)
    {
        for(i=1; i<=120; i++) printf("-");
        printf("\n|");
        printf("                                          Market Exploration Menu                                                     |");
        printf("\n");
        for(i=1; i<=120; i++) printf("-");
        printf("\n");

        printf("13) View All Stocks \n23) Sort stocks By Price \n33) Convert Stock prices to another country \n43) Exit This menu \n ");
		int i;
        for( i=0; i<39; i++)
		{
		 printf(" ");
	    }
	    for( i=0; i<39; i++)
		{
		 printf("-");
	    }
	    printf("\n");
	    for( i=0; i<40; i++)
		{
		 printf(" ");
	    }
	    for( i=0; i<39; i++)
		{
		 printf("-");
	    }
		printf("\n Kindly Enter the option to continue with: ");
    
        int c3;
        scanf("%d", &c3);
        printf("\n");

        switch(c3)
        {
            case 13:
                viewAllStocks();
                break;
            case 23:
                searchForStock();
                break;
            case 33:
                 convertStockPrice();
                break;
            case 43:
                printf("\nReturning to the main menu...\n");
                goto menu;
            default:
                printf("\nInvalid option! Please try again.\n");
                goto l4;
        }

            printf("Press Enter to return to the main menu: ");
            getchar();
            getchar();
            printf("\n \n \n\n\n\n");
     }
            case 5: 
            printf("Exiting the Code.....\nThank You For Using our Services");
            exit(0);
      }
        }
   

    return 0;
}