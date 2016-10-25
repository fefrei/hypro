/**
 * Your story.
 */

#include "../src/hypro/representations/SupportFunction/SupportFunctionContent.h"
#include "../src/hypro/representations/Polytopes/HPolytope/HPolytope.h"
//#include "../src/hypro/datastructures/Halfspace.h"

using namespace hypro;

int main(int argc, char const *argv[])
{
	typedef double Number;

	Halfspace<Number> hp1({1,0},2);
	Halfspace<Number> hp2({0,1},2);
	Halfspace<Number> hp3({-1,0},2);
	Halfspace<Number> hp4({0,-1},2);

	std::vector<Halfspace<Number>> planes;
	planes.push_back(hp1);
	planes.push_back(hp2);
	planes.push_back(hp3);
	planes.push_back(hp4);

	HPolytope<Number> poly1(planes);

	std::cout << poly1 << std::endl;

	std::shared_ptr<SupportFunctionContent<Number>> sf1 = SupportFunctionContent<Number>::create(SF_TYPE::POLY, poly1.matrix(), poly1.vector());

	return 0;
}