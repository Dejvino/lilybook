#include <stdlib.h>
#include <stdio.h>

class TextReader
{
public:
    TextReader(FILE* file);

    size_t read(long pos, char* text, size_t len);

    void close();
    
private:
    
    FILE* f;
};
