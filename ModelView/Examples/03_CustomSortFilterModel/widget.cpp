#include "widget.h"
#include "filterlineedit.h"
#include "mysortfilterproxymodel.h"

#include <QtWidgets>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_originalView(new QTreeView(this))
    , m_proxyView(new QTreeView(this))
    , m_proxyModel(new MySortFilterProxyModel(this))
{
    m_originalView->setRootIsDecorated(false);
    m_originalView->setAlternatingRowColors(true);



    QVBoxLayout *topLayout = new QVBoxLayout(this);
    topLayout->addWidget(m_originalView);
    QGroupBox *originalGroup = new QGroupBox(tr("Orignal Model"));
    originalGroup->setLayout(topLayout);

    QLabel *patternLabel = new QLabel(tr("&Filter pattern:"), this);
    patternLineEdit = new FilterLineEdit(this);
    patternLabel->setBuddy(patternLineEdit);
    connect(patternLineEdit, &FilterLineEdit::filterChanged, this, &Widget::textFilterChanged);
    connect(patternLineEdit, &QLineEdit::textChanged, this, &Widget::textFilterChanged);
    patternLineEdit->setText(tr("Grace|Sports"));

    QLabel *fromLabel = new QLabel(tr("&From:"), this);
    fromDateEdit = new QDateEdit(QDate(2007, 01, 02), this);
    fromLabel->setBuddy(fromDateEdit);
    connect(fromDateEdit, &QDateTimeEdit::dateChanged, this, &Widget::dateFilterChanged);

    QLabel *toLabel = new QLabel(tr("&To:"));
    toDateEdit = new QDateEdit(QDate(2007, 1, 06), this);
    toLabel->setBuddy(toDateEdit);
    connect(toDateEdit, &QDateTimeEdit::dateChanged, this, &Widget::dateFilterChanged);

    QGridLayout *bottomLayout = new QGridLayout;
    bottomLayout->addWidget(m_proxyView, 0, 0, 1, 3);
    bottomLayout->addWidget(patternLabel, 1, 0);
    bottomLayout->addWidget(patternLineEdit, 1, 1, 1, 2);
    bottomLayout->addWidget(fromLabel, 2, 0);
    bottomLayout->addWidget(fromDateEdit, 2, 1, 1, 2);
    bottomLayout->addWidget(toLabel, 3, 0);
    bottomLayout->addWidget(toDateEdit, 3, 1, 1, 2);

    QGroupBox *sortFilterGroup = new QGroupBox(tr("Sort/Filter Model"), this);
    sortFilterGroup->setLayout(bottomLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(originalGroup);
    mainLayout->addWidget(sortFilterGroup);
    setLayout(mainLayout);

    resize(500, 450);

    // 设置代理模型按哪一列排序
    m_proxyView->setRootIsDecorated(false);
    m_proxyView->setAlternatingRowColors(true);
    m_proxyView->setModel(m_proxyModel);
    m_proxyView->setSortingEnabled(true);
    m_proxyView->sortByColumn(1, Qt::AscendingOrder);

}

Widget::~Widget()
{
}

void Widget::setSourceModel(QAbstractItemModel *model)
{
    m_proxyModel->setSourceModel(model);
    m_originalView->setModel(model);
    for(int i=0;i<m_proxyModel->columnCount(); ++i){
        m_proxyView->resizeColumnToContents(i);
    }
    for(int i=0;i<model->columnCount(); ++i){
        m_originalView->resizeColumnToContents(i);
    }
}

void Widget::textFilterChanged()
{
    // FilterLineEdit::PatternSyntax syntax = patternLineEdit->patternSyntax();
    // QString pattern = patternLineEdit->text();
    // switch (syntax) {
    // case FilterLineEdit::Wildcard:
    //     pattern = QRegularExpression::wildcardToRegularExpression(pattern);
    //     break;
    // case FilterLineEdit::FixedString:
    //     pattern = QRegularExpression::escape(pattern);
    //     break;
    // default:
    //     break;
    // }

    // QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    // if(patternLineEdit->caseSensitivity() == Qt::CaseInsensitive)
    //     options |= QRegularExpression::CaseInsensitiveOption;

    // QRegularExpression regularExpression(pattern, options);
    // if(regularExpression.isValid()){
    //     m_proxyModel->setFilterRegularExpression(regularExpression);
    // } else {
    //     m_proxyModel->setFilterRegularExpression(QRegularExpression());
    // }
    FilterLineEdit::PatternSyntax s = patternLineEdit->patternSyntax();
    QString pattern = patternLineEdit->text();
    switch (s) {
    case FilterLineEdit::Wildcard:
        pattern = QRegularExpression::wildcardToRegularExpression(pattern);
        break;
    case FilterLineEdit::FixedString:
        pattern = QRegularExpression::escape(pattern);
        break;
    default:
        break;
    }

    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    if (patternLineEdit->caseSensitivity() == Qt::CaseInsensitive)
        options |= QRegularExpression::CaseInsensitiveOption;
    QRegularExpression regularExpression(pattern, options);
    m_proxyModel->setFilterRegularExpression(regularExpression);

}

void Widget::dateFilterChanged()
{
    m_proxyModel->setFilterMinimumDate(fromDateEdit->date());
    m_proxyModel->setFilterMaximumDate(toDateEdit->date());
}
