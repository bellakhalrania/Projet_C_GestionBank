#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

char email[100];
char password[50];
char id[20];
int menuChoice = 0,r; // 1 = admin, 2 = user

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
void view();
void userView();
void option();
void add();
void edit();
void del();
void about();
void menu();
void Cash_withdrawal();
void search();
void search_acc();
void search_name();
void menuExit();
void transaction();
void gotoxy(int x, int y);
int isValidEmail(char *email);
int isEmailExist(char *check_email);


void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// fonctions fictives pour le moment
void menu(){printf("Menu placeholder\n"); getch();}
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



// Login avec rôle
int login(int role) {
    char file_email[100], file_pass[50], file_role[10];
    system("CLS");
    gotoxy(44, 4);
    printf("Enter Your Email: ");
    gotoxy(44, 8);
    scanf(" %[^\n]", email);
    gotoxy(44, 10);
    printf("Enter Your Password: ");
    gotoxy(44, 12);
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
    /*gotoxy(44, 14); // Décaler les options suivantes
    printf("[7] Transaction History");*/
    gotoxy(44, 15);
    printf("[8] Log Out");
    gotoxy(44, 16);
    printf("[9] About Us");
    gotoxy(43, 20);
    printf("Please Enter Your Choice [1-8]: ");
    option();
}

void userView() {
    system("CLS");
    gotoxy(48, 4);
    printf("WELCOME TO MAIN MENU (USER)");

}

// Option menu
void option() {
    int i;
    scanf("%d", &i);
    system("CLS");
    switch (i) {
        case 1: view(); break;
        case 2: add(); break;
        case 3: edit(); break;
        case 4: del(); break;
        case 5: search(); break;
        case 6: transaction(); break;
       /* case 7: system("CLS");
            int acc;
            gotoxy(44, 8);
            printf("Enter Account Number: ");
            scanf("%d", &acc);
            viewTransactionHistory(acc);
            break;*/
        case 8: menuExit (); break;
        case 9: about(); break;
        default: menu(); break;
    }
}


int chkacc(int a) {
    FILE *f;
    f = fopen("record.bin", "rb");
    //check whether we have reached end of file or not
    while (!feof(f)) {
        fread(&rec, sizeof(rec), 1, f);

        if (a == rec.account) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
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

void view() {
    int i = 7;
    struct record rec;
    FILE *f;
    //rb = reading binary
    f = fopen("record.bin", "rb");
    gotoxy(40, 3);
    printf("CUSTOMERS LIST");
    gotoxy(5, 5);
    printf("A/C No.");
    gotoxy(13, 5);
    printf("Account Name");
    gotoxy(34, 5);
    printf("UserID");
    gotoxy(49, 5);
    printf("Email Address");
    gotoxy(85, 5);
    printf("Phone No.");
    gotoxy(99, 5);
    printf("Balance");
    gotoxy(5, 6);
    // fread() = for reading binary data
    // This reads the contents of a structure variable from the file and stores it in the variable rec.
    // The fread() function will keep returning 1 until there are records in the file.
    // As soon as the EOF is encountered fread() will return a value less than 1 and the condition in the while loop become false and the control comes out of the while loop.
    while (fread(&rec, sizeof(rec), 1, f)) {
        gotoxy(7, i);
        printf("%d", rec.account);
        gotoxy(13, i);
        printf("%s", rec.name);
        gotoxy(34, i);
        for (r = 0; r < 10; r++) {
            printf("%d", rec.UserID[r]);
        }
        gotoxy(49, i);
        printf("%s", rec.email);
        gotoxy(85, i);
        printf("%s", rec.phone);
        gotoxy(99, i);
        printf("$%.2lf", rec.blnc);
        i++;
    }
    // close the file
    fclose(f);
    int x;
    gotoxy(42, i + 5);
    printf("Press [Enter] to return back to main menu. ");
    x = getch();
    // 13 = '\r' i.e carriage return
    if (x == 13) {
        menu();
    } else
        view();
}

void edit() {
    FILE *x, *y, *f_users, *temp_users;
    int a;
    char old_email[100], new_email[100];

re:
    system("CLS");
    gotoxy(48, 4);
    printf(" EDIT CUSTOMER ACCOUNT ");
    gotoxy(43, 7);
    printf("Enter Account Number To Edit: ");
    scanf("%d", &a);

    if (chkacc(a) == 0) {
        gotoxy(52, 21);
        printf("Account Doesn't Exist.");
        Sleep(1500);
        goto re;
    }

    // --- Edit record.bin ---
    x = fopen("record.bin", "rb");
    y = fopen("new.bin", "wb");
    if (!x || !y) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&rec, sizeof(rec), 1, x)) {
        if (rec.account != a) {
            fwrite(&rec, sizeof(rec), 1, y);
        } else {
            strcpy(old_email, rec.email); // sauvegarder l'ancien email
            gotoxy(52, 21);
            printf("Account Number Matched.");
            gotoxy(51, 10);
            printf("Enter New Details");
            gotoxy(31, 12);
            printf("[1] Name           : ");
            scanf(" %[^\n]", rec.name);
            gotoxy(31, 13);
            printf("[2] Account Number : ");
            scanf(" %d", &rec.account);
            gotoxy(31, 14);
            printf("[3] Phone Number   : ");
            scanf(" %[^\n]", rec.phone);
            gotoxy(31, 15);
            printf("[4] Address        : ");
            scanf(" %[^\n]", rec.address);

            // Email avec vérification
            int valid_email = 0;
            do {
                gotoxy(31, 16);
                printf("[5] E-mail         : ");
                scanf(" %[^\n]", new_email);
                if (!isValidEmail(new_email)) {
                    gotoxy(31, 18);
                    printf("Invalid email format! Try again.");
                    Sleep(1500);
                    gotoxy(31, 18);
                    printf("                                "); // effacer message
                } else {
                    valid_email = 1;
                }
            } while (!valid_email);

            strcpy(rec.email, new_email);
            gotoxy(31, 17);
            printf("[6] Citizenship No : ");
            scanf(" %[^\n]", rec.citiz);

            fwrite(&rec, sizeof(rec), 1, y);
        }
    }

    fclose(x);
    fclose(y);

    remove("record.bin");
    rename("new.bin", "record.bin");

    // --- Mettre à jour l'email dans users.txt ---
    f_users = fopen("users.txt", "r");
    temp_users = fopen("temp.txt", "w");
    if (!f_users || !temp_users) {
        printf("Error opening users file.\n");
        return;
    }

    char file_email[100], file_pass[50], file_role[10];
    while (fscanf(f_users, "%[^,],%[^,],%s\n", file_email, file_pass, file_role) != EOF) {
        if (strcmp(file_email, old_email) == 0) {
            fprintf(temp_users, "%s,%s,%s\n", new_email, file_pass, file_role);
        } else {
            fprintf(temp_users, "%s,%s,%s\n", file_email, file_pass, file_role);
        }
    }

    fclose(f_users);
    fclose(temp_users);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    gotoxy(45, 21);
    printf("CUSTOMER ACCOUNT UPDATED SUCCESSFULLY");
    gotoxy(42, 24);
    printf("Press any key to return back to main menu.");
    getch();
    menu();
}






