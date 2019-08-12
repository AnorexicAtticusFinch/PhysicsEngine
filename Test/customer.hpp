#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

class CUSTOMER
{
	private:
		
		NODE source;
		NODE destination;
				
	public:
		
		bool inCar;
		bool reached;
		vector<NODE> path;
		
		CUSTOMER(int, int);
		
		NODE getSource();
};

CUSTOMER::CUSTOMER(NODE source, NODE destination)
{
	this->source = source;
	this->destination = destination;
	inCar = false;
	reached = false;
}

NODE CUSTOMER::getSource()
{
	return source;
}

#endif
