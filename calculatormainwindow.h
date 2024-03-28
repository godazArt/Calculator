#ifndef CALCULATORMAINWINDOW_H
#define CALCULATORMAINWINDOW_H

#include "qpushbutton.h"
#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <vector>
#include <map>
#include <stack>

class QSignalMapper;
class QPushButton;
class QLineEdit;


class CalculatorMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CalculatorMainWindow(QWidget *parent = nullptr);
    ~CalculatorMainWindow();

private slots:
    void slotEngine();
private:

    QString state;
    std::map<QString,QString> operations;
    std::map<QString,void (CalculatorMainWindow::*)()> engineFuncs;
    std::map<QString,void (CalculatorMainWindow::*)()> drawingFuncs;

    std::stack<double> nums;

    int numOfDigits;
    int degree;
    bool zeros;
    bool isUnOpPrev;
    bool dotFlag;
    bool dotPressed;
    bool isButtonPressed;

    void isNeedSolv();
    void funcPlus();
    void funcMinus();
    void funcMult();
    void funcDevot();
    void funcDot();
    void funcSqrt();
    void funcEqual();
    void funcErase();
    void funcClear();

    void drawFuncDot();
    void drawFuncSqrt();
    void drawFuncEqual();
    void drawFuncErase();
    void drawFuncClear();
    void drawFuncPlusMinus();

    void clearSteck();
    void configVals();
    void unOpCleaner();    
    void placeWidgets();
    void standartVals();
    void funcPlusMinus();
    void configButtons();
    void configOperations();
    void createWidgets();
    void createButtons();
    void engineMapCongig();
    void drawingMapCongig();
    void digitFunc(int num, int temp);
    void digitAnalys();
    void solveExample(QPushButton *button);
    void moveToNextNum(QPushButton *button);

    void binOperationsDraw();
    void digitsDraw();

    void dotDigitFunc(double num, int temp);

    double op_plus();
    double op_mult();
    double op_devot();
    double op_minus();
    double (CalculatorMainWindow::*operation_ptr)();


    template<typename PtrSlot>
    QPushButton *createButton(const QString &text, const PtrSlot &slot)
    {
        QPushButton *Button = new QPushButton(text, this);
        connect(Button, &QPushButton::clicked, this, slot);
        Button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        return Button;
    }

    std::vector<QPushButton*> numButtons;
    QPushButton *pushButtonPlusMinus;
    QPushButton *pushButtonDevot;
    QPushButton *pushButtonMinus;
    QPushButton *pushButtonEqual;
    QPushButton *pushButtonErase;
    QPushButton *pushButtonPlus;
    QPushButton *pushButtonMult;
    QPushButton *pushButtonSqrt;
    QPushButton *pushButtonDot;
    QPushButton *pushButtonC;
    QLineEdit *display;
    QLineEdit *histDisplay;
};
#endif // CALCULATORMAINWINDOW_H
