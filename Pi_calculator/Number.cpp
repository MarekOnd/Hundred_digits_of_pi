#include "Number.h"

Number::Number()
{
	start = new Digit(0, nullptr, nullptr);
	end = start;
	decimalPosition = 1;
	positive = true;
}

int Number::getDigitOnPosition(double number, int position)
{
	if (position == 0)
	{
		double floored = floor(number);
		return (int)(floored - floor(floored / 10) * 10);
	}
	else
	{
		return getDigitOnPosition(number * pow(10, -position), 0);
	}
}

Number::Number(double num)
{
	start = nullptr;
	end = nullptr;
	decimalPosition = 0;
	if (num >= 0) // decides +-
	{
		positive = true;
	}
	else
	{
		positive = false;
		num = -num;
	}
	addToEnd(getDigitOnPosition(num, 0)); // firstly adds digit on 10^0
	for (int i = 1; i <= 10; i++) // loads 10 digits to the left and to the right
	{
		int digitRight = getDigitOnPosition(num,i);
		addToEnd(digitRight);
		int digitLeft = getDigitOnPosition(num, -i);
		addToStart(digitLeft);
	}
	addToStart(0);
	addToEnd(0);
	cutEnds();
}

Number::Number(const Number& num)
{
	start = nullptr;
	end = nullptr;
	Digit* tmp = num.start;
	while (tmp)
	{
		addToEnd(tmp->digit);
		tmp = tmp->next;
	}
	decimalPosition = num.decimalPosition;
	positive = num.positive;
}

Number::~Number()
{

	clear();
}

// output_______________________________
void Number::print()
{
	cout << toString() << endl;
}

string Number::toString()
{
	Digit* tmp = end;
	string s = "";
	
	int counter = size() - 1 - decimalPosition;// counts to zero, where the decimal dot should be

	if (!positive)// writes - or nothing for +
	{
		s += '-';
	}
	while (tmp)
	{
		s += to_string(tmp->digit);// adds digit
		if (counter == 0)// adds decimal dot
		{
			s += ',';
		}
		counter--;
		tmp = tmp->previous;
	}
	return s;
}

// info_______________________________
int Number::size()
{
	if (start == nullptr)//if number doesnt contain anything
	{
		return 0;
	}
	Digit* tmp = start;
	int counter = 1;
	while (tmp->next)
	{
		counter++;
		tmp = tmp->next;
	}
	return counter;
}

bool Number::isSmallerThanPrecision(int byDecimalPlaces)
{
	Number precis = Number(1);
	precis.multByTenTo(-(precision+byDecimalPlaces));
	if (*this < precis)
	{
		return true;
	}
	return false;
}

// add remove_______________________________
void Number::addToStart(Digit* dig)
{
	if (start == nullptr)// empty number case
	{
		start = dig;
		end = dig;
		dig->next = nullptr;
		dig->previous = nullptr;
		decimalPosition++;
		return;
	}
	dig->next = start;
	dig->previous = nullptr;
	start = dig;
	decimalPosition++;
}

void Number::addToStart(int dig)
{
	addToStart(new Digit(dig, nullptr, nullptr));
}

void Number::addToEnd(Digit* dig)
{
	if (start == nullptr)// empty number case
	{
		start = dig;
		end = dig;
		dig->next = nullptr;
		dig->previous = nullptr;
		return;
	}
	end->next = dig;
	dig->next = nullptr;
	dig->previous = end;
	end = dig;
}

void Number::addToEnd(int dig)
{
	addToEnd(new Digit(dig, nullptr, nullptr));
}

void Number::clear()
{
	while (start)// deletes first pointer while there are pointers
	{
		Digit* tmp = start;
		start = start->next;
		if (start)// sets the previous pointer for the digit it went to to nullptr
		{
			start->previous = nullptr;
		}
		delete tmp;
	}
	decimalPosition = 0;
}

void Number::equalize(Number& a, Number& b)
{
	// adds zeros to start
	while (a.decimalPosition < b.decimalPosition)
	{
		a.addToStart(0);
	}
	while (a.decimalPosition > b.decimalPosition)
	{
		b.addToStart(0);
	}
	// adds zeros to end
	int AminusB = a.size() - b.size();// optimalization
	while (AminusB < 0)// means a.size < b.size
	{
		a.addToEnd(0);
		AminusB++;
	}
	while (AminusB > 0)// means a.size > b.size
	{
		b.addToEnd(0);
		AminusB--;
	}
}

void Number::cutEnds()
{
	while (start->digit == 0 && removeFromStart()){}// cuts zeros at start 
	while (end->digit == 0 && removeFromEnd()){}// cuts zeros at end
	while (decimalPosition > precision)
	{
		removeFromStart();
	}
}

void Number::cutEnds(int p)
{
	while (start->digit == 0 && removeFromStart()) {}// cuts zeros at start 
	while (end->digit == 0 && removeFromEnd()) {}// cuts zeros at end
	while (decimalPosition > p)
	{
		removeFromStart();
	}
}

