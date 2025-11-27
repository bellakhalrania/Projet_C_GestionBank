#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

char email[100];
char password[50];




int isValidEmail(char *email) {
    int at_count = 0;
    int dot_after_at = 0;
    char *at_ptr = NULL;

    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            at_count++;
            at_ptr = &email[i];
        }
    }

    if (at_count != 1) return 0; // doit contenir exactement un '@'

    // vérifier qu'il y a un '.' après le '@'
    for (char *p = at_ptr; *p != '\0'; p++) {
        if (*p == '.') {
            dot_after_at = 1;
            break;
        }
    }

    return dot_after_at; // retourne 1 si valide, 0 sinon
}



int isEmailExist(char *check_email) {
    FILE *f = fopen("users.txt", "r");
    if (!f) return 0;

    char file_email[100], file_pass[50];
    while (fscanf(f, "%[^,],%s\n", file_email, file_pass) != EOF) {
        if (strcmp(check_email, file_email) == 0) {
            fclose(f);
            return 1; // email existe déjà
        }
    }

    fclose(f);
    return 0; // email libre
}

void createUser() {
    char new_email[100], new_pass[50];

    printf("Enter Your Email: ");
    scanf(" %[^\n]", new_email);
if (!isValidEmail(new_email)) {
    printf("Invalid email format! Try again.\n");
    getch();
    return;
}

    if (isEmailExist(new_email)) {
        printf("Email already registered! Try another.\n");
        getch();
        return;
    }

    printf("Enter Your Password: ");
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r') {
        if (ch == 8 && i > 0) {
            i--;
            printf("\b \b");
        } else if (ch != 8) {
            new_pass[i++] = ch;
            printf("*");
        }
    }
    new_pass[i] = '\0';

    FILE *f = fopen("users.txt", "a");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(f, "%s,%s\n", new_email, new_pass);
    fclose(f);

    printf("\nUser Created Successfully!\n");
    getch();
}



/*
int login() {
    printf("Enter Username: ");
    scanf(" %[^\n]", id);

    printf("Enter Password: ");
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r') {
        if (ch == 8 && i > 0) {
            i--;
            printf("\b \b");
        } else if (ch != 8) {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';

    FILE *f = fopen("users.txt", "r");
    if (!f) {
        printf("No users registered yet!\n");
        return 0;
    }

    char file_id[50], file_pass[50];
    while (fscanf(f, "%[^,],%s\n", file_id, file_pass) != EOF) {
        if (strcmp(id, file_id) == 0 && strcmp(password, file_pass) == 0) {
            fclose(f);
            printf("\nLogin Successful!\n");
            return 1;
        }
    }

    fclose(f);
    printf("\nIncorrect Username or Password!\n");
    return 0;
}*/

int login() {
    printf("Enter Your Email: ");
    scanf(" %[^\n]", email);

    printf("Enter Your Password: ");
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r') {
        if (ch == 8 && i > 0) {
            i--;
            printf("\b \b");
        } else if (ch != 8) {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';

    FILE *f = fopen("users.txt", "r");
    if (!f) {
        printf("\nNo users registered yet!\n");
        return 0;
    }

    char file_email[100], file_pass[50];
    while (fscanf(f, "%[^,],%s\n", file_email, file_pass) != EOF) {
        if (strcmp(email, file_email) == 0 && strcmp(password, file_pass) == 0) {
            fclose(f);
            printf("\nLogin Successful!\n");
            getch();
            return 1;
        }
    }

    fclose(f);
    printf("\nIncorrect Email or Password!\n");
    return 0;
}



void resetPassword() {
    char target_email[100];
    printf("Enter Your Email: ");
    scanf(" %[^\n]", target_email);

    FILE *f = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!f || !temp) {
        printf("Error opening file.\n");
        return;
    }

    char file_email[100], file_pass[50];
    int found = 0;

    while (fscanf(f, "%[^,],%s\n", file_email, file_pass) != EOF) {
        if (strcmp(file_email, target_email) == 0) {
            found = 1;
            char new_pass[50];
            printf("Enter New Password: ");
            int i = 0;
            char ch;
            while ((ch = getch()) != '\r') {
                if (ch == 8 && i > 0) {
                    i--;
                    printf("\b \b");
                } else if (ch != 8) {
                    new_pass[i++] = ch;
                    printf("*");
                }
            }
            new_pass[i] = '\0';
            fprintf(temp, "%s,%s\n", file_email, new_pass);
            printf("\nPassword Updated Successfully!\n");
        } else {
            fprintf(temp, "%s,%s\n", file_email, file_pass);
        }
    }

    fclose(f);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (!found) {
        printf("\nEmail not found!\n");
    }
}



int main() {
    int choice;
    while (1) {
        system("CLS");
        system("color f6");
        printf("\n--- Welcome To BTH Banck ---\n");
        printf("1. Create User\n");
        printf("2. Login\n");
        //printf("3. Reset Password\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: createUser(); break;
            case 2:
                if (!login()) {
                    int ans;
                    printf("Do you want to reset your password? (1 = Yes, 0 = No): ");
                    scanf("%d", &ans);
                    if (ans == 1) resetPassword();
                }
                break;
            //case 3: resetPassword(); break;
            case 3: exit(0);
            default: printf("Invalid Choice!\n");
        }
    }
}


