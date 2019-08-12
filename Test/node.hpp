#ifndef NODE_HPP
#define NODE_HPP

class NODE
{
	public:
		
		phy::vec3 location;
		
		vector< pair<int, int> > neighbor;
		
		NODE();
};

#endif
