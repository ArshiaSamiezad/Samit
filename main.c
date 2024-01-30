#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glob.h>

#define MAX_FILENAME_LENGTH 1000

char main_dir[MAX_FILENAME_LENGTH];

int create_configs(char *username, char *email)
{
    FILE *file = fopen(".sammit/config", "w");
    if (file == NULL)
        return 1;

    fprintf(file, "username: %s\n", username);
    fprintf(file, "email: %s\n", email);
    fprintf(file, "branch: %s\n", "master");

    fclose(file);

    chdir(".sammit");

    if (mkdir("staging", 0755) != 0)
    {
        return 1;
    }
    if (mkdir("commits", 0755) != 0)
    {
        return 1;
    }
    if (mkdir("branches", 0755) != 0)
    {
        return 1;
    }

    chdir("..");

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

        // printf("we are at %s\n", tmp_dir);

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
        strcpy(main_dir, cwd);
    }
    else
        return 1;
    return 0;
}

int run_add(int argc, char *const argv[])
{
    // finds current directory
    char cwd[MAX_FILENAME_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get main directory!");
        return 1;
    }
    // printf("current directory is %s\n", cwd);

    // checks if repo has been initialized
    if (doesHaveInit(cwd) != 1)
    {
        perror("Repo hasn't initialized!");
        return 1;
    }

    // checks if it is a valid command
    if (argc < 3)
    {
        perror("Please enter a valid command");
        return 1;
    }

    // add multiple files
    if (strcmp(argv[2], "-f") == 0)
    {
        if (argc < 4)
        {
            perror("No files are mentioned!");
            return 1;
        }
    }

    // depth searching
    else if (strcmp(argv[2], "-n") == 0)
    {
        if (argc < 4)
        {
            perror("Depth level isnt mentioned!");
            return 1;
        }
    }

    // single file add
    else
    {
        chdir(main_dir);

        struct dirent *entry;
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Could not DIR current directory!");
            return 2;
        }

        // finding wildcards
        // check if there is no match
        glob_t globbuf;
        if (glob(argv[2], 0, NULL, &globbuf) != 0)
        {
            perror("No match.\n");
            return 1;
        }
        for (int i = 0; i < globbuf.gl_pathc; i++)
        {
            //printf("%s %d\n", globbuf.gl_pathv[i], globbuf.gl_pathc);
            
        }
        globfree(&globbuf);
        closedir(dir);
    }
    return 0;
}

int copy_file(char *src_path, char *dest_path)
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
        perror("Please enter a valid command!");
        return 1;
    }
    if (strcmp(argv[1], "init") == 0)
    {
        if (argc > 2)
        {
            perror("Too much arguements are added!");
            return 1;
        }
        run_init(argc, argv);
    }
    else if (strcmp(argv[1], "add") == 0)
    {
        run_add(argc, argv);
    }

    return 0;
}

// Arshia Samiezad 402111498

/*Github token is:

Username: ArshiaSamiezad
Password: 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
*/