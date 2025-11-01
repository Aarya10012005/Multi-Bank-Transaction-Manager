#include "m-tree.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER 256
#define BANKNAME 100
#define NUM_QUARTERS 4
#define ln printf("\n");

void init(tree *bank, int numberOfFiles, char **bankName) {
    (*bank) = (tree)malloc(sizeof(root) * numberOfFiles);
    if((*bank) == NULL) {
        printf("Memory allocation for banks failed.\n");
        exit(EXIT_FAILURE);
    }

    tree rootNode;
    for(int i = 0; i < numberOfFiles; i++) {
        char tmp[BANKNAME];
        strcpy(tmp, bankName[i + 1]);
        rootNode = &((*bank)[i]);
        // assigning bank name to the root node element
        char *token = strtok(tmp, "/");
        token = strtok(NULL, "/");
        token = strtok(NULL, ".");
        strcpy(rootNode->bankName, token);
        //printf("%s\n", rootNode->bankName);

        // mallocing 2 children of root node
        rootNode->credit = (transaction *)malloc(sizeof(transaction));
        transaction *credit = rootNode->credit;
        strcpy(credit->str, "CR");
        credit->balance = 0;
        //printf("%s ", credit->str);
        rootNode->debit = (transaction *)malloc(sizeof(transaction));
        transaction *debit = rootNode->debit;
        strcpy(debit->str, "DR");
        debit->balance = 0;
        //printf("%s\n", debit->str);

        // mallocing year node
        credit->year = (YEAR *)malloc(sizeof(YEAR));
        strcpy(credit->year->year, "2023"); // year is to be allocated according to the date
        debit->year = (YEAR *)malloc(sizeof(YEAR));
        strcpy(debit->year->year, "2023");  // year is to be allocated according to the date

        // mallocing quarter nodes
        for(int i = 0; i < 4; i++){
            credit->year->qArr[i] = (quarter *)malloc(sizeof(quarter));
            credit->year->qArr[i]->amounts = NULL;
            credit->year->qArr[i]->balance = 0;
            char temp1[10];
            strcpy(temp1, "Quarter ");
            temp1[8] = i + '1'; 
            temp1[9] = '\0';    
            strcpy(credit->year->qArr[i]->quarter, temp1);		

            debit->year->qArr[i] = (quarter *)malloc(sizeof(quarter));
            debit->year->qArr[i]->amounts = NULL;
            debit->year->qArr[i]->balance = 0;
            char temp2[10];
            strcpy(temp2, "Quarter ");
            temp2[8] = i + '1'; 
            temp2[9] = '\0';    
            strcpy(debit->year->qArr[i]->quarter, temp2);
        }
    }
    return;
}

void destroy(tree *bank, int numberOfFiles) {
    tree rootNode;
    transaction *credit, *debit;
    YEAR *credit_year, *debit_year;
    quarter *cr_y_q1, *cr_y_q2, *cr_y_q3, *cr_y_q4, *dr_y_q1, *dr_y_q2, *dr_y_q3, *dr_y_q4;
    amount *amounts, *prevAmt;
    for(int i = 0; i < numberOfFiles; i++) {
        rootNode = &((*bank)[i]);

        credit = rootNode->credit;
        debit = rootNode->debit;

        credit_year = credit->year;
        debit_year = debit->year;

        cr_y_q1 = credit_year->qArr[0];
        cr_y_q2 = credit_year->qArr[1];
        cr_y_q3 = credit_year->qArr[2];
        cr_y_q4 = credit_year->qArr[3];
        dr_y_q1 = debit_year->qArr[0];
        dr_y_q2 = debit_year->qArr[1];
        dr_y_q3 = debit_year->qArr[2];
        dr_y_q4 = debit_year->qArr[3];

        // freeing amount nodes
        amounts = cr_y_q1->amounts;
        while(amounts) {
            prevAmt = amounts;
            amounts = amounts->next;
            free(prevAmt);
        }
        amounts = cr_y_q2->amounts;
        while(amounts) {
            prevAmt = amounts;
            amounts = amounts->next;
            free(prevAmt);
        }
        amounts = cr_y_q3->amounts;
        while(amounts) {
            prevAmt = amounts;
            amounts = amounts->next;
            free(prevAmt);
        }
        amounts = cr_y_q4->amounts;
        while(amounts) {
            prevAmt = amounts;
            amounts = amounts->next;
            free(prevAmt);
        }

        amounts = dr_y_q1->amounts;
        while(amounts) {
            prevAmt = amounts;
            amounts = amounts->next;
            free(prevAmt);
        }
        amounts = dr_y_q2->amounts;
        while(amounts) {
            prevAmt = amounts;
            amounts = amounts->next;
            free(prevAmt);
        }
        amounts = dr_y_q3->amounts;
        while(amounts) {
            prevAmt = amounts;
            amounts = amounts->next;
            free(prevAmt);
        }
        amounts = dr_y_q4->amounts;
        while(amounts) {
            prevAmt = amounts;
            amounts = amounts->next;
            free(prevAmt);
        }

        // freeing quarter nodes
        free(cr_y_q1);
        free(cr_y_q2);
        free(cr_y_q3);
        free(cr_y_q4);

        free(dr_y_q1);
        free(dr_y_q2);
        free(dr_y_q3);
        free(dr_y_q4);

        // freeing year nodes
        free(credit_year);
        free(debit_year);

        // freeing cr/dr (transaction) nodes
        free(credit);
        free(debit);
    }
    // freeing the malloced memory of root nodes
    free((*bank));
    (*bank) = NULL;
    return;
}

