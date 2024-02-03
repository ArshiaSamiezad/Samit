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
#define MAX_NAME_LENGTH 1000

char main_dir[MAX_FILENAME_LENGTH];
char config_global_dir[MAX_FILENAME_LENGTH];

char staging_dir[MAX_FILENAME_LENGTH];
char commits_dir[MAX_FILENAME_LENGTH];
char branches_dir[MAX_FILENAME_LENGTH];
char master_dir[MAX_FILENAME_LENGTH];
char commits_dir[MAX_FILENAME_LENGTH];
char config_dir[MAX_FILENAME_LENGTH];
char alias_dir[MAX_FILENAME_LENGTH];
char shortcuts_dir[MAX_FILENAME_LENGTH];

char destination_file[MAX_FILENAME_LENGTH];
char destination_file_stage[MAX_FILENAME_LENGTH]; // for status reverse checking

int add_n_depth;

int create_configs(char *username, char *email)
{
    chdir(".samit");
    // config folder
    if (mkdir("config", 0755) != 0)
    {
        return 1;
    }

    if (getcwd(config_dir, sizeof(config_dir)) == NULL)
    {
        perror("Could not write config directory!");
        return 1;
    }
    chdir("config");
    FILE *file = fopen("username", "w");
    fprintf(file, "%s", username);
    if (file == NULL)
        return 1;
    fclose(file);
    file = fopen("email", "w");
    fprintf(file, "%s", email);
    if (file == NULL)
        return 1;
    fclose(file);
    file = fopen("branch", "w");
    fprintf(file, "%s", "master");
    if (file == NULL)
        return 1;
    fclose(file);
    chdir("..");

    // alias folder
    if (mkdir("alias", 0755) != 0)
    {
        return 1;
    }
    chdir("alias");
    if (getcwd(alias_dir, sizeof(alias_dir)) == NULL)
    {
        perror("Could not write alias directory!");
        return 1;
    }
    chdir("..");

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
    if (mkdir("master", 0755) != 0)
    {
        return 1;
    }
    chdir("master");
    if (getcwd(master_dir, sizeof(master_dir)) == NULL)
    {
        perror("Could not write branches directory!");
        return 1;
    }

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
    chdir("..");
    chdir("..");

    chdir("..");

    return 0;
}

// create alias global
int run_alias_global(int argc, char *argv[])
{
    // finds current directorychar cwd[MAX_FILENAME_LENGTH];
    char cwd[MAX_FILENAME_LENGTH];
    char first_cwd[MAX_FILENAME_LENGTH];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get current directory!");
        return 1;
    }
    strcpy(first_cwd, cwd);

    chdir(config_global_dir);
    chdir("alias");

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get current directory!");
        return 1;
    }

    char *inputList[3];

    char temp_argv[MAX_FILENAME_LENGTH];
    strcpy(temp_argv, argv[3]);

    inputList[0] = strtok(temp_argv, ".");
    int index = 0;
    while (inputList[index])
    {
        index++;
        inputList[index] = strtok(NULL, ".");
    }
    inputList[index] = '\0';

    FILE *file = fopen(inputList[1], "w");
    if (file == NULL)
        return 1;
    fprintf(file, "%s", argv[4]);
    fclose(file);

    chdir(first_cwd);

    char **argv_local;
    argv_local = malloc((4) * sizeof(char *));
    for (int i = 0; i < 4; i++)
    {
        argv_local[i] = malloc(MAX_FILENAME_LENGTH);
    }
    strcpy(argv_local[0], argv[0]);
    strcpy(argv_local[1], argv[1]);
    strcpy(argv_local[2], argv[3]);
    strcpy(argv_local[3], argv[4]);

    argc--;

    run_alias_local(argc, argv_local);
    return 0;
}

