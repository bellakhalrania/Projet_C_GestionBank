#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

char email[100];
char password[50];
int menuChoice = 0; // 1 = admin, 2 = user

struct record {
    char name[25];
    int account;
    char phone[15];
    char address[25];
    char email[35];
    char citiz[20];
    double blnc;
    char UserID[10];
} rec;

// prototypes
int login(int role);
void createUser();
void resetPassword();
void adminView();
void userView();
void option();
void add();
void edit();
void del();
void about();
void menu();
void transaction();
void gotoxy(int x, int y);
int isValidEmail(char *email);
int isEmailExist(char *check_email);

// gotoxy pour Windows
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// fonctions fictives pour le moment
void menu(){printf("Menu placeholder\n"); getch();}
void transaction(){printf("Transaction placeholder\n"); getch();}
void edit(){printf("Edit placeholder\n"); getch();}
void del(){printf("Delete placeholder\n"); getch();}
void about(){printf("About placeholder\n"); getch();}

// Vérification simple email
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
    if (at_count != 1) return 0;

    for (char *p = at_ptr; *p != '\0'; p++) {
        if (*p == '.') {
            dot_after_at = 1;
            break;
        }
    }

    return dot_after_at;
}

// Vérifie si email déjà existant
int isEmailExist(char *check_email) {
    FILE *f = fopen("users.txt", "r");
    if (!f) return 0;

    char file_email[100], file_pass[50], file_role[10];
    while (fscanf(f, "%[^,],%[^,],%s\n", file_email, file_pass, file_role) != EOF) {
        if (strcmp(check_email, file_email) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// Création utilisateur
void createUser() {
    char new_email[100], new_pass[50], role[10];

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

    printf("\nEnter Role (admin/user): ");
    scanf(" %s", role);

    FILE *f = fopen("users.txt", "a");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(f, "%s,%s,%s\n", new_email, new_pass, role);
    fclose(f);

    printf("\nUser Created Successfully!\n");
    getch();
}

// Login avec rôle
int login(int role) {
    char file_email[100], file_pass[50], file_role[10];

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

    int loginSuccess = 0;
    while (fscanf(f, "%[^,],%[^,],%s\n", file_email, file_pass, file_role) != EOF) {
        if (strcmp(email, file_email) == 0 && strcmp(password, file_pass) == 0) {
            if ((role == 1 && strcmp(file_role, "admin") == 0) ||
                (role == 2 && strcmp(file_role, "user") == 0)) {
                loginSuccess = 1;
                break;
            } else {
                printf("\nThis account does not match the selected role!\n");
                fclose(f);
                return 0;
            }
        }
    }

    fclose(f);

    if (loginSuccess) {
        printf("\nLogin Successful!\n");
        getch();
        return 1;
    } else {
        printf("\nIncorrect Email or Password!\n");
        return 0;
    }
}

// Reset password
void resetPassword() {
    char target_email[100];
    printf("Enter Your Email (or 0 to return to menu): ");
    scanf(" %[^\n]", target_email);

    // si l'utilisateur tape 0 -> retour au menu
    if (strcmp(target_email, "0") == 0) {
        printf("Returning to menu...\n");
        getch();
        return; // quitte la fonction
    }

    FILE *f = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!f || !temp) {
        printf("Error opening file.\n");
        getch();
        return;
    }

    char file_email[100], file_pass[50], file_role[10];
    int found = 0;

    while (fscanf(f, "%[^,],%[^,],%s\n", file_email, file_pass, file_role) != EOF) {
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
            fprintf(temp, "%s,%s,%s\n", file_email, new_pass, file_role);
            printf("\nPassword Updated Successfully!\n");
        } else {
            fprintf(temp, "%s,%s,%s\n", file_email, file_pass, file_role);
        }
    }

    fclose(f);
    fclose(temp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (!found) {
        printf("\nAccount not found!\n");
        getch();  // pause pour que l'utilisateur voie le message
        return;   // retourne au menu principal
    }
}



// Admin et User Views
void adminView() {
    system("CLS");
    gotoxy(48, 4);
    printf("WELCOME TO MAIN MENU (ADMIN)");
    gotoxy(44, 8);
    printf("[1] View Customer Accounts");
    gotoxy(44, 9);
    printf("[2] Customer Account Registration");
    gotoxy(44, 10);
    printf("[3] Edit Customer Account");
    gotoxy(44, 11);
    printf("[4] Delete Customer Account");
    gotoxy(44, 12);
    printf("[5] Search Customer Account");
    gotoxy(44, 13);
    printf("[6] Transaction");
    gotoxy(44, 14);
    printf("[7] Log Out");
    gotoxy(44, 15);
    printf("[8] About Us");
    gotoxy(43, 20);
    printf("Please Enter Your Choice [1-8]: ");
    option();
}

void userView() {
    system("CLS");
    gotoxy(48, 4);
    printf("WELCOME TO MAIN MENU (USER)");
    // tu peux ajouter les options spécifiques au user ici
}

// Option menu
void option() {
    int i;
    scanf("%d", &i);
    system("CLS");
    switch (i) {
        case 1: adminView(); break;
        case 2: add(); break;
        case 3: edit(); break;
        case 4: del(); break;
        case 5: menu(); break; // search placeholder
        case 6: transaction(); break;
        //case 7: main(); break; // logout
        case 7: about(); break;
        default: menu(); break;
    }
}



void add() {
    char c;
    struct record rec;
    FILE *f_bin, *f_users;

    f_bin = fopen("record.bin", "ab"); // binaire pour infos clients
    if (!f_bin) {
        printf("Error opening record file.\n");
        return;
    }

    int i = 0;
    do {
        system("CLS");
        gotoxy(24, 4);
        printf("\t\t\tCUSTOMER ACCOUNT REGISTRATION");

        gotoxy(36, 8);
        printf("[1] Enter Name           : ");
        scanf(" %[^\n]", rec.name);

        gotoxy(36, 9);
        printf("[2] Enter Account Number : ");
        scanf(" %d", &rec.account);

        gotoxy(36, 10);
        printf("[3] Enter Phone Number   : ");
        scanf(" %[^\n]", rec.phone);

        gotoxy(36, 11);
        printf("[4] Enter Address        : ");
        scanf(" %[^\n]", rec.address);

        gotoxy(36, 12);
        printf("[5] Enter Email          : ");
        scanf(" %[^\n]", rec.email);

        gotoxy(36, 13);
        printf("[6] Enter Citizenship No.: ");
        scanf(" %[^\n]", rec.citiz);

        gotoxy(36, 14);
        printf("[7] Enter Amount To Deposit: $");
        scanf(" %lf", &rec.blnc);

        // Génération aléatoire UserID
        for (int r = 0; r < 10; r++) {
            rec.UserID[r] = '0' + (rand() % 10); // caractères '0'-'9'
        }

        // Écriture dans record binaire
        fwrite(&rec, sizeof(rec), 1, f_bin);

        // --- Ajout login par défaut pour le user ---
        char default_pass[50] = "123456"; // mot de passe par défaut
        if (!isEmailExist(rec.email)) {
            f_users = fopen("users.txt", "a");
            if (f_users) {
                fprintf(f_users, "%s,%s,user\n", rec.email, default_pass);
                fclose(f_users);
            } else {
                printf("Error opening users file.\n");
            }
        }

        gotoxy(38, 17);
        printf("CUSTOMER ACCOUNT REGISTRATION SUCCESSFUL!");
        i++;

        gotoxy(36, 19);
        printf("Do You Want To Add Another Record? (Y/N) : ");
        scanf(" %c", &c);
    } while (c == 'Y' || c == 'y');

    fclose(f_bin);
    gotoxy(40, 22);
    printf("Press any key to return back to main menu. ");
    getch();
    menu();
}

// Main
int main() {
    // Choix du rôle dès le démarrage
    menuChoice = 0;
    while (menuChoice != 1 && menuChoice != 2) {
        system("CLS");
        system("color f6");

        gotoxy(43, 4);
        printf("WELCOME TO TBC BANKING SYSTEM");

        gotoxy(50, 8);
        printf("SELECT ACCOUNT TYPE");

        gotoxy(44, 10);
        printf("[1] ADMINISTRATOR");

        gotoxy(44, 11);
        printf("[2] USER");

        gotoxy(44, 14);
        printf("Enter Your Choice: ");
        scanf("%d", &menuChoice);

        if (menuChoice != 1 && menuChoice != 2) {
            gotoxy(44, 16);
            printf("Error: Please choose 1 or 2 !");
            Sleep(2000);
        }
    }

    system("CLS");

    // Login direct selon rôle choisi
    if (!login(menuChoice)) {
        int ans;
        printf("Do you want to reset your password? (1 = Yes, 0 = No): ");
        scanf("%d", &ans);
        if (ans == 1) resetPassword();
    } else {
        if (menuChoice == 1) adminView();
        else userView();
    }

    return 0;
}
