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
#define MAX_LINE_LENGTH 8000

char main_dir[MAX_FILENAME_LENGTH];
char staging_dir[MAX_FILENAME_LENGTH];
char commits_dir[MAX_FILENAME_LENGTH];
char branches_dir[MAX_FILENAME_LENGTH];
char destination_file[MAX_FILENAME_LENGTH];

int create_configs(char *username, char *email)
{
    FILE *file = fopen(".samit/config", "w");
    if (file == NULL)
        return 1;

    fprintf(file, "username: %s\n", username);
    fprintf(file, "email: %s\n", email);
    fprintf(file, "branch: %s\n", "master");

    fclose(file);

    chdir(".samit");

    // staging directory
    if (mkdir("staging", 0755) != 0)
    {
        return 1;
    }
    chdir("staging");
    if (getcwd(staging_dir, sizeof(staging_dir)) == NULL)
    {
        perror("Could not write staging directory!");
        return 1;
    }
    chdir("..");

    // commits directory
    if (mkdir("commits", 0755) != 0)
    {
        return 1;
    }
    chdir("commits");
    if (getcwd(commits_dir, sizeof(commits_dir)) == NULL)
    {
        perror("Could not write commits directory!");
        return 1;
    }
    chdir("..");

    // branches directory
    if (mkdir("branches", 0755) != 0)
    {
        return 1;
    }
    chdir("branches");
    if (getcwd(branches_dir, sizeof(branches_dir)) == NULL)
    {
        perror("Could not write branches directory!");
        return 1;
    }
    chdir("..");

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

        // checks if .samit is in the current temp directory or not
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".samit") == 0)
            {
                // printf(".samit exists\n");
                dirExists = 1;
                if (getcwd(main_dir, sizeof(main_dir)) == NULL)
                {
                    perror("Error getting main directory!\n");
                    return 2;
                }
                break;
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
    // printf("current directory is %s\n", cwd);

    // checks if .samit exists
    int dirExists = doesHaveInit(cwd);

    if (dirExists == 1)
    {
        perror("samit repo already exists!");
    }
    else if (dirExists == 0)
    {
        // MUST MAKE .samit AND CONFIGS
        if (mkdir(".samit", 0755) != 0)
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

int run_add(int argc, char *const argv[], int is_first_iteration)
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
        for (int i = 3; i < argc; i++)
        {
            if (is_first_iteration)
            {
                glob_t globbuf;
                if (glob(argv[i], 0, NULL, &globbuf) != 0)
                {
                    perror("No match.\n");
                    return 1;
                }

                for (int i = 0; i < globbuf.gl_pathc; i++)
                {
                    // printf("%s %d\n", globbuf.gl_pathv[i], globbuf.gl_pathc);

                    struct dirent *entry;
                    DIR *dir = opendir(globbuf.gl_pathv[i]);
                    // creates destination file directory
                    strcpy(destination_file, main_dir);
                    strcat(destination_file, "/.samit/staging/");
                    strcat(destination_file, globbuf.gl_pathv[i]);

                    // checks if dir is valid and file is modified/not made
                    if (dir == NULL && compare_file(destination_file, globbuf.gl_pathv[i]) != 0)
                    {
                        copy_file(globbuf.gl_pathv[i], destination_file);
                    }
                    else
                    {
                        // makes a new directory in destination, switches there and run_adds
                        mkdir(destination_file, 0755);
                        chdir(globbuf.gl_pathv[i]);
                        run_add(argc, argv, 0);
                        chdir("..");
                    }

                    closedir(dir);
                }
                globfree(&globbuf);
            }
            else
            {
                struct dirent *entry;
                DIR *dir = opendir(".");
                char output[1000];
                char tmp_dest_file[MAX_FILENAME_LENGTH];
                strcpy(tmp_dest_file, destination_file);

                while ((entry = readdir(dir)) != NULL)
                {
                    // skips these files
                    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".samit") == 0)
                    {
                        continue;
                    }

                    // creates destination file directory
                    strcat(destination_file, "/");
                    strcat(destination_file, entry->d_name);
                    if (entry->d_type == DT_DIR)
                    {
                        // makes a new directory in destination, switches there and run_adds
                        mkdir(destination_file, 0755);
                        chdir(entry->d_name);
                        run_add(argc, argv, 0);
                        chdir("..");
                    }

                    // checks if file is modified/not made
                    else if (compare_file(destination_file, entry->d_name) != 0)
                    {
                        copy_file(entry->d_name, destination_file);
                    }

                    strcpy(destination_file, tmp_dest_file);
                }
                closedir(dir);
            }
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

        if (is_first_iteration)
        {
            glob_t globbuf;
            if (glob(argv[2], 0, NULL, &globbuf) != 0)
            {
                perror("No match.\n");
                return 1;
            }

            for (int i = 0; i < globbuf.gl_pathc; i++)
            {
                // printf("%s %d\n", globbuf.gl_pathv[i], globbuf.gl_pathc);

                struct dirent *entry;
                DIR *dir = opendir(globbuf.gl_pathv[i]);
                // creates destination file directory
                strcpy(destination_file, main_dir);
                strcat(destination_file, "/.samit/staging/");
                strcat(destination_file, globbuf.gl_pathv[i]);

                // checks if dir is valid and file is modified/not made
                if (dir == NULL && compare_file(destination_file, globbuf.gl_pathv[i]) != 0)
                {
                    copy_file(globbuf.gl_pathv[i], destination_file);
                }
                else
                {
                    // makes a new directory in destination, switches there and run_adds
                    mkdir(destination_file, 0755);
                    chdir(globbuf.gl_pathv[i]);
                    run_add(argc, argv, 0);
                    chdir("..");
                }

                closedir(dir);
            }
            globfree(&globbuf);
        }
        else
        {
            struct dirent *entry;
            DIR *dir = opendir(".");
            char output[1000];
            char tmp_dest_file[MAX_FILENAME_LENGTH];
            strcpy(tmp_dest_file, destination_file);

            while ((entry = readdir(dir)) != NULL)
            {
                // skips these files
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".samit") == 0)
                {
                    continue;
                }

                // creates destination file directory
                strcat(destination_file, "/");
                strcat(destination_file, entry->d_name);
                if (entry->d_type == DT_DIR)
                {
                    // makes a new directory in destination, switches there and run_adds
                    mkdir(destination_file, 0755);
                    chdir(entry->d_name);
                    run_add(argc, argv, 0);
                    chdir("..");
                }

                // checks if file is modified/not made
                else if (compare_file(destination_file, entry->d_name) != 0)
                {
                    copy_file(entry->d_name, destination_file);
                }

                strcpy(destination_file, tmp_dest_file);
            }
            closedir(dir);
        }
    }
    return 0;
}

int copy_file(char *src_path, char *dest_path)
{
    FILE *src_file = fopen(src_path, "r");
    if (src_file == NULL)
    {
        perror("Could not open source file.");
        return 1;
    }
    FILE *dest_file = fopen(dest_path, "w");
    if (dest_file == NULL)
    {
        perror("Could not open destination file.");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, src_file))
    {
        fprintf(dest_file, line);
    }

    fclose(src_file);
    fclose(dest_file);

    return 0;
}

int compare_file(char *first_file, char *second_file)
{
    FILE *first = fopen(first_file, "r");
    if (first == NULL)
    {
        return 1;
    }
    FILE *second = fopen(second_file, "r");
    if (second == NULL)
    {
        return 1;
    }

    char line1[MAX_LINE_LENGTH];
    char line2[MAX_LINE_LENGTH];
    while (fgets(line1, MAX_LINE_LENGTH, first))
    {
        fgets(line2, MAX_LINE_LENGTH, second);
        if (strcmp(line1, line2))
        {
            return 2;
        }
    }
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
        run_add(argc, argv, 1);
    }

    return 0;
}

// Arshia Samiezad 402111498

/*Github token is:

Username: ArshiaSamiezad
Password: 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
*/