// create config global
int run_config_global(int argc, char *argv[])
{
    // finds current directorychar cwd[MAX_FILENAME_LENGTH];
    char cwd[MAX_FILENAME_LENGTH];
    char first_cwd[MAX_FILENAME_LENGTH];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get current directory!");
        return 1;
    }
    strcpy(first_cwd, cwd);

    chdir(config_global_dir);

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get current directory!");
        return 1;
    }
    // printf("cwd is %s\n", cwd);

    if (strcmp(argv[3], "user.name") == 0)
    {
        FILE *file = fopen("username", "w");
        fprintf(file, "%s", argv[4]);
        if (file == NULL)
        {
            return 1;
        }
        fclose(file);
    }

    else if (strcmp(argv[3], "user.email") == 0)
    {
        FILE *file = fopen("email", "w");
        fprintf(file, "%s", argv[4]);
        if (file == NULL)
            return 1;
        fclose(file);
    }

    else
    {
        perror("Invalid config!\n");
        return 1;
    }
    chdir(first_cwd);
    char **argv_local;
    argv_local = malloc((4) * sizeof(char *));
    for (int i = 0; i < 4; i++)
    {
        argv_local[i] = malloc(MAX_FILENAME_LENGTH);
    }
    strcpy(argv_local[0], argv[0]);
    strcpy(argv_local[1], argv[1]);
    strcpy(argv_local[2], argv[3]);
    strcpy(argv_local[3], argv[4]);

    argc--;

    run_config_local(argc, argv_local);
    return 0;
}

// alias local
int run_alias_local(int argc, char *argv[])
{

    char cwd[MAX_FILENAME_LENGTH];
    char first_cwd[MAX_FILENAME_LENGTH];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get current directory!");
        return 1;
    }
    strcpy(first_cwd, cwd);

    if (doesHaveInit(cwd) != 1)
        return 1;

    chdir(".samit");
    chdir("alias");

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get current directory!");
        return 1;
    }

    char *inputList[3];

    inputList[0] = strtok(argv[2], ".");
    int index = 0;
    while (inputList[index])
    {
        index++;
        inputList[index] = strtok(NULL, ".");
    }

    inputList[index] = '\0';

    FILE *file = fopen(inputList[1], "w");
    if (file == NULL)
    {
        printf("%s\n", inputList[1]);
        return 1;
    }

    fprintf(file, "%s", argv[3]);
    fclose(file);

    chdir(first_cwd);
}

// config local
int run_config_local(int argc, char *argv[])
{
    // finds current directory
    char cwd[MAX_FILENAME_LENGTH];
    char first_cwd[MAX_FILENAME_LENGTH];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get current directory!");
        return 1;
    }
    strcpy(first_cwd, cwd);

    if (doesHaveInit(cwd) != 1)
        return 1;

    chdir(".samit");
    chdir("config");

    if (strcmp(argv[2], "user.name") == 0)
    {
        FILE *file = fopen("username", "w");
        fprintf(file, "%s", argv[3]);
        if (file == NULL)
        {
            return 1;
        }
        fclose(file);
    }
    else if (strcmp(argv[2], "user.email") == 0)
    {
        FILE *file = fopen("email", "w");
        fprintf(file, "%s", argv[3]);
        if (file == NULL)
            return 1;
        fclose(file);
    }
    else
    {
        perror("Invalid config!\n");
        return 1;
    }
}

// checks if repo is made, and inserts the path in main_dir. 0: not found. 1: found. 2:errors
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
        strcpy(main_dir, cwd);
        chdir(config_global_dir);
        char username[MAX_NAME_LENGTH];
        char email[MAX_NAME_LENGTH];

        FILE *file = fopen("username", "r");
        fgets(username, MAX_NAME_LENGTH, file);
        fclose(file);
        file = fopen("email", "r");
        fgets(email, MAX_NAME_LENGTH, file);
        fclose(file);
        chdir(main_dir);
        create_configs(username, email);

        chdir(".samit/alias");
        if (getcwd(alias_dir, sizeof(alias_dir)) == NULL)
        {
            perror("Could not get main directory!");
            return 1;
        }

        chdir(config_global_dir);
        chdir("alias");

        struct dirent *entry;
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            return 1;
        }
        char temp_file_name[MAX_FILENAME_LENGTH];
        strcpy(temp_file_name, alias_dir);
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            strcat(alias_dir, "/");
            strcat(alias_dir, entry->d_name);
            copy_file(entry->d_name, alias_dir);
            strcpy(alias_dir, temp_file_name);
        }

        chdir(main_dir);
        chdir(".samit");
        if (mkdir("shortcuts", 0755) != 0)
        {
            return 1;
        }
        chdir(main_dir);
    }
    else
        return 1;
    return 0;
}

