#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

#include <QDebug>

#define LABEL_PRECISION 12
#define LABEL_PRINT_MODE 'g'

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    clearOnNextNumericInput(false),
    arithm_operation_confirmed(false),
    ui(new Ui::MainWindow)
{
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);

    // Buttons [0-9]
    connect(ui->pushButton_0,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_1,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_2,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_4,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_5,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_6,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_7,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_8,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_9,SIGNAL(released()),this,SLOT(digit_pressed()));

    // Buttons unary
    connect(ui->pushButton_clear,SIGNAL(released()),this,SLOT(clearLabel()));
    connect(ui->pushButton_plusMinus,SIGNAL(released()),this,SLOT(unary_operation_pressed()));
    connect(ui->pushButton_percent,SIGNAL(released()),this,SLOT(unary_operation_pressed()));

    // Buttons arythmetic operations
    connect(ui->pushButton_divide,SIGNAL(released()),this,SLOT(binary_operations_pressed()));
    connect(ui->pushButton_equals,SIGNAL(released()),this,SLOT(binary_operations_pressed()));
    connect(ui->pushButton_multiply,SIGNAL(released()),this,SLOT(binary_operations_pressed()));
    connect(ui->pushButton_add,SIGNAL(released()),this,SLOT(binary_operations_pressed()));
    connect(ui->pushButton_subtract,SIGNAL(released()),this,SLOT(binary_operations_pressed()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digit_pressed()
{
    QPushButton * button = (QPushButton*)sender();
    QString newLabel;
    double labelNumber;

    if(this->clearOnNextNumericInput)
        ui->label->setText("0");

    labelNumber = (ui->label->text() + button->text()).toDouble();

    newLabel = QString::number(labelNumber,LABEL_PRINT_MODE,LABEL_PRECISION);

    if(newLabel.size() < LABEL_PRECISION)
        ui->label->setText(newLabel);
    this->clearOnNextNumericInput = false;
    this->arithm_operation_confirmed = true;
    this->CALU.input2.isset = false;
}

void MainWindow::clearLabel()
{
    ui->label->setText("0");
    this->clearOnNextNumericInput = false;

    // Clears first input field
    this->CALU.input1.value = 0;
    this->CALU.input1.isset = false;

    // Clears second input field
    this->CALU.input2.value = 0;
    this->CALU.input2.isset = false;

    // Clears operation field
    this->CALU.op.type = CALU_NULL;

}

void MainWindow::on_pushButton_dot_released()
{
    if(!ui->label->text().contains("."))
        ui->label->setText(ui->label->text() + ".");
}

void MainWindow::unary_operation_pressed()
{
       QPushButton* button = (QPushButton*) sender();
       double labelNumber = ui->label->text().toDouble();

       if(button->text() == "+/-")
       {
           labelNumber *= -1;

           QString newLabel = QString::number(labelNumber,LABEL_PRINT_MODE,LABEL_PRECISION);
           ui->label->setText(newLabel);
       }
       else if(button->text() == '%')
       {
            labelNumber *= 0.01;

            QString newLabel = QString::number(labelNumber,LABEL_PRINT_MODE,LABEL_PRECISION);
            ui->label->setText(newLabel);
       }
}

void MainWindow::binary_operations_pressed()
{
    QPushButton* button = (QPushButton*) sender();

    if(button->text() == '/')
    {
        if(this->CALU.input1.isset && this->CALU.op.type
                && !this->CALU.input2.isset && this->arithm_operation_confirmed)
        {
            // Doing operation in background and then restroes old state of ALU
            // Used when user inputs multiple artihemtic functions in one line
            this->CALU.input2.value = ui->label->text().toDouble();
            this->CALU.op.doOperation(this->CALU.input1,this->CALU.input2);
            this->CALU.input2.value = 0;
            this->CALU.input2.isset = false;

            // Printing result of background operation to label
            QString newLabel = QString::number(this->CALU.input1.value,LABEL_PRINT_MODE,LABEL_PRECISION);
            ui->label->setText(newLabel);

            // Standard setting. Clears label when next
            // argument is being inserted
            this->clearOnNextNumericInput = true;

            // Prevents from making operation on recent result
            this->arithm_operation_confirmed = false;

            this->CALU.op.type = CALU_DIVIDE;
            return;
        }

        if(this->arithm_operation_confirmed)
        {
            this->CALU.input1.value = ui->label->text().toDouble();
            this->CALU.input1.isset = true;
        }

        this->CALU.op.type = CALU_DIVIDE;
        this->clearOnNextNumericInput = true;
        this->arithm_operation_confirmed = false;
    }

    if(button->text() == 'X')
    {
        if(this->CALU.input1.isset && this->CALU.op.type
                && !this->CALU.input2.isset && this->arithm_operation_confirmed)
        {
            // Doing operation in background and then restroes old state of ALU
            // Used when user inputs multiple artihemtic functions in one line
            this->CALU.input2.value = ui->label->text().toDouble();
            this->CALU.op.doOperation(this->CALU.input1,this->CALU.input2);
            this->CALU.input2.value = 0;
            this->CALU.input2.isset = false;

            // Printing result of background operation to label
            QString newLabel = QString::number(this->CALU.input1.value,LABEL_PRINT_MODE,LABEL_PRECISION);
            ui->label->setText(newLabel);

            // Standard setting. Clears label when next
            // argument is being inserted
            this->clearOnNextNumericInput = true;

            // Prevents from making operation on recent result
            this->arithm_operation_confirmed = false;

            this->CALU.op.type = CALU_MULTIPLY;
            return;
        }

        if(this->arithm_operation_confirmed)
        {
            this->CALU.input1.value = ui->label->text().toDouble();
            this->CALU.input1.isset = true;
        }

        this->CALU.op.type = CALU_MULTIPLY;
        this->clearOnNextNumericInput = true;
        this->arithm_operation_confirmed = false;
    }

    if(button->text() == '+')
    {
        if(this->CALU.input1.isset && this->CALU.op.type
                && !this->CALU.input2.isset && this->arithm_operation_confirmed)
        {
            // Doing operation in background and then restroes old state of ALU
            // Used when user inputs multiple artihemtic functions in one line
            this->CALU.input2.value = ui->label->text().toDouble();
            this->CALU.op.doOperation(this->CALU.input1,this->CALU.input2);
            this->CALU.input2.value = 0;
            this->CALU.input2.isset = false;

            // Printing result of background operation to label
            QString newLabel = QString::number(this->CALU.input1.value,LABEL_PRINT_MODE,LABEL_PRECISION);
            ui->label->setText(newLabel);

            // Standard setting. Clears label when next
            // argument is being inserted
            this->clearOnNextNumericInput = true;

            // Prevents from making operation on recent result
            this->arithm_operation_confirmed = false;

            this->CALU.op.type = CALU_PLUS;
            return;
        }

        if(this->arithm_operation_confirmed)
        {
            this->CALU.input1.value = ui->label->text().toDouble();
            this->CALU.input1.isset = true;
        }

        this->CALU.op.type = CALU_PLUS;
        this->clearOnNextNumericInput = true;
        this->arithm_operation_confirmed = false;
    }

    if(button->text() == '-')
    {
        if(this->CALU.input1.isset && this->CALU.op.type
                && !this->CALU.input2.isset && this->arithm_operation_confirmed)
        {
            // Doing operation in background and then restroes old state of ALU
            // Used when user inputs multiple artihemtic functions in one line
            this->CALU.input2.value = ui->label->text().toDouble();
            this->CALU.op.doOperation(this->CALU.input1,this->CALU.input2);
            this->CALU.input2.value = 0;
            this->CALU.input2.isset = false;

            // Printing result of background operation to label
            QString newLabel = QString::number(this->CALU.input1.value,LABEL_PRINT_MODE,LABEL_PRECISION);
            ui->label->setText(newLabel);

            // Standard setting. Clears label when next
            // argument is being inserted
            this->clearOnNextNumericInput = true;

            // Prevents from making operation on recent result
            this->arithm_operation_confirmed = false;

            this->CALU.op.type = CALU_MINUS;
            return;
        }

        if(this->arithm_operation_confirmed)
        {
            this->CALU.input1.value = ui->label->text().toDouble();
            this->CALU.input1.isset = true;
        }

        this->CALU.op.type = CALU_MINUS;
        this->clearOnNextNumericInput = true;
        this->arithm_operation_confirmed = false;
    }

    if(button->text() == '=')
    {
        if(!this->CALU.input2.isset)
            // Copy existing input into input2 of customALU
            this->CALU.input2.value = ui->label->text().toDouble();

        // Make operation on data that is stored in input1 and input2
        this->CALU.op.doOperation(this->CALU.input1,this->CALU.input2);

        // Returned value of operation from customALU
        double resultL = this->CALU.input1.value;

        // Needed when user clicks another artihemtic operation
        // before clicking '=' sign
        this->CALU.input1.isset = true;

        // Making result of operation visible in label
        ui->label->setText(QString::number(resultL,LABEL_PRINT_MODE,LABEL_PRECISION));

        this->clearOnNextNumericInput = true;
        this->CALU.input2.isset = true;
    }

}


void Operation::doOperation(Input &input1, Input &input2)
{
    if ( this->type == CALU_DIVIDE )
    {
        // IMPLEMENT OF DIVIDING BY 0 IS NEEDED !!!
        input1.value = input1 / input2;
        input2.isset = false;
    }
    else if(this->type == CALU_MULTIPLY)
    {
        input1.value = input1 * input2;
        input2.isset = false;
    }
    else if(this->type == CALU_MINUS)
    {
        input1.value = input1 - input2;
        input2.isset = false;
    }
    else if(this->type == CALU_PLUS)
    {
        input1.value = input1 + input2;
        input2.isset = false;
    }
}

Input::Input()
:value(0),isset(false) {}

// Overloading operators for arithmetic operations
double Input::operator /(Input& input)
{
    return this->value / input.value;
}

double Input::operator *(Input& input)
{
    return this->value * input.value;
}

double Input::operator +(Input& input)
{
    return this->value + input.value;
}

double Input::operator -(Input& input)
{
    return this->value - input.value;
}

CustomALU::CustomALU()
{
    this->op.type = CALU_NULL;
}
