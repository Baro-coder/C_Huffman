#ifndef BS_SUFFIXES_H
#define BS_SUFFIXES_H

#pragma GCC diagnostic ignored "-Wunused-function"

#include <sys/stat.h>
#include <string.h>

#define APP_NAME "Huffman"                                                                    // Application name
#define APP_VERSION "1.0.0"                                                                   // Application version
#define APP_AUTHOR "Bartłomiej Szykuła"                                                       // Application author
#define APP_YEAR "2024"                                                                       // Application year
#define APP_INFO "Huffman algorithm implementation in C"                                      // Application info
#define APP_COPYRIGHT "Copyright © " APP_YEAR " | " APP_AUTHOR " | " APP_OS " | " APP_VERSION // Application copyright

#ifdef _WIN32
#define APP_OS "Windows"
#else
#define APP_OS "Linux"
#endif

#include <limits.h>

#ifdef _WIN32
#include <windows.h>
#include <Lmcons.h>

static char *get_username()
{
    static char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserName(username, &username_len);
    return username;
}

static char *get_executable_path()
{
    static char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    // Remove the executable name to get the directory path
    char *last_backslash = strrchr(path, '\\');
    if (last_backslash != NULL)
    {
        *last_backslash = '\0';
    }
    return path;
}

static char *get_parent_directory(const char *path)
{
    static char parent_path[MAX_PATH];
    strcpy(parent_path, path);

    char *last_backslash = strrchr(parent_path, '\\');
    if (last_backslash != NULL)
    {
        *last_backslash = '\0';
    }

    return parent_path;
}

#define APP_WORKSPACE get_executable_path() // Workspace directory
#define USER get_username()                 // User name
#define PATH_SEP "\\"                       // Windows path separator

#else
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

static char *get_username()
{
    struct passwd *pw = getpwuid(getuid());
    return pw ? pw->pw_name : "unknown";
}

static char *get_executable_path()
{
    static char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count != -1)
    {
        path[count] = '\0';
        // Remove the executable name to get the directory path
        char *last_slash = strrchr(path, '/');
        if (last_slash != NULL)
        {
            *last_slash = '\0';
        }
    }
    return path;
}

static char *get_parent_directory(const char *path)
{
    static char parent_path[PATH_MAX];
    strcpy(parent_path, path);

    char *last_slash = strrchr(parent_path, '/');
    if (last_slash != NULL)
    {
        *last_slash = '\0';
    }

    return parent_path;
}

#define APP_WORKSPACE get_executable_path() // Workspace directory
#define USER get_username()                 // User name
#define PATH_SEP "/"                        // Linux path separator
#endif

#define DIR_LOGS_NAME "logs"                 // Logs directory name
#define DIR_OUT_NAME "out"                   // Output directory name
#define DIR_TMP_NAME "tmp"                   // Temporary directory name
#define DIR_MODELS_NAME "models"             // Models directory name
#define DIR_TREES_NAME "trees"               // Coding Trees directory name
#define DIR_TABLES_NAME "tables"             // Code Tables directory name
#define DIR_CRC_NAME "crc"                   // CRC32 directory name
#define DIR_COMPRESSED_NAME "compressed"     // Compressed files directory name
#define DIR_DECOMPRESSED_NAME "decompressed" // Decompressed files directory name

#define PATH_MAX 4096

static char *concat_paths(const char *path1, const char *path2)
{
    static char result[PATH_MAX];

    strcpy(result, path1);

    if (result[strlen(result) - 1] != PATH_SEP[0])
    {
        strcat(result, PATH_SEP);
    }

    strcat(result, path2);

    if (result[strlen(result) - 1] != PATH_SEP[0])
    {
        strcat(result, PATH_SEP);
    }

    return result;
}

#define DIR_LOGS concat_paths(get_parent_directory(APP_WORKSPACE), DIR_LOGS_NAME) // Logs directory
#define DIR_TMP concat_paths(get_parent_directory(APP_WORKSPACE), DIR_TMP_NAME)   // Temporary directory
#define DIR_OUT concat_paths(get_parent_directory(APP_WORKSPACE), DIR_OUT_NAME)   // Output directory

#define DIR_MODELS concat_paths(DIR_TMP, DIR_MODELS_NAME) // Models directory
#define DIR_TREES concat_paths(DIR_TMP, DIR_TREES_NAME)   // Coding Trees directory
#define DIR_TABLES concat_paths(DIR_TMP, DIR_TABLES_NAME) // Code Tables directory
#define DIR_CRC concat_paths(DIR_TMP, DIR_CRC_NAME)       // CRC32 directory

#define DIR_COMPRESSED concat_paths(DIR_OUT, DIR_COMPRESSED_NAME)     // Compressed files directory
#define DIR_DECOMPRESSED concat_paths(DIR_OUT, DIR_DECOMPRESSED_NAME) // Decompressed files directory

#define ALPHABET_SIZE 1024 // ?? ASCII table size

#define SUFIX_COMPRESSED ".huff"  // Compressed file sufix
#define SUFIX_MODEL ".model"      // Info source model sufix
#define SUFIX_CODE_TABLE ".code"  // Code table sufix
#define SUFIX_CODING_TREE ".tree" // Coding tree sufix
#define SUFIX_CRC ".crc"          // CRC32 sufix

#define SUFIX_COMPRESSED_SIZE strlen(SUFIX_COMPRESSED)   // Compressed file sufix size
#define SUFIX_MODEL_SIZE strlen(SUFIX_MODEL)             // Info source model file sufix size
#define SUFIX_CODE_TABLE_SIZE strlen(SUFIX_CODE_TABLE)   // Code table file sufix size
#define SUFIX_CODING_TREE_SIZE strlen(SUFIX_CODING_TREE) // Coding tree file sufix size
#define SUFIX_CRC_SIZE strlen(SUFIX_CRC)                 // CRC32 file sufix size

#endif