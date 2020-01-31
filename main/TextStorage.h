#include "TextReader.h"

class TextStorage
{
public:
    TextStorage();

    TextReader* open(char* filename);
    void close(TextReader* reader);
};
