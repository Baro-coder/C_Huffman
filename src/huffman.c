#include <huffman.h>

// Print help information
static void _print_help(char *program_name)
{
    logs_log_info(USER, "Printing help information...");

    printf("\n%s v%s\n", APP_NAME, APP_VERSION);
    printf("%s\n", APP_INFO);
    printf("%s\n\n", APP_COPYRIGHT);
    printf("\nUsage: %s <OPERATOR> <FILEPATH> <OPTIONAL_ARGS>\n\n", program_name);

    printf("OPERATORS:\n");
    printf("  -c : compress\n");
    printf("  -d : decompress\n");
    printf("  -k : calculate CRC32 for file\n");

    printf("For example:\n\n");

    printf("  %s -c file.txt\t\t- compress `file.txt` into `file.txt.huff`\n", program_name);
    printf("  %s -d file.txt%s\t- decompress `file.txt%s` into `file.txt`\n", program_name, SUFIX_COMPRESSED, SUFIX_COMPRESSED);
    printf("  %s -k file.txt\t\t- calculate CRC for `file.txt`\n\n", program_name);
}

// Print bad usage information and return STATUS_ERROR
static STATUS _bad_usage(char *program_name)
{
    logs_log_error(USER, "Error! Bad usage!");
    _print_help(program_name);
    return STATUS_ERROR_BAD_USAGE;
}

// Run a procedure, print status and return it
static STATUS _run_procedure(STATUS (*func)(char *), char *filepath)
{
    STATUS status = func(filepath);

    switch (status)
    {
    case STATUS_OK:
        logs_log_info(USER, "Procedure finished successfully!");
        break;
    case STATUS_ERROR_FILE_NOT_FOUND:
        logs_log_error(USER, "Error! File not found!");
        break;
    case STATUS_ERROR_FILE_CANNOT_CREATE:
        logs_log_error(USER, "Error! Output file cannot be created!");
        break;
    case STATUS_ERROR_COMPRESS:
        logs_log_error(USER, "Error! Error compressing file!");
        break;
    case STATUS_ERROR_DECOMPRESS:
        logs_log_error(USER, "Error! Error decompressing file!");
        break;
    case STATUS_ERROR_CRC:
        logs_log_error(USER, "Error! CRC32 checksum error!");
        break;
    case STATUS_ERROR:
        logs_log_fatal(USER, "Error! Fatal error!");
        break;
    default:
        break;
    }

    return status;
}

int _mkdir(const char *dir)
{
    struct stat st = {0};

    if (stat(dir, &st) == -1)
    {
#ifdef __WIN32
        return mkdir(dir);
#else
        return mkdir(dir, 0755);
#endif
    }

    return 1;
}

int _create_dirs(void)
{
    logs_log_info(USER, "Creating directories...");

    // Create tmp directory
    logs_log_debug(USER, " - Creating tmp directory [%s]...", DIR_TMP);
    if (_mkdir(DIR_TMP) == -1 && errno != EEXIST)
    {
        logs_log_fatal(USER, "Error creating tmp directory!");
        return 1;
    }

    // Create models directory
    logs_log_debug(USER, " - Creating models directory [%s]...", DIR_MODELS);
    if (_mkdir(DIR_MODELS) == -1 && errno != EEXIST)
    {
        logs_log_fatal(USER, "Error creating models directory!");
        return 1;
    }

    // Create trees directory
    logs_log_debug(USER, " - Creating trees directory [%s]...", DIR_TREES);
    if (_mkdir(DIR_TREES) == -1 && errno != EEXIST)
    {
        logs_log_fatal(USER, "Error creating trees directory!");
        return 1;
    }

    // Create tables directory
    logs_log_debug(USER, " - Creating tables directory [%s]...", DIR_TABLES);
    if (_mkdir(DIR_TABLES) == -1 && errno != EEXIST)
    {
        logs_log_fatal(USER, "Error creating tables directory!");
        return 1;
    }

    // Create crc directory
    logs_log_debug(USER, " - Creating crc directory [%s]...", DIR_CRC);
    if (_mkdir(DIR_CRC) == -1 && errno != EEXIST)
    {
        logs_log_fatal(USER, "Error creating crc directory!");
        return 1;
    }

    // Create out directory
    logs_log_debug(USER, " - Creating out directory [%s]...", DIR_OUT);
    if (_mkdir(DIR_OUT) == -1 && errno != EEXIST)
    {
        logs_log_fatal(USER, "Error creating out directory!");
        return 1;
    }

    // Create compressed directory
    logs_log_debug(USER, " - Creating compressed directory [%s]...", DIR_COMPRESSED);
    if (_mkdir(DIR_COMPRESSED) == -1 && errno != EEXIST)
    {
        logs_log_fatal(USER, "Error creating compressed directory!");
        return 1;
    }

    // Create decompressed directory
    logs_log_debug(USER, " - Creating decompressed directory [%s]...", DIR_DECOMPRESSED);
    if (_mkdir(DIR_DECOMPRESSED) == -1 && errno != EEXIST)
    {
        logs_log_fatal(USER, "Error creating decompressed directory!");
        return 1;
    }

    return 0;
}

