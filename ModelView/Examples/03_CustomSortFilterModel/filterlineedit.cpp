#include "filterlineedit.h"

#include <QtWidgets>

FilterLineEdit::FilterLineEdit(QWidget *parent)
    : QLineEdit{parent}
{
    // 设置清楚按钮
    setClearButtonEnabled(true);
    connect(this, &FilterLineEdit::textChanged, this, &FilterLineEdit::filterChanged);

    // 创建菜单项
    QMenu *menu = new QMenu(this);
    caseSensitiveAction = menu->addAction(tr("Case Sensitive"));
    caseSensitiveAction->setCheckable(true);
    connect(caseSensitiveAction, &QAction::toggled, this, &FilterLineEdit::filterChanged);

    menu->addSeparator();
    patternActionGroup = new QActionGroup(this);
    QAction *patternAction = menu->addAction(tr("Regular Expression"));
    patternAction->setCheckable(true);
    patternAction->setChecked(true);
    patternAction->setData(QVariant(int(RegularExpression)));
    patternActionGroup->addAction(patternAction);
    patternAction = menu->addAction(tr("Wildcard"));
    patternAction->setCheckable(true);
    patternAction->setData(QVariant(int(Wildcard)));
    patternActionGroup->addAction(patternAction);
    patternAction = menu->addAction(tr("Fixed String"));
    patternAction->setCheckable(true);
    patternAction->setData(QVariant(int(FixedString)));
    patternActionGroup->addAction(patternAction);
    connect(patternActionGroup, &QActionGroup::triggered, this, &FilterLineEdit::filterChanged);

    QToolButton *optionsButton = new QToolButton;
    // 设置工具按钮图标
    optionsButton->setIcon(QIcon(QPixmap(":/images/find.png")));
    // 设置无边界
    optionsButton->setStyleSheet("* {border : none;};");  // *匹配该类的所有部件
    optionsButton->setFocusPolicy(Qt::NoFocus);  // 不接受焦点
    // 设置当鼠标移动到工具按钮上时鼠标的样式
#ifndef QT_NO_CURSOR
    optionsButton->setCursor(Qt::ArrowCursor);
#endif
    // 设置工具按钮的菜单项
    optionsButton->setMenu(menu);
    // 设置工具按钮的菜单项立即弹出
    optionsButton->setPopupMode(QToolButton::InstantPopup);  // 立即弹出菜单项


    // 将自定义部件插入到基于容器的动作中
    QWidgetAction *optionsAction = new QWidgetAction(this);
    optionsAction->setDefaultWidget(optionsButton);  // 设置默认的部件
    addAction(optionsAction, QLineEdit::LeadingPosition);

}

Qt::CaseSensitivity FilterLineEdit::caseSensitivity() const
{
    return caseSensitiveAction->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
}

void FilterLineEdit::setCaseSensitivity(Qt::CaseSensitivity caseSensitivity)
{
    caseSensitiveAction->setChecked(caseSensitivity == Qt::CaseSensitive);
}

static inline FilterLineEdit::PatternSyntax patternSyntaxFromAction(const QAction *a)
{
    return static_cast<FilterLineEdit::PatternSyntax>(a->data().toInt());
}

FilterLineEdit::PatternSyntax FilterLineEdit::patternSyntax() const
{
    return patternSyntaxFromAction(patternActionGroup->checkedAction());
}

void FilterLineEdit::setPatternSyntax(PatternSyntax syntax)
{
    const QList<QAction*> actions = patternActionGroup->actions();
    for(QAction *action : actions){
        if(patternSyntaxFromAction(action) == syntax){
            action->setChecked(true);
            break;
        }
    }
}
