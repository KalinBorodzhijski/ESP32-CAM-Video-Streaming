#include "utils/hash_utils.h"
#include "mbedtls/sha256.h"
#include <string.h>
#include <stdio.h>

void hash_password(const char *password, char *hashed_password)
{
    unsigned char hash[32];
    mbedtls_sha256_context sha256_ctx;

    mbedtls_sha256_init(&sha256_ctx);
    mbedtls_sha256_starts(&sha256_ctx, 0);
    mbedtls_sha256_update(&sha256_ctx, (unsigned char *)password, strlen(password));
    mbedtls_sha256_finish(&sha256_ctx, hash);
    mbedtls_sha256_free(&sha256_ctx);

    for (int i = 0; i < 32; i++)
    {
        sprintf(hashed_password + (i * 2), "%02x", hash[i]);
    }
    hashed_password[64] = 0;
}
