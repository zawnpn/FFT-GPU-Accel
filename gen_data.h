#ifndef GEN_DATA_H
#define GEN_DATA_H

#include <QString>
#include <fstream>
#include <ctime>

void gen_data(QString data_file,int compute_cnt){
    int cnt=0;
//    const int k=compute_cnt;
    int k=1;
    while(compute_cnt--){
        k*=2;
    }
    QByteArray tmp = data_file.toUtf8();
    const char *file_name = tmp.data();
    srand(time(NULL));
    std::ofstream outFile(file_name);
    int n=10000;
    double data;
    while(cnt<k){
        data = 1+ (double)100*(rand() % n)/n;
        outFile<<data<<"\t";
        ++cnt;
    }
    return;
}

#endif // GEN_DATA_H
