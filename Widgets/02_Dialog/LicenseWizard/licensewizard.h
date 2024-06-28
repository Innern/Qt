#ifndef LICENSEWIZARD_H
#define LICENSEWIZARD_H

#include <QWizard>
#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QLabel;
class QRadioButton;
class QLineEdit;
class QCheckBox;
QT_END_NAMESPACE

class LicenseWizard : public QWizard
{
    Q_OBJECT
public:
    enum{Page_Intro, Page_Evaluate, Page_Register, Page_Details, Page_Conclusion};
    explicit LicenseWizard(QWidget *parent = nullptr);

};

class LicenseIntroPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit LicenseIntroPage(QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *topLabel;
    QRadioButton *regCopyRadioBtn;
    QRadioButton *evalRadioBtn;
};

class LicenseRegisterPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit LicenseRegisterPage(QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *nameLabel;
    QLineEdit *nameLineedit;
    QLabel *upgradeKeyLabel;
    QLineEdit *upgradeKeyLineedit;


};

class LicenseEvaluatePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit LicenseEvaluatePage(QWidget *parent = nullptr);

    int nextId() const override;

private:

    QLabel *nameLabel;
    QLineEdit *nameLineedit;
    QLabel *emailLabel;
    QLineEdit *emailLineedit;
};

class LicenseDetailsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit LicenseDetailsPage(QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *companyNameLabel;
    QLineEdit *companyNameLineedit;
    QLabel *emailAddrLabel;
    QLineEdit*emailAddrLineedit;
    QLabel *postalAddrLabel;
    QLineEdit *postalAddrLineedit;
};

class LicenseConclusionPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit LicenseConclusionPage(QWidget *parent = nullptr);

    int nextId() const override;
protected:
    void initializePage() override;
private:
    QLabel *topLabel;
    QCheckBox *agreeTermCheckbox;

};

#endif // LICENSEWIZARD_H