void del() {
    FILE *x, *y, *f_users, *temp_users;
    int a;
    char user_email[100];

re:
    system("CLS");
    gotoxy(48, 4);
    printf(" DELETE CUSTOMER ACCOUNT ");

    gotoxy(41, 9);
    printf("Enter Account Number To Delete: ");
    scanf("%d", &a);

    // Vérifier si le compte existe
    if (chkacc(a) == 0) {
        gotoxy(51, 15);
        printf("Account Doesn't Exist");
        Sleep(1500);
        goto re;  // retour à la saisie
    }

    // --- Suppression dans record.bin ---
    x = fopen("record.bin", "rb");
    y = fopen("new.bin", "wb");
    if (!x || !y) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&rec, sizeof(rec), 1, x)) {
        if (rec.account != a) {
            fwrite(&rec, sizeof(rec), 1, y);
        } else {
            strcpy(user_email, rec.email); // sauvegarder l'email pour supprimer login
        }
    }

    fclose(x);
    fclose(y);

    // Remplacer l'ancien fichier par le nouveau
    remove("record.bin");
    rename("new.bin", "record.bin");

    // --- Suppression dans users.txt ---
    f_users = fopen("users.txt", "r");
    temp_users = fopen("temp.txt", "w");
    if (!f_users || !temp_users) {
        printf("Error opening users file.\n");
        return;
    }

    char file_email[100], file_pass[50], file_role[10];
    while (fscanf(f_users, "%[^,],%[^,],%s\n", file_email, file_pass, file_role) != EOF) {
        if (strcmp(file_email, user_email) != 0) {
            fprintf(temp_users, "%s,%s,%s\n", file_email, file_pass, file_role);
        }
    }

    fclose(f_users);
    fclose(temp_users);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    gotoxy(44, 15);
    printf("CUSTOMER ACCOUNT DELETED SUCCESSFULLY");
    gotoxy(42, 18);
    printf("Press any key to return back to main menu. ");
    getch();
    menu();
}


void menuExit() {
    system("cls");
    gotoxy(48, 10);
    printf("!!! THANK YOU !!!");
    gotoxy(50, 12);
    printf("USER :: %s", id);
    getch();
    gotoxy(0, 26);
    exit(0);
}

