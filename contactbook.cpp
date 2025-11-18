#include "contactbook.h"
#include "ui_contactbook.h"

#include <QFile>
#include <QDebug>
#include <QFontDatabase>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>
#include <QDir>
#include <QStandardPaths>

QString mFilename = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ContactBook_autosave.txt";

void Write(QString Filename,QString str){
    QFile mFile(Filename);

    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug()<<"could not open file for write";
        return;
    }
    QTextStream out(&mFile);
    out<<str;
    mFile.flush();
    mFile.close();
}

ContactBook::ContactBook(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactBook)
{
    ui->setupUi(this);
    
    // Load Iansui font and apply to entire application
    int fontId = QFontDatabase::addApplicationFont(":/fonts/fonts/Iansui-Regular.ttf");
    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.isEmpty()) {
            QString family = fontFamilies.at(0);
            QFont iansuitFont(family);
            
            // Apply font to all UI elements
            this->setFont(iansuitFont);
            ui->tableWidget->setFont(iansuitFont);
            ui->tableWidget->horizontalHeader()->setFont(iansuitFont);
            
            // Center align table headers
            ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
        }
    }
    
    QStringList ColTotle;
    ui->tableWidget->setColumnCount(4);
    ColTotle<<"學號"<<"班級"<<"姓名"<<"電話";
    ui->tableWidget->setHorizontalHeaderLabels(ColTotle);
    
    // Make columns stretch to fill the table widget
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

ContactBook::~ContactBook()
{
    delete ui;
}

void ContactBook::on_pushButton_clicked()
{
    QTableWidgetItem *inputRow1,*inputRow2,*inputRow3,*inputRow4;

    inputRow1 = new QTableWidgetItem(QString(ui->lineEdit->text()));
    inputRow2 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    inputRow3 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    inputRow4 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,inputRow4);
    
    // Clear input fields after adding
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}

void ContactBook::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("匯出聯絡人"), "ContactBook.txt",
        tr("文字檔案 (*.txt);;逗號分隔檔案 (*.csv);;所有檔案 (*)"));

    if (fileName.isEmpty())
        return;

    QString saveFile="";

    for(int i=0;i<ui->tableWidget->rowCount();i++){
        for(int j=0;j<ui->tableWidget->columnCount();j++){
            if (ui->tableWidget->item(i,j)) {
                saveFile+=ui->tableWidget->item(i,j)->text()+",";
            } else {
                saveFile+=",";
            }
        }
        saveFile+="\n";
    }
    Write(fileName,saveFile);
}

void ContactBook::on_pushButton_3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("匯入聯絡人"), "",
        tr("文字檔案 (*.txt);;逗號分隔檔案 (*.csv);;所有檔案 (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, tr("錯誤"), tr("無法開啟檔案"));
        return;
    }

    QTextStream in(&file);
    ui->tableWidget->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        
        if (fields.size() >= 4) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            
            for (int i = 0; i < 4 && i < fields.size(); i++) {
                QTableWidgetItem *item = new QTableWidgetItem(fields[i]);
                ui->tableWidget->setItem(row, i, item);
            }
        }
    }
    file.close();
}

void ContactBook::saveData()
{
    QString saveFile="";

    for(int i=0;i<ui->tableWidget->rowCount();i++){
        for(int j=0;j<ui->tableWidget->columnCount();j++){
            if (ui->tableWidget->item(i,j)) {
                saveFile+=ui->tableWidget->item(i,j)->text()+",";
            } else {
                saveFile+=",";
            }
        }
        saveFile+="\n";
    }
    Write(mFilename,saveFile);
}

void ContactBook::on_pushButton_4_clicked()
{
    saveData();
    close();
}
