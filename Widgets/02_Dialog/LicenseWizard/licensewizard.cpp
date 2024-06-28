#include "licensewizard.h"

#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QRegularExpressionValidator>

const QString emailRegExp = QStringLiteral(".+@.+");

LicenseWizard::LicenseWizard(QWidget *parent) : QWizard(parent)
{
    setPage(Page_Intro, new LicenseIntroPage);
    setPage(Page_Register, new LicenseRegisterPage);
    setPage(Page_Evaluate, new LicenseEvaluatePage);
    setPage(Page_Details, new LicenseDetailsPage);
    setPage(Page_Conclusion, new LicenseConclusionPage);

    setStartId(Page_Intro);

    setWindowTitle(tr("License Wizard"));

    // 设置help按钮
    setOption(HaveHelpButton, true);

    // 设置向导的logo
    setPixmap(LogoPixmap, QPixmap(":/imgs/logo.png"));

}

LicenseIntroPage::LicenseIntroPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/imgs/watermark.png"));

    topLabel = new QLabel(tr("This wizard will help you register your copy of "
                             "<i>Super Product One</i>&trade; or start evaluating the product."));
    topLabel->setWordWrap(true);

    regCopyRadioBtn = new QRadioButton(tr("&Register your copy"));
    evalRadioBtn = new QRadioButton(tr("&Evaluate the product for 30 days"));
    regCopyRadioBtn->setChecked(true);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(topLabel);
    vLayout->addWidget(regCopyRadioBtn);
    vLayout->addWidget(evalRadioBtn);
    setLayout(vLayout);

}

int LicenseIntroPage::nextId() const
{
    if(regCopyRadioBtn->isChecked())
        return LicenseWizard::Page_Register;
    else if(evalRadioBtn->isChecked())
        return LicenseWizard::Page_Evaluate;
}

LicenseEvaluatePage::LicenseEvaluatePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Evaluate <i>Super Product One</i>&trade;"));
    setSubTitle(tr("Please fill both fields."
                   " Make sure to provide a valid "
                   "email address(eg., zoya.zhou@qq.com)."));

    nameLabel = new QLabel(tr("&Name:"));
    nameLineedit = new QLineEdit;
    nameLabel->setBuddy(nameLineedit);

    emailLabel = new QLabel(tr("&Email address:"));
    emailLineedit = new QLineEdit;
    emailLabel->setBuddy(emailLineedit);
    emailLineedit->setValidator(new QRegularExpressionValidator(QRegularExpression(emailRegExp)));

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(nameLineedit, 0, 1);
    gridLayout->addWidget(emailLabel, 1, 0);
    gridLayout->addWidget(emailLineedit, 1, 1);
    setLayout(gridLayout);

    registerField("evaluate.name*", nameLineedit);
    registerField("evaluate.email*", emailLineedit);

}

int LicenseEvaluatePage::nextId() const
{

    return LicenseWizard::Page_Conclusion;
}

LicenseRegisterPage::LicenseRegisterPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Register Your Copy of <i>Super Product One</i>&trade;"));
    setSubTitle(tr("If you have an upgrade key, please fill in the appropriate field."));

    nameLabel = new QLabel(tr("N&ame:"));
    nameLineedit = new QLineEdit;
    nameLabel->setBuddy(nameLineedit);

    upgradeKeyLabel = new QLabel(tr("&Upgrade key:"));
    upgradeKeyLineedit = new QLineEdit;
    upgradeKeyLabel->setBuddy(upgradeKeyLineedit);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(nameLabel, 0,0);
    gridLayout->addWidget(nameLineedit, 0, 1);
    gridLayout->addWidget(upgradeKeyLabel, 1, 0);
    gridLayout->addWidget(upgradeKeyLineedit,1 ,1);

    setLayout(gridLayout);

    registerField("register.name*", nameLineedit);
    registerField("register.upgradeKey", upgradeKeyLineedit);
}

int LicenseRegisterPage::nextId() const
{
    if(upgradeKeyLineedit->text().isEmpty())
        return LicenseWizard::Page_Details;
    else
        return LicenseWizard::Page_Conclusion;
}

LicenseDetailsPage::LicenseDetailsPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Fill In Your Details"));
    setSubTitle(tr("Please fill all three fields."
                   " Make sure to provide a valid "
                   "email address(eg., zoya.zhou@qq.com)."));

    companyNameLabel = new QLabel(tr("&Company Name:"));
    companyNameLineedit = new QLineEdit;
    companyNameLabel->setBuddy(companyNameLineedit);

    emailAddrLabel = new QLabel(tr("&Email Address:"));
    emailAddrLineedit = new QLineEdit;
    emailAddrLabel->setBuddy(emailAddrLineedit);
    emailAddrLineedit->setValidator(new QRegularExpressionValidator(QRegularExpression(emailRegExp)));

    postalAddrLabel = new QLabel(tr("&Postal address:"));
    postalAddrLineedit = new QLineEdit;
    postalAddrLabel->setBuddy(postalAddrLineedit);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(companyNameLabel, 0,0);
    gridLayout->addWidget(companyNameLineedit,0,1);
    gridLayout->addWidget(emailAddrLabel, 1,0);
    gridLayout->addWidget(emailAddrLineedit,1,1);
    gridLayout->addWidget(postalAddrLabel, 2,0);
    gridLayout->addWidget(postalAddrLineedit,2,1);
    setLayout(gridLayout);

    registerField("details.company*", companyNameLineedit);
    registerField("details.email*", emailAddrLineedit);
    registerField("details.postal*", postalAddrLineedit);
}

int LicenseDetailsPage::nextId() const
{
    return LicenseWizard::Page_Conclusion;
}

LicenseConclusionPage::LicenseConclusionPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Complete Your Registration"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/imgs/watermark.png"));

    topLabel = new QLabel;
    topLabel->setWordWrap(true);

    agreeTermCheckbox = new QCheckBox(tr("I agree to the terms of the License"));

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(topLabel);
    vLayout->addWidget(agreeTermCheckbox);
    setLayout(vLayout);

    registerField("conclusion.agree",agreeTermCheckbox);
}

int LicenseConclusionPage::nextId() const
{
    return -1;
}

void LicenseConclusionPage::initializePage()
{
    QString licenseText;
    if(wizard()->hasVisitedPage(LicenseWizard::Page_Evaluate)){
        licenseText = tr("<U>Evaluation License Agreement:</U> "
                         "You can use this software for 30 days and make one "
                         "backup, but you are not allowed to distribute it.");
    } else if(wizard()->hasVisitedPage(LicenseWizard::Page_Details)){
        const QString emailAddress = field("details.email").toString();
        licenseText = tr("<U>First-Time License Agreement:</U> "
                         "You can use this software subject to the license "
                         "you will receive by email sent to %1").arg(emailAddress);
    } else {
        licenseText = tr("<U>Upgrade License Agreement:</U> "
                         "This software is licensed under the terms of your "
                         "current license.");
    }
    topLabel->setText(licenseText);
}
