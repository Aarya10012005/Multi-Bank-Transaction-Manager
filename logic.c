#include "m-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// creating the data structure
void init(tree *bank, int numberOfFiles, char *bankName) {
    root rootNode;
    for(int i = 0; i < numberOfFiles; i++) {
        rootNode = (*bank)[i];
        // mallocing the root node
        // (*bank) = (tree)malloc(sizeof(root));

        // assigning bank name to the root node element
        strcpy(rootNode.bankName, bankName[i + 1]); 

        // mallocing 2 children of root node
        rootNode.credit = (transaction *)malloc(sizeof(transaction));
        transaction *credit = rootNode.credit;
        strcpy(credit->str, "Cr");
        rootNode.debit = (transaction *)malloc(sizeof(transaction));
        transaction *debit = rootNode.debit;
        strcpy(debit->str, "Dr");     

        /*
        // callocing year and month nodes so that each points to null
        credit->year1 = (year *)calloc(1, sizeof(year));
        credit->year2 = (year *)calloc(1, sizeof(year));
        debit->year1 = (year *)calloc(1, sizeof(year));
        debit->year2 = (year *)calloc(1, sizeof(year));
        */

        // mallocing year node
        credit->year1 = (year *)malloc(sizeof(year));
        strcpy(credit->year1->year, "2023");
        credit->year2 = (year *)malloc(sizeof(year));
        strcpy(credit->year2->year, "2024");
        debit->year1 = (year *)malloc(sizeof(year));
        strcpy(debit->year1->year, "2023");
        debit->year2 = (year *)malloc(sizeof(year));
        strcpy(debit->year2->year, "2024");


        // mallocing quarter nodes
        credit->year1->quarter1 = (quarter *)malloc(sizeof(quarter));
        credit->year1->quarter1->amounts = NULL;
        strcpy(credit->year1->quarter1, "Quarter 1");
        credit->year1->quarter2 = (quarter *)malloc(sizeof(quarter));
        credit->year1->quarter2->amounts = NULL;
        strcpy(credit->year1->quarter2, "Quarter 2");
        credit->year1->quarter3 = (quarter *)malloc(sizeof(quarter));
        credit->year1->quarter3->amounts = NULL;
        strcpy(credit->year1->quarter3, "Quarter 3");
        credit->year1->quarter4 = (quarter *)malloc(sizeof(quarter));
        credit->year1->quarter4->amounts = NULL;
        strcpy(credit->year1->quarter4, "Quarter 4");

        credit->year2->quarter1 = (quarter *)malloc(sizeof(quarter));
        credit->year2->quarter1->amounts = NULL;
        strcpy(credit->year2->quarter1, "Quarter 1");
        credit->year2->quarter2 = (quarter *)malloc(sizeof(quarter));
        credit->year2->quarter2->amounts = NULL;
        strcpy(credit->year2->quarter2, "Quarter 2");
        credit->year2->quarter3 = (quarter *)malloc(sizeof(quarter));
        credit->year2->quarter3->amounts = NULL;
        strcpy(credit->year2->quarter3, "Quarter 3");
        credit->year2->quarter4 = (quarter *)malloc(sizeof(quarter));
        credit->year2->quarter4->amounts = NULL;
        strcpy(credit->year2->quarter4, "Quarter 4");
        
        debit->year1->quarter1 = (quarter *)malloc(sizeof(quarter));
        debit->year1->quarter1->amounts = NULL;
        strcpy(debit->year1->quarter1, "Quarter 1");
        debit->year1->quarter2 = (quarter *)malloc(sizeof(quarter));
        debit->year1->quarter2->amounts = NULL;
        strcpy(debit->year1->quarter2, "Quarter 2");
        debit->year1->quarter3 = (quarter *)malloc(sizeof(quarter));
        debit->year1->quarter3->amounts = NULL;
        strcpy(debit->year1->quarter3, "Quarter 3");
        debit->year1->quarter4 = (quarter *)malloc(sizeof(quarter));
        debit->year1->quarter4->amounts = NULL;
        strcpy(debit->year1->quarter4, "Quarter 4");

        debit->year2->quarter1 = (quarter *)malloc(sizeof(quarter));
        debit->year2->quarter1->amounts = NULL;
        strcpy(debit->year2->quarter1, "Quarter 1");
        debit->year2->quarter2 = (quarter *)malloc(sizeof(quarter));
        debit->year2->quarter2->amounts = NULL;
        strcpy(debit->year2->quarter2, "Quarter 2");
        debit->year2->quarter3 = (quarter *)malloc(sizeof(quarter));
        debit->year2->quarter3->amounts = NULL;
        strcpy(debit->year2->quarter3, "Quarter 3");
        debit->year2->quarter4 = (quarter *)malloc(sizeof(quarter));
        debit->year2->quarter4->amounts = NULL;
        strcpy(debit->year2->quarter4, "Quarter 4");
    }
    return;


    /*/
    // mallocing the root node
    (*bank) = (tree)malloc(sizeof(root));

    // mallocing 2 children of root node
    (*root)->left = (transaction *)malloc(sizeof(transaction));
    transaction *credit = (*root)->left;
    (*root)->right = (transaction *)malloc(sizeof(transaction));
    transaction *debit = (*root)->right;
    */
    /*
    // mallocing year and month nodes
    credit->year1 = (year *)malloc(sizeof(year));
    credit->year2 = (year *)malloc(sizeof(year));
    debit->year1 = (year *)malloc(sizeof(year));
    debit->year2 = (year *)malloc(sizeof(year));

    // assigning NULL to every year element
    credit->year1->jan = credit->year1->feb = credit->year1->march = credit->year1->april = credit->year1->may = credit->year1->june = credit->year1->july = credit->year1->aug = credit->year1->sept = credit->year1->oct = credit->year1->nov = credit->year1->dec = NULL;
    credit->year2->jan = credit->year2->feb = credit->year2->march = credit->year2->april = credit->year2->may = credit->year2->june = credit->year2->july = credit->year2->aug = credit->year2->sept = credit->year2->oct = credit->year2->nov = credit->year2->dec = NULL;
    debit->year1->jan = debit->year1->feb = debit->year1->march = debit->year1->april = debit->year1->may = debit->year1->june = debit->year1->july = debit->year1->aug = debit->year1->sept = debit->year1->oct = debit->year1->nov = debit->year1->dec = NULL;
    debit->year2->jan = debit->year2->feb = debit->year2->march = debit->year2->april = debit->year2->may = debit->year2->june = debit->year2->july = debit->year2->aug = debit->year2->sept = debit->year2->oct = debit->year2->nov = debit->year2->dec = NULL;
    */

    /*
    // callocing year and month nodes so that each points to null
    credit->year1 = (year *)calloc(1, sizeof(year));
    credit->year2 = (year *)calloc(1, sizeof(year));
    debit->year1 = (year *)calloc(1, sizeof(year));
    debit->year2 = (year *)calloc(1, sizeof(year));
    */
}

void destroy(tree *root) {
    transaction *left, *right;
}