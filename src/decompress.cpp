#include "decompress.h"

#include <zlib.h>

#include <iostream>
#include <vector>
#include <cstring>

std::string decompress_gzip(const std::string& compressed_data) {
    std::vector<char> decompressed_data;
    z_stream strm;
    memset(&strm, 0, sizeof(strm));

    if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK) {
        std::cerr << "Failed to initialize zlib decompression." << std::endl;
        return "";
    }

    strm.avail_in = compressed_data.size();
    strm.next_in = reinterpret_cast<unsigned char*>(const_cast<char*>(compressed_data.c_str()));

    char buffer[4096];

    // Lambda to process the decompressed chunk and append to the decompressed data
    auto process_decompressed_chunk = [&decompressed_data, &buffer](size_t decompressed_size) {
        decompressed_data.insert(decompressed_data.end(), buffer, buffer + decompressed_size);
    };

    do {
        strm.avail_out = sizeof(buffer);
        strm.next_out = reinterpret_cast<unsigned char*>(buffer);
        int ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            std::cerr << "Error during decompression." << std::endl;
            inflateEnd(&strm);
            return "";
        }
        size_t decompressed_size = sizeof(buffer) - strm.avail_out;
        process_decompressed_chunk(decompressed_size);
    } while (strm.avail_out == 0);

    inflateEnd(&strm);

    return std::string(decompressed_data.begin(), decompressed_data.end());
}