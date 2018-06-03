#ifndef FFT_ACCEL_H
#define FFT_ACCEL_H

#include <QString>
#include <fstream>

QString* get_accel_result(QString accel_exec, QString data_file, int compute_cnt){
    QString *result = new QString[3];
    int p1 = accel_exec.lastIndexOf('/');
    QString dir_loc = accel_exec.left(p1);
    QString accel_proc = accel_exec+" -d " + data_file + " -r " + dir_loc + "/accel_result.txt -n " + QString::number(compute_cnt);
    qDebug()<<accel_proc;
    QProcess *proc = new QProcess;
    proc->start(accel_proc);
    while (false == proc->waitForFinished()){;}
    FILE *f;
    double d1,d2,d3;
    QString tmp_loc = dir_loc + "/fft_accel_tmp";
    QByteArray tmp = tmp_loc.toUtf8();
    const char *accel_tmp = tmp.data();
    f = fopen(accel_tmp,"r+");
    fscanf(f,"%lf%lf%lf",&d1,&d2,&d3);
    result[0] = QString::number(d1);
    result[1] = QString::number(d2);
    result[2] = QString::number(d3);
    return result;
}

#endif // FFT_ACCEL_H
