#include "calculatormainwindow.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QSizePolicy>
#include <QFont>


CalculatorMainWindow::CalculatorMainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Simple Calculator");

    configVals();
    createWidgets();

    configOperations();
    engineMapCongig();
    drawingMapCongig();
}

void CalculatorMainWindow::configVals()
{
    dotFlag = false;
    state = "";
    isUnOpPrev=false;
    zeros = true;
    isButtonPressed = true;
    nums.push(0);
    standartVals();
    operation_ptr = nullptr;
    resize(300,400);
    numOfDigits = 10;
}

void CalculatorMainWindow::standartVals()
{
    degree = 10;
    dotPressed = false;
    dotFlag = false;
}

void CalculatorMainWindow::unOpCleaner()
{
    if(isUnOpPrev)
    {
        nums = std::stack<double>();
        nums.push(0);
        histDisplay->clear();
        isUnOpPrev = false;
    }
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
    lCalcLayout->addWidget ( pushButtonEqual , 6 , 3 ) ;
    lCalcLayout->addWidget ( pushButtonPlus , 5 , 3  ) ;
    lCalcLayout->addWidget ( pushButtonMult , 3 , 3  ) ;
    lCalcLayout->addWidget ( pushButtonDevot , 2 , 3  ) ;
    lCalcLayout->addWidget ( pushButtonMinus , 4 , 3  ) ;
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
        numButtons[i]->setObjectName(QString::number(i));

    pushButtonPlus->setObjectName("plus");
    pushButtonMult->setObjectName("mult");
    pushButtonDevot->setObjectName("devotion");
    pushButtonMinus->setObjectName("minus");
    pushButtonC->setObjectName("clear");
    pushButtonEqual->setObjectName("equal");
    pushButtonSqrt->setObjectName("sqrt");
    pushButtonDot->setObjectName("dot");
    pushButtonErase->setObjectName("erase");
    pushButtonPlusMinus->setObjectName("plus_minus");
}

void CalculatorMainWindow::configOperations()
{
    operations[pushButtonPlus->objectName()] = "+";
    operations[pushButtonMinus->objectName()] = "-";
    operations[pushButtonMult->objectName()] = "*";
    operations[pushButtonDevot->objectName()] = "/";

    operations[pushButtonSqrt->objectName()] = "√";
}

void CalculatorMainWindow::engineMapCongig()
{
    for (int i = 0; i < numOfDigits; ++i)
        engineFuncs[numButtons[i]->objectName()] = &CalculatorMainWindow::digitAnalys;

    engineFuncs[pushButtonPlus->objectName()] = &CalculatorMainWindow::funcPlus;
    engineFuncs[pushButtonMinus->objectName()] = &CalculatorMainWindow::funcMinus;
    engineFuncs[pushButtonMult->objectName()] = &CalculatorMainWindow::funcMult;
    engineFuncs[pushButtonDevot->objectName()] = &CalculatorMainWindow::funcDevot;

    engineFuncs[pushButtonC->objectName()] = &CalculatorMainWindow::funcClear;
    engineFuncs[pushButtonEqual->objectName()] = &CalculatorMainWindow::funcEqual;
    engineFuncs[pushButtonErase->objectName()] = &CalculatorMainWindow::funcErase;

    engineFuncs[pushButtonSqrt->objectName()] = &CalculatorMainWindow::funcSqrt;
    engineFuncs[pushButtonDot->objectName()] = &CalculatorMainWindow::funcDot;
    engineFuncs[pushButtonPlusMinus->objectName()] = &CalculatorMainWindow::funcPlusMinus;
}

void CalculatorMainWindow::drawingMapCongig()
{
    for (int i = 0; i < numOfDigits; ++i)
        drawingFuncs[numButtons[i]->objectName()] = &CalculatorMainWindow::digitsDraw;

    drawingFuncs[pushButtonPlus->objectName()] = &CalculatorMainWindow::binOperationsDraw;
    drawingFuncs[pushButtonMinus->objectName()] = &CalculatorMainWindow::binOperationsDraw;
    drawingFuncs[pushButtonMult->objectName()] = &CalculatorMainWindow::binOperationsDraw;
    drawingFuncs[pushButtonDevot->objectName()] = &CalculatorMainWindow::binOperationsDraw;

    drawingFuncs[pushButtonC->objectName()] = &CalculatorMainWindow::drawFuncClear;
    drawingFuncs[pushButtonEqual->objectName()] = &CalculatorMainWindow::drawFuncEqual;
    drawingFuncs[pushButtonErase->objectName()] = &CalculatorMainWindow::drawFuncErase;

    drawingFuncs[pushButtonSqrt->objectName()] = &CalculatorMainWindow::drawFuncSqrt;
    drawingFuncs[pushButtonDot->objectName()] = &CalculatorMainWindow::drawFuncDot;
    drawingFuncs[pushButtonPlusMinus->objectName()] = &CalculatorMainWindow::drawFuncPlusMinus;
}

void CalculatorMainWindow::slotEngine()
{

    QObject * button = qobject_cast<QObject *>(sender());
    state = button->objectName();
    (this->*(engineFuncs[state]))();
    (this->*(drawingFuncs[state]))();
}

