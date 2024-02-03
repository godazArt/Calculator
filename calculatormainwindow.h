#ifndef CALCULATORMAINWINDOW_H
#define CALCULATORMAINWINDOW_H

#include "qpushbutton.h"
#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <vector>

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
    std::vector< void (CalculatorMainWindow::*)(QPushButton*)> funcs;
    int numOfDigits;
    int degree;
    int countOfEvents;
    bool dotPressed;
    bool zeros;
    void standartVals();
    void funcSqrt();
    void funcEqual();
    void funcErase();
    void funcClear();
    void funcPlusMinus();
    void configButtons();
    void startNewNum();
    double resNumber;
    double dispNumber;
    double savedNumber;
    double op_plus();
    double op_mult();
    double op_devot();
    double op_minus();
    double (CalculatorMainWindow::*operation_ptr)();
    void createWidgets();
    void createButtons();
    void placeWidgets();
    void digitFunc(int num);
    void dotDigitFunc(double num);
    void digitAnalys(QPushButton *button);
    void binOperations(QPushButton *button);
    void unOperations(QPushButton *button);

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