int run_add(int argc, char *argv[], int level)
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
        perror("Please enter a valid add command");
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
        for (int j = 3; j < argc; j++)
        {
            if (level == 0)
            {
                glob_t globbuf;
                if (glob(argv[j], 0, NULL, &globbuf) != 0)
                {
                    perror("No match.");
                    return 1;
                }

                for (int i = 0; i < globbuf.gl_pathc; i++)
                {

                    // printf("%s %d\n", globbuf.gl_pathv[i], globbuf.gl_pathc);
                    if (strcmp(".", globbuf.gl_pathv[i]) == 0 || strcmp(".samit", globbuf.gl_pathv[i]) == 0 || strcmp("..", globbuf.gl_pathv[i]) == 0 || strcmp(".git", globbuf.gl_pathv[i]) == 0)
                        continue;
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
                    else if (dir != NULL)
                    {
                        // makes a new directory in destination, switches there and run_adds
                        mkdir(destination_file, 0755);
                        chdir(globbuf.gl_pathv[i]);
                        level++;
                        run_add(argc, argv, level);
                        level--;
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
                    if (strcmp(entry->d_name, ".git") == 0 || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".samit") == 0)
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
                        level++;
                        run_add(argc, argv, level);
                        level--;
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

        if (level > add_n_depth)
        {
            return 0;
        }
        if (level == 0)
        {
            // creates destination file directory
            strcpy(destination_file, main_dir);
            strcat(destination_file, "/.samit/staging");
        }

        struct dirent *entry;
        DIR *dir = opendir(".");
        char output[1000];
        char tmp_dest_file[MAX_FILENAME_LENGTH];
        strcpy(tmp_dest_file, destination_file);

        while ((entry = readdir(dir)) != NULL)
        {
            // skips these files
            if (strcmp(entry->d_name, ".git") == 0 || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".samit") == 0)
            {
                continue;
            }

            // creates destination file directory
            strcat(destination_file, "/");
            strcat(destination_file, entry->d_name);
            if (entry->d_type == DT_DIR)
            {
                for (int i = 0; i < level; i++)
                {
                    printf("    ");
                }
                printf("Entering directory %s{\n", entry->d_name);
                chdir(entry->d_name);
                level++;
                run_add(argc, argv, level);
                level--;
                chdir("..");
                for (int i = 0; i < level; i++)
                {
                    printf("    ");
                }
                printf("}\n");
            }

            // checks if file is modified/not made
            else
            {
                for (int i = 0; i < level; i++)
                {
                    printf("    ");
                }
                if (compare_file(entry->d_name, destination_file) == 1)
                {
                    printf("%s is staged but modified\n", entry->d_name);
                }
                else if (compare_file(entry->d_name, destination_file) == 0)
                {
                    printf("%s is staged\n", entry->d_name);
                }
                else
                {
                    printf("%s is untracked\n", entry->d_name);
                }
            }

            strcpy(destination_file, tmp_dest_file);
        }
        closedir(dir);
        return 0;
    }

    // single file add
    else
    {

        if (level == 0)
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
                if (strcmp(".git", globbuf.gl_pathv[i]) == 0 || strcmp(".samit", globbuf.gl_pathv[i]) == 0 || strcmp("..", globbuf.gl_pathv[i]) == 0)
                    continue;
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
                else if (dir != NULL)
                {
                    // makes a new directory in destination, switches there and run_adds
                    mkdir(destination_file, 0755);
                    chdir(globbuf.gl_pathv[i]);
                    level++;
                    run_add(argc, argv, level);
                    level--;
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
                if (strcmp(entry->d_name, ".git") == 0 || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".samit") == 0)
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
                    level++;
                    run_add(argc, argv, level);
                    level--;
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
    FILE *src_file = fopen(src_path, "rb");
    if (src_file == NULL)
    {
        printf("source files is %s\n", src_path);
        perror("Could not open source file.");
        return 1;
    }
    FILE *dest_file = fopen(dest_path, "wb");
    if (dest_file == NULL)
    {
        fclose(src_file);
        perror("Could not open destination file.");
        return 1;
    }
    const size_t bufferSize = 10240;
    char buffer[bufferSize];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, bufferSize, src_file)) > 0)
    {
        if (fwrite(buffer, 1, bytesRead, dest_file) != bytesRead)
        {
            perror("Error writing to destination file");
            fclose(src_file);
            fclose(dest_file);
            return 1;
        }
    }

    fclose(src_file);
    fclose(dest_file);

    return 0;
}

