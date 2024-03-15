#include "calculatormainwindow.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QSizePolicy>
#include <QFont>


CalculatorMainWindow::CalculatorMainWindow(QWidget *parent)
    : QWidget(parent)
{
    isUnOpPrev=false;
    zeros = true;
    isButtonPressed = true;
    nums.push(0);
    standartVals();
    resize(300,400);
    numOfDigits = 10;
    funcs.push_back(&CalculatorMainWindow::digitAnalys);
    funcs.push_back(&CalculatorMainWindow::binOperations);
    funcs.push_back(&CalculatorMainWindow::unOperations);
    setWindowTitle("Simple Calculator");
    createWidgets();
}

void CalculatorMainWindow::standartVals()
{
    degree = 10;
    dotPressed = false;
    operation_ptr = nullptr;
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

void CalculatorMainWindow::digitFunc(int num, int temp)
{
    nums.top() = ((nums.top() * temp) * 10 + num) * temp;
}

void CalculatorMainWindow::dotDigitFunc(double num, int temp)
{
    nums.top() = ((nums.top() * temp) + num / degree)* temp;
    degree *=10;
}

void CalculatorMainWindow::digitAnalys(QPushButton *button)
{
    unOpCleaner();
    isButtonPressed = true;
    int temp = 1;
    if(nums.top() < 0)
        temp = -1;
    if(dotPressed)
        dotDigitFunc(button->text().toInt(),temp);
    else
        digitFunc(button->text().toInt(), temp);
    display->clear();
    display->insert(QString::number(nums.top()));
}

void CalculatorMainWindow::solveExample(QPushButton *button)
{
    if(!isButtonPressed)
    {
        histDisplay->backspace();
        histDisplay->insert(button->text());
    }
    else
    {
        if(isUnOpPrev)
        {
            histDisplay->insert(button->text());
            isUnOpPrev = false;
        }
        else
        {
            if(nums.top() >= 0)
                histDisplay->insert(display->text() + button->text());
            else
                histDisplay->insert('(' + display->text() + ')' + button->text());
        }
        nums.push((this->*operation_ptr)());
        display->clear();
        display->insert(QString::number(nums.top()));
        nums.push(0);
        standartVals();
    }
}

void CalculatorMainWindow::moveToNextNum(QPushButton *button)
{
    if(isUnOpPrev)
    {
        histDisplay->insert(button->text());
        isUnOpPrev = false;
    }
    else
    {
        if(nums.top() >= 0)
            histDisplay->insert(display->text() + button->text());
        else
            histDisplay->insert('(' + display->text() + ')' + button->text());
    }
    isButtonPressed = false;
    nums.push(0);
    standartVals();
}

void CalculatorMainWindow::binOperations(QPushButton *button)
{    
    if(nums.size() == 2)    
        solveExample(button);
    else if(nums.size() == 1)    
        moveToNextNum(button);


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
        {            
            funcSqrt();
            isUnOpPrev = true;
        }
        break;
    }
}

void CalculatorMainWindow::funcSqrt()
{
    unOpCleaner();
    histDisplay->insert("√(" + display->text() +')');
    nums.top() = sqrt(display->text().toDouble());
    display->clear();
    display->insert(QString::number(nums.top()));
}

void CalculatorMainWindow::funcPlusMinus()
{
    nums.top() = -1 * display->text().toDouble();
    display->clear();
    display->insert(QString::number(nums.top()));
}

void CalculatorMainWindow::funcClear()
{
    display->clear();
    histDisplay->clear();
    display->insert("0");
    nums = std::stack<double>();
    nums.push(0);
    standartVals();
}

void CalculatorMainWindow::funcErase()
{
    int tempNum;
    unOpCleaner();
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
    display->clear();
    display->insert(QString::number(nums.top()));
}

void CalculatorMainWindow::funcEqual()
{
    if(operation_ptr == nullptr)
        return;
    nums.push((this->*operation_ptr)());
    display->clear();
    histDisplay->clear();
    display->insert(QString::number(nums.top()));
    histDisplay->insert(QString::number(nums.top()));
    isUnOpPrev = true;
    standartVals();
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
