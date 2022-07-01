#ifndef DATAPAK_HPP
    #define DATAPAK_HPP
    #include <vector>
    #include <string>
    #include <stdio.h>

    #define DATAPAK_VERSION "1.0.0"
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
            int getBaseSize(const char* alias); // Get the size of the file decompressed
            int getCompSize(const char* alias); // Get the size of the compressed file

            void write(const char* alias, const std::string& data); // Compress & write given data to the datapak
            std::string read(const char* alias); // Decompress & read data from the datapak 

            void remove(const char* alias); // Remove data under a given alias 
            void purge(); // Reset the datapak (USE VERY CAREFULLY!)
            void close(); // Close the file

    };
#endif