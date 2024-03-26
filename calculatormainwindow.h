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
    enum objectNames
    {
        digits = 0,
        binOp = 1,
        unOp = 2
    };
    std::map<QString,void (CalculatorMainWindow::*)(QObject*)> engineFuncs;
    std::map<QString,void (CalculatorMainWindow::*)(QObject*)> drawingFuncs;
    std::vector< void (CalculatorMainWindow::*)(QPushButton*)> funcs;
    std::vector< void (CalculatorMainWindow::*)(QPushButton*)> drawFuncs;
    std::stack<double> nums;
    int numOfDigits;
    int degree;
    bool zeros;
    bool isUnOpPrev;
    bool dotPressed;
    bool isButtonPressed;

    void funcSqrt();
    void funcEqual();
    void funcErase();
    void funcClear();    
    void clearSteck();
    void unOpCleaner();    
    void placeWidgets();
    void standartVals();
    void funcPlusMinus();
    void configButtons();
    void createWidgets();
    void createButtons();
    void engineMapCongig();
    void drawingMapCongig();
    void digitFunc(int num, int temp);
    void digitAnalys(QPushButton *button);
    void solveExample(QPushButton *button);
    void unOperations(QPushButton *button);
    void moveToNextNum(QPushButton *button);
    void binOperations(QPushButton *button);

    void binOperationsDraw(QPushButton *button);
    void unOperationsDraw(QPushButton *button);
    void digitsDraw(QPushButton *button);

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