bool Number::removeFromStart()
{
	if (!start || decimalPosition <= 1)// checks conditions when it can be removed
	{
		return false;
	}
	Digit* tmp = start;
	start = start->next;
	decimalPosition--;
	if (!start)// if it was the last number
	{
		end = nullptr;
	}
	else
	{
		start->previous = nullptr;
	}
	delete tmp;
	return true;
}

bool Number::removeFromEnd()
{
	if (!start || decimalPosition + 1 >= size())// checks conditions when it can be removed
	{
		return false;
	}
	Digit* tmp = start;
	while (tmp->next != end)
	{
		tmp = tmp->next;
	}
	delete end;
	tmp->next = nullptr;
	end = tmp;

	return true;
}

Number Number::makeCopy(Number &copy)
{
	copy = *this;
	return copy;
}

// operators_______________________________
void Number::operator=(Number num)
{
	clear();
	Digit* tmp = num.start;
	while(tmp) // goes through num and copies all digits into this number
	{
		addToEnd(tmp->digit);
		tmp = tmp->next;
	}
	decimalPosition = num.decimalPosition;
	positive = num.positive;
}

void Number::operator+=(Number num)
{
	Number copy;
	makeCopy(copy);// saves copy of this number
	equalize(copy, num);
	copy.addToEnd(0);// for adding numbers that increase number of digits (like 8.2 + 9.4 = 17.6 is now 08.2 + 09.4 = 17.6)
	num.addToEnd(0);

	clear();// clears output space

	int additionalNumberToAdd = 0;
	Digit* tmpCopy = copy.start;
	Digit* tmpNum = num.start;
	while (tmpCopy)// goes the digits and adds them together to this class
	{
		int result = tmpCopy->digit + tmpNum->digit + additionalNumberToAdd;
		addToEnd(result%10);

		additionalNumberToAdd = floor(result / 10); 

		tmpCopy = tmpCopy->next;
		tmpNum = tmpNum->next;
	}

	//----------------
	if (copy.positive && num.positive)//both positive
	{
		positive = true;
	}
	else if (copy.positive && !num.positive)//positive negative
	{
		if (copy < num)
		{
			positive = false;
		}
		else
		{
			positive = true;
		}
	}
	else if (!copy.positive && num.positive)//negative positive
	{
		if (copy > num)
		{
			positive = false;
		}
		else
		{
			positive = true;
		}
	}
	else//both negative
	{
		positive = false;
	}
	//----------------
	decimalPosition = copy.decimalPosition;
	cutEnds();
}

void Number::operator-=(Number num)
{
	Number copy;
	makeCopy(copy);// saves copy of this number
	equalize(copy, num);
	copy.addToEnd(0);// for adding numbers that increase number of digits (like 8.2 + 9.4 = 17.6 is now 08.2 + 09.4 = 17.6)
	num.addToEnd(0);
	
	clear();// clears output space
	int additionalNumberToSubtract = 0;
	Digit* tmpCopy = copy.start;
	Digit* tmpNum = num.start;
	while (tmpCopy)// goes the digits and subtracts num from copy
	{
		int result = tmpCopy->digit - tmpNum->digit + additionalNumberToSubtract;
		if (result < 0)
		{
			result += 10;
			additionalNumberToSubtract = -1;
		}
		else
		{
			additionalNumberToSubtract = 0;
		}

		addToEnd(result % 10);

		tmpCopy = tmpCopy->next;
		tmpNum = tmpNum->next;
	}

	//---------------- 
	if (copy.positive && num.positive)//both positive
	{
		if (copy < num)
		{
			positive = false;
		}
		else
		{
			positive = true;
		}
	}
	else if (copy.positive && !num.positive)//positive negative
	{
		positive = true;
	}
	else if (!copy.positive && num.positive)//negative positive
	{
		positive = false;
	}
	else//both negative
	{
		if (copy > num)
		{
			positive = false;
		}
		else
		{
			positive = true;
		}
	}
	//----------------
	decimalPosition = copy.decimalPosition;
	cutEnds();
}

void Number::operator*=(Number num)
{
	Number copy;
	makeCopy(copy);// saves copy of this number
	equalize(copy, num);
	for (int i = copy.size() - copy.decimalPosition; i > 0;i--)//adds double the amount of whole digits because 10^a*10^b = 10^(a+b)
	{
		copy.addToEnd(0);
		num.addToEnd(0);
	}
	for (int i = copy.decimalPosition; i > 0;i--)//adds double the amount of whole digits because 10^a*10^b = 10^(a+b)
	{
		copy.addToStart(0);
		num.addToStart(0);
	}
	clear();// clears output space
	int additionalNumberToAdd = 0;// if multiplied number is bigger than 10
	Digit* tmpCopy = copy.start;
	Number temporaryNumber = Number();
	int decimalPositionCounter = 2*copy.decimalPosition;//when starting at 10^(-decimalposition) the smallest is (10^(-decimalposition))^2
	while (tmpCopy)// multiplication of multiple digit numbers
	{
		Digit* tmpNum = num.start;
		if (tmpCopy->digit == 0)
		{
			decimalPositionCounter--;
			tmpCopy = tmpCopy->next;
			continue;
		}
		temporaryNumber.clear();
		while (tmpNum)
		{
			int result = tmpCopy->digit * tmpNum->digit + additionalNumberToAdd;
			temporaryNumber.addToEnd(result % 10);
			additionalNumberToAdd = (int)floor(result / 10);
			tmpNum = tmpNum->next;//moves in second number
		}
		temporaryNumber.decimalPosition = decimalPositionCounter;
		*this += temporaryNumber;//adds number to the result
		
		decimalPositionCounter--;//decreases by 1 because process moves a digit higher
		tmpCopy = tmpCopy->next;//moves in first number
	}

	//----------------
	if ((copy.positive && !num.positive) || (!copy.positive && num.positive))//one negative
	{
		positive = false;
	}
	else//both negative or positive
	{
		positive = true;
	}
	//----------------
}

