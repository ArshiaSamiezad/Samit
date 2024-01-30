#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_FILENAME_LENGTH 1000

//used functions

int create_configs(char *username, char *email)
{
    FILE *file = fopen(".sammit/config", "w");
    if (file == NULL)
        return 1;

    fprintf(file, "username: %s\n",username);
    fprintf(file, "email: %s\n",email);
    fprintf(file, "branch: %s\n","master");

    fclose(file);
    return 0;
}

int add_to_staging(char *filepath){
    
}

//called in main functions

int run_init(int argc, char *const argv[])
{
    char cwd[MAX_FILENAME_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;
    printf("current directory is %s\n", cwd);

    bool dirExists = false;
    struct dirent *entry;
    char tmp_dir[MAX_FILENAME_LENGTH];
    do
    {
        // opens current directory
        DIR *dir = opendir(".");
        if (dir == NULL)
            return 1;

        // inputs current directy in tmp_dir
        if (getcwd(tmp_dir, sizeof(tmp_dir)) == NULL)
            return 1;

        // checks if we are at "/" or not
        if (strcmp(tmp_dir, "/") == 0)
        {
            break;
        }

        // checks if .sammit is in the current temp directory or not
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".sammit") == 0)
            {
                // printf(".sammit exists\n");
                dirExists = true;
            }
        }

        printf("we are at %s\n", tmp_dir);
        // goes to the parent directory
        if (chdir("..") != 0)
            return 1;

    } while (strcmp(tmp_dir, "/") != 0);

    if (chdir(cwd) != 0)
        return 1;

    if (dirExists)
    {
        perror("sammit repo already exists.\n");
    }
    else
    {
        // MUST MAKE .sammit AND CONFIGS
        if (mkdir(".sammit", 0755) != 0)
        {
            return 1;
        }
        create_configs("ArshiaSamiezad","arshiasamiezad@gmail.com");
    }
    return 0;
}

int run_add(int argc, char *const argv[]){
    if(argc < 3){
        perror("Please enter a valid command");
        return 1;
    }

    char * filepath = argv[2];
}

void print_command(int argc, char *const argv[])
{
    for (int i = 0; i < argc; i++)
    {
        fprintf(stdout, "%s ", argv[i]);
    }
    fprintf(stdout, "\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Please enter a valid command");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0)
    {
        run_init(argc, argv);
    }
    else if (strcmp(argv[1],"add")==0){
        run_add(argc,argv);
    }

    return 0;
}