// checks if 2 files are identical or not. 0 for identical, 1 for not found, 2 for not identical.
int compare_file(char *first_file, char *second_file)
{
    FILE *first = fopen(first_file, "rb");
    if (first == NULL)
    {
        return 3; // 1
    }
    FILE *second = fopen(second_file, "rb");
    if (second == NULL)
    {
        fclose(first);
        return 2; // 1
    }

    // 0 is not same, 1 is same
    int status = 1;

    while (1)
    {
        char first_file_byte = fgetc(first);
        char second_file_byte = fgetc(second);

        if (feof(first) || feof(second))
        {
            if (!feof(first) || !feof(second))
            {
                // printf("%c and %c\n", first_file_byte, second_file_byte);
                status = 0;
                break;
            }
            break;
        }

        if (first_file_byte != second_file_byte)
        {
            status = 0;
            break;
        }
    }
    fclose(first);
    fclose(second);

    if (status == 0)
    {
        return 1; // 2
    }
    if (status == 1)
    {
        return 0;
    }
}

// reset
int run_reset(int argc, char *argv[], int level)
{
    // finds current directory
    char cwd[MAX_FILENAME_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get main directory!");
        return 1;
    }

    // checks if repo has been initialized
    if (doesHaveInit(cwd) != 1)
    {
        perror("Repo hasn't initialized!");
        return 1;
    }

    // reset multiple files
    if (strcmp(argv[2], "-f") == 0)
    {
        if (argc < 4)
        {
            perror("No files are mentioned!");
            return 1;
        }
        for (int j = 3; j < argc; j++)
        {
            if (level == 0)
            {
                glob_t globbuf;
                if (glob(argv[j], 0, NULL, &globbuf) != 0)
                {
                    perror("No match.");
                    return 1;
                }

                for (int i = 0; i < globbuf.gl_pathc; i++)
                {
                    if (strcmp(".samit", globbuf.gl_pathv[i]) == 0 || strcmp("..", globbuf.gl_pathv[i]) == 0 || strcmp(".git", globbuf.gl_pathv[i]) == 0)
                        continue;
                    struct dirent *entry;
                    DIR *dir = opendir(globbuf.gl_pathv[i]);
                    // creates destination file directory
                    strcpy(destination_file, main_dir);
                    strcat(destination_file, "/.samit/staging/");
                    strcat(destination_file, globbuf.gl_pathv[i]);

                    // checks if dir is valid, removes files
                    if (dir == NULL)
                    {
                        remove(destination_file);
                    }

                    else if (dir != NULL)
                    {
                        // goes to the directory in destination, switches there and run_resets
                        chdir(globbuf.gl_pathv[i]);
                        level++;
                        run_reset(argc, argv, level);

                        // removes directory
                        rmdir(destination_file);
                        level--;
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
                    if (strcmp(entry->d_name, ".git") == 0 || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".samit") == 0)
                    {
                        continue;
                    }

                    // creates destination file directory
                    strcat(destination_file, "/");
                    strcat(destination_file, entry->d_name);
                    if (entry->d_type == DT_DIR)
                    {
                        // goes to the directory in destination, switches there and run_resets
                        chdir(entry->d_name);
                        level++;
                        run_reset(argc, argv, level);

                        // removes directory
                        rmdir(destination_file);
                        level--;
                        chdir("..");
                    }

                    // removes files
                    else
                    {
                        remove(destination_file);
                    }

                    strcpy(destination_file, tmp_dest_file);
                }
                closedir(dir);
            }
        }
    }

    else
    {
        if (level == 0)
        {
            glob_t globbuf;
            if (glob(argv[2], 0, NULL, &globbuf) != 0)
            {
                perror("No match.");
                return 1;
            }

            for (int i = 0; i < globbuf.gl_pathc; i++)
            {
                if (strcmp(".", globbuf.gl_pathv[i]) == 0 || strcmp(".samit", globbuf.gl_pathv[i]) == 0 || strcmp("..", globbuf.gl_pathv[i]) == 0 || strcmp(".git", globbuf.gl_pathv[i]) == 0)
                    continue;
                struct dirent *entry;
                DIR *dir = opendir(globbuf.gl_pathv[i]);
                // creates destination file directory
                strcpy(destination_file, main_dir);
                strcat(destination_file, "/.samit/staging/");
                strcat(destination_file, globbuf.gl_pathv[i]);

                // checks if dir is valid, removes files
                if (dir == NULL)
                {
                    remove(destination_file);
                }

                else if (dir != NULL)
                {
                    // goes to the directory in destination, switches there and run_resets
                    chdir(globbuf.gl_pathv[i]);
                    level++;
                    run_reset(argc, argv, level);

                    // removes directory
                    rmdir(destination_file);
                    level--;
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
                if (strcmp(entry->d_name, ".git") == 0 || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".samit") == 0)
                {
                    continue;
                }

                // creates destination file directory
                strcat(destination_file, "/");
                strcat(destination_file, entry->d_name);
                if (entry->d_type == DT_DIR)
                {
                    // goes to the directory in destination, switches there and run_resets
                    chdir(entry->d_name);
                    level++;
                    run_reset(argc, argv, level);

                    // removes directory
                    rmdir(destination_file);
                    level--;
                    chdir("..");
                }

                // removes files
                else
                {
                    remove(destination_file);
                }

                strcpy(destination_file, tmp_dest_file);
            }
            closedir(dir);
        }
    }
}