int _create_log_file(void)
{
    // Create logs directory
    logs_log_debug(USER, "Creating logs directory [%s]...", DIR_LOGS);
    if (_mkdir(DIR_LOGS) == -1 && errno != EEXIST)
    {
        logs_log_fatal(USER, "Error creating logs directory!");
        return 1;
    }

    logs_log_info(USER, " - Creating log file...");
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *tm = localtime(&tv.tv_sec);
    char log_filename[256];
    snprintf(log_filename, sizeof(log_filename), "%s/%04d-%02d-%02d_%02d-%02d-%02d_%06ld.log", DIR_LOGS, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec);
    FILE *log_stream = fopen(log_filename, "w");

    if (log_stream == NULL)
    {
        return 1;
    }
    logs_set_output_stream(log_stream);
    logs_log_info(USER, " + Log file created: %s", log_filename);
    return 0;
}

void init(FILE **log_stream)
{
    logs_log_info(USER, "Initialization started...");

    if (_create_log_file() != 0)
    {
        logs_log_fatal(USER, "Error creating log file!");
        exit(1);
    }

    if (_create_dirs() != 0)
    {
        logs_log_fatal(USER, "Error creating directories!");
        exit(1);
    }
    logs_log_info(USER, " + Directories created.");

#ifdef __linux__
    logs_threads_safety_enable("huffman");
    logs_log_info(USER, "Threads safety enabled.");
#endif

    logs_log_info(USER, " + Initialization completed.");
}

/* *************************************************************************** */

#include <stdio.h>

int swap_endian(int num)
{
    return ((num >> 24) & 0x000000FF) |
           ((num >> 8) & 0x0000FF00) |
           ((num << 8) & 0x00FF0000) |
           ((num << 24) & 0xFF000000);
}

/* *************************************************************************** */
// Main function
int main(int argc, char **argv)
{
    STATUS status = STATUS_OK;
    FILE *log_stream = NULL;

    init(&log_stream);
    logs_log_info(USER, "Version: %s", APP_VERSION);
    logs_log_info(USER, "%s", APP_COPYRIGHT);

    if (argc >= 2)
    {
        char *mode = argv[1];

        if (strlen(mode) != 2 && strcmp(mode, "-c") != 0 && strcmp(mode, "-d") != 0)
        {
            status = _bad_usage(argv[0]);
        }

        if (strcmp(mode, "-h") == 0)
        {
            logs_log_debug(USER, "Help requested.");
            _print_help(argv[0]);

            logs_log_info(USER, "End of work - status code: %d", status);
            if (log_stream != NULL)
            {
                if (fclose(log_stream) != 0)
                {
                    logs_log_fatal(USER, "Error closing log file!");
                    return STATUS_ERROR;
                }
                logs_log_debug(USER, "Log file closed.");
            }
            return status;
        }

        char *filepath = argv[2];

        if (strcmp(mode, MODE_COMPRESS) == 0)
        {
            logs_log_info(USER, "MODE: Compress");
            logs_log_info(USER, "FILE: %s", filepath);
            status = _run_procedure(procedure_compress, filepath);
        }
        else if (strcmp(mode, MODE_DECOMPRESS) == 0)
        {
            logs_log_info(USER, "MODE: Decompress");
            logs_log_info(USER, "FILE: %s", filepath);
            status = _run_procedure(procedure_decompress, filepath);
        }
        else if (strcmp(mode, MODE_CRC_CALC) == 0)
        {
            logs_log_info(USER, "MODE: CRC32 Calculation");
            logs_log_info(USER, "FILE: %s", filepath);
            status = _run_procedure(procedure_crc_calc, filepath);
        }
        else
        {
            status = _bad_usage(argv[0]);
        }
    }
    else
    {
        status = _bad_usage(argv[0]);
    }

    logs_log_info(USER, "End of work - status code: %d", status);
#ifdef __linux__
    logs_threads_safety_disable("huffman");
    logs_log_info(USER, "Threads safety disabled.");
#endif

    if (log_stream != NULL)
    {
        if (fclose(log_stream) != 0)
        {
            logs_log_fatal(USER, "Error closing log file!");
            return STATUS_ERROR;
        }
        logs_log_debug(USER, "Log file closed.");
    }
    return status;
}