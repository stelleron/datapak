#ifndef DATAPAK_H
    #define DATAPAK_H
    #include <string>
    #include <fstream>
    #include <vector>
    // Macros to define .datapak access type
    #define DATAPAK_READ 'r'
    #define DATAPAK_WRITE 'w'
    #define DATAPAK_APPEND 'a'
    // Stores a .datapak instance
    struct DataPak {
        private:
            struct DataChunk {
                std::string alias;
                int startline;
                int endline;
                DataChunk(){}
            };
            std::fstream file;
            std::string filename;
            char accessType;   
            std::vector<DataChunk> chunks; // Store all of the chunks in the .datapak
        public:
            // Open an existing .datapak file or load a currently existing one
            DataPak(const char* filename, const char accessType);
            // Destroy the datapak object
            ~DataPak();

            // Check for the presence of data under a given alias
            bool findData(const char* alias);
            // Get the size of file data stored under a given alias
            int getDataSize(const char* alias);
            // Decompress and read file data from a given alias
            void readData(const char* alias, unsigned char* dataptr);
            // Compress and write file data under a given alias
            void writeData(const char* alias, unsigned char* dataptr, int size);
    };
#endif