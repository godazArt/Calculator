#include "calculatormainwindow.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QSizePolicy>
#include <QFont>
#include <math.h>


CalculatorMainWindow::CalculatorMainWindow(QWidget *parent)
    : QWidget(parent)
{
    zeros = true;
    resNumber = 0;
    standartVals();
    savedNumber = 0;
    resize(300,400);
    numOfDigits = 10;
    countOfEvents = 0;
    funcs.push_back(&CalculatorMainWindow::digitAnalys);
    funcs.push_back(&CalculatorMainWindow::binOperations);
    funcs.push_back(&CalculatorMainWindow::unOperations);
    setWindowTitle("Simple Calculator");
    createWidgets();
}

void CalculatorMainWindow::standartVals()
{
    degree = 10;
    dispNumber = 0;
    dotPressed = false;
    operation_ptr = nullptr;
}

void CalculatorMainWindow::createButtons()
{
    for (int i = 0; i < numOfDigits; ++i)
        numButtons.push_back(createButton(QString::number(i), &CalculatorMainWindow::slotEngine));
    pushButtonPlus = createButton("+", &CalculatorMainWindow::slotEngine);
    pushButtonMult = createButton("*", &CalculatorMainWindow::slotEngine);
    pushButtonDevot = createButton("/", &CalculatorMainWindow::slotEngine);
    pushButtonMinus = createButton("-", &CalculatorMainWindow::slotEngine);
    pushButtonC = createButton("C", &CalculatorMainWindow::slotEngine);
    pushButtonEqual = createButton("=", &CalculatorMainWindow::slotEngine);
    pushButtonSqrt = createButton("√", &CalculatorMainWindow::slotEngine);
    pushButtonDot = createButton(".", &CalculatorMainWindow::slotEngine);
    pushButtonErase = createButton("<-", &CalculatorMainWindow::slotEngine);
    pushButtonPlusMinus = createButton("+/-", &CalculatorMainWindow::slotEngine);
}

void CalculatorMainWindow::placeWidgets()
{
    QGridLayout *lCalcLayout = new QGridLayout ;
    setLayout( lCalcLayout);
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    QFont dispFont = display->font();
    dispFont.setPixelSize(30);
    display->setFont(dispFont);
    display->setFixedHeight(50);
    lCalcLayout->addWidget ( display , 1 , 0 , 1 , 4 ) ;
    histDisplay = new QLineEdit();
    histDisplay->setReadOnly(true);
    histDisplay->setAlignment(Qt::AlignRight);
    dispFont.setPixelSize(20);
    histDisplay->setFont(dispFont);
    histDisplay->setFixedHeight(35);
    lCalcLayout->addWidget ( histDisplay , 0 , 0 , 1 , 4 ) ;
    lCalcLayout->addWidget ( numButtons[0] , 6 , 1 ) ;
    for (int i = 1 ; i < numOfDigits; i += 3)
        for(int j = 0; j < 3; ++j)
            lCalcLayout->addWidget(numButtons[i + j], (i / 3) + 3, j);
    lCalcLayout->addWidget ( pushButtonSqrt , 2 , 0  ) ;
    lCalcLayout->addWidget ( pushButtonC , 2 , 1 ) ;
    lCalcLayout->addWidget ( pushButtonErase , 2 , 2  ) ;
    lCalcLayout->addWidget ( pushButtonEqual , 2 , 3 ) ;
    lCalcLayout->addWidget ( pushButtonPlus , 6 , 3  ) ;
    lCalcLayout->addWidget ( pushButtonMult , 4 , 3  ) ;
    lCalcLayout->addWidget ( pushButtonDevot , 3 , 3  ) ;
    lCalcLayout->addWidget ( pushButtonMinus , 5 , 3  ) ;
    lCalcLayout->addWidget ( pushButtonDot , 6 , 2  ) ;
    lCalcLayout->addWidget ( pushButtonPlusMinus , 6 , 0  ) ;

}

void CalculatorMainWindow::createWidgets()
{
    createButtons();
    configButtons();
    placeWidgets();
}

void CalculatorMainWindow::configButtons()
{
    for (int i = 0; i < numOfDigits; ++i)
        numButtons[i]->setObjectName(QString::number(objectNames::digits));
    pushButtonPlus->setObjectName(QString::number(objectNames::binOp));
    pushButtonMult->setObjectName(QString::number(objectNames::binOp));
    pushButtonDevot->setObjectName(QString::number(objectNames::binOp));
    pushButtonMinus->setObjectName(QString::number(objectNames::binOp));
    pushButtonC->setObjectName(QString::number(objectNames::unOp));
    pushButtonEqual->setObjectName(QString::number(objectNames::unOp));
    pushButtonSqrt->setObjectName(QString::number(objectNames::unOp));
    pushButtonDot->setObjectName(QString::number(objectNames::unOp));
    pushButtonErase->setObjectName(QString::number(objectNames::unOp));
    pushButtonPlusMinus->setObjectName(QString::number(objectNames::unOp));
}

void CalculatorMainWindow::slotEngine()
{
    QPushButton * button = qobject_cast<QPushButton *>(sender());
    (this->*(funcs[button->objectName().toInt()]))(button);
}

