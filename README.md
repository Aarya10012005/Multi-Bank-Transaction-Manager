# **Centralized Multi-Bank Account Transaction Search Engine**

### **Introduction**

Managing multiple bank accounts can be tedious, especially when analyzing transactions across different banks. This project provides a unified interface to consolidate, search, and analyze transactions from various bank accounts, saving time and effort.

---

### **Features**
- Consolidates transaction data from multiple bank accounts into a single interface.
- Performs operations such as:
  - Finding total debits/credits till a specific date.
  - Searching transactions based on filters like bank name, quarter, and amount range.
  - Generating summaries including average, minimum, and maximum transactions.
  - Visualizing transaction trends using graphs.
- Combines transaction data into a single output file for streamlined analysis.

---

### **Installation**

#### **Prerequisites**
- GCC Compiler
- gnuplot
- Basic knowledge of the command line

#### **Setup**
1. **Clone the Repository**
   ```bash
   git clone https://github.com/Aarya10012005/DSA-Mini-project.git
   cd DSA-Mini-project

2. **Compile the Code**  
   Use `gcc` to compile the source files:
   ```bash
   gcc -Wall main.c logic.c list.c -o multi_bank

3. **Prepare Input Files**  
   Place the bank statement files (CSV format) in a "bank_statements" directory. Ensure the files are formatted correctly, with columns such as:

   - Serial Number
   - Transaction Date
   - Value Date
   - Description
   - Credit/Debit (CR/DR)
   - Amount
   - Balance

4. Prepare a "Graphs" directory in your main directory. The graphs will get saved in this directory if the user wishes to download graphs.

5. **Run the Application**  
   Execute the program with the following command, passing your bank statement files (in CSV format) as arguments:
   ```bash
   ./multi_bank ./bank_statements/<file1.csv> ./bank_statements/<file2.csv> ...

---

### **Usage**

#### **Menu Options**
1. **Summary**  
   Displays transaction statistics such as:
   - Transaction counts for each quarter.
   - Average debit and credit amounts per quarter.
   - Minimum and maximum transactions across all banks.

2. **Search**  
   Allows users to search for transactions based on:
   - Bank name.
   - Quarter (Q1-Q4).
   - Transaction type (Credit/Debit/Both).
   - Amount range.

3. **Terminate**  
   Exits the application.

#### **Output**
- The combined transaction data from all input files will be saved in `./bank_statements/collective.csv`.
- Summaries and graphs are displayed during execution.

---

### **File Structure**

- **`main.c`**: Entry point of the application. Handles menu navigation and calls relevant functions.  
- **`m-tree.h`**: Defines the data structures and function prototypes.  
- **`logic.c`**: Implements core functionalities like data structure initialization, population, and search.  
- **`list.c`**: Manages linked list operations such as transaction insertion and node creation.  
- **`bank_statements/`**: Directory for input/output CSV files.

---

### **Technologies**

- **Programming Language:** C  
- **Data Structures:** Multi-node trees, linked lists, arrays  
- **Visualization:** Graph generation using Gnuplot (if installed)

---

### **Testing**

Run the application with sample CSV files provided in the repository and test features like transaction search, summary generation, and graph visualization.

---

### **Known Issues**

- Requires properly formatted CSV files for accurate data parsing.
- Absence of UI.
- Limited support for currency and localization.

---

### **Authors**

- **Aarya Nandal** (612303124)  
- **Manas Mulay** (612303121)  

---

### **Contact**

For questions or feedback, reach out to:
- **Aarya Nandal:** [nandalas23.comp@coeptech.ac.in](mailto:nandalas23.comp@coeptech.ac.in)  
- **Manas Mulay:** [mulaymp23.comp@coeptech.ac.in](mailto:mulaymp23.comp@coeptech.ac.in)