void combineFile(int argc, char *argv[]){
	/* Allocate memory for file pointers and other variables */
	FILE **fp = NULL, *newFile;
	char line[BUFFER], *token, bank[BANKNAME];
	
	fp = (FILE **)calloc(argc - 1, sizeof(FILE *));
	
	/* Opening the bank statements in read mode */
	for(int i = 0; i < (argc - 1); i++){
		fp[i] = fopen(argv[i + 1], "r");
		if(fp[i] == NULL){
			//printf("Error while opening the file!");
			perror("Error opening file");
            exit(1);
		}
	}

	/* Opening new file to store all the combined bank statements */
	newFile = fopen("./bank_statements/collective.csv", "a");
	if(newFile == NULL){
		printf("Error while creating a new file");
		exit(1);
	}

	/* Writing the header row (from the first file) to the new file */
	fgets(line, BUFFER, fp[0]);         /* Get the header row of the first file */
	fputs("Bank Name,", newFile);       /* Add an extra column for bank name */
	fputs(line, newFile);               /* Write the header to the new file */

	/* Process each bank file */
	for(int i = 1; i <= argc - 1; i++){
		strcpy(bank, argv[i]);
		token = strtok(bank, "/");  /* Extract bank name from the filename */
		token = strtok(NULL, "/");
        token = strtok(NULL, ".");

		/* For the first file, the header has already been processed, so no need to skip the header */
		if(i > 1){
			fgets(line, BUFFER, fp[i - 1]);  /* Skip the header line for subsequent files */ 
		}
		
		/* Read and write the transactions */
		while(fgets(line, BUFFER, fp[i - 1]) != NULL){
			/* Write the bank name and transaction data */
			fputs(token, newFile);
			fputs(",", newFile);
			fputs(line, newFile);
		}
	}

	/* Close all the files */
	for(int i = 0; i < argc - 1; i++){
		fclose(fp[i]);
	}
	fclose(newFile);

	/* Free allocated memory */
	free(fp);
	return;
}

void fill_data_structure(tree *bank, int numberOfFiles) {
    char *fileName = "./bank_statements/collective.csv";
    char line[BUFFER], srn[10];
    char *token;

    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open the collective file: %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    // Skip the header line
    fgets(line, BUFFER, fp);

    while (fgets(line, BUFFER, fp) != NULL) {
        tree rootNode = NULL;
        int qtr = 0, currBank;
        char year[5];

        // Get bank name
        token = strtok(line, ",");
        for (currBank = 0; currBank < numberOfFiles; currBank++) {
            if (strcmp(token, (*bank)[currBank].bankName) == 0) {
                rootNode = &((*bank)[currBank]);
                break;
            }
        }

        if (rootNode == NULL) {
            fprintf(stderr, "Bank name not found: %s\n", token);
            exit(EXIT_FAILURE); // This shouldn't happen if all inputs are valid
        }

        // Print bank name
        //printf("%s,", rootNode->bankName);

        // SRN
        token = strtok(NULL, ",");
        strcpy(srn, token);

        // Extract transaction date
        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(year, token + 6, 4);        // Extract year (characters 7 to 10)
            year[4] = '\0';                     // Null-terminate the string

            char month[3];
            strncpy(month, token + 3, 2);       // Extract month (characters 4 and 5)
            month[2] = '\0';                    // Null-terminate the string

            int month_num = atoi(month);        // Convert month to an integer

            // Determine quarter based on month
            if (month_num >= 1 && month_num <= 3) {
                qtr = 1;
            } else if (month_num >= 4 && month_num <= 6) {
                qtr = 2;
            } else if (month_num >= 7 && month_num <= 9) {
                qtr = 3;
            } else if (month_num >= 10 && month_num <= 12) {
                qtr = 4;
            }
        }

        // Skip value date and description
        token = strtok(NULL, ",");
        token = strtok(NULL, ",");

        quarter *q = NULL;
        // Determine credit or debit
        token = strtok(NULL, ","); // CR/DR
        if (strcmp(token, "CR") == 0) {
            //printf("%s,%s,", rootNode->credit->str, year);
            if (qtr == 1) {
                //printf("%s,", rootNode->credit->year->quarter1->quarter);
                q = rootNode->credit->year->qArr[0];
            } else if (qtr == 2) {
                //printf("%s,", rootNode->credit->year->quarter2->quarter);
                q = rootNode->credit->year->qArr[1];
            } else if (qtr == 3) {
                //printf("%s,", rootNode->credit->year->quarter3->quarter);
                q = rootNode->credit->year->qArr[2];
            } else {
                //printf("%s,", rootNode->credit->year->quarter4->quarter);
                q = rootNode->credit->year->qArr[3];
            }
        } else if (strcmp(token, "DR") == 0) {
            //printf("%s,%s,", rootNode->debit->str, year);
            if (qtr == 1) {
                //printf("%s,", rootNode->debit->year->quarter1->quarter);
                q = rootNode->debit->year->qArr[0];
            } else if (qtr == 2) {
                //printf("%s,", rootNode->debit->year->quarter2->quarter);
                q = rootNode->debit->year->qArr[1];
            } else if (qtr == 3) {
                //printf("%s,", rootNode->debit->year->quarter3->quarter);
                q = rootNode->debit->year->qArr[2];
            } else {
                //printf("%s,", rootNode->debit->year->quarter4->quarter);
                q = rootNode->debit->year->qArr[3];
            }
        }

        // Skip currency
        token = strtok(NULL, ",");

        // Amount
        token = strtok(NULL, ",");
        int amt = atoi(token);
        insert(q, amt, srn);
        //printf("%d\n", q->amounts->amount);

        // Skip balance
        token = strtok(NULL, ",");
    }

    // Storing total credited amount and total debited amount
    for(int i = 0; i < numberOfFiles; i++) {
        tree rootNode = &((*bank)[i]);
        for(int j = 0; j < 4; j++) {
            rootNode->credit->balance += rootNode->credit->year->qArr[j]->balance;
            rootNode->debit->balance += rootNode->debit->year->qArr[j]->balance;
        }
    }

    // Closing the file
    fclose(fp);
}

