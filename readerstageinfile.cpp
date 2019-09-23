#include "readerstageinfile.h"
#include <QDebug>

ReaderStageInFile::ReaderStageInFile(){}

std::vector<char> & ReaderStageInFile::readFile(int stage)
{
    std::fstream in;

    int tempInt = stage - ((stage-1)/3)*3;

    std::string fileName =  "..\\BattleCity\\" + std::to_string(tempInt) + "stage.txt";
//    std::string fileName =  "../BattleCity/" + std::to_string(tempInt) + "stage.txt";

    qDebug() << fileName.data() << "\n" ;
    in.open(fileName);

    if(!in){
        qDebug() << "Can not open file: "<< "1stage.txt" << " for read\n" ;
        return fields;
    }
    char tempChar;
    if (fields.size() > 0)
        fields.clear();
    qDebug() << "fields.size()" << fields.size() ;
    while ((tempChar = in.get()) !=  EOF) {
        fields.push_back(tempChar);
    }
    in.close();
    qDebug() << "read full" ;
    return fields;
}
