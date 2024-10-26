#include <crc.h>

int crc32(const unsigned char *buf, int len)
{
    int crc = CRC32_INIT;
    for (int i = 0; i < len; i++)
    {
        crc ^= buf[i];
        for (int j = 0; j < 8; j++)
        {
            crc = (crc >> 1) ^ ((crc & 1) ? CRC32_POLYNOMIAL_REV : 0);
        }
    }
    return ~crc;
}

int crc32_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        return -1;
    }
    int crc = CRC32_INIT;
    unsigned char buf[4096];
    size_t len;
    while ((len = fread(buf, 1, sizeof(buf), file)) > 0)
    {
        crc = crc32(buf, len);
    }
    fclose(file);

    return crc;
}

int crc32_file_read(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        return -1;
    }
    int crc = CRC32_INIT;

    fseek(file, -4, SEEK_END);
    unsigned char buf[4];
    if (fread(buf, 1, 4, file) == 4)
    {
        crc = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
    }

    fclose(file);
    return crc;
}