void print_data_structure(tree *bank, int numberOfFiles) {
    printf("\n\n---------------------------------------------------------------------\n\n");
    for(int i = 0; i < numberOfFiles; i++) {
        tree rootNode = &((*bank)[i]);
        printf("%s:\n", rootNode->bankName);

        transaction *credit = rootNode->credit, *debit = rootNode->debit;

        /* CREDIT */
        printf("%s amount of ", credit->str);
        YEAR *yc = credit->year;
        printf("%s\n", yc->year);

        quarter *q1c = yc->qArr[0], *q2c = yc->qArr[1], *q3c = yc->qArr[2], *q4c = yc->qArr[3];
        amount *a1c = q1c->amounts, *a2c = q2c->amounts, *a3c = q3c->amounts, *a4c = q4c->amounts;

        printf("\n%s:\n", q1c->quarter);
        while(a1c) {
            printf("%d ", a1c->amount);
            a1c = a1c->next;
        }
        printf("\n\n%s:\n", q2c->quarter);
        while(a2c) {
            printf("%d ", a2c->amount);
            a2c = a2c->next;
        }
        printf("\n\n%s:\n", q3c->quarter);
        while(a3c) {
            printf("%d ", a3c->amount);
            a3c = a3c->next;
        }
        printf("\n\n%s:\n", q4c->quarter);
        while(a4c) {
            printf("%d ", a4c->amount);
            a4c = a4c->next;
        }
        printf("\n");

        /* DEBIT */
        printf("%s amount of", debit->str);
        YEAR *yd = debit->year;
        printf("%s\n", yd->year);

        quarter *q1d = yd->qArr[0], *q2d = yd->qArr[1], *q3d = yd->qArr[2], *q4d = yd->qArr[3];
        amount *a1d = q1d->amounts, *a2d = q2d->amounts, *a3d = q3d->amounts, *a4d = q4d->amounts;

        printf("\n%s:\n", q1d->quarter);
        while(a1d) {
            printf("%d ", a1d->amount);
            a1d = a1d->next;
        }
        printf("\n\n%s:\n", q2d->quarter);
        while(a2d) {
            printf("%d ", a2d->amount);
            a2d = a2d->next;
        }
        printf("\n\n%s:\n", q3d->quarter);
        while(a3d) {
            printf("%d ", a3d->amount);
            a3d = a3d->next;
        }
        printf("\n\n%s:\n", q4d->quarter);
        while(a4d) {
            printf("%d ", a4d->amount);
            a4d = a4d->next;
        }
        printf("\n\n---------------------------------------------------------------------\n\n");
    }
}

int avg_quarterly_deb(tree *banks, int numberOfFiles, int Quart) {
    int avg_sum = 0;
    int count_txn = 0;

    for (int i = 0; i < numberOfFiles; i++) {
        tree rootnode = &((*banks)[i]);
        amount *a_i = NULL;
        a_i = rootnode->debit->year->qArr[Quart - 1]->amounts;
        while (a_i) {
            count_txn++;
            avg_sum += a_i->amount;
            a_i = a_i->next;
        }
    }
    if (count_txn == 0) {
        return 0; // Avoid division by zero
    }
    int avg_deb = avg_sum / count_txn;
    return avg_deb;
}

