#ifndef MDICHILD_H
#define MDICHILD_H

#include <QTextEdit>
#include <QCloseEvent>

class MdiChild : public QTextEdit
{
    Q_OBJECT
public:
    explicit MdiChild(QWidget *parent = nullptr);

    void newFile();
    bool loadFile(const QString &filename);
    bool save();
    QString currentFile() const { return curFile; }
    QString userFriendlyCurrentFile();
signals:

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void documentWasModified();
private:
    void setCurrentFile(const QString &filename);
    // 获取文件名
    QString strippedName(const QString &filename);
    bool saveFile(const QString &filename);
    bool saveAs();
    // 由用户决定是否要保存文件
    bool maybeSave();
private:
    QString curFile;
    bool isUntitled;  // 是否未命名，true未命名，false，已经命名
};

#endif // MDICHILD_H