// helping function for searching destination file
int run_status_destination(int argc, char *argv[], int level, int is_first)
{
    char cwd_stage[MAX_FILENAME_LENGTH];
    if (is_first)
    {
        // finds current directory

        if (getcwd(cwd_stage, sizeof(cwd_stage)) == NULL)
        {
            perror("Could not get main directory!");
            return 1;
        }
    }
    // printf("cwd stage is %s\n",cwd_stage);
    // printf("dest stage is %s\n",destination_file_stage);

    struct dirent *entry_stage;
    DIR *dir_stage = opendir(destination_file_stage);

    if (dir_stage == NULL)
    {
        return 1;
    }
    char tmp_dest_file_stage[MAX_FILENAME_LENGTH];

    strcpy(tmp_dest_file_stage, destination_file_stage);

    while ((entry_stage = readdir(dir_stage)) != NULL)
    {

        // skips these files
        if (strcmp(entry_stage->d_name, ".git") == 0 || strcmp(entry_stage->d_name, ".") == 0 || strcmp(entry_stage->d_name, "..") == 0 || strcmp(entry_stage->d_name, ".samit") == 0)
        {
            continue;
        }

        strcat(destination_file_stage, "/");
        strcat(destination_file_stage, entry_stage->d_name);

        // checks if file is modified/not made
        if (entry_stage->d_type != DT_DIR)
        {

            if (compare_file(destination_file_stage, entry_stage->d_name) == 2)
            {
                for (int i = 0; i < level; i++)
                {
                    printf("    ");
                }
                printf("%s is +D\n", entry_stage->d_name);
            }
        }
        else
        {
            strcat(cwd_stage, "/");
            strcat(cwd_stage, entry_stage->d_name);
            DIR *dir_check = opendir(cwd_stage);
            if (dir_check == NULL)
            {
                // is_first = 0;
                for (int i = 0; i < level; i++)
                {
                    printf("    ");
                }
                printf("[DIRECTORY] %s +D\n", entry_stage->d_name);
                // level++;
                // run_status_destination(argc,argv,level,is_first);
                closedir(dir_check);
            }

            else
                closedir(dir_check);
        }

        strcpy(destination_file_stage, tmp_dest_file_stage);
    }
    closedir(dir_stage);
    return 0;
}

