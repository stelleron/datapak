#ifndef DATAPAK_HPP
    #define DATAPAK_HPP
    #include <vector>
    #include <string>
    #include <stdio.h>
    #define DATAPAK_VERSION "0.0.0"
    // Struct for storing the datapak file (extension blind)
    struct Datapak {
        private:
            // Struct for the file header
            struct FileHeader {
                char version[15];
                int dataCount;
            };

            // Struct for every data header
            struct DataHeader {
                char alias[200];
                int baseSize;
                int compSize;
            };

            // Struct to store chunks of data
            struct DataChunk {
                DataHeader header;
                std::string data;
            };

            // Fields
            FILE* file;
            std::string filename;
            FileHeader header;
            std::vector<DataChunk> chunks;

            bool isClosed;
        public:
            Datapak(const char* filepath); // Load the datapak
            ~Datapak();

            bool find(const char* alias); // Find whether the given file exists in the datapak
            int getCompSize(const char* alias); // Get the size of the compressed file in the datapak (in bytes)
            int getSize(const char* alias); // Get the size of the file decompressed (in bytes)

            void write(const char* alias, const std::string& data); // Compress and write given data to the datapak
            std::string read(const char* alias); // Decompress and read data  

            void purge(); // Reset the datapak (USE VERY CAREFULLY!)
            void close(); // Close the file

    };
#endif