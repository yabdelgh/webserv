
#include <fstream>
#include <sstream>
#include <vector>

std::string get_file_contents(std::string const& filename, std::string &buffer)
{
    std::ifstream       file(filename);
    if (file)
    {
        /*
         * Get the size of the file
         */
        file.seekg(0,std::ios::end);
        std::streampos          length = file.tellg();
        file.seekg(0,std::ios::beg);

        /*
         * Use a vector as the buffer.
         * It is exception safe and will be tidied up correctly.
         * This constructor creates a buffer of the correct length.
         * Because char is a POD data type it is not initialized.
         *
         * Then read the whole file into the buffer.
         */
        
        std::vector<char> tmp_buffer(length);
        file.read(tmp_buffer.data(),length);
        buffer = tmp_buffer.data();
        return buffer;
    }
    throw std::runtime_error("cannot open file");
}