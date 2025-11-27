#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>




char id[50];
char password[50];

void createUser() {
    char new_id[50], new_pass[50];
    FILE *f = fopen("users.txt", "a");
    if (!f) {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    printf("Enter New Username: ");
    scanf(" %[^\n]", new_id);

    printf("Enter New Password: ");
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r') {  // jusqu'à ENTER
        if (ch == 8 && i > 0) {       // backspace
            i--;
            printf("\b \b");
        } else if (ch != 8) {
            new_pass[i++] = ch;
            printf("*");
        }
    }
    new_pass[i] = '\0';

    fprintf(f, "%s,%s\n", new_id, new_pass);
    fclose(f);

    printf("\nUser Created Successfully!\n");
}



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
}


void resetPassword() {
    char target_id[50];
    printf("Enter Your Username: ");
    scanf(" %[^\n]", target_id); /*lit tous les caracteres(^), sauf le \n*/

    FILE *f = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!f || !temp) {
        printf("Error opening file.\n");
        return;
    }

    char file_id[50], file_pass[50];
    int found = 0;

    while (fscanf(f, "%[^,],%s\n", file_id, file_pass) != EOF) {
        if (strcmp(file_id, target_id) == 0) {
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
            fprintf(temp, "%s,%s\n", file_id, new_pass);
            printf("\nPassword Updated Successfully!\n");
        } else {
            fprintf(temp, "%s,%s\n", file_id, file_pass);
        }
    }

    fclose(f);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (!found) {
        printf("\nUser not found!\n");
    }
}


int main() {
    int choice;
    while (1) {
        printf("\n--- User System ---\n");
        printf("1. Create User\n");
        printf("2. Login\n");
        printf("3. Reset Password\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: createUser(); break;
            case 2: login(); break;
            case 3: resetPassword(); break;
            case 4: exit(0);
            default: printf("Invalid Choice!\n");
        }
    }
     system("CLS");
    return 0;
}

