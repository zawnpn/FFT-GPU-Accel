#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QProcess>
#include "fft_accel.h"
#include "fft_matlab.h"
#include "gen_data.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    matlab_loc = "/usr/local/MATLAB/R2017b/bin/matlab";
    compute_cnt = 10;
    data_loc = "/home/zwp/Test/fft_test/fft_data.txt";
    accel_exec = "/home/zwp/Test/fft_test/fft_accel";
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_accel_clicked()
{
    ui->label_accel_time->setText(tr("生成结果中..."));
    ui->label_accel_counts->setText(tr("生成结果中..."));
    ui->label_accel_once->setText(tr("生成结果中..."));
    QString *accel_result;
    accel_result = get_accel_result(this->accel_exec, this->data_loc, this->compute_cnt);
    ui->label_accel_time->setText(accel_result[0]);
    ui->label_accel_counts->setText(accel_result[1]);
    ui->label_accel_once->setText(accel_result[2]);
}

void Widget::on_pushButton_matlab_clicked()
{
    ui->label_matlab_time->setText(tr("生成结果中..."));
    ui->label_matlab_counts->setText(tr("生成结果中..."));
    ui->label_matlab_once->setText(tr("生成结果中..."));
    QMessageBox::information(this,tr("Matlab FFT 算法"), tr("正在调用Matlab运行FFT算法，请稍等..."));
    QString *matlab_result = get_matlab_result(this->matlab_loc, this->matlab_script, this->data_loc, this->compute_cnt);
    QMessageBox::information(this,tr("Matlab FFT 算法"), tr("运行完毕，请检查结果！"));
    ui->label_matlab_time->setText(matlab_result[0]);
    ui->label_matlab_counts->setText(matlab_result[1]);
    ui->label_matlab_once->setText(matlab_result[2]);
}

void Widget::on_pushButton_matlab_outside_clicked()
{
    QMessageBox::information(this,tr("Matlab FFT 算法"), tr("请先运行Matlab算法，结束后点击确定."));
    QString *matlab_result = get_matlab_result(this->matlab_loc, this->matlab_script, this->data_loc, this->compute_cnt,false);
    QMessageBox::information(this,tr("Matlab FFT 算法"), tr("运行完毕，请检查结果！"));
    ui->label_matlab_time->setText(matlab_result[0]);
    ui->label_matlab_counts->setText(matlab_result[1]);
    ui->label_matlab_once->setText(matlab_result[2]);
}


void Widget::on_pushButton_matlab_loc_clicked()
{
    this->matlab_loc = QFileDialog::getOpenFileName(this,tr("选择Matlab程序"),"/usr/local/MATLAB/R2017b/bin/","");
    qDebug()<<this->matlab_loc;
}

void Widget::on_pushButton_matlab_script_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this,tr("选择Matlab算法脚本"),"/home/zwp/Test/fft_test/",tr("Matlab(*.m)"));
    this->matlab_script = tmp;
    qDebug()<<this->matlab_script;
}

void Widget::on_pushButton_report_clicked()
{
//    QString save_dir = QFileDialog::getExistingDirectory(this, tr("选择目录"), "/home/zwp/Test/fft_test", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
//    this->save_dir = save_dir;
    float accel_time = ui->label_accel_once->text().toFloat();
    float matlab_time = ui->label_matlab_once->text().toFloat();
    float ratio = matlab_time/accel_time;
    ui->label_result_ratio->setText(QString::number(ratio));
    ui->label_result_1->setText(tr("双精度"));
    ui->label_result_2->setText(tr("双精度"));
}

void Widget::on_comboBox_currentTextChanged(const QString &arg1)
{
    this->compute_cnt = arg1.toInt();
    qDebug()<<arg1;
}
void Widget::on_pushButton_data_loc_clicked()
{
    this->data_loc = QFileDialog::getOpenFileName(this,tr("选择data数据"),"/home/zwp/Test/fft_test/",tr("Data(*.txt)"));
    qDebug()<<this->data_loc;
}

void Widget::on_pushButton_gen_data_clicked()
{
    gen_data(this->data_loc,this->compute_cnt);
    QString msg = tr("数据生成完毕，已保存至\n")+data_loc;
    QMessageBox::information(this,tr("生成数据"), msg);
}
void Widget::on_pushButton_accel_result_clicked()
{
    int p1 = this->data_loc.lastIndexOf('/');
    QString dir_loc = this->data_loc.left(p1);
    QProcess *proc = new QProcess();
    proc->start("kate " + dir_loc + "/accel_result.txt");
}

void Widget::on_pushButton_matlab_result_clicked()
{
    int p1 = this->matlab_script.lastIndexOf('/');
    QString dir_loc = this->matlab_script.left(p1);
    QProcess *proc = new QProcess();
    proc->start("kate " + dir_loc + "/matlab_result.txt");
}

void Widget::on_pushButton_accel_loc_clicked()
{
    QString tmp = QFileDialog::getOpenFileName(this,tr("选择加速算法程序"),"/home/zwp/Test/fft_test/",tr("Executable(*)"));
    this->accel_exec = tmp;
    qDebug()<<this->matlab_script;
}

