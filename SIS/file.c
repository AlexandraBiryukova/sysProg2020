#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#include <pwd.h>
#include <grp.h>

#include <utmp.h>
#include <sys/utsname.h>

#include <unistd.h>

void listDir(char *dirName, int choice) {
    DIR* dir;
    struct dirent *dirEntry;
    struct stat inode;
    dir = opendir(dirName);

    if (dir == 0) {
        perror ("Error while openning file");
        exit(1);
    }
    while ((dirEntry=readdir(dir)) != NULL) { 
        char fp[1000];
        snprintf(fp, sizeof(fp), "%s/%s", dirName, dirEntry->d_name);
        stat(fp, &inode);
        if (*dirEntry->d_name != '.'){
            printf((S_ISDIR(inode.st_mode)) ? "Directory: %s\n" : "File: %s\n",dirEntry->d_name);
            printf("Path: %s\n", fp);
            struct passwd *pw = getpwuid(inode.st_uid);
            struct group *gr = getgrgid(inode.st_gid);
            switch (choice) {
                case 1:
                    printf("OWNER: \n\tUsername: %s\n\tId: %d\n\tGroup: %s\n",pw->pw_name, pw->pw_uid, gr->gr_name);
                    printf("\n");
                    break;
                case 2:
                    printf("\n");
                    break;
            
                default:
                    break;
            }
            if S_ISDIR(inode.st_mode) {
                listDir(fp, choice);
            }
        } 
    }
}

void listUsers() {
    struct utmp *u;
    setutent();
    u = getutent();
    printf("%s\n", "USERS:");
    while (u){
        if(u-> ut_type == USER_PROCESS){
            printf("User: %s ID: %d\n", u->ut_user, getpwnam(u->ut_user)->pw_uid);
        }
        u = getutent();
    }
}

void changeOwner(char *name, char *file, int id){
    system("clear");
    char fp[1000];
    snprintf(fp, sizeof(fp), "%s/%s", name, file);
    if (chown(fp, id, id) == 0) {
        printf("Owner of %s was changed successfully\n", file);
    }else {
        printf("Error while changing owner of %s\n", file);
    }
}

