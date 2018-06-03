#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <string>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_pushButton_accel_clicked();

    void on_pushButton_matlab_clicked();

    void on_pushButton_matlab_loc_clicked();

    void on_pushButton_matlab_script_clicked();

    void on_pushButton_report_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_data_loc_clicked();

    void on_pushButton_gen_data_clicked();

    void on_pushButton_accel_result_clicked();

    void on_pushButton_matlab_result_clicked();

    void on_pushButton_accel_loc_clicked();

private:
    Ui::Widget *ui;
    QString matlab_loc;
    QString matlab_script;
    QString data_loc;
    QString save_dir;
    QString accel_exec;
    int compute_cnt;
};

#endif // WIDGET_H