// status
int run_status(int argc, char *argv[], int level)
{
    // finds current directory
    char cwd[MAX_FILENAME_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get main directory!");
        return 1;
    }

    // checks if repo has been initialized
    if (doesHaveInit(cwd) != 1)
    {
        perror("Repo hasn't initialized!");
        return 1;
    }
    if (level == 0)
    {
        // creates destination file directory
        strcpy(destination_file, main_dir);
        strcpy(destination_file_stage, main_dir);
        strcat(destination_file, "/.samit/staging");
        strcat(destination_file_stage, "/.samit/staging");
    }

    struct dirent *entry;
    DIR *dir = opendir(".");
    char output[1000];
    char tmp_dest_file[MAX_FILENAME_LENGTH];

    strcpy(tmp_dest_file, destination_file);

    while ((entry = readdir(dir)) != NULL)
    {
        // skips these files
        if (strcmp(entry->d_name, ".git") == 0 || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".samit") == 0)
        {
            continue;
        }

        // creates destination file directory
        strcat(destination_file, "/");
        strcat(destination_file, entry->d_name);

        if (entry->d_type == DT_DIR)
        {
            for (int i = 0; i < level; i++)
            {
                printf("    ");
            }
            printf("Entering directory %s{\n", entry->d_name);
            strcat(destination_file_stage, "/");
            strcat(destination_file_stage, entry->d_name);
            chdir(entry->d_name);
            level++;
            run_status(argc, argv, level);

            level--;
            chdir("..");
            for (int i = 0; i < level; i++)
            {
                printf("    ");
            }
            printf("}\n");
        }

        // checks if file is modified/not made
        else
        {
            for (int i = 0; i < level; i++)
            {
                printf("    ");
            }
            if (compare_file(destination_file, entry->d_name) == 3)
            {
                printf("%s -", entry->d_name);
                if (compare_file(entry->d_name, destination_file) == 3)
                {
                    printf("D\n");
                }
                else
                {
                    printf("A\n");
                }
            }

            else if (compare_file(destination_file, entry->d_name) == 1)
            {
                printf("%s +M\n", entry->d_name);
            }
            else if (compare_file(destination_file, entry->d_name) == 0)
            {
                printf("%s is +A\n", entry->d_name);
            }
        }

        strcpy(destination_file, tmp_dest_file);
        strcpy(destination_file_stage, tmp_dest_file);
    }
    run_status_destination(argc, argv, level, 1);
    closedir(dir);
    return 0;
}

// commit
int run_commit(int argc, char *argv[])
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
    if (argc < 4)
    {
        perror("Please enter a valid add command");
        return 1;
    }
}

// shortcut set
int run_set(int argc, char *argv[])
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

    chdir(".samit");
    chdir("shortcuts");
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get main directory!");
        return 1;
    }

    FILE *file;

    if (file = fopen(argv[5], "r"))
    {
        perror("Shortcut already exists!");
        fclose(file);
        return 1;
    }
    else
    {
        file = fopen(argv[5], "w");
        fprintf(file, "%s", argv[3]);
    }

    fclose(file);
}