void CalculatorMainWindow::digitFunc(int num)
{
    dispNumber = dispNumber * 10 +  num;
    display->clear();
    display->insert(QString::number(dispNumber));
}

void CalculatorMainWindow::dotDigitFunc(double num)
{
    dispNumber = dispNumber +  num / degree;
    degree *=10;
    display->clear();
    display->insert(QString::number(dispNumber));
}

void CalculatorMainWindow::digitAnalys(QPushButton *button)
{
    if(countOfEvents % 2 == 0)
        countOfEvents++;
    if(dotPressed)
        dotDigitFunc(button->text().toInt());
    else
        digitFunc(button->text().toInt());
}

void CalculatorMainWindow::binOperations(QPushButton *button)
{
    if(countOfEvents > 2)
    {
        histDisplay->insert(QString::number(dispNumber) + button->text());
        countOfEvents = 2;
        resNumber = (this->*operation_ptr)();
        standartVals();
        display->clear();
        display->insert(QString::number(resNumber));
    }

    if(countOfEvents % 2 != 0)
    {
        histDisplay->insert(QString::number(dispNumber) + button->text());
        countOfEvents++;
        resNumber = dispNumber;
        standartVals();
    }
    else
    {
        histDisplay->backspace();
        histDisplay->insert(button->text());
    }

    switch (button->text().toStdString()[0]) {
    case '+':
        operation_ptr = &CalculatorMainWindow::op_plus;
        break;
    case '-':
        operation_ptr = &CalculatorMainWindow::op_minus;
        break;
    case '*':
        operation_ptr = &CalculatorMainWindow::op_mult;
        break;
    case '/':
        operation_ptr = &CalculatorMainWindow::op_devot;
        break;
    }
}

void CalculatorMainWindow::unOperations(QPushButton *button)
{
    switch (button->text().toStdString()[0]) {
    case 'C':
        funcClear();
        break;
    case '=':
        funcEqual();
        break;
    case '<':
        funcErase();
        break;
    case '+':
        funcPlusMinus();
        break;
    case '.':
        dotPressed = true;
        break;
    default:
        if(button->text() == "√")
            funcSqrt();
        break;
    }
}

void CalculatorMainWindow::funcSqrt()
{
    savedNumber = sqrt(display->text().toInt());
    dispNumber = 0;
    display->clear();
    display->insert(QString::number(savedNumber));
}

void CalculatorMainWindow::funcPlusMinus()
{
    dispNumber = -1 * display->text().toInt();
    display->clear();
    display->insert(QString::number(dispNumber));
}

void CalculatorMainWindow::funcClear()
{
    display->clear();
    display->insert("0");
    resNumber = 0;
    savedNumber = 0;
    standartVals();
    countOfEvents = 0;
}

void CalculatorMainWindow::funcErase()
{
    int tempNum;
    if(dotPressed)
    {
        tempNum = dispNumber * degree / 100;
        dispNumber = static_cast<double>(tempNum) * 100 / degree;
        degree /= 10;
    }
    else
    {
        tempNum = dispNumber / 10;
        dispNumber = tempNum;
    }
    display->clear();
    display->insert(QString::number(dispNumber));
}

void CalculatorMainWindow::funcEqual()
{
    if(operation_ptr == nullptr)
        return;
    resNumber = (this->*operation_ptr)();
    savedNumber = 0;
    standartVals();
    countOfEvents = 2;
    display->clear();
    histDisplay->clear();
    histDisplay->insert(QString::number(resNumber));
}

double CalculatorMainWindow::op_plus()
{
    if(savedNumber != 0  && (dispNumber == 0 || resNumber == 0))
    {
        if(dispNumber != 0)
            return savedNumber + dispNumber;
        return resNumber + savedNumber;
    }
    return resNumber + dispNumber;

}

double CalculatorMainWindow::op_mult()
{
    if(savedNumber != 0 && (dispNumber == 0 || resNumber == 0))
    {
        if(dispNumber != 0)
            return savedNumber * dispNumber;
        return resNumber * savedNumber;
    }
    return resNumber * dispNumber;
    /*if(savedNumber ==0 || dispNumber != 0)
        return resNumber * dispNumber;
    if(countOfEvents > 1)
        return resNumber * savedNumber;
    return savedNumber * dispNumber;*/
}

double CalculatorMainWindow::op_devot()
{
    if(savedNumber != 0 && (dispNumber == 0 || resNumber == 0))
    {
        if(dispNumber != 0)
            return dispNumber == 0 ? 0: savedNumber / dispNumber;
        return resNumber / savedNumber;
    }
    return dispNumber == 0 ? 0: resNumber / dispNumber;
}

double CalculatorMainWindow::op_minus()
{
    if(savedNumber != 0 && (dispNumber == 0 || resNumber == 0))
    {
        if(dispNumber != 0)
            return savedNumber - dispNumber;
        return resNumber - savedNumber;
    }
    return resNumber - dispNumber;
}

CalculatorMainWindow::~CalculatorMainWindow(){}
