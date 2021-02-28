#include "mainwindow.h"
#include "ui_mainwindow.h"

// Konstruktor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Pobieramy współrzędne (względem głównego okna) lewego, górnego naroznika ramki
    startX = ui->frame->x();
    startY = ui->frame->y();

    // Pobieramy wymiary ramki
    width = ui->frame->width();
    height = ui->frame->height();

    // Tworzymy obiekt klasy QImage o wymiarach równych wymiarom ramki
    // Format_RGB32 to 32 bitowe RGB (z kanałem alfa ustawionym na wartość 255)
    img = new QImage(width, height, QImage::Format_RGB32);
}
//*****************************************************************************************************


// Destruktor
MainWindow::~MainWindow()
{
    delete ui;
}
//*****************************************************************************************************


// Funkcja ,,odrysowująca'' okno aplikacji. Wywoływana automatycznie podczas uruchamiania aplikacji
// i w innych sytuacjach, w których zachodzi konieczność ponownego narysowania okna.
// Można też wymusić jej wywołanie za pomocą funkcji update()
void MainWindow::paintEvent(QPaintEvent*)
{
    // Obiekt przy pomocy, którego możemy rysować po elementach interfejsu graficznego aplikacji
    QPainter p(this);

    // Metoda drawImage wyświetla zawartość obiektu *img klasy QImage w głównym oknie aplikacji
    // startX i startY to współrzędne (względem głównego okna) lewego górnego narożnika
    // wyświetlanego obiektu
    p.drawImage(startX, startY, *img);
}
//*****************************************************************************************************


// Funkcja zamalowująca na czarno wszystkie piksele obrazu *img
void MainWindow::clean()
{
    // deklaracja wskaźnika do poruszania się po pikselach obrazu
    unsigned char *wsk;

    // przechodzimy po wszystkich wierszach obrazu
    for(int i=0; i<height; i++)
    {
        // ustawiamy wskaźnik na początku i-tego wiersza
        wsk = img->scanLine(i);

        // przechodzimy po wszystkich piselach i-tego wiersza
        // i nadajemy im wartość (0,0,0) odpowiadającą kolorowi czarnemu
        for(int j=0; j<width; j++)
        {
            wsk[4*j] = 0;   // ustawiamy składową ,,niebieską'' na 0
            wsk[4*j+1] = 0; // ustawiamy składową ,,zielonąą'' na 0
            wsk[4*j+2] = 0; // ustawiamy składową ,,czerwoną'' na 0
        }
    }
}
//*****************************************************************************************************

// zamalowuje piksel (x,y) na kolor (red,green,blue), domyślnie na biało
void MainWindow::drawPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char *wsk;

    // sprawdzamy czy (x,y) leży w granicach rysunku
    if(x>=0 && y>=0 && x<width && y<height)
    {
        wsk = img->scanLine(y);
        wsk[4*x] = blue;
        wsk[4*x+1] = green;
        wsk[4*x+2] = red;
    }

}

// zamalowuje odcinek (x0,y0 -> x1,y1) na kolor (red,green,blue), domyślnie na biało
void MainWindow::draw_section(int x1, int y1, int x2, int y2, unsigned char red, unsigned char green, unsigned char blue)
{
        float a,b,y,dl,g;
        if(x1>x2){
            int sw;
            sw = x1;
            x1 = x2;
            x2 = sw;
            sw = y1;
            y1 = y2;
            y2 = sw;
        }
        a = ((float)y2 - y1)/(x2-x1);
        b = y1 - a * x1;
        dl = sqrt(pow((x2-x1),2)+pow((y2-y1),2));
        g=(x2-x1)/dl;
        float x=x1;
        for(int i=0; i<dl ; i++){

            y = a*x+b;
            drawPixel(round(x), round(y));
            x+=g;
        }
}


//*****************************************************************************************************