void changePermission(char *name, char *file){
    system("clear");
    struct stat inode;
    char fp[1000];
    snprintf(fp, sizeof(fp), "%s/%s", name, file);
    stat(fp, &inode);
    printf("Choose permissions to change:\n");
    printf("1.Owner\n2.Group\n3.Others\n4.Back\n");
    int choice;
    printf("Your choice: ");
    scanf("%d",&choice);
    system("clear");
    while(choice != 4){
        switch (choice) {
            case 1:
                system("clear");
                printf("Permissions for owner: \t");
                printf( (inode.st_mode & S_IRUSR) ? " READ " : " - ");
                printf( (inode.st_mode & S_IWUSR) ? " WRITE " : " - ");
                printf( (inode.st_mode & S_IXUSR) ? " EXECUTE " : " - ");
                int permission;
                printf("\nType permission to change (1 - read, 2 - write, 3 - execute): ");
                scanf("%d", &permission);
                switch (permission) {
                    case 1:
                        if (inode.st_mode & S_IRUSR){
                            inode.st_mode &= ~(S_IRUSR);
                        }else {
                            inode.st_mode |= S_IRUSR;
                        }
                        chmod(fp, inode.st_mode);
                        break;
                    case 2:
                        if (inode.st_mode & S_IWUSR){
                            inode.st_mode &= ~(S_IWUSR);
                        }else {
                            inode.st_mode |= S_IWUSR;
                        }
                        chmod(fp, inode.st_mode);
                        break;                        
                    case 3:
                       if (inode.st_mode & S_IXUSR){
                            inode.st_mode &= ~(S_IXUSR);
                        }else {
                            inode.st_mode |= S_IXUSR;
                        }
                        chmod(fp, inode.st_mode);
                        break;
                    default:
                        break;
                }
                stat(fp,&inode);
                system("clear");
                printf("New permissions for owner: ");
                printf( (inode.st_mode & S_IRUSR) ? " READ " : " - ");
                printf( (inode.st_mode & S_IWUSR) ? " WRITE " : " - ");
                printf( (inode.st_mode & S_IXUSR) ? " EXECUTE " : " - ");                       
                printf("\n");
                break;
            case 2:
                system("clear");
                printf("Permissions for group: \t");
                printf( (inode.st_mode & S_IRGRP) ? " READ " : " - ");
                printf( (inode.st_mode & S_IWGRP) ? " WRITE " : " - ");
                printf( (inode.st_mode & S_IXGRP) ? " EXECUTE " : " - ");
                printf("\nType permission to change (1 - read, 2 - write, 3 - execute): ");
                scanf("%d", &permission);
                switch (permission) {
                    case 1:
                        if (inode.st_mode & S_IRGRP){
                            inode.st_mode &= ~(S_IRGRP);
                        }else {
                            inode.st_mode |= S_IRGRP;
                        }
                        chmod(fp, inode.st_mode);
                        break;
                    case 2:
                        if (inode.st_mode & S_IWGRP){
                            inode.st_mode &= ~(S_IWGRP);
                        }else {
                            inode.st_mode |= S_IWGRP;
                        }
                        chmod(fp, inode.st_mode);
                        break;                        
                    case 3:
                       if (inode.st_mode & S_IXGRP){
                            inode.st_mode &= ~(S_IXGRP);
                        }else {
                            inode.st_mode |= S_IXGRP;
                        }
                        chmod(fp, inode.st_mode);
                        break;
                    default:
                        break;
                }
                stat(fp,&inode);
                system("clear");
                printf("New permissions for group: ");
                printf( (inode.st_mode & S_IRGRP) ? " READ " : " - ");
                printf( (inode.st_mode & S_IWGRP) ? " WRITE " : " - ");
                printf( (inode.st_mode & S_IXGRP) ? " EXECUTE " : " - ");                       
                printf("\n");
                break;
            case 3:
                system("clear");
                printf("Permissions for others: \t");
                printf( (inode.st_mode & S_IROTH) ? " READ " : " - ");
                printf( (inode.st_mode & S_IWOTH) ? " WRITE " : " - ");
                printf( (inode.st_mode & S_IXOTH) ? " EXECUTE " : " - ");
                printf("\nType permission to change (1 - read, 2 - write, 3 - execute): ");
                scanf("%d", &permission);
                switch (permission) {
                    case 1:
                        if (inode.st_mode & S_IROTH){
                            inode.st_mode &= ~(S_IROTH);
                        }else {
                            inode.st_mode |= S_IROTH;
                        }
                        chmod(fp, inode.st_mode);
                        break;
                    case 2:
                        if (inode.st_mode & S_IWOTH){
                            inode.st_mode &= ~(S_IWOTH);
                        }else {
                            inode.st_mode |= S_IWOTH;
                        }
                        chmod(fp, inode.st_mode);
                        break;                        
                    case 3:
                       if (inode.st_mode & S_IXOTH){
                            inode.st_mode &= ~(S_IXOTH);
                        }else {
                            inode.st_mode |= S_IXOTH;
                        }
                        chmod(fp, inode.st_mode);
                        break;
                    default:
                        break;
                }
                stat(fp,&inode);
                system("clear");
                printf("New permissions for others: ");
                printf( (inode.st_mode & S_IROTH) ? " READ " : " - ");
                printf( (inode.st_mode & S_IWOTH) ? " WRITE " : " - ");
                printf( (inode.st_mode & S_IXOTH) ? " EXECUTE " : " - ");                       
                printf("\n");
            default:
                break;
        }
        printf("Choose permissions to change:\n");
        printf("1.Owner\n2.Group\n3.Others\n4.Back\n");
        printf("Your choice: ");
        scanf("%d",&choice);
        if( choice == 4){ system("clear"); }
    }
}

int main(int argc, char **argv) {

    if (argc != 2) {
      printf ("Missed directory name\n");
      exit(0);
    }
    int choice;
    printf("%s", "Choose function you want to execute:\n1.Change owner of file\n2.Change file permissions\n3.Stop program\n\nYour choice (number):");
    scanf("%d", &choice);
    while(choice != 3){
        int id;
        char name[1000];
        listDir(argv[1],choice);
        switch (choice) {
            case 1:
                listUsers();
                printf("File or Directory name: ");
                scanf("%s", name);
                printf("New owner's ID: ");
                scanf("%d", &id);
                changeOwner(argv[1], name, id);
                break;
            default:
                printf("File or Directory name: ");
                scanf("%s", name);
                changePermission(argv[1], name);
                break;
        }
        system("clear");
        printf("%s", "Choose function you want to execute:\n1.Change owner of file\n2.Change file permissions\n3.Stop program\n\nYour choice (number):");
        scanf("%d", &choice);
    }
     printf("save");
    
    
}