// shortcut replace
int run_replace(int argc, char *argv[])
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

    chdir(".samit");
    chdir("shortcuts");
    if (getcwd(shortcuts_dir, sizeof(shortcuts_dir)) == NULL)
    {
        perror("Could not get main directory!");
        return 1;
    }

    FILE *file = fopen(argv[5], "r");
    if (file)
    {
        fclose(file);
        file = fopen(argv[5], "w");
        fprintf(file, "%s", argv[3]);
    }
    else
    {
        perror("Shortcut doesn't exist!");
        fclose(file);
        return 1;
    }

    fclose(file);
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
    char cwd[MAX_FILENAME_LENGTH];
    char first_cwd[MAX_FILENAME_LENGTH];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not get current directory!");
        return 1;
    }
    strcpy(first_cwd, cwd);

    chdir("/");
    strcpy(config_global_dir, "/home/");
    strcat(config_global_dir, getlogin());
    strcat(config_global_dir, "/.config");
    chdir(config_global_dir);

    // checking if config for samit is made
    DIR *dir;
    struct dirent *entry;
    mkdir("samit", 0755);
    dir = opendir("samit");
    if (dir == NULL)
    {
        return 1;
    }
    chdir("samit");
    strcat(config_global_dir, "/samit");
    if (access("username", F_OK))
    {
        FILE *file = fopen("username", "w");
        fclose(file);
    }
    if (access("email", F_OK))
    {
        FILE *file = fopen("email", "w");
        fclose(file);
    }

    mkdir("alias", 0755);

    chdir(cwd);

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

    else if (strcmp(argv[1], "config") == 0)
    {
        if (argc < 4)
        {
            perror("config requires atleast 4 arguements!");
            return 1;
        }
        if (strcmp(argv[2], "-global") == 0)
        {
            if (argc < 5)
            {
                perror("config global requires atleast 5 arguements!");
                return 1;
            }
            if (strncmp(argv[3], "alias.", 6) == 0)
            {
                run_alias_global(argc, argv);
            }
            else
            {
                run_config_global(argc, argv);
            }
        }
        else
        {
            if (strncmp(argv[2], "alias.", 6) == 0)
            {
                run_alias_local(argc, argv);
            }
            else
            {
                run_config_local(argc, argv);
            }
        }
    }

    else if (strcmp(argv[1], "add") == 0)
    {
        if (strcmp(argv[2], "-n") == 0)
        {
            add_n_depth = strtol(argv[3], NULL, 10);
        }
        run_add(argc, argv, 0);
    }

    else if (strcmp(argv[1], "reset") == 0)
    {
        run_reset(argc, argv, 0);
    }

    else if (strcmp(argv[1], "status") == 0)
    {
        if (argc > 2)
        {
            perror("Too much arguements are added!");
            return 1;
        }
        run_status(argc, argv, 0);
    }

    else if (strcmp(argv[1], "commit") == 0)
    {
        if (argc < 2)
        {
            perror("Too much arguements are added!");
            return 1;
        }
        run_commit(argc, argv);
    }

    else if (strcmp(argv[1], "set") == 0)
    {

        if (argc < 6)
        {
            perror("Too less arguements are added!");
            return 1;
        }
        if (strcmp(argv[2], "-m") == 0 && strcmp(argv[4], "-s") == 0)
        {
            run_set(argc, argv);
        }
        else
        {
            perror("Set command not given correctly!");
            return 1;
        }
    }

    else if (strcmp(argv[1], "replace") == 0)
    {
        if (argc < 6)
        {
            perror("Too less arguements are added!");
            return 1;
        }
        if (strcmp(argv[2], "-m") == 0 && strcmp(argv[4], "-s") == 0)
        {
            run_replace(argc, argv);
        }
        else
        {
            perror("Replace command not given correctly!");
            return 1;
        }
    }

    return 0;
}

// Arshia Samiezad 402111498

/*Github token is:

Username: ArshiaSamiezad
Password: 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
*/