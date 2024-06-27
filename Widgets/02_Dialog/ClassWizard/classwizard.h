#ifndef CLASSWIZARD_H
#define CLASSWIZARD_H

#include <QWizard>
#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QCheckBox;
QT_END_NAMESPACE

class ClassWizard : public QWizard
{
    Q_OBJECT
public:
    explicit ClassWizard(QWidget *parent = nullptr);

    void accept() override;
};
// 介绍页
class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit IntroPage(QWidget *parent = nullptr);
    ~IntroPage();
};
// 类信息页
class ClassInfoPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ClassInfoPage(QWidget *parent = nullptr);
    ~ClassInfoPage();
};
// 代码风格页
class CodeStyle : public QWizardPage
{
    Q_OBJECT
public:
    explicit CodeStyle(QWidget *parent = nullptr);
protected:
    void initializePage() override;

private:
    QLabel *macroNameLabel;
    QLineEdit *macroNameLineedit;
    QLabel *baseClassIncLabel;
    QLineEdit *baseClassIncLineedit;
    QCheckBox *includeBaseDefCheckbox;
};
// 文件输出页
class OutputFilePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit OutputFilePage(QWidget *parent = nullptr);
protected:
    void initializePage() override;

private:
    QLineEdit *outputDirLineedit;
    QLineEdit *headerFilenameLineedit;
    QLineEdit *impleFilenameLineedit;
};
// 完成页
class ConclusionPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ConclusionPage(QWidget *parent = nullptr);

protected:
    void initializePage() override;
private:
    QLabel *label;
};

#endif // CLASSWIZARD_H
