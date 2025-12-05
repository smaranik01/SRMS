#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int rollno;
    char name[50];
    float marks;
};

char currentUserRole[10];
char currentUser[50];

// Function declarations
int loginSystem();
void mainMenu();
void adminMenu();
void userMenu();
void guestMenu();
void addStudent();
void viewStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("Login failed. Exiting program.\n");
    }
    return 0;
}

int loginSystem() {
    char username[20], password[20];
    char fileuser[20], filepass[20], fileRole[10];

    printf("=== Login System ===\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");  // FIXED
    if (!fp) {
        printf("Error: credentials.txt not found!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileuser, filepass, fileRole) == 3) {
        if (strcmp(username, fileuser) == 0 && strcmp(password, filepass) == 0) {
            strcpy(currentUserRole, fileRole);
            strcpy(currentUser, fileuser);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void mainMenu() {  // FIXED (correct function name)
    if (strcmp(currentUserRole, "ADMIN") == 0) {
        adminMenu();
    } else if (strcmp(currentUserRole, "STAFF") == 0 || strcmp(currentUserRole, "USER") == 0) {
        userMenu();
    } else {
        guestMenu();
    }
}

void adminMenu() {
    int choice;
    do {
        printf("\n=== ADMIN MENU ===\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return;   // FIXED exit from menu
            default: printf("Invalid choice.\n");
        }
    } while (1);
}

void userMenu() {
    int choice;
    do {
        printf("\n=== STAFF/USER MENU ===\n");
        printf("1. View Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid choice.\n");
        }
    } while (1);
}

void guestMenu() {
    int choice;
    do {
        printf("\n=== GUEST MENU ===\n");
        printf("1. View Students\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) viewStudents();
        else if (choice == 2) return;
        else printf("Invalid choice.\n");

    } while (1);
}

void addStudent() {
    FILE *fp = fopen(STUDENT_FILE, "a");
    struct Student st;

    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter Roll No: ");
    scanf("%d", &st.rollno);
    printf("Enter Name: ");
    scanf("%s", st.name);
    printf("Enter Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.rollno, st.name, st.marks);

    fclose(fp);
    printf("Student added successfully.\n");
}

void viewStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;

    if (!fp) {
        printf("No student records found.\n");
        return;
    }

    printf("\n=== STUDENT RECORDS ===\n");
    printf("RollNo\tName\tMarks\n");

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        printf("%d\t%s\t%.2f\n", st.rollno, st.name, st.marks);
    }

    fclose(fp);
}

void searchStudent() {
    int rollno, found = 0;
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter Roll No to search: ");
    scanf("%d", &rollno);

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        if (st.rollno == rollno) {
            printf("Found: %d %s %.2f\n", st.rollno, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student with Roll No %d not found.\n", rollno);

    fclose(fp);
}

void updateStudent() {
    int rollno;
    int found = 0;

    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter Roll No to update: ");
    scanf("%d", &rollno);

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        if (st.rollno == rollno) {
            printf("Current: %d %s %.2f\n", st.rollno, st.name, st.marks);
            printf("Enter New Name: ");
            scanf("%s", st.name);
            printf("Enter New Marks: ");
            scanf("%f", &st.marks);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", st.rollno, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (!found)
        printf("Student not found.\n");
    else
        printf("Record updated.\n");
}

void deleteStudent() {
    int rollno, found = 0;
    struct Student st;

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter Roll No to delete: ");
    scanf("%d", &rollno);

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        if (st.rollno != rollno)
            fprintf(temp, "%d %s %.2f\n", st.rollno, st.name, st.marks);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (!found)
        printf("Student not found.\n");
    else
        printf("Student deleted successfully.\n");
}