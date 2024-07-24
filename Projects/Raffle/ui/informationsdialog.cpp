#pragma execution_character_set("utf-8")
#include "informationsdialog.h"
#include "ui_informationsdialog.h"

#include <QtSql>
#include <QtWidgets>

InformationsDialog::InformationsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InformationsDialog)
  , m_model(new QSqlRelationalTableModel)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle(tr("Informations"));

    // 添加下拉列表内容
    m_tableNames.clear();
    m_tableNames = QStringList() << tr("staff")
                                 << tr("prizes")
                                 << tr("luckyDog")
                                 << tr("configurations");

    getConfigurationFromDB();

    ui->comboBox_tableName->addItems(m_tableNames);
    ui->comboBox_tableName->setCurrentText(m_tableNames.at(m_tableNames.size()-1));
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
    on_comboBox_tableName_activated(ui->comboBox_tableName->currentText());
#else
    on_comboBox_tableName_activated(ui->comboBox_tableName->currentIndex());
#endif
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setResizeContentsPrecision(QHeaderView::ResizeToContents);
    // 为表格视图设置数据模型
    ui->tableView->setModel(m_model);
    // 设置数据模型的编辑策略为手动提交
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);  // 设置编辑策略

    getStaffsFromDB();
    getPrizesFromDB();
}

InformationsDialog::~InformationsDialog()
{
    delete ui;
}

StaffList InformationsDialog::getStaffs()
{
    getStaffsFromDB();
    return m_staffs;
}

PrizeList InformationsDialog::getPrizes()
{
    getPrizesFromDB();
    return m_prizes;
}

int InformationsDialog::getOwnPrizeCnt()
{
    getStaffsFromDB();
    int owned = 0;
    foreach (const Staff &staff, m_staffs) {
        if(staff.ownPrize != 0)
            owned++;
    }

    return owned;
}

void InformationsDialog::changeStaffState(int prizeID, const QList<int> &luckyIDs)
{
    // 获取当前luckyDog中已经有几条记录了
    static int luckyDogId = getLuckyDogsCountFromDB()+1;
    qDebug() << luckyDogId;
    QSqlQuery query;
    foreach (const int &id, luckyIDs) {
        if(!query.exec(QString("UPDATE %1 SET ownPrize = %2 WHERE id='%3';").arg(m_tableNames.at(Table_Staff))
                       .arg(prizeID).arg(m_staffs.at(id).id))){
            qDebug() << query.lastError().text();
        }

        if(!query.exec(QString("INSERT INTO %1 (id, name, prize) VALUES (%2, '%3', %4);").arg(m_tableNames.at(Table_LuckyDog))
                       .arg(luckyDogId).arg(m_staffs.at(id).id).arg(prizeID))){
            qDebug() << query.lastError().text();
        }
        luckyDogId++;

    }

    // 获取奖品变化
    int remaining = m_prizes.at(prizeID-1).remaining - luckyIDs.size();
    // qDebug() << tr("remaining(%1) : ").arg(m_prizes.at(prizeID-1).name) << remaining;
    query.exec(QString("UPDATE %1 SET remaining = %2 WHERE id=%3")
               .arg(m_tableNames.at(Table_Prize))
               .arg(remaining).arg(prizeID));

    on_pushButton_showAll_clicked();
    getStaffsFromDB();
    getPrizesFromDB();
}

void InformationsDialog::changePrizeState(int prizeID, int remaining, int onceDraw)
{
    QSqlQuery query;
    query.exec(QString("UPDATE %1 SET remaining = %2,onceDraw=%3 WHERE id=%4")
               .arg(m_tableNames.at(Table_Prize))
               .arg(remaining).arg(onceDraw).arg(prizeID));

    on_pushButton_showAll_clicked();
    getPrizesFromDB();
}
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
void InformationsDialog::on_comboBox_tableName_activated(const QString &arg1)
{
    if(arg1.isEmpty())
        return;
    // 下拉列表更改后更新数据模型
    m_model->clear();
    on_pushButton_showAll_clicked();
}
#else
void InformationsDialog::on_comboBox_tableName_activated(int index)
{
    const QString arg = ui->comboBox_tableName->currentText();
    if(arg.isEmpty())
        return;
    // 下拉列表更改后更新数据模型
    m_model->clear();
    on_pushButton_showAll_clicked();
}
#endif