int avg_quarterly_cred(tree *banks, int numberOfFiles, int Quart) {
    int avg_sum = 0;
    int count_txn = 0;

    for (int i = 0; i < numberOfFiles; i++) {
        tree rootnode = &((*banks)[i]);
        amount *a_i = NULL;
        a_i = rootnode->credit->year->qArr[Quart - 1]->amounts;
        while (a_i) {
            count_txn++;
            avg_sum += a_i->amount;
            a_i = a_i->next;
        }
    }

    if (count_txn == 0) {
        return 0; // Avoid division by zero
    }

    int avg_cred = avg_sum / count_txn;
    return avg_cred;
}


int txn_count_quart(tree *banks, int numberOfFiles, int Quart) {
    int count_txn = 0;
    for (int i = 0; i < numberOfFiles; i++) {
        tree rootnode = &((*banks)[i]);

        amount *a_i = NULL;
        if (Quart == 1) {
            a_i = rootnode->credit->year->qArr[0]->amounts;
        } else if (Quart == 2) {
            a_i = rootnode->credit->year->qArr[1]->amounts;
        } else if (Quart == 3) {
            a_i = rootnode->credit->year->qArr[2]->amounts;
        } else if (Quart == 4) {
            a_i = rootnode->credit->year->qArr[3]->amounts;
        }

        while (a_i) {
            count_txn++;
            a_i = a_i->next;
        }

        if (Quart == 1) {
            a_i = rootnode->debit->year->qArr[0]->amounts;
        } else if (Quart == 2) {
            a_i = rootnode->debit->year->qArr[1]->amounts;
        } else if (Quart == 3) {
            a_i = rootnode->debit->year->qArr[2]->amounts;
        } else if (Quart == 4) {
            a_i = rootnode->debit->year->qArr[3]->amounts;
        }

        while (a_i) {
            count_txn++;
            a_i = a_i->next;
        }
        
    }

    return count_txn;
}

void generate_txn_count_graph(tree *banks, int numberOfFiles) {
    // Array to store transaction counts for each quarter
    int txn_counts[4] = {0};

    // Calculate transaction counts for each quarter
    for (int q = 1; q <= 4; q++) {
        txn_counts[q - 1] = txn_count_quart(banks, numberOfFiles, q);
    }

    // Open a pipe to Gnuplot
    FILE *gp = popen("gnuplot -persistent", "w");
    if (!gp) {
        printf("Error: Could not open Gnuplot.\n");
        return;
    }

    // Send Gnuplot commands to save the graph
    fprintf(gp, "set terminal png size 800,600\n"); // Set the terminal to PNG with size
    fprintf(gp, "set output './Graphs/transaction_counts.png'\n"); // Set the output file name
    fprintf(gp, "set title 'Transaction Counts Per Quarter'\n");
    fprintf(gp, "set xlabel 'Quarter'\n");
    fprintf(gp, "set ylabel 'Transaction Count'\n");
    fprintf(gp, "set style data histograms\n");
    fprintf(gp, "set style fill solid\n");
    fprintf(gp, "set boxwidth 0.5\n");
    fprintf(gp, "set grid\n");

    // Define the plot with inline data
    fprintf(gp, "plot '-' using 2:xtic(1) title 'Transactions' with histogram\n");

    // Pass the calculated data to Gnuplot
    for (int i = 0; i < 4; i++) {
        fprintf(gp, "Q%d %d\n", i + 1, txn_counts[i]); // e.g., Q1 10
    }
    fprintf(gp, "e\n"); // End of data

    // Close the pipe
    pclose(gp);

    printf("Graph saved as 'transaction_counts.png'.\n");
}

