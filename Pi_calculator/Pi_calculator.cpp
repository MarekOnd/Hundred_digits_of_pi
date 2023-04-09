// Pi_calculator.cpp
//

using namespace std;

#include <iostream>
#include <fstream>
#include "Number.h"

// PI for reference
const string PI = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989380952572010654858632788659361533818279682303019520353018529689957736225994138912497217752834791315155748572424541506959508295331168617278558890750983817546374649393192550604009277016711390098488240128583616035637076601047101819429555961989467678374494482553797747268471040475346462080466842590694912933136770289891521047521620569660240580381501935";


// functions and variables not required to calculate pi are marked with @

Number calculatePi(int numOfDecimalPlaces); // estimates the degree of Taylor polynomials for this precision and calls second calculatePi function
Number calculatePi(int firstDegree, int secondDegree); // calculates pi for input degrees of arctan Taylor polynomials
Number arctan(Number x, int taylorDegree); // return value of Taylor polynomial for arctan(x)

Number lagrangeRemainderOfArctan(int n, Number x); // calculates the estimation from above of Lagrange remainder of arctan
Number remainderOfPi(int firstDegree, int secondDegree); // calculates the mistake of the identity pi is calculated from

Number factorial(int fac, bool showProgress = false); // @ test function


int main()
{

    int numberOfDecPlaces;
    cout << "______________Pi calculator______________" << "\n";
    cout << "How many decimal places of pi do you want to calculate: ";
    cin >> numberOfDecPlaces;
    while (cin.fail() == true || numberOfDecPlaces < 0) // input correction
    {
        if (cin.fail() == true)
        {
            cout << "!!Input in the wrong format" << endl;
        }
        else
        {
            cout << "!!Input must be a positive number" << endl;
        }
        cin.clear();
        cin.ignore(256, '\n');
        cout << "How many decimal places of pi do you want to calculate: ";
        cin >> numberOfDecPlaces;
    }

    Number myPi = calculatePi(numberOfDecPlaces); 
    myPi.cutEnds(numberOfDecPlaces); // outputs only digits program is sure about
    cout << "The first " << numberOfDecPlaces << " digits of pi: " << myPi.toString() << "\n";


    // saving to file
    ofstream stream = ofstream("piCalculatorOutput.txt");
    stream << "Pi_calculator" << "\n";
    stream << "The first " << numberOfDecPlaces << " digits of pi on the first line and pi for reference on the second" << "\n";
    stream << myPi.toString() << "\n";
    stream << PI << "\n";
    cout << "Pi was saved to file piCalculatorOutput.txt" << "\n";
    system("pause");


    //Number numbers[] = // @ test numbers
    //{
    //    Number(3.14159),   //(0)
    //    Number(3.14),      //(1)
    //    Number(-541.16544),//(2)
    //    Number(5.0124),    //(3)
    //    Number(007.420),   //(4)
    //    Number(98.8988),   //(5)
    //    Number(.45),       //(6)
    //    Number(6.241),     //(7)
    //    Number(1),         //(8)
    //};
    
    

    
}

Number calculatePi(int numOfDecimalPlaces)
{
    Number firstNum = Number(1) / Number(5);
    Number secondNum = Number(1) / Number(239);
    //lagrange remainders
    int firstDegree = 1;
    int secondDegree = 1;
    Number firstRemainder = firstNum;
    Number secondRemainder = secondNum;
    Number targetRemainder = Number(1);
    targetRemainder.multByTenTo(-numOfDecimalPlaces);
    while (firstRemainder >= targetRemainder)
    {

        firstRemainder *= firstNum;
        firstRemainder *= firstDegree;
        firstDegree++;
        firstRemainder /= firstDegree;
    }
    while (secondRemainder >= targetRemainder)
    {

        secondRemainder *= secondNum;
        secondRemainder *= secondDegree;
        secondDegree++;
        secondRemainder /= secondDegree;
    }
    cout << "Taylor degrees have been set to " << firstDegree << " and " << secondDegree << "\n";
    cout << "Calculating pi..." << "\n";
    return calculatePi(firstDegree, secondDegree);
}

Number calculatePi(int firstDegree, int secondDegree)
{
    Number firstNum = Number(1) / Number(5);
    Number secondNum = Number(1) / Number(239);
    Number pi = Number(16) * arctan(firstNum, firstDegree) - Number(4) * arctan(secondNum, secondDegree);
    return pi;
}

Number arctan(Number x, int taylorDegree)
{
    Number result = Number(0);
    for (int i = 0; i < taylorDegree; i++)
    {
        Number toadd = x ^ (2 * i + 1);
        toadd /= Number(2 * i + 1);
        if (i % 2 == 0)
        {
            result += toadd;

        }
        else
        {
            result -= toadd;
        }

    }
    return result;
}

Number lagrangeRemainderOfArctan(int n, Number x)
{
    Number result = (Number(x) ^ (n + 1));
    result /= (Number(n + 1));
    return result;
}

Number remainderOfPi(int firstDegree, int secondDegree)
{
    Number firstNum = Number(1) / Number(5);
    Number secondNum = Number(1) / Number(239);
    Number firstArctanMistake = lagrangeRemainderOfArctan(firstDegree, firstNum);
    Number secondArctanMistake = lagrangeRemainderOfArctan(secondDegree, secondNum);
    Number finalMistake = firstArctanMistake * Number(16) + secondArctanMistake * Number(4);
    return finalMistake;
}

Number factorial(int fac, bool showProgress)
{
    Number result = Number(1);
    for (int i = 1; i <= fac; i++)
    {
        result *= Number(i);
        if (showProgress && i % 1 == 0)
        {
            cout << (float)i / fac << endl;
        }
    }
    return result;
}