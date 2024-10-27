#include <compress/info_src_model.h>

int dump_model_to_file(ism_t ***symbols_arr, const char *filepath)
{
    logs_log_trace(USER, "Dumping the model into %s... ", filepath);

    FILE *pFile = fopen(filepath, "w");
    if (pFile == NULL)
    {
        logs_log_error(USER, "Error opening the file %s", filepath);
        return -1;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if ((*symbols_arr)[i] != NULL)
        {
            fprintf(pFile, "0x%02X;%d\n", (*symbols_arr)[i]->symbol, (*symbols_arr)[i]->count);
        }
    }

    fclose(pFile);

    return 0;
}

int build_info_model(FILE *pFile, ism_t ***symbols_arr)
{
    if (pFile == NULL)
    {
        return -1;
    }

    char ch;

    logs_log_trace(USER, "Reading the file ... ");
    while ((ch = fgetc(pFile)) != EOF)
    {
        unsigned char index = (unsigned char)ch;

        if ((*symbols_arr)[index] == NULL)
        {
            logs_log_trace(USER, " * New symbol: 0x%02X", ch);
            (*symbols_arr)[index] = (ism_t *)malloc(sizeof(ism_t));
            (*symbols_arr)[index]->symbol = ch;
            (*symbols_arr)[index]->count = 1;
        }
        else
        {
            logs_log_trace(USER, "  + Symbol [0x%02X] count updated: %d", ch, (*symbols_arr)[index]->count);
            (*symbols_arr)[index]->count++;
        }
    }

    return 0;
}

int sort_model(ism_t ***symbols_arr)
{
    ism_t *tmp;

    for (int i = 0; i < ALPHABET_SIZE - 1; i++)
    {
        for (int j = 0; j < ALPHABET_SIZE - i - 1; j++)
        {
            if ((*symbols_arr)[j] != NULL && (*symbols_arr)[j + 1] != NULL)
            {
                if ((*symbols_arr)[j]->count < (*symbols_arr)[j + 1]->count)
                {
                    tmp = (*symbols_arr)[j];
                    (*symbols_arr)[j] = (*symbols_arr)[j + 1];
                    (*symbols_arr)[j + 1] = tmp;
                }
            }
            else if ((*symbols_arr)[j] == NULL && (*symbols_arr)[j + 1] != NULL)
            {
                tmp = (*symbols_arr)[j];
                (*symbols_arr)[j] = (*symbols_arr)[j + 1];
                (*symbols_arr)[j + 1] = tmp;
            }
        }
    }

    logs_log_trace(USER, "Sorted array ([index] : [symbol] - count): ");
    int elements_count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if ((*symbols_arr)[i] != NULL)
        {
            logs_log_trace(USER, "  - [%d] : [0x%02X] - %d", i, (*symbols_arr)[i]->symbol, (*symbols_arr)[i]->count);
            elements_count++;
        }
    }

    return elements_count;
}