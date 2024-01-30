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

// init

int create_configs(char *username, char *email)
{
    FILE *file = fopen(".sammit/config", "w");
    if (file == NULL)
        return 1;

    fprintf(file, "username: %s\n", username);
    fprintf(file, "email: %s\n", email);
    fprintf(file, "branch: %s\n", "master");

    fclose(file);

    // file = fopen(".sammit/staging")

    return 0;
}

int doesHaveInit(char cwd[])
{

    int dirExists = 0;
    struct dirent *entry;
    char tmp_dir[MAX_FILENAME_LENGTH];

    do
    {
        // opens current directory
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Could not DIR current directory!");
            return 2;
        }

        // inputs current directy in tmp_dir
        if (getcwd(tmp_dir, sizeof(tmp_dir)) == NULL)
        {
            perror("Error getting current temp directory!\n");
            return 2;
        }

        // checks if we are at "/" or not
        if (strcmp(tmp_dir, "/") == 0)
        {
            closedir(dir);
            break;
        }

        // checks if .sammit is in the current temp directory or not
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".sammit") == 0)
            {
                // printf(".sammit exists\n");
                dirExists = 1;
            }
        }
        closedir(dir);

        printf("we are at %s\n", tmp_dir);
        // goes to the parent directory
        if (chdir("..") != 0)
        {
            perror("Failed to change directory to parent folder");
            return 2;
        }

    } while (strcmp(tmp_dir, "/") != 0);

    // goes to main directory
    if (chdir(cwd) != 0)
    {
        perror("Could not go to main directory!");
        return 2;
    }

    return dirExists;
}

int run_init(int argc, char *const argv[])
{
    // finds current directory
    char cwd[MAX_FILENAME_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get main directory!");
        return 1;
    }
    printf("current directory is %s\n", cwd);

    // checks if .sammit exists
    int dirExists = doesHaveInit(cwd);

    if (dirExists == 1)
    {
        perror("sammit repo already exists!");
    }
    else if (dirExists == 0)
    {
        // MUST MAKE .sammit AND CONFIGS
        if (mkdir(".sammit", 0755) != 0)
        {
            return 1;
        }
        create_configs("ArshiaSamiezad", "arshiasamiezad@gmail.com");
    }
    else
        return 1;
    return 0;
}

// add

int run_add(int argc, char *const argv[])
{
    // finds current directory
    char cwd[MAX_FILENAME_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get main directory!");
        return 1;
    }
    printf("current directory is %s\n", cwd);

    // checks if repo has been initialized
    if(doesHaveInit(cwd) != 1){
        perror("Repo hasn't initialized!");
        return 1;
    }

    // checks if it is a valid command
    if (argc < 3)
    {
        perror("Please enter a valid command");
        return 1;
    }

    char *filepath = argv[2];
}

int add_to_staging(char *filepath)
{
    return 0;
}

// testing command

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
    else if (strcmp(argv[1], "add") == 0)
    {
        run_add(argc, argv);
    }

    return 0;
}