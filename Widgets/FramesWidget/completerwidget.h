#ifndef COMPLETERWIDGET_H
#define COMPLETERWIDGET_H

#include <QWidget>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QSpinBox;
class QCheckBox;
class QLineEdit;
class QCompleter;
class QStringListModel;
QT_END_NAMESPACE

class FsModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit FsModel(QObject *parent = nullptr);
    ~FsModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

class CompleterWidget : public QWidget
{
    Q_OBJECT
    enum{Model_FileSystem, Model_FSShowFullPath, Model_Country, Model_Words};
public:
    explicit CompleterWidget(QWidget *parent = nullptr);

signals:

private slots:
    void updateModel(int index);
    void updateMode(int index);
    void updateCaseSensitivity(int index);

private:
    QAbstractItemModel *modelFromFile(const QString &filename);
private:
    QLabel *modelLabel;
    QComboBox *modelComboBox;
    QLabel *modeLabel;
    QComboBox *modeComboBox;
    QLabel *caseSensLabel;
    QComboBox *caseSensComboBox;
    QLabel *maxItemsLabel;
    QSpinBox *maxItemsSpinbox;
    QCheckBox *wrapCheckbox;
    QLabel *filePathLabel;
    QLineEdit *filePathLineedit;

    QCompleter *m_completer;
};

#endif // COMPLETERWIDGET_H
