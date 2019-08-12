#ifndef MAP_HPP
#define MAP_HPP

class MAP
{
	private:
		
		vector<NODE> nodes;
		vector<CUSTOMER> customers;
		vector<CAR> cars;
		int numOfRoads;
		
		int dist[10001];
		int previous[10001];
		vector<NODE> path;
		
	public:
		
		MAP();
		
		void makePathTable(int);
		void findPath(int, int);
		
		void addCustomer();
		void addCar();
};

#endif