void InformationsDialog::setTableHeaderData(QSqlRelationalTableModel *model, const QString &tablename)
{
    // 根据表名设置表格标头
    int index = 0;
    if(tablename == m_tableNames.at(Table_Staff)){
        model->setHeaderData(index++, Qt::Horizontal, tr("工号"));
        model->setHeaderData(index++, Qt::Horizontal, tr("姓名"));
        model->setHeaderData(index++, Qt::Horizontal, tr("部门"));
        model->setHeaderData(index++, Qt::Horizontal, tr("职位"));
        model->setHeaderData(index++, Qt::Horizontal, tr("抽到的奖品"));
        model->setRelation(4, QSqlRelation(m_tableNames.at(1), "id", "name"));
    } else if(tablename == m_tableNames.at(Table_Prize)){
        model->setHeaderData(index++, Qt::Horizontal, tr("编号"));
        model->setHeaderData(index++, Qt::Horizontal, tr("名称"));
        model->setHeaderData(index++, Qt::Horizontal, tr("数量"));
        model->setHeaderData(index++, Qt::Horizontal, tr("剩余数量"));
        model->setHeaderData(index++, Qt::Horizontal, tr("一次抽取数量"));
        model->setHeaderData(index++, Qt::Horizontal, tr("奖品级别"));
    } else if(tablename == m_tableNames.at(Table_LuckyDog)){
        // 设置中奖人信息与职员、奖品信息的关系
        model->setRelation(1, QSqlRelation(m_tableNames.at(0), "id", "name"));
        model->setRelation(2, QSqlRelation(m_tableNames.at(1), "id", "name"));
        model->setHeaderData(0, Qt::Horizontal, tr("序号"));
        model->setHeaderData(1, Qt::Horizontal, tr("姓名"));
        model->setHeaderData(2, Qt::Horizontal, tr("奖品"));
    }
}

void InformationsDialog::getStaffsFromDB()
{
    m_staffs.clear();
    QSqlQuery query;
    query.exec(QString("select * from staff;"));
    Staff staff;
    while (query.next()) {
        staff.id = query.value("id").toString();
        staff.name = query.value("name").toString();
        staff.department = query.value("department").toString();
        staff.position = query.value("position").toString();
        staff.ownPrize = query.value("ownPrize").toInt();
        m_staffs.append(staff);
    }
    query.finish();
}

void InformationsDialog::getPrizesFromDB()
{
    m_prizes.clear();
    QSqlQuery query;
    query.exec(QString("select * from prizes;"));
    Prize prize;
    QString tmpval;
    while (query.next()) {
        prize.name = query.value("name").toString();
        if(prize.name.compare("Empty", Qt::CaseInsensitive) == 0)
            continue;
        prize.id = query.value("id").toInt();

        prize.num = query.value("num").toInt();
        prize.remaining = query.value("remaining").toInt();
        prize.onceDraw = query.value("onceDraw").toInt();
        prize.level = query.value("level").toString();
        m_prizes.append(prize);
    }
    query.finish();
}

void InformationsDialog::getConfigurationFromDB()
{
    m_staffs.clear();
    QSqlQuery query;
    query.exec(QString("select * from %1;").arg(m_tableNames.at(Table_Configuration)));
    query.next();
    m_companyName = query.value("company_name").toString();
    m_drawTime = query.value("draw_time").toInt();
    m_backgroundMusic = query.value("background_music").toString();
    m_rollMusic = query.value("roll_music").toString();
    m_luckyMusic = query.value("lucky_music").toString();

    query.finish();
}

int InformationsDialog::getLuckyDogsCountFromDB()
{
    QSqlQuery query;
    query.exec(tr("select * from luckyDog;"));
    int result=0;
    while (query.next()) {
        result++;
    }
    query.finish();
    return result;
}

void InformationsDialog::on_pushButton_showAll_clicked()
{
    // 显示表中所有数据
    m_model->clear();
    const QString tableName = ui->comboBox_tableName->currentText();
    m_model->setTable(tableName);
    setTableHeaderData(m_model, tableName);
    m_model->select();

}

void InformationsDialog::on_pushButton_select_clicked()
{
    // 根据表中的id查询
    const QString tableName = ui->comboBox_tableName->currentText();
    QString inputText = QInputDialog::getText(this, tr("Input id"), tr("Please input table %1's id:").arg(tableName));
    if(inputText.isEmpty())
        return;
    m_model->setFilter(tr("id=%1").arg(inputText));
    m_model->select();
}

void InformationsDialog::on_pushButton_add_clicked()
{
    // 添加表数据
    int rowCount = m_model->rowCount();
    m_model->insertRow(rowCount);

}

void InformationsDialog::on_pushButton_commit_clicked()
{
    // 提交数据
    if(m_model->database().driver()->hasFeature(QSqlDriver::Transactions)){
        m_model->database().transaction();  // 开始事务操作
        if(m_model->submitAll()){
            m_model->database().commit();  // 提交
        }

        m_model->database().commit();
        getStaffsFromDB();
        getPrizesFromDB();
        emit infoChanged();
    }
}

void InformationsDialog::on_pushButton_rollback_clicked()
{
    m_model->revertAll();  // 撤销修改
    getStaffsFromDB();
    getPrizesFromDB();
    emit infoChanged();
}

void InformationsDialog::on_pushButton_delete_clicked()
{
    const int curRow = ui->tableView->currentIndex().row();
    //    m_model->setTable(ui->comboBox_tableName->currentText());
    m_model->removeRow(curRow);
#if 0
    int ok = QMessageBox::question(this, tr("Delete"), tr("Are you sure to delete this row information?"),
                                   QMessageBox::Yes, QMessageBox::Cancel);
    if(ok == QMessageBox::Yes){
        on_pushButton_commit_clicked();
    } else {
        on_pushButton_rollback_clicked();
    }
#else
    on_pushButton_commit_clicked();
#endif
}

