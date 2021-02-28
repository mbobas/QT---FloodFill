#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QTextStream>
#include <bits/stdc++.h>
#include <QColorDialog>
using namespace std;

namespace Ui {
class MainWindow;
}

// Deklaracja głównej klasy naszego programu
// Jest ona podklasą klasy QMainWindow
// standardowej klasy Qt reprezentującej główne okno aplikacji
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// Sloty to specjalne funkcje (metody) wywoływane
// po wystąpieniu pewnych zdarzeń np. po kliknięciu przycisku
private slots:
    void on_exitButton_clicked();
    void on_cleanButton_clicked();

    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void on_XChanged(int arg1);
    void on_YChanged(int arg1);
    void on_X1Changed(int arg1);
    void on_Y1Changed(int arg1);
    //bool check(int x0, int y0, int x1, int y1);




    void on_radioButton_clicked();

    void on_wypelnienie_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    // ui to wskaźnik za pomocą którego mamy dostęp
    // do elementów interfejsu użytkownika
    Ui::MainWindow *ui;

    QImage *img; 
    int width, height, startX, startY,x0, y0, x1, y1,option;
    int newColorR=53;
    int newColorG=151;
    int newColorB=70;
    int colorToChangeR=0, colorToChangeG=0, colorToChangeB=0;
    int sectionColorR=255, sectionColorG=255, sectionColorB=255;

    void clean();
    void drawPixel(int x, int y, unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255);
    void draw_section(int x0, int y0, int x1, int y1, unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255);
    void floodFill(int x, int y,int colorToChangeR, int colorToChangeG, int colorToChangeB, int newColorR, int newColorG, int newColorB);
    int check_color(int x, int y, int red, int green, int blue);
    int checkColorWithColorPicker(int x,int y, int red, int green, int blue);

};

#endif // MAINWINDOW_H
