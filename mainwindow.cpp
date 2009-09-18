#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QPrintPreviewDialog>
#include <QtGui>
#include <QtGui/QApplication>
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

#ifdef Q_WS_MAC
const QString rsrcPath = ":/images/mac";
#else
const QString rsrcPath = ":/images/win";
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    setCurrentFileName();
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addToDefinedList()));
    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(displayCompound()));

    setupFileActions();
    setupEditActions();
    setupToolsActions();
    setupWindowActions();
    setupHelpActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupFileActions()
{
    QMenu *menu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(menu);

    QAction *a;

    a = new QAction(QIcon(rsrcPath + "/filenew.png"), tr("&New"), this);
    a->setShortcut(QKeySequence::New);
    connect(a, SIGNAL(triggered()), this, SLOT(fileNew()));
    menu->addAction(a);

    a = new QAction(QIcon(rsrcPath + "/fileopen.png"), tr("&Open"), this);
    a->setShortcut(QKeySequence::Open);
    connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
    menu->addAction(a);

    menu->addSeparator();

    a = new QAction(tr("&Quit"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(a, SIGNAL(triggered()), qApp, SLOT(quit()));
    menu->addAction(a);
}

void MainWindow::setupEditActions()
{
    QMenu *menu = new QMenu(tr("&Edit"), this);
    menuBar()->addMenu(menu);
}

void MainWindow::setupToolsActions()
{
    QMenu *menu = new QMenu(tr("&Tools"), this);
    menuBar()->addMenu(menu);
}

void MainWindow::setupWindowActions()
{
    QMenu *menu = new QMenu(tr("&Window"), this);
    menuBar()->addMenu(menu);
}

void MainWindow::setupHelpActions()
{
    QMenu *menu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(menu);

    QAction *a;

    a = new QAction(tr("&About"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(about()));
    menu->addAction(a);
}

void MainWindow::fileNew()
{
    //to be completed    
}

void MainWindow::fileOpen()
{    
    QString fn = QFileDialog::getOpenFileName(this, tr("Open a File..."),               
        tr("Card Files (*.cd);;Text Files (*.txt *.rtf);;All Files (*)"));

//"/home/alex/Desktop", tr("All Files(*)"));

    if (!fn.isEmpty() && fn != this->fileName)
        load(fn);
    else
    {
        QMessageBox msgBox;
        msgBox.setText("File does not exist or is already open");
        msgBox.exec();
    }    
}

bool MainWindow::hasBeenSaved()
{
    return this->fileName != "";
}

bool MainWindow::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;

    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    //Parse the file into a string
    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);

    //Split the string so it can be added to the compound list; remove the eof
    //string (empty) from the list
    compoundList = str.split("\n");
    compoundList.removeAt(compoundList.indexOf(""));

    foreach(QString compound, compoundList)
    {                
        ui->comboBox->addItem(compound.split("@")[0]);        
        compoundInfo.insert(compound.split("@")[0], compound.split("@")[1]);                        
        ui->listWidget->insertItem(compoundInfo.size()-1, compound.split("@")[0]);
    } 

    setCurrentFileName(f);    
    displayCompound();
    return true;
}

void MainWindow::displayCompound()
{   
    //Initialize graphics view objects and dependents
    QGraphicsScene *scn = new QGraphicsScene(ui->graphicsView);
    scn->setSceneRect(ui->graphicsView->rect());
    ui->graphicsView->setScene(scn);

    //Display the currently selected compound
    QString currentCompoundImagePath = compoundInfo.value(ui->comboBox->currentText());   
    QPixmap pix(currentCompoundImagePath);
    scn->addPixmap(pix);
    ui->graphicsView->show();
}

void MainWindow::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;
    //textEdit->document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.cd";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("ESP v2.0")));
    setWindowModified(false);
}

void MainWindow::addToDefinedList()
{      
    QString name = ui->titleTextBox->text(), path = ui->pathTextBox->text();   

    if(name != "" && path != "")
    {
        ui->comboBox->addItem(name);
        compoundInfo.insert(name, path);
        ui->listWidget->insertItem(compoundInfo.size(), name);
    }
    else
    {
        QMessageBox q;
        q.setText("Please provide a path and name for the compound.");
        q.exec();
    }

    displayCompound();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About"), tr("ESP v2.0 \n\n" "Written by Alex Wood and "
                                             "Aaron McKellar \n\n This software models different "
                                             "chemical compounds and places their information in the selected"
                                             " journal format."));
}