void CalculatorMainWindow::binOperationsDraw()
{
    if(!isButtonPressed)
    {
        histDisplay->backspace();
        histDisplay->insert(operations[state]);
    }
    else
    {
        if(isUnOpPrev)
        {
            histDisplay->insert(operations[state]);
            isUnOpPrev = false;
        }
        else
        {
            if(nums.top() >= 0)
                histDisplay->insert(display->text() + operations[state]);
            else
                histDisplay->insert('(' + display->text() + ')' + operations[state]);
        }
        display->clear();
        display->insert(QString::number(nums.top()));
        nums.push(0);
        standartVals();
        isButtonPressed = false;
    }
}

void CalculatorMainWindow::digitsDraw()
{
    unOpCleaner();
    display->clear();
    display->insert(QString::number(nums.top()));
}

void CalculatorMainWindow::drawFuncClear()
{
    display->clear();
    histDisplay->clear();
    display->insert("0");
}

void CalculatorMainWindow::drawFuncEqual()
{
    display->clear();
    histDisplay->clear();
    display->insert(QString::number(nums.top()));
    histDisplay->insert(QString::number(nums.top()));
    isUnOpPrev = true;
}

void CalculatorMainWindow::drawFuncErase()
{
    unOpCleaner();
    display->clear();
    display->insert(QString::number(nums.top()));
}

void CalculatorMainWindow::drawFuncPlusMinus()
{
    display->clear();
    display->insert(QString::number(nums.top()));
}

void CalculatorMainWindow::drawFuncSqrt()
{
    if(isUnOpPrev)
        histDisplay->clear();
    histDisplay->insert(operations[state] + "(" + display->text() +')');
    display->clear();
    display->insert(QString::number(nums.top()));
    unOpCleaner();
    isUnOpPrev = true;
}

void CalculatorMainWindow::drawFuncDot()
{
    if(dotFlag)
        display->insert(".");
}


void CalculatorMainWindow::digitFunc(int num, int temp)
{
    nums.top() = ((nums.top() * temp) * 10 + num) * temp;
}

void CalculatorMainWindow::dotDigitFunc(double num, int temp)
{
    nums.top() = ((nums.top() * temp) + num / degree)* temp;
    degree *=10;
}

void CalculatorMainWindow::digitAnalys()
{

    isButtonPressed = true;
    int temp = 1;
    if(nums.top() < 0)
        temp = -1;
    if(dotPressed)
        dotDigitFunc(state.toInt(),temp);
    else
        digitFunc(state.toInt(), temp);
}

void CalculatorMainWindow::isNeedSolv()
{
    if(nums.size() == 2)
        if(isButtonPressed)
            nums.push((this->*operation_ptr)());
}

void CalculatorMainWindow::funcPlus()
{
    isNeedSolv();
    operation_ptr = &CalculatorMainWindow::op_plus;
}

void CalculatorMainWindow::funcMinus()
{
    isNeedSolv();
    operation_ptr = &CalculatorMainWindow::op_minus;
}

void CalculatorMainWindow::funcMult()
{
    isNeedSolv();
    operation_ptr = &CalculatorMainWindow::op_mult;
}

void CalculatorMainWindow::funcDevot()
{
    isNeedSolv();
    operation_ptr = &CalculatorMainWindow::op_devot;
}

void CalculatorMainWindow::funcDot()
{
    if(!dotPressed)
        dotFlag = true;
    else
        dotFlag = false;
    dotPressed = true;
}

void CalculatorMainWindow::funcSqrt()
{
    nums.top() = sqrt(display->text().toDouble());    
}

void CalculatorMainWindow::funcPlusMinus()
{
    nums.top() = -1 * display->text().toDouble();
}

void CalculatorMainWindow::funcClear()
{    
    nums = std::stack<double>();
    nums.push(0);
    standartVals();
    operation_ptr = nullptr;
}

void CalculatorMainWindow::funcErase()
{
    int tempNum;
    if(dotPressed)
    {
        tempNum = nums.top() * degree / 100;
        nums.top() = static_cast<double>(tempNum) * 100 / degree;
        degree /= 10;
    }
    else
    {
        tempNum = nums.top() / 10;
        nums.top() = tempNum;
    }
}

void CalculatorMainWindow::funcEqual()
{
    if(operation_ptr == nullptr)
        return;
    nums.push((this->*operation_ptr)());
    standartVals();
    operation_ptr = nullptr;
}

double CalculatorMainWindow::op_plus()
{
    double secondNum{nums.top()};
    nums.pop();
    double firstNum{nums.top()};
    nums.pop();
    return firstNum + secondNum;

}

double CalculatorMainWindow::op_mult()
{
    double secondNum{nums.top()};
    nums.pop();
    double firstNum{nums.top()};
    nums.pop();
    return firstNum * secondNum;
}

double CalculatorMainWindow::op_devot()
{
    double secondNum{nums.top()};
    nums.pop();
    double firstNum{nums.top()};
    nums.pop();
    return secondNum == 0 ? 0: firstNum / secondNum;
}

double CalculatorMainWindow::op_minus()
{
    double secondNum{nums.top()};
    nums.pop();
    double firstNum{nums.top()};
    nums.pop();
    return firstNum - secondNum;
}

CalculatorMainWindow::~CalculatorMainWindow(){}
