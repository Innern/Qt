#include "completerwidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QCompleter>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QStringListModel>
#include <QGuiApplication>
#include <QStandardItemModel>
#include <QTreeView>
#include <QDebug>
#include <QHeaderView>

CompleterWidget::CompleterWidget(QWidget *parent) : QWidget(parent)
  , m_completer(nullptr)
{
    modelLabel = new QLabel(tr("&Model"), this);
    modelComboBox = new QComboBox(this);
    modelLabel->setBuddy(modelComboBox);
    modelComboBox->addItem(tr("QFileSystemModel"), Model_FileSystem);
    modelComboBox->addItem(tr("QFileSystemModel Show Full Path"), Model_FSShowFullPath);
    modelComboBox->addItem(tr("Country List"), Model_Country);
    modelComboBox->addItem(tr("Word List"), Model_Words);
    connect(modelComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateModel(int)));
    modelComboBox->setCurrentIndex(Model_FileSystem);

    modeLabel = new QLabel(tr("&Completion Mode:"), this);
    modeComboBox = new QComboBox(this);
    modeLabel->setBuddy(modeComboBox);
    modeComboBox->addItem(tr("filtered Popup"), QCompleter::PopupCompletion);
    modeComboBox->addItem(tr("Unfiltered Popup"), QCompleter::UnfilteredPopupCompletion);
    modeComboBox->addItem(tr("Inline"), QCompleter::InlineCompletion);
    connect(modeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateMode(int)));
    modeComboBox->setCurrentIndex(QCompleter::PopupCompletion);

    caseSensLabel = new QLabel(tr("Case &Sensitivity:"), this);
    caseSensComboBox = new QComboBox(this);
    caseSensLabel->setBuddy(caseSensComboBox);
    caseSensComboBox->addItem(tr("Case Insensitive"), Qt::CaseInsensitive);
    caseSensComboBox->addItem(tr("Case Sensitive"), Qt::CaseSensitive);
    connect(caseSensComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateCaseSensitivity(int)));
    caseSensComboBox->setCurrentIndex(Qt::CaseInsensitive);

    maxItemsLabel = new QLabel(tr("Max Visible &Items:"), this);
    maxItemsSpinbox = new QSpinBox(this);
    maxItemsLabel->setBuddy(maxItemsSpinbox);
    maxItemsSpinbox->setValue(10);

    wrapCheckbox = new QCheckBox(tr("&Wrap around completions"));
    wrapCheckbox->setChecked(true);

    filePathLabel = new QLabel(tr("&Enter file path:"), this);
    filePathLineedit = new QLineEdit(this);
    filePathLabel->setBuddy(filePathLineedit);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(modelLabel,modelComboBox);
    formLayout->addRow(modeLabel, modeComboBox);
    formLayout->addRow(caseSensLabel, caseSensComboBox);
    formLayout->addRow(maxItemsLabel, maxItemsSpinbox);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addLayout(formLayout);
    vLayout->addWidget(wrapCheckbox);
    vLayout->addWidget(filePathLabel);
    vLayout->addWidget(filePathLineedit);

    setLayout(vLayout);


    updateModel(modelComboBox->currentIndex());
}

void CompleterWidget::updateModel(int index)
{
    delete m_completer;
    m_completer = new QCompleter(this);
    m_completer->setMaxVisibleItems(maxItemsSpinbox->value());
    switch (index) {
    case Model_FileSystem:  // QFileSystemModel
    {
        filePathLabel->setText(tr("Enter file path:"));
        QFileSystemModel *model = new QFileSystemModel(this);
        model->setRootPath(QString());
        m_completer->setModel(model);
    }
        break;
    case Model_FSShowFullPath:  // QFileSystemModel show full path
    {
        filePathLabel->setText(tr("Enter file path:"));
        FsModel *model = new FsModel(this);
        model->setRootPath(QString());
        m_completer->setModel(model);
    }
        break;
    case Model_Country:  // country
    {
        filePathLabel->setText(tr("Enter name of your country:"));
        m_completer->setModel(modelFromFile(QString(":/resources/countries.txt")));
        qDebug() <<m_completer->model()->data(m_completer->model()->index(0,0));
        m_completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
        QTreeView *view = new QTreeView;
        m_completer->setPopup(view);
        view->setRootIsDecorated(false);  // 不显示用于展开和折叠顶级项的控件
        view->header()->hide();
        view->header()->setStretchLastSection(false);
        view->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        view->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    }
        break;
    case Model_Words:  // words
    {
        filePathLabel->setText(tr("Enter a word:"));
        m_completer->setModel(modelFromFile(QString(":/resources/wordlist.txt")));
        m_completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    }
        break;
    default:
        break;
    }

    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCompletionMode(static_cast<QCompleter::CompletionMode>(modeComboBox->itemData(modeComboBox->currentIndex()).toInt()));
    m_completer->setCaseSensitivity(static_cast<Qt::CaseSensitivity>(caseSensComboBox->itemData(caseSensComboBox->currentIndex()).toUInt()));
    m_completer->setWrapAround(wrapCheckbox->isChecked());
    filePathLineedit->setCompleter(m_completer);
}

void CompleterWidget::updateMode(int index)
{
    m_completer->setCompletionMode(static_cast<QCompleter::CompletionMode>(modeComboBox->itemData(index).toInt()));
}

void CompleterWidget::updateCaseSensitivity(int index)
{
    m_completer->setCaseSensitivity(static_cast<Qt::CaseSensitivity>(caseSensComboBox->itemData(index).toInt()));
}

QAbstractItemModel *CompleterWidget::modelFromFile(const QString &filename)
{
    // 文件打开失败的情况
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
        return new QStringListModel(m_completer);

    // 文件打开成功

    // 读取文件时设置鼠标样式
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif

    QStringList words;
    while(!file.atEnd()){
        QByteArray line = file.readLine();
        if(!line.isEmpty())
            words << line.trimmed();
    }

    // 文件读取结束时设置正常的鼠标样式
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    file.close();


    // 如果是国家列表，要显示两列
    if(!filename.contains(QString("countries"),Qt::CaseInsensitive)){
        return new QStringListModel(words, m_completer);
    }

    // 国家列表设置两列模型
    QStandardItemModel *model = new QStandardItemModel(words.count(),2,m_completer);
    for(int i=0; i<words.size();i++) {
        QString word = words.at(i).trimmed();
        QString country = word.mid(0, word.length() - 2).trimmed();
        QString symbol = word.right(2).trimmed();
        QModelIndex countryIdx = model->index(i, 0);
        QModelIndex symbolIdx = model->index(i, 1);
        qDebug() << country << symbol;
        model->setData(countryIdx, country);
        model->setData(symbolIdx, symbol);
    }
    return model;
}

FsModel::FsModel(QObject *parent) : QFileSystemModel(parent)
{

}

FsModel::~FsModel()
{

}

QVariant FsModel::data(const QModelIndex &index, int role) const
{
    if(0 == index.column() && role == Qt::DisplayRole){
        QString path = QDir::toNativeSeparators(filePath(index));
        if(path.endsWith(QDir::separator()))
            path.chop(1);
        return path;
    }
    return QFileSystemModel::data(index, role);
}
