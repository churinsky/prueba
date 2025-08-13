#include <avr/eeprom.h>
#include <stdint.h>
#include <string.h>
#include "eeprom_store.h"

/* Layout en EEPROM
 * [magic:16] [k_fria:32] [k_cal:32] [crc16:16]
 * Total: 2 + 4 + 4 + 2 = 12 bytes
 */
#define K_MAGIC       (0x4B4Fu)   /* 'KO' */
#define K_ADDR_BASE   ((uint16_t)0x0000)

typedef struct __attribute__((packed)) {
    uint16_t magic;
    float    k_fria;
    float    k_cal;
    uint16_t crc;
} kblob_t;

/* CRC16-IBM sencillo sobre bytes del blob excepto el campo crc */
static uint16_t crc16_ibm(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFFu;
    for (uint16_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; ++b) {
            if (crc & 1u) crc = (crc >> 1) ^ 0xA001u;
            else          crc >>= 1;
        }
    }
    return crc;
}

bool KStore_Load(float *k_fria_out, float *k_cal_out)
{
    kblob_t blob;
    eeprom_read_block(&blob, (const void*)K_ADDR_BASE, sizeof(blob));

    if (blob.magic != K_MAGIC) return false;

    /* calcular CRC sobre {magic,k_fria,k_cal} */
    uint16_t calc = crc16_ibm((const uint8_t*)&blob, (uint16_t)(sizeof(blob) - sizeof(blob.crc)));
    if (calc != blob.crc) return false;

    if (k_fria_out) *k_fria_out = blob.k_fria;
    if (k_cal_out)  *k_cal_out  = blob.k_cal;
    return true;
}

bool KStore_Save(float k_fria, float k_cal)
{
    kblob_t blob;
    blob.magic = K_MAGIC;
    blob.k_fria = k_fria;
    blob.k_cal  = k_cal;
    blob.crc = crc16_ibm((const uint8_t*)&blob, (uint16_t)(sizeof(blob) - sizeof(blob.crc)));

    /* eeprom_update_* evita escribir si no cambia (prolonga vida) */
    eeprom_update_block(&blob, (void*)K_ADDR_BASE, sizeof(blob));

    /* Verificación de escritura */
    kblob_t rd;
    eeprom_read_block(&rd, (const void*)K_ADDR_BASE, sizeof(rd));
    return (memcmp(&rd, &blob, sizeof(blob)) == 0);
}
