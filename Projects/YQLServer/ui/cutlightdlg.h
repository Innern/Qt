#ifndef CUTLIGHTDLG_H
#define CUTLIGHTDLG_H
#include "commondef.h"

#include <QDialog>

QT_FORWARD_DECLARE_CLASS(QProcess)

namespace Ui {
class CutLightDlg;
}

class CutLightDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CutLightDlg(const QString &com,QWidget *parent = nullptr);
    ~CutLightDlg();
    bool changeLight(int channel);
    QString LastErrorString();

signals:
    void signal_showCutlightResult(const QString &msg,int level);
    void signal_comChange(const QString &com);
private slots:
    void on_comboBox_COM_currentIndexChanged(const QString &arg1);

    void on_comboBox_lightChannel_currentIndexChanged(int index);

    void showProcessResult();

    void on_comboBox_lightType_currentIndexChanged(int index);

private:
    bool changeLightForG4C(int channel);
private:
    Ui::CutLightDlg *ui;
    int m_com;
    int m_lightType;
    QString m_errorString;
    QProcess *process;
};

#endif // CUTLIGHTDLG_H
