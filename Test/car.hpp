#ifndef CAR_HPP
#define CAR_HPP

class CAR
{
	private:
		
		CUSTOMER* customer;
		
		vector<NODE> path;
		float maxSpeed;
		
	public:
		
		phy::PhysicsObj phyObj;
		
		CAR(phy::vec3, float);
		
		void addCustomer(CUSTOMER*);
		void removeCustomer();
		
		bool isCloseEnough(phy::vec3); //How close is close enough
		
		void move_Path(float);
		void move(float);
};

CAR::CAR(phy::vec3 initLocation, float speed) : phyObj(1, initLocation, phy::vec3(0, 0, 0))
{
	customer = NULL;
	path.clear();
	this->speed = speed;
}

void CAR::addCustomer(CUSTOMER* customer, vector<int> path)
{
	this->customer = customer;
	this->path = path;
}

void CAR::deleteCustomer()
{
	customer = NULL;
	path.clear;
}

bool CAR::isCloseEnough(phy::vec3 location)
{
	return (phyObj.COM - location).calcMag() < 0.5);
}

void CAR::move_Path(float timeInterval)
{
	if (isCloseEnough(path[0].location))
	{
		phyObj.COM = path[0].location;
		path.erase(path.begin());
		phyObj.vel = phy::vec3(0, 0, 0);
		
		if (path.size() != 0)
		{
			phy::vec3 newDir = (path[0].location - phyObj.COM).normalize();
		
			phyObj.vel = newDir * maxSpeed;	//Add slowing down and speeding up
		}
	}
	
	phyObj.updateMembers(timeInterval);
}

void CAR::move(float timeInterval)
{	

	if (customer == NULL)
	{
		return;
	}
	
	if (!customer.inCar)
	{
		if (isCloseEnough(customer.getSource().location))
		{
			phyObj.COM = customer.getSource().location;
			path.erase(path.begin());
			phyObj.vel = phy::vec3(0, 0, 0);
			
			path = customer.path;
			customer.inCar = true;
		}
		
		move_Path(timeInterval);
		return;
	}
	
	if (path.size() == 0)
	{
		customer.reached = true;
		customer = NULL;
		return;
	}
	
	if (customer.inCar)
	{
		move_Path(timeInterval);
		return;
	}
}

#endif
