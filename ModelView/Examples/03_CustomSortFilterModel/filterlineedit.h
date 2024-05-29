#ifndef FILTERLINEEDIT_H
#define FILTERLINEEDIT_H

#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
QT_END_NAMESPACE

class FilterLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(Qt::CaseSensitivity caseSensitivity READ caseSensitivity WRITE setCaseSensitivity)
    Q_PROPERTY(PatternSyntax patternSyntax READ patternSyntax WRITE setPatternSyntax)
public:
    explicit FilterLineEdit(QWidget *parent = nullptr);

    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(Qt::CaseSensitivity caseSensitivity);

    enum PatternSyntax{
        RegularExpression,
        Wildcard,
        FixedString
    };
    Q_ENUM(PatternSyntax)

    PatternSyntax patternSyntax() const;
    void setPatternSyntax(PatternSyntax syntax);

signals:
    void filterChanged();

private:
    QAction *caseSensitiveAction;
    QActionGroup *patternActionGroup;
};

#endif // FILTERLINEEDIT_H
