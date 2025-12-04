#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credential.txt"

// ---------- GLOBAL VARIABLES ----------
char currentUser[20];
char currentRole[20];

// ---------- STRUCT ----------
struct Student {
    int roll;
    char name[50];
    float marks;
};

// ---------- FUNCTION DECLARATIONS ----------
int loginSystem();
void mainMenu();

void adminMenu();
void userMenu();
void staffMenu();
void guestMenu();

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

// ---------- MAIN ----------
int main() 
{
    if (loginSystem()) {
        mainMenu();
    } 
    else {
        printf("\nLogin Failed. Exiting...\n");
    }
    return 0;
}

// ---------- LOGIN SYSTEM (3 ATTEMPTS) ----------
int loginSystem() 
{
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[20];
    int attempts = 3;

    while (attempts > 0) 
    {
        printf("============ Login Screen ============\n");
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);

        FILE *fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp) {
            printf("Error: credential.txt not found!\n");
            return 0;
        }

        int loginSuccess = 0;

        while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) 
        {
            if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) 
            {
                strcpy(currentUser, fileUser);
                strcpy(currentRole, fileRole);
                fclose(fp);
                return 1;  // Login successful
            }
        }

        fclose(fp);
        attempts--;
        printf("\nInvalid credentials! Attempts left: %d\n\n", attempts);

        if (attempts == 0) {
            printf("Too many failed attempts. Access Denied.\n");
            return 0;
        }
    }

    return 0;
}

// ---------- MAIN MENU ----------
void mainMenu() 
{
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else if (strcmp(currentRole, "USER") == 0)
        userMenu();
    else
        guestMenu();
}

// ---------- ADMIN MENU ----------
void adminMenu() 
{
    int choice;

    do {
        printf("\n====== ADMIN MENU ======\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

// ---------- STAFF MENU ----------
void staffMenu() 
{
    int choice;

    do {
        printf("\n====== STAFF MENU ======\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

// ---------- USER MENU ----------
void userMenu() {
    printf("\n====== USER MENU ======\n");
    displayStudents();
}

// ---------- GUEST MENU ----------
void guestMenu() {
    printf("\n====== GUEST MENU ======\n");
    displayStudents();
}

// ---------- ADD STUDENT ----------
void addStudent() 
{
    FILE *fp = fopen(STUDENT_FILE, "a");
    struct Student st;

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll: ");
    scanf("%d", &st.roll);
    printf("Enter Name: ");
    scanf("%s", st.name);
    printf("Enter Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("Student Added Successfully!\n");
}

// ---------- DISPLAY ----------
void displayStudents() 
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;

    if (!fp) {
        printf("Student file not found!\n");
        return;
    }

    printf("\nRoll\tName\tMarks\n");
    printf("----------------------------\n");

    while (fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3)
        printf("%d\t%-10s\t%.2f\n", st.roll, st.name, st.marks);

    fclose(fp);
}

// ---------- SEARCH ----------
void searchStudent() 
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;
    int roll, found = 0;

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll to Search: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3) 
    {
        if (st.roll == roll) {
            printf("\nRecord Found:\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Record Not Found!\n");

    fclose(fp);
}

// ---------- UPDATE ----------
void updateStudent()
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    struct Student st;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll to Update: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3) 
    {
        if (st.roll == roll) {
            found = 1;
            printf("Enter New Name: ");
            scanf("%s", st.name);
            printf("Enter New Marks: ");
            scanf("%f", &st.marks);
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Updated Successfully!\n");
    else
        printf("Record Not Found!\n");
}

// ---------- DELETE ----------
void deleteStudent()
{
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    struct Student st;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll to Delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3)
    {
        if (st.roll == roll) {
            found = 1;
            continue;  
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Deleted Successfully!\n");
    else
        printf("Record Not Found!\n");
}
