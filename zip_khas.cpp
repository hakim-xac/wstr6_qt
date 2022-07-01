#include "zip_khas.h"
#include <cassert>
#include<iomanip>
#include<iostream>
namespace KHAS {


Zip::Zip(const std::string &zipfilename)
{
//    struct zip_t *zip = zip_open("foo.zip", ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
//    {
//        zip_entry_open(zip, "foo-1.txt");
//        {
//            const char *buf = "Some data here...\0";
//            zip_entry_write(zip, buf, strlen(buf));
//        }
//        zip_entry_close(zip);

//        zip_entry_open(zip, "foo-2.txt");
//        {
//            // merge 3 files into one entry and compress them on-the-fly.
//            zip_entry_fwrite(zip, "foo-2.1.txt");
//            zip_entry_fwrite(zip, "foo-2.2.txt");
//            zip_entry_fwrite(zip, "foo-2.3.txt");
//        }
//        zip_entry_close(zip);
//    }
//    zip_close(zip);
}


void Zip::compress_memory(void *in_data, size_t in_data_size, std::vector<uint8_t> &out_data)
{
    std::vector<uint8_t> buffer;

    const size_t BUFSIZE = 128 * 1024;
    uint8_t temp_buffer[BUFSIZE];

    z_stream strm;
    strm.zalloc = 0;
    strm.zfree = 0;
    strm.next_in = reinterpret_cast<uint8_t *>(in_data);
    strm.avail_in = in_data_size;
    strm.next_out = temp_buffer;
    strm.avail_out = BUFSIZE;

    deflateInit(&strm, Z_BEST_COMPRESSION);

    while (strm.avail_in != 0)
    {
        int res = deflate(&strm, Z_NO_FLUSH);
        assert(res == Z_OK);
        if (strm.avail_out == 0)
        {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
    }

    int deflate_res = Z_OK;
    while (deflate_res == Z_OK)
    {
        if (strm.avail_out == 0)
        {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
        deflate_res = deflate(&strm, Z_FINISH);
    }

    assert(deflate_res == Z_STREAM_END);
    buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
    deflateEnd(&strm);

    out_data.swap(buffer);
}

void Zip::add_buffer_to_vector(std::vector<char> &vector, const char *buffer, uLongf length) {
    for (int character_index = 0; character_index < length; character_index++) {
        char current_character = buffer[character_index];
        vector.push_back(current_character);
    }
}

int Zip::compress_vector(std::vector<char> source, std::vector<char> &destination) {
    unsigned long source_length = source.size();
    uLongf destination_length = compressBound(source_length);

    char *destination_data = (char *) malloc(destination_length);
    if (destination_data == nullptr) {
        return Z_MEM_ERROR;
    }

    Bytef *source_data = (Bytef *) source.data();
    int return_value = compress2((Bytef *) destination_data, &destination_length, source_data, source_length,
                                 Z_BEST_COMPRESSION);
    add_buffer_to_vector(destination, destination_data, destination_length);
    free(destination_data);
    return return_value;
}

int Zip::decompress_vector(std::vector<char> source, std::vector<char> &destination) {
    unsigned long source_length = source.size();
    uLongf destination_length = compressBound(source_length);

    char *destination_data = (char *) malloc(destination_length);
    if (destination_data == nullptr) {
        return Z_MEM_ERROR;
    }

    Bytef *source_data = (Bytef *) source.data();
    int return_value = uncompress((Bytef *) destination_data, &destination_length, source_data, source.size());
    add_buffer_to_vector(destination, destination_data, destination_length);
    free(destination_data);
    return return_value;
}

void Zip::add_string_to_vector(std::vector<char> &uncompressed_data,
                          const char *my_string) {
    int character_index = 0;
    while (true) {
        char current_character = my_string[character_index];
        uncompressed_data.push_back(current_character);

        if (current_character == '\00') {
            break;
        }

        character_index++;
    }
}

void Zip::test_compression() {
    std::vector<char> uncompressed(0);
    auto *my_string = (char *) "Hello, world!";
    add_string_to_vector(uncompressed, my_string);

    std::vector<char> compressed(0);
    int compression_result = compress_vector(uncompressed, compressed);
    assert(compression_result == Z_OK);

    std::vector<char> decompressed(0);
    int decompression_result = decompress_vector(compressed, decompressed);
    assert(decompression_result == Z_OK);
    std::cout << "Uncompressed: " << uncompressed.data() << std::endl;
    std::cout << "Compressed: ";

    std::ostream &standard_output = std::cout;
    print_bytes(standard_output, (const unsigned char *) compressed.data(), compressed.size(), false);

    std::cout << "Decompressed: " << decompressed.data() << std::endl;
}

void Zip::print_bytes(std::ostream &stream, const unsigned char *data, size_t data_length, bool format) {
    stream << std::setfill('0');
    for (size_t data_index = 0; data_index < data_length; ++data_index) {
        stream << std::hex << std::setw(2) << (int) data[data_index];
        if (format) {
            stream << (((data_index + 1) % 16 == 0) ? "\n" : " ");
        }
    }
    stream << std::endl;
}

}
