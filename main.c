#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int digit(char);
long int romanToDecimal(char[]);
void decimalToWords(long int, char*);
void numberToWords(int, char*);
void numberToWordsUnder20(int, char*);
void numberToWordsTens(int, char*);

int main() {
    FILE *inputFile, *outputFile;
    char inputLine[1000], roman1[100], roman2[100], operator;
    long int num1, num2, result;
    char words[1000];

    // Open input file for reading
    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening input file\n");
        return 1;
    }

    // Open output file for writing
    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening output file\n");
        fclose(inputFile);
        return 1;
    }

    // Read and process each line from the input file
    while (fgets(inputLine, sizeof(inputLine), inputFile) != NULL) {
        // Remove newline character from the input line if present
        inputLine[strcspn(inputLine, "\n")] = '\0';

        // Clear buffers for safety
        memset(roman1, 0, sizeof(roman1));
        memset(roman2, 0, sizeof(roman2));
        memset(words, 0, sizeof(words));

        // Parse the input line into two Roman numerals and an operator
        if (sscanf(inputLine, "%99s %c %99s", roman1, &operator, roman2) != 3) {
            fprintf(outputFile, "Error parsing input expression: %s\n", inputLine);
            printf("Error parsing input expression: %s\n", inputLine);
            continue;
        }

        // Convert Roman numerals to decimal
        num1 = romanToDecimal(roman1);
        num2 = romanToDecimal(roman2);

        // Check if the Roman numerals are valid
        if (num1 == -1 || num2 == -1) {
            fprintf(outputFile, "Invalid Roman numeral(s): %s\n", inputLine);
            printf("Invalid Roman numeral(s): %s\n", inputLine);
            continue;
        }

        // Perform the arithmetic operation based on hypothetical rules
        switch (operator) {
            case '+':
                // Standard addition
                result = num1 + num2;
                printf("%ld %c %ld = %ld\n", num1, operator, num2, result);
                break;
            case '-':
                // Simulate subtraction behaving like addition
                if (num1 >= num2) {
                    result = num1 - (-num2); // Adding the negation of num2
                } else {
                    result = num2 - (-num1); // Adding the negation of num1
                }
                printf("%ld %c %ld = %ld\n", num1, operator, num2, result);
                break;
            case '*':
                // Combine parts of the Roman numerals
                result = (num1 / 1000) * 1000 + (num2 / 1000) * 1000 + (num1 % 1000) + (num2 % 1000); // Example of combining
                printf("%ld %c %ld = %ld\n", num1, operator, num2, result);
                break;
            default:
                fprintf(outputFile, "Unsupported operator: %c in line: %s\n", operator, inputLine);
                printf("Unsupported operator: %c in line: %s\n", operator, inputLine);
                continue;
        }

        // Convert the result to words
        decimalToWords(result, words);

        // Write the result to the output file
        fprintf(outputFile, "%s\n", words);
    }

    // Close the files
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

int digit(char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return -1; // Invalid character
    }
}

long int romanToDecimal(char roman[]) {
    long int total = 0;
    int i = 0;
    while (roman[i]) {
        if (digit(roman[i]) < 0) return -1; // Invalid Roman numeral
        if (i + 1 < strlen(roman) && digit(roman[i]) < digit(roman[i + 1])) {
            total += digit(roman[i + 1]) - digit(roman[i]);
            i += 2; // Skip the next character
        } else {
            total += digit(roman[i]);
            i++;
        }
    }
    return total;
}

void decimalToWords(long int n, char* result) {
    result[0] = '\0';  // Clear the result string
    if (n < 0) {
        strcat(result, "Negative ");
        n = -n;  // Make n positive for processing
    }
    if (n == 0) {
        strcat(result, "Zero");
    } else {
        numberToWords(n, result);
    }

    // Capitalize the first letter of the result
    if (result[0] != '\0') {
        result[0] = toupper(result[0]);
    }
}

void numberToWords(int n, char* result) {
    if (n >= 1000000000) {
        numberToWords(n / 1000000000, result);
        strcat(result, " Billion");
        if (n % 1000000000 != 0) {
            strcat(result, " ");
        }
        numberToWords(n % 1000000000, result);
    } else if (n >= 1000000) {
        numberToWords(n / 1000000, result);
        strcat(result, " Million");
        if (n % 1000000 != 0) {
            strcat(result, " ");
        }
        numberToWords(n % 1000000, result);
    } else if (n >= 1000) {
        numberToWords(n / 1000, result);
        strcat(result, " Thousand");
        if (n % 1000 != 0) {
            strcat(result, " ");
        }
        numberToWords(n % 1000, result);
    } else if (n >= 100) {
        numberToWords(n / 100, result);
        strcat(result, " Hundred");
        if (n % 100 != 0) {
            strcat(result, " ");
        }
        numberToWords(n % 100, result);
    } else if (n >= 20) {
        numberToWordsTens(n / 10, result);
        strcat(result, " ");
        numberToWordsUnder20(n % 10, result);
    } else {
        numberToWordsUnder20(n, result);
    }
}

void numberToWordsUnder20(int n, char* result) {
    const char* words[] = {
        "Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
        "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen",
        "Seventeen", "Eighteen", "Nineteen"
    };
    strcat(result, words[n]);
}

void numberToWordsTens(int n, char* result) {
    const char* tens[] = {
        "", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"
    };
    strcat(result, tens[n]);
}

void combineRomanNumerals(long int num1, long int num2, long int* result1, long int* result2) {
    // Logic to combine parts of the Roman numerals (thousands, hundreds, etc.)
    long int thousands1 = num1 / 1000;
    long int hundreds1 = (num1 % 1000) / 100;
    long int tens1 = (num1 % 100) / 10;
    long int ones1 = num1 % 10;

    long int thousands2 = num2 / 1000;
    long int hundreds2 = (num2 % 1000) / 100;
    long int tens2 = (num2 % 100) / 10;
    long int ones2 = num2 % 10;

    // Combine values (simplified approach)
    *result1 = (thousands1 + thousands2) * 1000 + (hundreds1 + hundreds2) * 100 + (tens1 + tens2) * 10 + (ones1 + ones2);
    *result2 = *result1;
}
