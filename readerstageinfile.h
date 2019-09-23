#ifndef READERSTAGEINFILE_H
#define READERSTAGEINFILE_H

#include <fstream>
#include <vector>

class ReaderStageInFile
{
public:
    ReaderStageInFile();
    std::vector<char> &readFile(int stage);

private:
    std::vector<char> fields;
};

#endif // READERSTAGEINFILE_H
