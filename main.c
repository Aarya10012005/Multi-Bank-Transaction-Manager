#include "m-tree.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#define TERMINATE 4
#define ln printf("\n");

int main(int argc, char *argv[]) {
    // printf("Number of arguments: %d\n", argc);
    // for (int i = 0; i < argc; i++) {
    //     printf("Argument %d: %s\n", i, argv[i]);
    // }

    if(argc < 2) {
        printf("Please give bank statements.\n");
        return 1;
    }

    tree banks;
    int numberOfFiles = argc - 1;
    int operation = 1;
    init(&banks, numberOfFiles, argv);

    combineFile(argc, argv);
    fill_data_structure(&banks, numberOfFiles);

    while(operation != TERMINATE) {
        
        printf("\n\n---------------------------------------------------------------------\n\n");
        
        printf("1. Summary"); ln;
        printf("2. Search"); ln;
        printf("3. Transaction details with minimum and maximum credit");ln;
        printf("4. Terminate"); ln;
        printf("Enter the operation : ");
        scanf(" %d", &operation);
        switch (operation) {
            case 1:
                printf("\nSummary:\n\n");
                summary(&banks, numberOfFiles, argv);
                
                
                break;

            case 2:
                search_transactions(&banks, numberOfFiles, argv);
                break;

            case 3:
                {
                    minTxn(&banks, numberOfFiles, argv);
                    maxTxn(&banks, numberOfFiles, argv);
                }
                break;

            default:
                break;
        }
    }

    // printing the data structure
    //print_data_structure(&banks, numberOfFiles);
    // destroying
    destroy(&banks, numberOfFiles);
    remove("./bank_statements/collective.csv");

    // delete collective
    return 0;
}