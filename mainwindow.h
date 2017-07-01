#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Input
{
public:
    double value;
    bool isset;
    Input();

    double operator /(Input&);
    double operator *(Input&);
    double operator +(Input&);
    double operator -(Input&);

};

class Operation
{
    #define CALU_NULL 0
    #define CALU_DIVIDE 1
    #define CALU_MULTIPLY 2
    #define CALU_MINUS 3
    #define CALU_PLUS 4

public:
    int type;

    // Doing binary operation on two inputs given by reference
    // Returning result in input1 (first argument)
    // Setting input2 (second argument) isset to false
    void doOperation(Input&, Input&);
};

class CustomALU
{
public:
    Input input1;
    Input input2;
    Operation op;

    CustomALU();

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Virtual ALU that is used to perform artihmetic operations
    // Contains two double fields for data and int field for specifing operation
    // that will be performed on given data
    // Example:
    // Input1 = 20, Input2 = 13, op = ADD
    CustomALU CALU;

    // Used to determine whether to clean label on next input or not
    // Set to true when user press binary operation button
    bool clearOnNextNumericInput;

    // Next numeric input confirms choice of arithmetic function
    // This let user to change next arithmetic function if
    // second input haven't been given
    // This mechanism is needed when user tries to click
    // more than once on arithemtic function button
    // Prevents from dividing by number in label (recent result)
    // before giving second argument to ALU
    bool arithm_operation_confirmed;


private slots:
    void digit_pressed();
    void clearLabel();
    void on_pushButton_dot_released();
    void unary_operation_pressed();
    void binary_operations_pressed();
};


#endif // MAINWINDOW_H
