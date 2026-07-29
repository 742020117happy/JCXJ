#pragma once
#include "Variable.h"
#include "HIPNUC_CH10X_Remote.h"
#include "ui_HIPNUC_CH10X.h" // Qt UIC 自动生成

#include <QSocketNotifier>
#include <QFile>

class c_HIPNUC_CH10X : public QWidget {
    Q_OBJECT
public:
    explicit c_HIPNUC_CH10X(QStringList info, QWidget *parent = nullptr);
    virtual ~c_HIPNUC_CH10X() override;

protected:
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void System_Scan();
    void Init();
    void Refresh_Ports(); // 刷新可用串口列表
    void readMessage(QString cmd);
    void Write_Communicate_DB(QString key, int value);
    void Write_Communicate_DB(QString key, QString value);

private:
    Ui::HIPNUC_CH10X ui; // 必须与 .ui 文件中的 <class> 标签一致
    bool m_Scan = false;
    bool m_Initialized = false;
    QString m_DB_Path;
    
    c_HIPNUC_CH10X_Remote *m_Remote;
    QThread *m_Remote_Thread;
    
    QFile m_file;
    QSocketNotifier *m_pNotifier;
    char m_arrRecv[128];
};