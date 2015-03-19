#include "../src/lib/config.h"
#include "../src/lib/representations/Polytope/Polytope.h"
#include "../src/lib/datastructures/Point.h"
#include "../src/lib/datastructures/Facet.h"
#include "../src/lib/datastructures/Ridge.h"
#include "../src/lib/representations/Polytope/convexHull_util.h"



using namespace hypro;

int main(int argc, char** argv) {
	std::cout << __func__ << " : " << __LINE__ << std::endl;
	vector_t<double> v01 = vector_t<double>(3);
	v01(0) = 0;
	v01(1) = 0;
	v01(2) = 0;
	Point<double> p01 = Point<double>(v01);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	vector_t<double> v02 = vector_t<double>(3);
	v02(0) = 0;
	v02(1) = 2;
	v02(2) = 0;
	Point<double> p02 = Point<double>(v02);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	vector_t<double> v03 = vector_t<double>(3);
	v03(0) = 2;
	v03(1) = 0;
	v03(2) = 0;
	Point<double> p03 = Point<double>(v03);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	vector_t<double> v04 = vector_t<double>(3);
	v04(0) = 0;
	v04(1) = 0;
	v04(2) = 2;
	Point<double> p04= Point<double>(v04);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	vector_t<double> v05 = vector_t<double>(3);
	v05(0) = 2;
	v05(1) = 2;
	v05(2) = 0;
	Point<double> p05 = Point<double>(v05);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	vector_t<double> v06 = vector_t<double>(3);
	v06(0) = 2;
	v06(1) = 0;
	v06(2) = 2;
	Point<double> p06 = Point<double>(v06);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	vector_t<double> v07 = vector_t<double>(3);
	v07(0) = 0;
	v07(1) = 2;
	v07(2) = 2;
	Point<double> p07 = Point<double>(v07);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	vector_t<double> v08 = vector_t<double>(3);
	v08(0) = 2;
	v08(1) = 2;
	v08(2) = 2;
	Point<double> p08 = Point<double>(v08);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	vector_t<double> v09 = vector_t<double>(3);
	v09(0) = 1;
	v09(1) = 1;
	v09(2) = 1;
	Point<double> p09 = Point<double>(v09);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	vector_t<double> v10 = vector_t<double>(3);
	v10(0) = 1;
	v10(1) = 3;
	v10(2) = 1;
	Point<double> p10 = Point<double>(v10);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	std::vector<Point<double>> points;
	points.push_back(p01);
	points.push_back(p02);
	points.push_back(p03);
	points.push_back(p04);
	points.push_back(p05);
	points.push_back(p06);
	points.push_back(p07);
	points.push_back(p08);
	points.push_back(p09);
	points.push_back(p10);

	std::cout << __func__ << " : " << __LINE__ << " : " << v08 << p08 << std::endl;

	vector_t<double> b = vector_t<double>::Zero(3);

	std::cout << __func__ << " : " << __LINE__ << " : " << b <<std::endl;

	matrix_t<double> matrix = matrix_t<double>(3,3);
		for(unsigned i = 0; i < 3; i++) {
	    	for(unsigned j = 0; j < 3; j++) {
	    		matrix(i,j) = i+j;
	    	}
		}

	std::cout << __func__ << " : " << __LINE__ << " : " << matrix << std::endl;

	std::cout << __func__ << " : " << __LINE__ << " : " << matrix.fullPivHouseholderQr().solve(b) << std::endl;

	std::cout << __func__ << " : " << __LINE__ << " : " << v09*v10 << std::endl;

	std::cout << __func__ << " : " << __LINE__ << " : " << v09.dot(v10) << std::endl;

	std::vector<double> zahlen;

	for(double i = 0; i<10; i++){
		zahlen.push_back(i);
	}

	std::cout << __func__ << " : " << __LINE__ << " : " << zahlen << std::endl;

	for(double z:zahlen){
		zahlen.erase(z);
	}

	std::cout << __func__ << " : " << __LINE__ << " : " << zahlen << std::endl;

	//std::vector<Facet<double>> facets = Polytope<double>::convexHull(points);

	std::cout << __func__ << " : " << __LINE__ << std::endl;

	for(unsigned i = 0; i<facets.size(); i++){
		std::cout << facets.at(i) << std::endl;
	}

	std::cout << __func__ << " : " << __LINE__ << std::endl;
}
