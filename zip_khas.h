#ifndef ZIP_H
#define ZIP_H

#include "src/zlib.h"
#include <vector>
#include <string>
#include <cstring>

namespace KHAS {

class Zip
{
private:

    ///
    /// \brief buffer_
    ///
    std::vector<std::string> buffer_;



public:

    ///
    /// \brief Zip
    ///
    Zip() = delete;

    Zip(const std::string& zipfilename);

    void compress_memory(void* in_data, size_t in_data_size, std::vector<uint8_t>& out_data);

    void add_buffer_to_vector(std::vector<char> &vector, const char *buffer, uLongf length);

    int compress_vector(std::vector<char> source, std::vector<char> &destination);

    int decompress_vector(std::vector<char> source, std::vector<char> &destination);

    void add_string_to_vector(std::vector<char> &uncompressed_data,
                              const char *my_string);

    void test_compression();

    void print_bytes(std::ostream &stream, const unsigned char *data, size_t data_length, bool format = true);
};



}
#endif // ZIP_H
