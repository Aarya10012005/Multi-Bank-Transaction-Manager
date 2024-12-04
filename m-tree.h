#define BANK_NAME 16
#define CR_DR 4
#define QUARTER 16

typedef struct amount {
    int srno;
    // char bankName[BANK_NAME];
    int amount;
    struct amount *next;
}amount;

typedef struct quarter {
    char quarter[QUARTER];
    amount *amounts;
}quarter;

typedef struct year {
    char year[4];
    quarter *quarter1, *quarter2, *quarter3, *quarter4;
    // amount *jan, *feb, *march, *april, *may, *june, *july, *aug, *sept, *oct, *nov, *dec;
}year;

typedef struct transaction {
    char str[CR_DR]; // cr/dr
    year *year;
}transaction;

typedef struct root {
    char bankName[BANK_NAME];
    transaction *credit, *debit;
}root;

typedef root* tree;

// file function declarations
void combineFile(int argc, char *argv[]);

// function definitions:
void init(tree *bank, int numberOfFiles, char **bankName);
void print_statement(tree *bank, int numberOfFiles);

tree balance_bank1();
tree balance_bank2();
tree total_credit_bank1();
tree total_credit_bank2();
tree total_debit_bank1();
tree total_debit_bank2();
void sort(); // on basis of amount/date/month
void destroy(tree *bank, int numberOfFiles);