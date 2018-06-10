#ifndef FFT_MATLAB_H
#define FFT_MATLAB_H

#include <QString>
#include <QProcess>
#include <QDebug>
#include <iostream>

QString* get_matlab_result(QString matlab_loc, QString matlab_script, QString data_file, int compute_cnt, bool run_matlab=true){
    QString *result = new QString[3];
    int p1 = matlab_script.lastIndexOf('/');
    int p2 = matlab_script.lastIndexOf('.');
    QString dir_loc = matlab_script.left(p1);
    if(run_matlab){
        QString script_name = matlab_script.mid(p1+1,p2-p1-1);
        QString matlab_proc = matlab_loc+" -nodesktop -r \"cd "+dir_loc+";"+script_name+"('"+data_file+"',"+QString::number(compute_cnt)+")\"";
        qDebug()<<matlab_proc;
        QProcess *proc = new QProcess;
        proc->start(matlab_proc);
        while (false == proc->waitForFinished()){;}
    }
    FILE *f;
    double d1,d2,d3;
    QString script_loc = dir_loc + "/fft_matlab_tmp";
    QByteArray tmp = script_loc.toUtf8();
    const char *mat_script = tmp.data();
    f = fopen(mat_script,"r+");
    fscanf(f,"%lf%lf%lf",&d1,&d2,&d3);
    result[0] = QString::number(d1);
    result[1] = QString::number(d2);
    result[2] = QString::number(d3);
    return result;
}

#endif // FFT_MATLAB_H
