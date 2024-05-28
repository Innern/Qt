#include "widget.h"

#include <QtWidgets>

// 根据输入的模式是否有效设置对应字体格式
static inline QColor textColor(const QPalette &palette)
{
    return palette.color(QPalette::Active, QPalette::Text);
}

static inline void setTextColor(QWidget *widget, const QColor &color)
{
    QPalette palette = widget->palette();
    if(textColor(palette) != color ){
        palette.setColor(QPalette::Active, QPalette::Text, color);
        widget->setPalette(palette);
    }
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_originalView(new QTreeView(this))
    , m_sortFilterView(new QTreeView(this))
    , m_proxyModel(new QSortFilterProxyModel(this))
{

    // 设置original试图相关属性
    m_originalView->setRootIsDecorated(false);  // 不显示用于展开和折叠顶级项的控件
    m_originalView->setAlternatingRowColors(true);  // 使用交替颜色绘制行

    // 设置代理模型过滤的默认列
    m_proxyModel->setFilterKeyColumn(0);
    // 设置排序过滤试图的相关属性
    m_sortFilterView->setRootIsDecorated(false);
    m_sortFilterView->setAlternatingRowColors(true);
    // 为排序过滤试图设置代理模型
    m_sortFilterView->setModel(m_proxyModel);

    // 创建窗口界面
    createUI();
    setWindowTitle(tr("Basic Sort/Filter Model"));

}

Widget::~Widget() {}

void Widget::setSourceModel(QAbstractItemModel *model)
{
    // 为代理模型设置源模型
    m_proxyModel->setSourceModel(model);
    // 为original试图设置模型
    m_originalView->setModel(model);
}

void Widget::updateFilterRegularExpression()
{
    // 设置正则表达式的语法，
    Syntax syntax = Syntax(syntaxComboBox->itemData(syntaxComboBox->currentIndex()).toInt());
    QString pattern = patternLineEdit->text();  // 过滤模式
    switch (syntax) {
    case Wildcard:
        pattern = QRegularExpression::wildcardToRegularExpression(pattern);  // 使用通配符模式过滤
        break;
    case FixedString:
        pattern = QRegularExpression::escape(pattern);  // 使用固定字符串
    default:
        break;
    }

    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;  // 字母大小写是否敏感
    if(!filterCaseSensitiveCheck->isChecked())
        options |= QRegularExpression::CaseInsensitiveOption;
    QRegularExpression regularExpression(pattern, options);

    // 根据输入的模式是否有效设置对应的文本格式，并显示相应的工具提示
    if(regularExpression.isValid()){
        patternLineEdit->setToolTip(QString());  //
        m_proxyModel->setFilterRegularExpression(regularExpression);
        setTextColor(patternLineEdit, textColor(style()->standardPalette()));
    } else {
        patternLineEdit->setToolTip(regularExpression.errorString());
        m_proxyModel->setFilterRegularExpression(QRegularExpression());
        setTextColor(patternLineEdit, Qt::red);
    }
}

void Widget::filterColumnChanged(int index)
{
    m_proxyModel->setFilterKeyColumn(index);
}

void Widget::sortCaseSensitiveChanged(bool checked)
{
    m_proxyModel->setSortCaseSensitivity(checked ? Qt::CaseSensitive : Qt::CaseInsensitive);
}

void Widget::createUI()
{
    // original model
    QGroupBox *topGroupBox = new QGroupBox(tr("Original Model"));
    QVBoxLayout *topLayout = new QVBoxLayout(this);
    topLayout->addWidget(m_originalView);
    topGroupBox->setLayout(topLayout);

    // sort/filter model
    QGroupBox *bottomGroupBox = new QGroupBox(tr("Sorted/Filtered Model"));

    // 过滤项设置
    // 过滤模式
    patternLineEdit = new QLineEdit(this);
    patternLineEdit->setClearButtonEnabled(true);

    // 正则表达式语法
    syntaxComboBox = new QComboBox(this);
    syntaxComboBox->addItem(tr("Regular expression"), RegularExpression);
    syntaxComboBox->addItem(tr("Wildcard"), Wildcard);
    syntaxComboBox->addItem(tr("Fixed string"), FixedString);

    // 过滤哪一列
    columnComboBox = new QComboBox(this);
    columnComboBox->addItem(tr("Subject"));
    columnComboBox->addItem(tr("Sender"));
    columnComboBox->addItem(tr("Date"));

    // 过滤/排序是否考虑字母大小写
    filterCaseSensitiveCheck = new QCheckBox(tr("Case sensitive filter"), this);
    sortCaseSensitiveCheck = new QCheckBox(tr("Case sensitive sorting"), this);

    connect(patternLineEdit, &QLineEdit::textChanged, this, &Widget::updateFilterRegularExpression);
    connect(syntaxComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilterRegularExpression()));
    connect(columnComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filterColumnChanged(int)));
    connect(filterCaseSensitiveCheck, &QCheckBox::toggled, this, &Widget::updateFilterRegularExpression);
    connect(sortCaseSensitiveCheck, &QCheckBox::toggled, this, &Widget::sortCaseSensitiveChanged);

    columnComboBox->setCurrentIndex(1);
    patternLineEdit->setText(tr("Andy|Grace"));
    filterCaseSensitiveCheck->setChecked(true);
    sortCaseSensitiveCheck->setChecked(true);

    // 布局
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Filter &pattern:"), patternLineEdit);
    formLayout->addRow(tr("Filter &syntax:"), syntaxComboBox);
    formLayout->addRow(tr("Filter &column:"), columnComboBox);
    formLayout->addRow(filterCaseSensitiveCheck, sortCaseSensitiveCheck);

    QVBoxLayout *bottomLayout = new QVBoxLayout(this);
    bottomLayout->addWidget(m_sortFilterView);
    bottomLayout->addLayout(formLayout);
    bottomGroupBox->setLayout(bottomLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(topGroupBox);
    mainLayout->addWidget(bottomGroupBox);
    setLayout(mainLayout);

}