void Number::operator/=(Number num)
{
	if (num == Number(0))
	{
		cout << "Dividing by 0" << endl;
		return;
	}
	Number copy;
	makeCopy(copy);
	*this = Number(0);
	//INFO
	// copy = divident
	// num = divisor
	// this = result
	

	//----------------
	if ((copy.positive && !num.positive) || (!copy.positive && num.positive))//one negative
	{
		positive = false;
	}
	else// both negative or positive
	{
		positive = true;
	}
	// negative and positive information is saved
	copy.positive = true;
	num.positive = true;
	//----------------

	int decimalCounter = -1;// counts on which power of division the calculation is

	// decreases divident until divisor doesnt fit in it, 
	// EXAMPLE: when doing 1000/2 moves divident by two to became 10, if 2 fits 1 time in 10 it means it fits 100 times in 1000
	while (copy > num)
	{
		copy.multByTenTo(-1);
		decimalCounter++;
	}
	// then moves back one decimal
	copy.multByTenTo(1);


	while (decimalCounter > - precision) // while in precision
	{
		Number wholeDivision = Number(0);
		while (copy >= num) // finds how many times a divisor can fit into the divident
		{
			copy -= num;
			wholeDivision += 1;
		}
		// adds to result
		wholeDivision.multByTenTo(decimalCounter);
		*this += wholeDivision;
		// moves a decimal place
		copy.multByTenTo(1);
		decimalCounter--;
		
	}
	cutEnds();// formats result
}

Number Number::operator+(Number num)
{
	Number a;
	makeCopy(a);
	a += num;
	return a;
}

Number Number::operator-(Number num)
{
	Number a;
	makeCopy(a);
	a -= num;
	return a;
}

Number Number::operator*(Number num)
{
	Number a;
	makeCopy(a);
	a *= num;
	return a;
}

Number Number::operator/(Number num)
{
	Number a;
	makeCopy(a);
	a /= num;
	return a;
}

bool Number::operator==(Number num)
{
	Number copy;
	makeCopy(copy);
	equalize(copy, num);
	Digit* tmpCopy = copy.end; // goes from the end
	Digit* tmpNum = num.end;
	while (tmpCopy && tmpNum)
	{
		if (tmpCopy->digit != tmpNum->digit) // numbers dont have same digit on some position => cant be equal
		{
			return false;
		}
		tmpCopy = tmpCopy->previous;
		tmpNum = tmpNum->previous;
	}
	return true;
}

bool Number::operator>(Number num)
{
	Number copy;
	makeCopy(copy);
	equalize(copy, num);
	Digit* tmpCopy = copy.end; // goes from the end
	Digit* tmpNum= num.end;
	while (tmpCopy && tmpNum)
	{
		if (tmpCopy->digit > tmpNum->digit) // left number is bigger
		{
			return true;
		}
		else if (tmpCopy->digit == tmpNum->digit) // left number can still be bigger
		{
			tmpCopy = tmpCopy->previous;
			tmpNum = tmpNum->previous;
			continue;
		}
		else // left number can no longer be bigger
		{
			return false;
		}
	}
	return false;
}

bool Number::operator>=(Number num)
{
	if (*this > num || *this == num)
	{
		return true;
	}
	return false;
}

bool Number::operator<(Number num)
{
	if (num > *this)
	{
		return true;
	}
	return false;
}

bool Number::operator<=(Number num)
{
	if (*this < num || *this == num)
	{
		return true;
	}
	return false;
}

Number Number::operator^(int n)
{
	Number a;
	makeCopy(a);
	for (int i = 0; i < n - 1; i++)
	{
		a *= *this;
	}
	return a;
}

void Number::multByTenTo(int power)
{
	if (power < 0) // divides by ten
	{
		for (int i = 0; i < -power; i++)
		{
			addToEnd(0);
			decimalPosition++;
		}
	}
	else if (power > 0) // multiplies by ten
	{
		for (int i = 0; i < power; i++)
		{
			addToStart(0);
			decimalPosition--; // addToStart adds a decimal, this moves it back
		}
	}

}