// Metoda wywoływana po wciśnięciu przycisku myszy
// Inne metody związane z obsługą myszy to
// void MainWindow::mouseReleaseEvent(QMouseEvent *event)
// wywoływana po puszczeniu przycisku myszy i
// void MainWindow::mouseMoveEvent(QMouseEvent *event)
// wywoływana po każdym ruchu myszy
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    on_X1Changed(x1);
    on_Y1Changed(y1);
    // Pobieramy współrzędne punktu puszczenia myszy
    x1 = event->x();
    y1 = event->y();
    // Współrzędne obliczane są względem głównego okna programu
    // aby uzyskać współrzędne względem obszaru rysowania (ramki) musimy je przesunąć
    // tak aby punkt (0,0) pokrywał się z lewym górnym naroznikiem obszaru rysowania
    x1 = x1 - startX;
    y1 = y1 - startY;

    // Jeżeli wciśnięto lewy przycisk to zamolowujemy piksel na biało
    if(event->button() == Qt::LeftButton)
    {
        drawPixel(x1,y1);
    }
    // a w przeciwnym wypadku na czerwono
    else
    {
         floodFill(x0,y0,0,0,0,153,53,28);
    }

    draw_section(x0,y0,x1,y1,255,0,0);

    update();
}
//*****************************************************************************************************

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //int x,y;
    on_XChanged(x0);
    on_YChanged(y0);

    // Pobieramy współrzędne punktu kliknięcia
    x0 = event->x();
    y0 = event->y();
    // Współrzędne obliczane są względem głównego okna programu
    // aby uzyskać współrzędne względem obszaru rysowania (ramki) musimy je przesunąć
    // tak aby punkt (0,0) pokrywał się z lewym górnym naroznikiem obszaru rysowania
    x0 = x0 - startX;
    y0 = y0 - startY;

    // Jeżeli wciśnięto lewy przycisk to zamolowujemy piksel na biało
    if(event->button() == Qt::LeftButton)
    {
        drawPixel(x0,y0);
    }
    // a w przeciwnym wypadku na czerwono
    else
    {


    }

    update();
}

//sprawdzenie czy piksel x,y ma odpowiedni kolor;
int MainWindow::check_color(int x, int y, int red, int green, int blue)
{

    unsigned char *wsk;

    if((x>=0)&&(y>=0)&&(x<width)&&(y<height))
    {
        wsk = img->bits();
        if ((wsk[width*4*y + 4*x] == blue ) && (wsk[width*4*y + 4*x+1] == green ) && (wsk[width*4*y + 4*x+2] == red )) {
            return(1);
        } else {
            return(0);
        }
    }
    else
        return(0);
}

int MainWindow::checkColorWithColorPicker(int x,int y, int red, int green, int blue){
    if((x>=0)&&(y>=0)&&(x<width)&&(y<height)){
         QRgb color = img->pixel(x,y);
         //QTextStream(stdout) << qRed(color) << "," << qGreen(color)<< "," << qBlue(color);

         if ( (int(qBlue(color)) == blue ) && (int(qGreen(color)) == green ) && (int(qRed(color)) == red )) {
             return(1);

         } else {
             return(0);
         }
     }
    else
        return(0);
}


//wypełnianie kolorem
void MainWindow::floodFill(int x, int y,int colorToChangeR, int colorToChangeG, int colorToChangeB, int newColorR, int newColorG, int newColorB)
{
    stack <int> sx;
    stack <int> sy;
    int xActive,yActive;

    sx.push(x);
    sy.push(y);

    while (!sx.empty() && !sy.empty()) {
        xActive = sx.top();
        yActive = sy.top();
        sx.pop();
        sy.pop();
      if (checkColorWithColorPicker(xActive,yActive, 0,0,0)){
              drawPixel(xActive,yActive,51, 153, 51);
              sx.push(xActive-1);
              sy.push(yActive);

            sx.push(xActive+1);
            sy.push(yActive);

            sx.push(xActive);
            sy.push(yActive+1);

            sx.push(xActive);
            sy.push(yActive-1);
     }

    }
        update();
}

// Metoda wywoływana po nacisnięciu przycisku exitButton (,,Wyjście'')
void MainWindow::on_exitButton_clicked()
{
    // qApp to globalny wskaźnik na obiekt aplikacji
    // metoda quit() powoduje zakończenie działania aplikacji z kodem wyjścia 0 (sukces)
    qApp->quit();
}
//*****************************************************************************************************

// Metoda wywoływana po nacisnięciu przycisku exitButton (,,Wyjście'')
void MainWindow::on_cleanButton_clicked()
{
    clean();

    // Po zmodyfikowaniu obiektu QImage odświeżamy okno aplikacji, aby zobaczyc zmiany
    update();
}

void MainWindow::on_XChanged(int x0)
{
   ui->labelX->setText(QString::number(x0));
}

void MainWindow::on_YChanged(int x0)
{
   ui->labelY->setText(QString::number(y0));
}
void MainWindow::on_X1Changed(int x0)
{
   ui->labelX_2->setText(QString::number(x1));
}

void MainWindow::on_Y1Changed(int x0)
{
   ui->labelY_2->setText(QString::number(y1));
}

