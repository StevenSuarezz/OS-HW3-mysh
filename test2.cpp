#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
    int result;
    const char* filename = "/tmp/myfile";
    struct stat buf;

    result = stat(filename, &buf);
    std::cout << result << std::endl;
    std::cout << buf.st_mode == S_IFREG << std::endl;
}