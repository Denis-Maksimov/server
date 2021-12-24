#ifndef CRC16_H
#define CRC16_H
#ifdef __cplusplus
#endif
#include <stddef.h>
#ifdef	__cplusplus
extern "C" {
#endif
//для нахождения ошибок
 unsigned short crc16(unsigned char * pcBlock, unsigned short len);
 unsigned long crc32(unsigned long crc32, unsigned char * buf, size_t cnt);

//для хэш-таблиц
 unsigned int PJWHash (void *data, size_t n, size_t table_size);


#ifdef	__cplusplus
}
#endif
#endif // !CRC16_H
