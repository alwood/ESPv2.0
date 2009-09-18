#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtGui/QMainWindow>
#include <QtGui/QApplication>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();    

protected:
    //virtual void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    QString fileName;
    QStringList compoundList;
    QMap<QString, QString> compoundInfo;

    //Setup menu bar
    void setupFileActions();    
    void setupEditActions();
    void setupWindowActions();
    void setupToolsActions();
    void setupHelpActions();

    void setCurrentFileName(const QString &f = "");
    bool load(const QString &f);    
    bool hasBeenSaved();    

public slots:
    void fileNew();
    void fileOpen();    
    void about();    

    void addToDefinedList();

private slots:    
    void displayCompound();


};

#endif // MAINWINDOW_H
