#include <assert.h>
#include <stdlib.h>

#include "Tpm.h"

int main(void)
{
    /* This is supposed to be the OBJECT that requires the most bytes
     * when it is marshalled: currently an RSA key
     */
    OBJECT object = {
        .attributes = {
            .occupied = SET,
        },
        .publicArea = {
            .type = TPM_ALG_RSA,
            .nameAlg = TPM_ALG_SHA512,
            .objectAttributes = 0,
            .authPolicy.t = {
                .size = sizeof(object.publicArea.authPolicy.t.buffer),
            },
            .parameters.rsaDetail = {
                .symmetric = {
                    .algorithm = TPM_ALG_AES,
                    .keyBits = 256,
                    .mode = TPM_ALG_ECB,
                },
                .scheme = TPM_ALG_RSAPSS,
                .keyBits = MAX_RSA_KEY_BITS,
                .exponent = 0x10001,
            },
            .unique.rsa.t = {
                .size = sizeof(object.publicArea.unique.rsa.t.buffer),
            },
        },
        .sensitive = {
            .sensitiveType = TPM_ALG_RSA,
            .authValue.t = {
                .size = sizeof(object.sensitive.authValue.t.buffer),
            },
            .seedValue.t = {
                .size = sizeof(object.sensitive.seedValue.t.buffer),
            },
            .sensitive.rsa.t = {
                .size = sizeof(object.sensitive.sensitive.rsa.t.buffer),
            },
        },
        .privateExponent = {
            .Q = {
                .size = CRYPT_WORDS(BITS_TO_BYTES(MAX_RSA_KEY_BITS / 2)),
            },
            .dP = {
                .size = CRYPT_WORDS(BITS_TO_BYTES(MAX_RSA_KEY_BITS / 2)),
            },
            .dQ = {
                .size = CRYPT_WORDS(BITS_TO_BYTES(MAX_RSA_KEY_BITS / 2)),
            },
            .qInv = {
                .size = CRYPT_WORDS(BITS_TO_BYTES(MAX_RSA_KEY_BITS / 2)),
            },
        },
        .qualifiedName.t = {
            .size = sizeof(object.qualifiedName.t.name),
        },
        .evictHandle = 0x12345678,
        .name.t = {
            .size = sizeof(object.name.t.name),
        },
        .seedCompatLevel = 1,
    };
    /* this buffer must only be filled to <= MAX_MARSHALLED_OBJECT_SIZE bytes */
    BYTE buffer[2 * MAX_MARSHALLED_OBJECT_SIZE];
    BYTE *buf = buffer;
    INT32 size = sizeof(buffer);
    UINT32 written;

    written = ANY_OBJECT_Marshal(&object, &buf, &size);
    if (written > MAX_MARSHALLED_OBJECT_SIZE) {
        fprintf(stderr,
                "Expected flattened OBJECT to have %zu bytes, but it has %u.\n",
                MAX_MARSHALLED_OBJECT_SIZE, written);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "  written = %d  < MAX_MARSHALLED_OBJECT_SIZE = %zu\n",
            written, MAX_MARSHALLED_OBJECT_SIZE);
    return EXIT_SUCCESS;
}