void search() {
    system("cls");
    int a;
    gotoxy(55, 4);
    printf(" SEARCH MENU ");
    gotoxy(49, 10);
    printf("[1] . Search By Account ");
    gotoxy(49, 12);
    printf("[2] . Search By Name ");
    gotoxy(47, 17);
    printf("Enter Your Choice [1-2] : ");
    scanf("%d", &a);
    system("cls");
    if (a == 1) {
        search_acc();
    } else if (a == 2) {
        search_name();
    } else
        menu();
}

void search_acc() {
    FILE *f;
    int a, x;
re:
    gotoxy(48, 4);
    printf(" SEARCH CUSTOMER ACCOUNT ");
    gotoxy(43, 7);
    printf("Enter Your Account Number To Search : ");
    scanf("%d", &a);
    if (chkacc(a) == 1) {
        f = fopen("record.bin", "rb");
        while (fread(&rec, sizeof(rec), 1, f)) {
            if (rec.account == a) {
                gotoxy(52, 21);
                printf("Account Number Matched. User is Active.");
                gotoxy(45, 10);
                printf("Detail Information of %s", strupr(rec.name));
                gotoxy(37, 12);
                printf("[1] . Account Number : %d", rec.account);
                gotoxy(37, 13);
                printf("[2] . Name           : %s", rec.name);
                gotoxy(37, 14);
                printf("[3] . UserID         : ");
                for (r = 0; r < 10; r++) {
                    printf("%d", rec.UserID[r]);
                }
                gotoxy(37, 15);
                printf("[4] . Phone Number   : %s", rec.phone);
                gotoxy(37, 16);
                printf("[5] . Address        : %s", rec.address);
                gotoxy(37, 17);
                printf("[6] . E-mail         : %s", rec.email);
                gotoxy(37, 18);
                printf("[7] . Citizenship No : %s", rec.citiz);
                gotoxy(37, 19);
                printf("[8] . Current Balance: $%.2lf", rec.blnc);
                break;
            }
        }
        fclose(f);
    }
    if (chkacc(a) == 0) {
        system("CLS");
        gotoxy(52, 21);
        printf("Account Doesn't Exist. User is Inactive. ");
        goto re;
    }
    gotoxy(42, 24);
    printf("Press [ENTER] to return back to main menu. ");
    char z = getch();
    if (z == 13)
        menu();
    else
        search();
}

void search_name() {
    int i = 0, b, x;
    char nam[30];
    gotoxy(48, 4);
    printf(" SEARCH CUSTOMER ACCOUNT ");
    gotoxy(43, 7);
    printf("Enter Your Full Name To Search : ");
    scanf(" %[^\n]", nam);
    FILE *f;
    f = fopen("record.bin", "rb");
    while (fread(&rec, sizeof(rec), 1, f)) {
        b = 0;
        strupr(rec.name);
        strupr(nam);
        if (strcmp(rec.name, nam) == 0) {
            gotoxy(52, 21);
            printf("Account Number Matched. User is Active.");
            gotoxy(45, 10);
            printf("Detail Information of %s", rec.name);
            gotoxy(37, 12);
            printf("[1] . Account Number : %d", rec.account);
            gotoxy(37, 13);
            printf("[2] . Name           : %s", rec.name);
            gotoxy(37, 14);
            printf("[3] . UserID         : ");
            for (r = 0; r < 10; r++) {
                printf("%d", rec.UserID[r]);
            }
            gotoxy(37, 15);
            printf("[4] . Phone Number   : %s", rec.phone);
            gotoxy(37, 16);
            printf("[5] . Address        : %s", rec.address);
            gotoxy(37, 17);
            printf("[6] . E-mail         : %s", rec.email);
            gotoxy(37, 18);
            printf("[7] . Citizenship No : %s", rec.citiz);
            gotoxy(37, 19);
            printf("[8] . Current Balance: $%.2lf", rec.blnc);
            break;
        } else
            b = 1;
    }
    fclose(f);
    if (b == 1) {
        system("cls");
        gotoxy(52, 21);
        printf("Account Doesn't Exist. User is Inactive.");
        search_name();
    }
    gotoxy(42, 24);
    printf("Press [ENTER] to return back to main menu. ");
    char z = getch();
    if (z == 13)
        menu();
    else
        search();
}