void plot_avg_credit_debit(tree *banks, int numberOfFiles) {

    int avg_debit[4] = {0};
    int avg_credit[4] = {0};

    for (int q = 1; q <= 4; q++) {
        avg_debit[q - 1] = avg_quarterly_deb(banks, numberOfFiles, q);
        avg_credit[q - 1] = avg_quarterly_cred(banks, numberOfFiles, q);
    }

    FILE *gp = popen("gnuplot -persistent", "w");
    if (!gp) {
        printf("Error: Could not open Gnuplot.\n");
        return;
    }

    fprintf(gp, "set terminal png size 800,600\n");
    fprintf(gp, "set title 'Average Credit and Debit Per Quarter'\n");
    fprintf(gp, "set xlabel 'Quarter'\n");
    fprintf(gp, "set ylabel 'Average Amount'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "set xtics 1\n");
    fprintf(gp, "set key outside\n");
    fprintf(gp, "set output './Graphs/avg_credit_debit.png'\n");

    fprintf(gp, "plot '-' using 1:2 with linespoints title 'Average Debit', "
                "'-' using 1:2 with linespoints title 'Average Credit'\n");

    for (int i = 0; i < 4; i++) {
        fprintf(gp, "%d %d\n", i + 1, avg_debit[i]);
    }
    fprintf(gp, "e\n");

    for (int i = 0; i < 4; i++) {
        fprintf(gp, "%d %d\n", i + 1, avg_credit[i]);
    }
    fprintf(gp, "e\n");

    pclose(gp);

    printf("Graph saved as './Graphs/avg_credit_debit.png'\n");
}


void minTxn(tree *banks, int numberOfFiles, char *argv[]) {
    int min_cred = INT_MAX;
    int min_deb = INT_MAX;

    char min_cred_srl[10] = "";
    char min_deb_srl[10] = "";

    char min_cred_bank[BANK_NAME] = "";
    char min_deb_bank[BANK_NAME] = "";

    char min_cred_line[BUFFER] = "";
    char min_deb_line[BUFFER] = "";

    int *min_store = (int *)malloc(sizeof(int) * 2);
    if (!min_store) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Find minimum credit and debit transactions
    for (int i = 0; i < numberOfFiles; i++) {
        root *rootnode = *banks + i;

        // Traverse all credit transactions
        for (int j = 1; j <= 4; j++) {
            amount *ptr = rootnode->credit->year->qArr[j - 1]->amounts;
            while (ptr) {
                if (ptr->amount < min_cred) {
                    min_cred = ptr->amount;
                    strncpy(min_cred_srl, ptr->srno, sizeof(min_cred_srl) - 1);
                    min_cred_srl[sizeof(min_cred_srl) - 1] = '\0';
                    strncpy(min_cred_bank, rootnode->bankName, sizeof(min_cred_bank) - 1);
                    min_cred_bank[sizeof(min_cred_bank) - 1] = '\0';
                }
                ptr = ptr->next;
            }
        }

        // Traverse all debit transactions
        for (int j = 1; j <= 4; j++) {
            amount *ptr = rootnode->debit->year->qArr[j - 1]->amounts;
            while (ptr) {
                if (ptr->amount < min_deb) {
                    min_deb = ptr->amount;
                    strncpy(min_deb_srl, ptr->srno, sizeof(min_deb_srl) - 1);
                    min_deb_srl[sizeof(min_deb_srl) - 1] = '\0';
                    strncpy(min_deb_bank, rootnode->bankName, sizeof(min_deb_bank) - 1);
                    min_deb_bank[sizeof(min_deb_bank) - 1] = '\0';
                }
                ptr = ptr->next;
            }
        }
    }
    // Find the minimum credit line
    char *token = NULL;
    char temp_bank_name[BANK_NAME];
    int flag = 0;

    for (int i = 0; i < numberOfFiles; i++) {
        strcpy(temp_bank_name, argv[i + 1]);
        token = strtok(temp_bank_name, "/");  // Extract bank name from the filename
        token = strtok(NULL, "/");
        token = strtok(NULL, ".");

        if (strncmp(min_cred_bank, token, 10) == 0) {
            FILE *fp = fopen(argv[i + 1], "r");
            if (!fp) {
                perror("Could not open file for finding min transaction credit");
                exit(EXIT_FAILURE);
            }
            fgets(min_cred_line, BUFFER, fp); // Skip the header line
            while (fgets(min_cred_line, BUFFER, fp)) {
                char temp_line[BUFFER];
                strcpy(temp_line, min_cred_line); // Copy the line for tokenizing
                char *line_token = strtok(temp_line, ","); // Extract serial number
                if (strcmp(min_cred_srl, line_token) == 0) {
                    printf("Minimum Transaction (Credit): %s\n", min_cred_line);
                    fclose(fp);
                    flag = 1;
                    break;
                }
            }
            if (flag) {
                break;
            }
            fclose(fp);
        }
    }

    // Reset flag for debit search
    flag = 0;

    // Find the minimum debit line
    for (int i = 0; i < numberOfFiles; i++) {
        strcpy(temp_bank_name, argv[i + 1]);
        token = strtok(temp_bank_name, "/");  // Extract bank name from the filename
        token = strtok(NULL, "/");
        token = strtok(NULL, ".");

        if (strncmp(min_deb_bank, token, 10) == 0) {
            FILE *fp = fopen(argv[i + 1], "r");
            if (!fp) {
                perror("Could not open file for finding min transaction debit");
                exit(EXIT_FAILURE);
            }
            fgets(min_deb_line, BUFFER, fp); // Skip the header line
            while (fgets(min_deb_line, BUFFER, fp)) {
                char temp_line[BUFFER];
                strcpy(temp_line, min_deb_line); // Copy the line for tokenizing
                char *line_token = strtok(temp_line, ","); // Extract serial number
                if (strcmp(min_deb_srl, line_token) == 0) {
                    printf("Minimum Transaction (Debit): %s\n", min_deb_line);
                    fclose(fp);
                    flag = 1;
                    break;
                }
            }
            if (flag) {
                break;
            }
            fclose(fp);
        }
    }


    free(min_store);
    return;
}

void maxTxn(tree *banks, int numberOfFiles, char *argv[]) {
    int max_cred = -1;
    int max_deb = -1;

    char max_cred_srl[10] = "";
    char max_deb_srl[10] = "";

    char max_cred_bank[BANK_NAME] = "";
    char max_deb_bank[BANK_NAME] = "";

    char max_cred_line[BUFFER] = "";
    char max_deb_line[BUFFER] = "";

    int *max_store = (int *)malloc(sizeof(int) * 2);
    if (!max_store) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Find maximum credit and debit transactions
    for (int i = 0; i < numberOfFiles; i++) {
        root *rootnode = *banks + i;

        // Traverse all credit transactions
        for (int j = 1; j <= 4; j++) {
            amount *ptr = rootnode->credit->year->qArr[j - 1]->amounts;
            while (ptr) {
                if (ptr->amount > max_cred) {
                    max_cred = ptr->amount;
                    strncpy(max_cred_srl, ptr->srno, sizeof(max_cred_srl) - 1);
                    max_cred_srl[sizeof(max_cred_srl) - 1] = '\0';
                    strncpy(max_cred_bank, rootnode->bankName, sizeof(max_cred_bank) - 1);
                    max_cred_bank[sizeof(max_cred_bank) - 1] = '\0';
                }
                ptr = ptr->next;
            }
        }

        // Traverse all debit transactions
        for (int j = 1; j <= 4; j++) {
            amount *ptr = rootnode->debit->year->qArr[j - 1]->amounts;
            while (ptr) {
                if (ptr->amount > max_deb) {
                    max_deb = ptr->amount;
                    strncpy(max_deb_srl, ptr->srno, sizeof(max_deb_srl) - 1);
                    max_deb_srl[sizeof(max_deb_srl) - 1] = '\0';
                    strncpy(max_deb_bank, rootnode->bankName, sizeof(max_deb_bank) - 1);
                    max_deb_bank[sizeof(max_deb_bank) - 1] = '\0';
                }
                ptr = ptr->next;
            }
        }
    }
    //find the transaction line with maximum credit
    char *token = NULL;
    char temp_bank_name[BANK_NAME];
    int flag = 0;

    // Find the maximum credit line
    for (int i = 0; i < numberOfFiles; i++) {
        strcpy(temp_bank_name, argv[i + 1]);
        token = strtok(temp_bank_name, "/");  // Extract bank name from the filename
        token = strtok(NULL, "/");
        token = strtok(NULL, ".");

        if (strncmp(max_cred_bank, token, 10) == 0) {
            FILE *fp = fopen(argv[i + 1], "r");
            if (!fp) {
                perror("Could not open file for finding max transaction credit");
                exit(EXIT_FAILURE);
            }
            fgets(max_cred_line, BUFFER, fp); // Skip the header line
            while (fgets(max_cred_line, BUFFER, fp)) {
                char temp_line[BUFFER];
                strcpy(temp_line, max_cred_line); // Copy the line for tokenizing
                char *line_token = strtok(temp_line, ","); // Extract serial number
                if (strcmp(max_cred_srl, line_token) == 0) {
                    printf("Maximum Transaction (Credit): %s\n", max_cred_line);
                    fclose(fp);
                    flag = 1;
                    break;
                }
            }
            if (flag) {
                break;
            }
            fclose(fp);
        }
    }

    // Reset flag for debit search
    flag = 0;

    // Find the maximum debit line
    for (int i = 0; i < numberOfFiles; i++) {
        strcpy(temp_bank_name, argv[i + 1]);
        token = strtok(temp_bank_name, "/");  // Extract bank name from the filename
        token = strtok(NULL, "/");
        token = strtok(NULL, ".");

        if (strncmp(max_deb_bank, token, 10) == 0) {
            FILE *fp = fopen(argv[i + 1], "r");
            if (!fp) {
                perror("Could not open file for finding max transaction debit");
                exit(EXIT_FAILURE);
            }
            fgets(max_deb_line, BUFFER, fp); // Skip the header line
            while (fgets(max_deb_line, BUFFER, fp)) {
                char temp_line[BUFFER];
                strcpy(temp_line, max_deb_line); // Copy the line for tokenizing
                char *line_token = strtok(temp_line, ","); // Extract serial number
                if (strcmp(max_deb_srl, line_token) == 0) {
                    printf("Maximum Transaction (Debit): %s\n", max_deb_line);
                    fclose(fp);
                    flag = 1;
                    break;
                }
            }
            if (flag) {
                break;
            }
            fclose(fp);
        }
    }

    free(max_store);
    return;
}

void search_transactions(tree *bank, int numberOfFiles, char **fileNames) {
    tree root = (*bank);
    int numBanks = 0, qFrom, qTo;
    char **bankNames, cr_dr[8];

    // taking a frequency array of banks
    int *freqBanks = (int *)malloc(sizeof(int) * numberOfFiles);
    for(int i = 0; i < numberOfFiles; i++)
        freqBanks[i] = 0;

    // asking number of banks from which transactions are to be extracted
    while(numBanks < 1) {
        printf("Enter number of Banks: ");
        scanf(" %d", &numBanks);
        if(numBanks < 1)
            printf("Please enter correct number of banks.\n");
    }

    // asking names of the banks to determine names of banks
    bankNames = (char **)malloc(sizeof(char *) * numBanks);
    for(int i = 0; i < numBanks; i++) {
        bankNames[i] = (char *)malloc(sizeof(char) * BANK_NAME);
        printf("Enter name of the bank %d: ", i + 1);
        scanf("%s", bankNames[i]);
        for(int j = 0; j < strlen(bankNames[i]); j++) {
            bankNames[i][j] = tolower(bankNames[i][j]);
        }
    }

    // checking whether entered bank names are in the data structure or not, and updating the frequency array
    for(int i = 0; i < numBanks; i++) {
        for(int j = 0; j < numberOfFiles; j++) {
            if(strcmp(root[j].bankName, bankNames[i]) == 0) {
                freqBanks[j] = 1;
                break;
            }
        }
    }

    // ask quarter
    printf("Enter FROM quarter: ");
    scanf("%d", &qFrom);
    printf("Enter TO quarter: ");
    scanf("%d", &qTo);

    // ask credit/debit/both
    printf("Credit/debit/both? ");
    scanf("%s", cr_dr);
    cr_dr[1] = '\0';
    cr_dr[0] = tolower(cr_dr[0]);

    // ask amount range
    char range[4];
    int lowAmt = -1, highAmt = -1;
    printf("Is there any amount range? [Y/N] ");
    scanf("%s", range);
    range[1] = '\0';
    range[0] = toupper(range[0]);
    if(range[0] == 'Y') {
        printf("Enter lower amount: ");
        scanf("%d", &lowAmt);
        printf("Enter higher amount: ");
        scanf("%d", &highAmt);
    }

    // printing the header line
    char tmp[BUFFER];
    FILE *temp = fopen(fileNames[1], "r");
    if(temp == NULL) {
        perror("File cannot open");
        exit(EXIT_FAILURE);
    }
    fgets(tmp, BUFFER, temp);
    printf("\n%s", tmp);
    fclose(temp);

    // printing required data
    for(int i = 0; i < numberOfFiles; i++) {
        if(freqBanks[i]) {
            tree rootNode = &((*bank)[i]);
            FILE *fp;// = fopen(fileNames[i], "r");
            transaction *cr = rootNode->credit, *dr = rootNode->debit;
            YEAR *yc = cr->year, *yd = dr->year;
            char line[BUFFER], buf[BUFFER], *token;

            // credit
            if(cr_dr[0] == 'c' || cr_dr[0] == 'b') {
                for(int j = qFrom; j <= qTo; j++) {
                    quarter *qc = yc->qArr[j - 1];
                    // print quarter name
                    //printf("%s:\n", qc->quarter);
                    amount *amtc = qc->amounts;
                    while(amtc) {
                        // print required statements from the file
                        if(strcmp(range, "Y") == 0) {
                            if(amtc->amount >= lowAmt && amtc->amount <= highAmt) {
                                fp = fopen(fileNames[i + 1], "r");
                                if(fp == NULL) {
                                    perror("File cannot open");
                                    exit(EXIT_FAILURE);
                                }
                                // skip header
                                fgets(line, BUFFER, fp);
                                // find amount statements
                                while(fgets(line, BUFFER, fp)) {
                                    strcpy(buf, line);
                                    token = strtok(buf, ",");
                                    if(strcmp(token, amtc->srno) == 0) {
                                        printf("%s", line);
                                        break;
                                    }
                                }
                                fclose(fp);
                            }
                        } else {
                            fp = fopen(fileNames[i + 1], "r");
                            if(fp == NULL) {
                                perror("File cannot open");
                                exit(EXIT_FAILURE);
                            }
                            // skip header
                            fgets(line, BUFFER, fp);
                            // find amount statements
                            while(fgets(line, BUFFER, fp)) {
                                strcpy(buf, line);
                                token = strtok(buf, ",");
                                if(strcmp(token, amtc->srno) == 0) {
                                    printf("%s", line);
                                    break;
                                }
                            }
                            fclose(fp);
                        }
                        //printf("%s. %d\n", amtc->srno, amtc->amount);
                        amtc = amtc->next;
                    }
                }
            }
            // debit
            if(cr_dr[0] == 'd' || cr_dr[0] == 'b') {
                for(int j = qFrom; j <= qTo; j++) {
                    quarter *qd = yd->qArr[j - 1];
                    // print quarter name
                    //printf("%s:\n", qc->quarter);
                    amount *amtd = qd->amounts;
                    while(amtd) {
                        // print required statements from the file
                        if(strcmp(range, "Y") == 0) {
                            if(amtd->amount >= lowAmt && amtd->amount <= highAmt) {
                                fp = fopen(fileNames[i + 1], "r");
                                // skip header
                                fgets(line, BUFFER, fp);
                                // find amount statement
                                while(fgets(line, BUFFER, fp)) {
                                    strcpy(buf, line);
                                    token = strtok(buf, ",");
                                    if(strcmp(token, amtd->srno) == 0) {
                                        printf("%s", line);
                                        break;
                                    }
                                }
                                fclose(fp);
                            }
                        } else {
                            fp = fopen(fileNames[i + 1], "r");
                            if(fp == NULL) {
                                perror("File cannot open");
                                exit(EXIT_FAILURE);
                            }
                            // skip header
                            fgets(line, BUFFER, fp);
                            // find amount statements
                            while(fgets(line, BUFFER, fp)) {
                                strcpy(buf, line);
                                token = strtok(buf, ",");
                                if(strcmp(token, amtd->srno) == 0) {
                                    printf("%s", line);
                                    break;
                                }
                            }
                            fclose(fp);
                        }
                        //printf("%s. %d\n", amtc->srno, amtc->amount);
                        amtd = amtd->next;
                    }
                }
            }
        }
    }
    // free allocated memory
    free(freqBanks);
    for(int i = 0; i < numBanks; i++)
        free(bankNames[i]);
    free(bankNames);
}

void summary(root **banks, int numberOfFiles, char *argv[]){
    
    // count transactions
    printf("*************************************************************************************************************");ln;ln;
    printf("Count of transactions over each quarter");ln;ln;
    int tot_txn = 0;
    for(int i = 1; i <= 4; i++){
        printf("Total transactions in quarter %d is : %d", i, txn_count_quart(banks, numberOfFiles, i)); ln;
        tot_txn += txn_count_quart(banks, numberOfFiles, i);
    }
    printf("Total number of transactions overall are : %d", tot_txn); ln;ln;
    printf("*************************************************************************************************************");ln;ln;
    printf("Average quarterly credit and debit");ln;ln;
    for(int i = 1; i <= 4; i++){
        printf("Average amount credited in quarter %d is : %d", i, avg_quarterly_cred(banks, numberOfFiles, i));ln;
        printf("Average amount debited in quarter %d is : %d", i, avg_quarterly_deb(banks, numberOfFiles, i));ln;ln;
    }
    printf("*************************************************************************************************************");ln;ln;
    printf("Minimum amount credited and debited overall ");ln;ln;
    int *min = minTxn_Summary(banks, numberOfFiles, argv);
    int *max = maxTxn_Summary(banks, numberOfFiles, argv);
    printf("Minimum amount credited is : %d", min[0]);ln;
    printf("Minimum amount debited is : %d", min[1]);ln;ln;

    printf("Maximum amount credited is : %d", max[0]);ln;
    printf("Maximum amount debited is : %d", max[1]);ln;ln;
    free(min);
    free(max);

    printf("**********  End of Summary  **********");ln;ln;
    
    printf("Do you want to generate plot for txn_count vs quarters [y / n] : ");
    char one;
    scanf(" %c", &one);  // Note the space before %c
    if(one == 'y'){
        generate_txn_count_graph(banks, numberOfFiles);
    }
    one = '\0';
    
    printf("Do you want to generate plot for avg_credit_debit vs quarters [y / n] : ");
    char two;
    scanf(" %c", &two);  // Note the space before %c
    if(two == 'y'){
        plot_avg_credit_debit(banks, numberOfFiles);
    }
    

    return;
}


int *minTxn_Summary(tree *banks, int numberOfFiles, char *argv[]) {
    int min_cred = INT_MAX;
    int min_deb = INT_MAX;
    int *min_store = (int *)malloc(sizeof(int) * 2);
    if (!min_store) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    // Find minimum credit and debit transactions
    for (int i = 0; i < numberOfFiles; i++) {
        root *rootnode = *banks + i;
        // Traverse all credit transactions
        for (int j = 1; j <= 4; j++) {
            amount *ptr = rootnode->credit->year->qArr[j - 1]->amounts;
            while (ptr) {
                if (ptr->amount < min_cred) {
                    min_cred = ptr->amount;                 
                }
                ptr = ptr->next;
            }
        }
        // Traverse all debit transactions
        for (int j = 1; j <= 4; j++) {
            amount *ptr = rootnode->debit->year->qArr[j - 1]->amounts;
            while (ptr) {
                if (ptr->amount < min_deb) {
                    min_deb = ptr->amount;
                }
                ptr = ptr->next;
            }
        }
    }
    min_store[0] = min_cred;
    min_store[1] = min_deb;
    return min_store;
}

int *maxTxn_Summary(tree *banks, int numberOfFiles, char *argv[]) {
    int max_cred = -1;
    int max_deb = -1;
    int *max_store = (int *)malloc(sizeof(int) * 2);
    if (!max_store) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    // Find maximum credit and debit transactions
    for (int i = 0; i < numberOfFiles; i++) {
        root *rootnode = *banks + i;
        // Traverse all credit transactions
        for (int j = 1; j <= 4; j++) {
            amount *ptr = rootnode->credit->year->qArr[j - 1]->amounts;
            while (ptr) {
                if (ptr->amount > max_cred) {
                    max_cred = ptr->amount;
                }
                ptr = ptr->next;
            }
        }
        // Traverse all debit transactions
        for (int j = 1; j <= 4; j++) {
            amount *ptr = rootnode->debit->year->qArr[j - 1]->amounts;
            while (ptr) {
                if (ptr->amount > max_deb) {
                    max_deb = ptr->amount;
                }
                ptr = ptr->next;
            }
        }
    }
    max_store[0] = max_cred;
    max_store[1] = max_deb;
    return max_store;
}
