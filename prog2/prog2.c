#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FILENAME "students.txt"

// ─── Structure Definition ───────────────────────────
struct Student {
    int    id;
    char   name[50];
    int    age;
    float  marks;
};

// ─── Function 1: Write array of structures to file in ASCII format ───
void writeToFile(struct Student s[], int n) {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("Error: Cannot open file!\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d %s %d %.2f\n", s[i].id, s[i].name, s[i].age, s[i].marks);
    }

    fclose(fp);
    printf(">> %d records written to '%s'\n\n", n, FILENAME);
}

// ─── Function 2: Build seek position array ───────────
int buildSeekArray(long seekArr[], int max) {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Error: Cannot open file!\n");
        exit(1);
    }

    char line[200];
    int  count = 0;

    while (count < max) {
        seekArr[count] = ftell(fp);              // store position BEFORE reading
        if (fgets(line, sizeof(line), fp) == NULL)
            break;                               // end of file
        count++;
    }

    fclose(fp);
    printf(">> Seek positions built for %d records\n\n", count);
    return count;
}

// ─── Function 3: Display record at given seek position ───
void displayRecord(long position) {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Error: Cannot open file!\n");
        exit(1);
    }

    fseek(fp, position, SEEK_SET);              // jump to the position

    struct Student s;
    if (fscanf(fp, "%d %s %d %f", &s.id, s.name, &s.age, &s.marks) == 4) {
        printf("  +---------------------------+\n");
        printf("  | ID    : %-18d|\n", s.id);
        printf("  | Name  : %-18s|\n", s.name);
        printf("  | Age   : %-18d|\n", s.age);
        printf("  | Marks : %-18.2f|\n", s.marks);
        printf("  +---------------------------+\n");
    } else {
        printf("  No record found at position %ld\n", position);
    }

    fclose(fp);
}

// ─── Main ────────────────────────────────────────────
int main() {
    // Sample array of structures
    struct Student students[] = {
        {101, "Lalit",   20, 88.50},
        {102, "Ravi",    21, 76.00},
        {103, "Priya",   20, 92.75},
        {104, "Anil",    22, 65.30},
        {105, "Sneha",   21, 83.40}
    };

    int n = sizeof(students) / sizeof(students[0]);

    // Step 1: Write structures to file in ASCII format
    writeToFile(students, n);

    // Step 2: Build the seek position array
    long seekArr[MAX];
    int  count = buildSeekArray(seekArr, MAX);

    // Print all seek positions
    printf("Seek positions of each record:\n");
    for (int i = 0; i < count; i++) {
        printf("  Record %d  -->  byte position: %ld\n", i + 1, seekArr[i]);
    }
    printf("\n");

    // Step 3: Display all records using seek positions
    printf("Displaying all records using seek positions:\n\n");
    for (int i = 0; i < count; i++) {
        printf("Record %d (position = %ld):\n", i + 1, seekArr[i]);
        displayRecord(seekArr[i]);
        printf("\n");
    }

    // Step 4: Random access — user picks a record
    int choice;
    printf("Enter record number to access directly (1 to %d): ", count);
    scanf("%d", &choice);

    if (choice >= 1 && choice <= count) {
        printf("\nDirectly accessing Record %d:\n", choice);
        displayRecord(seekArr[choice - 1]);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}