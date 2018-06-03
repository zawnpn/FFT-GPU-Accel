#ifndef GEN_DATA_H
#define GEN_DATA_H

#include <QString>
#include <fstream>
#include <ctime>

//void gen_data(QString data_file,int compute_cnt){
//    double a,b,t,l,s,r,m,y;
//    const double eps=1e-6;
//    int cnt=0;
//    int k=compute_cnt;
//    QByteArray tmp = data_file.toUtf8();
//    const char *file_name = tmp.data();
//    srand(time(NULL));
//    std::ofstream outFile(file_name);
//    int n=10000;
//    while(cnt<k){
//        a= 1+(1.0 + (rand() % n))/n;
//        b= 1+(1.0 + (rand() % n))/n;
//        t= 1+(1.0 + (rand() % n))/n;
//        l= 1+(1.0 + (rand() % n))/n;
//        y=l*exp(a*b*t/4);
//        s=0; r=100;
//        s=a*s; r=a*r;
//        while(r-s>eps){
//            m=(s+r)/2;
//            if(BESSI0(m)>y) r=m;
//            else s=m;
//        }
//        if(b_check(a*s,y)>-0.01&&b_check(a*s,y)<0.01){
//            outFile<<a<<'\t'<<b<<'\t'<<t<<'\t'<<l<<'\t';
//            ++cnt;
//        }
//    }
//    return;
//}

#endif // GEN_DATA_H
