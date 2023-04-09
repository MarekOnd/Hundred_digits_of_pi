#pragma once
using namespace std;
#include <math.h>
#include <iostream>
#include <string>
#include <string.h>

struct Digit
{
	short unsigned int digit; // only 0-9 used
	Digit* next;
	Digit* previous;

	Digit(int d, Digit* n, Digit* p)
	{
		digit = d;
		next = n;
		previous = p;
	}

	~Digit()
	{
		
	}
};


class Number
{
private:
	Digit* start; // the first digit is the lowest rank (= digit furthest after the decimal point)
	Digit* end; // last is the highest rank
	int decimalPosition; // place where is decimal point located
	static const int precision = 110; // max number of decimal places each number can have
	bool positive; // +-

public:
	Number();
	Number(double num);// gets digits in num and adds them one by one to this number
					   // IMPORTANT: takes only ranks from 10^10 to 10^-10 because double stores values as fraction so its not precise anymore 
					   //			 its best to input values with not many decimal places
	int getDigitOnPosition(double number, int position);
	Number(const Number& num);
	~Number();

	// output_______________________________
	string toString();// return number as a string
	void print();// prints number on a seperate line
	// info_______________________________
	int size();// goes through the pointer and counts them
	bool isSmallerThanPrecision(int byDecimalPlaces = 0); // creates a number smaller than precision and compares it with this

	// add remove
	void addToStart(Digit* dig);
	void addToStart(int dig);
	void addToEnd(Digit* dig);
	void addToEnd(int dig);
	void clear(); // deletes all digits
	void cutEnds(); // deletes unnecessary zeros at the ends and digits after precision
	void cutEnds(int p); // deletes all digits after 10^(-p)
	void equalize(Number& a, Number& b); // add zeros to the end of the numbers 'a', 'b' so they align with their rank in equations
	bool removeFromStart(); // tries to remove first digit, return true if it there is another digit to remove
	bool removeFromEnd(); // tries to remove last digit, return true if it there is another digit to remove
	Number makeCopy(Number& copy); // takes this number and copies it in 'copy'


	// operators_______________________________
	void operator=(Number num);
	void operator+=(Number num);
	void operator-=(Number num);
	void operator*=(Number num);
	void operator/=(Number num);
	Number operator+(Number num);
	Number operator-(Number num);
	Number operator*(Number num);
	Number operator/(Number num);
	bool operator==(Number num);
	bool operator>(Number num);
	bool operator>=(Number num);
	bool operator<(Number num);
	bool operator<=(Number num);
	Number operator^(int n);

	void multByTenTo(int power);// faster multiplication by 10



};



