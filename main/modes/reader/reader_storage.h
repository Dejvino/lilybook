#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

void reader_storage_get_filename(char* buf, int len);
long reader_storage_get_length();
void reader_storage_store_file(char* source_path);
long reader_storage_get_position();
void reader_storage_set_position(long position);

#ifdef __cplusplus
}
#endif
