#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "records.bin"

// Structure definition
struct Student {
    int id;
    char name[50];
    float marks;
};

// Store n records
void storeRecords(int n) {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    struct Student s;
    for (int i = 0; i < n; i++) {
        printf("\nEnter details for record %d:\n", i + 1);
        printf("ID: ");
        scanf("%d", &s.id);
        printf("Name: ");
        scanf("%s", s.name);
        printf("Marks: ");
        scanf("%f", &s.marks);

        fwrite(&s, sizeof(struct Student), 1, fp);
    }

    fclose(fp);
    printf("\nRecords stored successfully!\n");
}
void getMthRecord(int m) {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Move to (m-1)th offset from beginning
    int result = fseek(fp, (m - 1) * sizeof(struct Student), SEEK_SET);
    if (result != 0) {
        printf("fseek failed!\n");
        fclose(fp);
        return;
    }

    struct Student s;
    if (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        printf("\n--- Record %d ---\n", m);
        printf("ID    : %d\n", s.id);
        printf("Name  : %s\n", s.name);
        printf("Marks : %.2f\n", s.marks);
    } else {
        printf("Record %d not found!\n", m);
    }

    fclose(fp);
}

// ─── Delete mth record ─────────────────────────────────────────────
// Strategy: Copy all records EXCEPT the mth one into a temp file,
//           then replace the original file with the temp file.
void deleteRecord(int m) {
    FILE *fp    = fopen(FILENAME, "rb");
    FILE *temp  = fopen("temp.bin", "wb");

    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    struct Student s;
    int recordNum = 1;
    int deleted = 0;

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (recordNum == m) {
            // Skip this record (effectively deleting it)
            deleted = 1;
        } else {
            fwrite(&s, sizeof(struct Student), 1, temp);
        }
        recordNum++;
    }

    fclose(fp);
    fclose(temp);

    if (!deleted) {
        printf("Record %d not found!\n", m);
        remove("temp.bin");
        return;
    }

    // Replace original file with temp file
    remove(FILENAME);
    rename("temp.bin", FILENAME);

    printf("Record %d deleted successfully!\n", m);
}

// ─── Display all records ───────────────────────────────────────────
void displayAll() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    struct Student s;
    int i = 1;
    printf("\n===== All Records =====\n");
    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        printf("\nRecord %d:\n", i++);
        printf("  ID    : %d\n", s.id);
        printf("  Name  : %s\n", s.name);
        printf("  Marks : %.2f\n", s.marks);
    }

    fclose(fp);
}

// ─── Main ──────────────────────────────────────────────────────────
int main() {
    int choice, n, m;

    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Store Records\n");
        printf("2. Get Mth Record\n");
        printf("3. Delete Mth Record\n");
        printf("4. Display All Records\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter number of records: ");
                scanf("%d", &n);
                storeRecords(n);
                break;
            case 2:
                printf("Enter record number to retrieve: ");
                scanf("%d", &m);
                getMthRecord(m);
                break;
            case 3:
                printf("Enter record number to delete: ");
                scanf("%d", &m);
                deleteRecord(m);
                break;
            case 4:
                displayAll();
                break;
            case 5:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}