#include "classwizard.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QRegularExpression>
#include <QStandardPaths>

ClassWizard::ClassWizard(QWidget *parent) : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new ClassInfoPage);
    addPage(new CodeStyle);
    addPage(new OutputFilePage);
    addPage(new ConclusionPage);

    // 设置页眉图片
    setPixmap(QWizard::BannerPixmap, QPixmap(":/imgs/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/imgs/background.png"));

    setWindowTitle(tr("Class Wizard"));
}

void ClassWizard::accept()
{
    QDialog::accept();
}

IntroPage::IntroPage(QWidget *parent):QWizardPage(parent)
{
    setTitle(tr("Introduction"));  // 设置标题
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/imgs/watermark1.png"));

    QLabel *label = new QLabel(tr("This wizard will generate a skeleton C++ class "
                                  "definition, including a few function. You simple "
                                  "need to specify the class name and set s few "
                                  "options to produce a header file and an "
                                  "implementation file for your new C++ class."));
    label->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

IntroPage::~IntroPage()
{

}

ClassInfoPage::ClassInfoPage(QWidget *parent):QWizardPage(parent)
{
    setTitle(tr("Class Information"));
    setSubTitle(tr("Specify basic information about the class for which you want"
                   "to generate skeleton source code files."));

    setPixmap(QWizard::LogoPixmap, QPixmap(":/imgs/logo1.png"));

    QLabel *classNameLabel = new QLabel(tr("&Class name:"));
    QLineEdit *classNameLineedit = new QLineEdit;
    classNameLabel->setBuddy(classNameLineedit);
    QLabel *baseClassLabel = new QLabel(tr("B&asic class:"));
    QLineEdit *baseClassLineedit = new QLineEdit;
    baseClassLabel->setBuddy(baseClassLineedit);

    QCheckBox *macroCheckbox = new QCheckBox(tr("Generate Q_OBJECT &macro"));

    QRadioButton *objectStyleConstructorRBtn = new QRadioButton(tr("&QObject-style constructor"));
    QRadioButton *widgetStyleConstructorRBtn = new QRadioButton(tr("Q&Widget-style constructor"));
    QRadioButton *defaultConstructorRBtn = new QRadioButton(tr("&Default constructor"));
    defaultConstructorRBtn->setChecked(true);
    QCheckBox *copyConstructorCheckbox = new QCheckBox(tr("&Generate copy constructor and operator="));
    connect(defaultConstructorRBtn, &QRadioButton::toggled,
            copyConstructorCheckbox, &QCheckBox::setChecked);


    QGroupBox *group = new QGroupBox(tr("C&onstructor"));
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(objectStyleConstructorRBtn);
    vLayout->addWidget(widgetStyleConstructorRBtn);
    vLayout->addWidget(defaultConstructorRBtn);
    vLayout->addWidget(copyConstructorCheckbox);
    group->setLayout(vLayout);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(classNameLabel, 0, 0);
    gridLayout->addWidget(classNameLineedit, 0, 1);
    gridLayout->addWidget(baseClassLabel, 1, 0);
    gridLayout->addWidget(baseClassLineedit, 1, 1);
    gridLayout->addWidget(macroCheckbox,2, 0, 1, 2);
    gridLayout->addWidget(group,3, 0, 1, 2);

    setLayout(gridLayout);

    // 注册字段以便在其它的页面使用
    registerField("className*", classNameLineedit);
    registerField("baseClass", baseClassLineedit);
    registerField("qobjectMacro", macroCheckbox);
    registerField("qobjectCtor", objectStyleConstructorRBtn);
    registerField("qwidgetCtor", widgetStyleConstructorRBtn);
    registerField("defaultCtor", defaultConstructorRBtn);
    registerField("copyCtor", copyConstructorCheckbox);
}

ClassInfoPage::~ClassInfoPage()
{

}

CodeStyle::CodeStyle(QWidget *parent):QWizardPage(parent)
{
    setTitle(tr("Code Style Options"));
    setSubTitle(tr("Choose the formatting of the generated code."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/imgs/logo2.png"));

    QCheckBox *commentCheckbox = new QCheckBox(tr("&Start generated files with a comment"));
    commentCheckbox->setChecked(true);
    QCheckBox *protectHeaderFileCheckbox = new QCheckBox(tr("&Protext header file against multiple inclusions"));
    protectHeaderFileCheckbox->setChecked(true);
    macroNameLabel = new QLabel(tr("&Macro name:"));
    macroNameLineedit = new QLineEdit;
    macroNameLabel->setBuddy(macroNameLineedit);
    includeBaseDefCheckbox = new QCheckBox(tr("&Include base class definition"));
    includeBaseDefCheckbox->setChecked(true);
    baseClassIncLabel = new QLabel(tr("&Base class include:"));
    baseClassIncLineedit = new QLineEdit;
    baseClassIncLabel->setBuddy(baseClassIncLineedit);

    connect(protectHeaderFileCheckbox, &QCheckBox::toggled,
            macroNameLabel, &QLabel::setEnabled);
    connect(protectHeaderFileCheckbox, &QCheckBox::toggled,
            macroNameLineedit, &QLineEdit::setEnabled);
    connect(includeBaseDefCheckbox, &QCheckBox::toggled,
            baseClassIncLabel, &QLabel::setEnabled);
    connect(includeBaseDefCheckbox, &QCheckBox::toggled,
            baseClassIncLineedit, &QLineEdit::setEnabled);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(commentCheckbox, 0, 0, 1, 2);
    gridLayout->addWidget(protectHeaderFileCheckbox, 1, 0, 1, 2);
    gridLayout->addWidget(macroNameLabel, 2, 0, 1, 1);
    gridLayout->addWidget(macroNameLineedit, 2, 1, 1, 1);
    gridLayout->addWidget(includeBaseDefCheckbox, 3, 0, 1, 2);
    gridLayout->addWidget(baseClassIncLabel, 4, 0, 1, 1);
    gridLayout->addWidget(baseClassIncLineedit, 4, 1, 1, 1);

    setLayout(gridLayout);


    registerField("comment", commentCheckbox);
    registerField("protect", protectHeaderFileCheckbox);
    registerField("macroName", macroNameLineedit);
    registerField("includeBase", includeBaseDefCheckbox);
    registerField("baseInclude", baseClassIncLineedit);

}

void CodeStyle::initializePage()
{
    QString className = field("className").toString();
    macroNameLineedit->setText(className.toUpper() + "_H");

    QString baseName = field("baseClass").toString();
    includeBaseDefCheckbox->setChecked(!baseName.isEmpty());
    macroNameLabel->setEnabled(!baseName.isEmpty());
    macroNameLineedit->setEnabled(!baseName.isEmpty());

    QRegularExpression rx("Q[A-Z]*");
    if(baseName.isEmpty())
            baseClassIncLineedit->clear();
    else if(rx.match(baseName).hasMatch()){
        baseClassIncLineedit->setText('<' + baseName + '>');
    } else {
        baseClassIncLineedit->setText('"' + baseName.toLower() + ".h\"");
    }

}

OutputFilePage::OutputFilePage(QWidget *parent):QWizardPage(parent)
{
    setTitle(tr("Output Files"));
    setSubTitle(tr("Specify where you want the wizard to put the generated skeleton code."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/imgs/logo3.png"));

    QLabel *outputDirLabel = new QLabel(tr("Output directory:"));
    outputDirLineedit = new QLineEdit;
    outputDirLabel->setBuddy(outputDirLineedit);
    QLabel *headerFilenameLabel = new QLabel(tr("&Header file name:"));
    headerFilenameLineedit = new QLineEdit;
    headerFilenameLabel->setBuddy(headerFilenameLineedit);
    QLabel *impleFilenameLabel = new QLabel(tr("&Implementation file name:"));
    impleFilenameLineedit = new QLineEdit;
    impleFilenameLabel->setBuddy(impleFilenameLineedit);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(outputDirLabel, 0, 0);
    gridLayout->addWidget(outputDirLineedit, 0, 1);
    gridLayout->addWidget(headerFilenameLabel, 1, 0);
    gridLayout->addWidget(headerFilenameLineedit, 1, 1);
    gridLayout->addWidget(impleFilenameLabel, 2, 0);
    gridLayout->addWidget(impleFilenameLineedit, 2, 1);

    setLayout(gridLayout);

    registerField("outputDir", outputDirLineedit);
    registerField("headerFilename", headerFilenameLineedit);
    registerField("impleFilename", impleFilenameLineedit);

}

void OutputFilePage::initializePage()
{
    outputDirLineedit->setText(QStandardPaths::standardLocations(QStandardPaths::TempLocation).first());
    QString className = field("className").toString();
    headerFilenameLineedit->setText(className.toLower() + ".h");
    impleFilenameLineedit->setText(className.toLower() + ".cpp");
}

ConclusionPage::ConclusionPage(QWidget *parent):QWizardPage(parent)
{
    setTitle(tr("Conclusion"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/imgs/watermark2.png"));
    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(label);
    setLayout(vLayout);
}

void ConclusionPage::initializePage()
{
    QString finishText = wizard()->buttonText(QWizard::FinishButton);
    finishText.remove('&');
    label->setText(tr("Click %1 to generate the class skeleton")
                   .arg(finishText));
}