void deposit() {
    int a;
    double amount;
    FILE *x, *y;
    gotoxy(54, 4);
    printf(" CASH DEPOSIT ");
    gotoxy(47, 10);
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    if (chkacc(a) == 1) {
        x = fopen("record.bin", "rb");
        y = fopen("new.bin", "wb");
        while (fread(&rec, sizeof(rec), 1, x)) {
            if (rec.account != a)
                fwrite(&rec, sizeof(rec), 1, y);
            else if (rec.account == a) {
                rec.name;
                rec.account;
                rec.phone;
                rec.address;
                rec.email;
                rec.citiz;
                gotoxy(45, 13);
                printf("Enter The Amount To Deposit : $ ");
                scanf("%lf", &amount);
                rec.blnc += amount;
                fwrite(&rec, sizeof(rec), 1, y);
            }
        }
        fclose(x);
        fclose(y);
        x = fopen("record.bin", "wb");
        y = fopen("new.bin", "rb");
        while (fread(&rec, sizeof(rec), 1, y))
            fwrite(&rec, sizeof(rec), 1, x);
        fclose(x);
        fclose(y);
    }
    if (chkacc(a) == 0) {
        system("CLS");
        gotoxy(52, 21);
        printf("Account Doesn't Exist. ");
        gotoxy(41, 22);
        deposit();
    }
    gotoxy(52, 21);
    printf("CASH DEPOSIT SUCCESSFULL");
    gotoxy(50, 24);
    printf("Press any key to return back...");
    getch();
    transaction();
}



void checkBalance() {
    FILE *f;
    int a;
    gotoxy(48, 4);
    printf(" BALANCE INQUIRY ");
    gotoxy(47, 12);
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    if (chkacc(a) == 1) {
        f = fopen("record.bin", "rb");
        while (fread(&rec, sizeof(rec), 1, f)) {
            if (rec.account == a) {
                gotoxy(50, 15);
                printf("Your Balance is : $%.2lf", rec.blnc);
            }
        }
        fclose(f);
    }
    if (chkacc(a) == 0) {
        system("cls");
        gotoxy(52, 15);
        printf("Account Doesn't Exist.");
        checkBalance();
    }
    gotoxy(46, 21);
    printf("Press any key to return back to main menu. ");
    getch();
    transaction();
}

void Cash_withdrawal() {
    FILE *x, *y;
    int a;
    double amount, z;
    gotoxy(54, 4);
    printf(" CASH WITHDRAWAL ");
    gotoxy(45, 10);
    printf("Enter Your Account Number : ");
    scanf("%d", &a);
    if (chkacc(a) == 1) {
        x = fopen("record.bin", "rb");
        y = fopen("new.bin", "wb");
        gotoxy(42, 13);
        printf("Enter The Amount To Withdraw : $ ");
        scanf("%lf", &amount);
        while (fread(&rec, sizeof(rec), 1, x)) {
            if (rec.account != a)
                fwrite(&rec, sizeof(rec), 1, y);
            else if (rec.account == a) {
                rec.name;
                rec.account;
                rec.phone;
                rec.address;
                rec.email;
                rec.citiz;
                z = rec.blnc;
                if (amount <= rec.blnc) {
                    rec.blnc -= amount;
                    gotoxy(44, 21);
                    printf("    CASH WITHDRAWAL SUCCESSFULL     ");
                }
                fwrite(&rec, sizeof(rec), 1, y);
            }
        }
        fclose(x);
        fclose(y);
        remove("record.bin");
        rename("new.bin", "record.bin");
        if (z < amount) {
            system("CLS");
            gotoxy(48, 21);
            printf("Current Balance is $%.2lf", z);
            gotoxy(42, 24);
            printf("Entered amount exceeds current balance");
            Cash_withdrawal();
        }
    }
    if (chkacc(a) == 0) {
        system("CLS");
        gotoxy(50, 21);
        printf("Account Doesn't Exist.");
        gotoxy(39, 22);
        Cash_withdrawal();
    }
    gotoxy(42, 24);
    printf("     Press Any Key To Continue ....   ");
    getch();
    transaction();
}

void transaction() {
    system("CLS");
    gotoxy(48, 4);
    printf(" TRANSACTION MENU ");
    gotoxy(49, 9);
    printf("[1] . Balance Inquiry");
    gotoxy(49, 10);
    printf("[2] . Cash Deposit");
    gotoxy(49, 11);
    printf("[3] . Cash Withdrawal");
    if ( menuChoice == 1) {
        gotoxy(49, 12);
        printf("[4] . Main Menu");
    } else {
        gotoxy(49, 12);
        printf("[4] . Exit");
    }
    gotoxy(45, 17);
    printf("Please Enter Your Choice [1-4] : ");
    int a;
    scanf("%d", &a);
    switch (a) {
        case 1:
            system("cls");
            checkBalance();
            break;
        case 2:
            system("cls");
            deposit();
            break;
        case 3:
            system("cls");
            Cash_withdrawal();
            break;
        case 4:
            if (menuChoice == 1)
                menu();
            else
                menuExit();
            break;
        default:
            transaction();
    }
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
