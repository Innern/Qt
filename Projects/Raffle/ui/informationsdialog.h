#ifndef INFORMATIONSDIALOG_H
#define INFORMATIONSDIALOG_H

#include <QDialog>

namespace Ui {
class InformationsDialog;
}

class QSqlRelationalTableModel;

struct Staff{
    int ownPrize;
    QString id;
    QString name;
    QString department;
    QString position;
};
struct Prize{
    int id;
    int num;
    int remaining;
    int onceDraw;
    QString name;
    QString level;
};

typedef QList<Staff> StaffList;
typedef QList<Prize> PrizeList;

class InformationsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InformationsDialog(QWidget *parent = nullptr);
    ~InformationsDialog();

    StaffList getStaffs();
    PrizeList getPrizes();
    int getOwnPrizeCnt();


signals:
    void infoChanged();

public slots:

    void changeStaffState(int prizeID, const QList<int> &luckyIDs);
    void changePrizeState(int prizeID, int remaining, int onceDraw);

private slots:
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
    void on_comboBox_tableName_activated(const QString &arg1);
#else
    void on_comboBox_tableName_activated(int index);
#endif
    void on_pushButton_showAll_clicked();

    void on_pushButton_select_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_commit_clicked();

    void on_pushButton_rollback_clicked();

    void on_pushButton_delete_clicked();

private:
    void setTableHeaderData(QSqlRelationalTableModel *model, const QString &tablename);
    void getStaffsFromDB();
    void getPrizesFromDB();
    int getLuckyDogsCountFromDB();


private:
    enum{Table_Staff, Table_Prize, Table_LuckyDog};
    Ui::InformationsDialog *ui;
    QSqlRelationalTableModel *m_model;
    QStringList m_tableNames;
    StaffList m_staffs;
    PrizeList m_prizes;


};



#endif // INFORMATIONSDIALOG_H
