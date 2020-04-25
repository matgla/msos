#include <stdio.h>

extern "C"
{

int fileno(FILE *stream)
{
    return stream->_file;
}

}
