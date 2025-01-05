#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include <iostream>
#include <sstream>
#include <string>
#include <Poco/StreamCopier.h>
#include <Poco/InflatingStream.h>
#include <Poco/Exception.h>

inline std::string decompress_gzip(const std::string& compressed_data) {
    std::istringstream compressed_stream(compressed_data);
    std::ostringstream decompressed_stream;

    try {
        Poco::InflatingInputStream inflater(compressed_stream, Poco::InflatingStreamBuf::STREAM_GZIP);
        Poco::StreamCopier::copyStream(inflater, decompressed_stream);
    } catch (const Poco::Exception& e) {
        std::cerr << "Decompression failed: " << e.displayText() << std::endl;
        return "";
    }

    return decompressed_stream.str();
}

#endif