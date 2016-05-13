/*
 * @file   HPolytope.h
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 *
 * @since 	2014-07-16
 * @version 	2015-03-16
 */

#pragma once

#include "../../../util/convexHull.h"
//#include "../../../util/vertexEnumeration.h"
#include "../../../util/Optimizer.h"
#include "../../../util/Permutator.h"

#include <algorithm>
#include <cassert>

#define REDUCE_NUMBERS
//#define DEBUG_MSG

namespace hypro {

template <typename Number, typename Converter>
class HPolytopeT {
public:
  	enum REDUCTION_STRATEGY {
                              DROP = 0,
                              DROP_SMOOTH,
                              UNITE,
                              UNITE_SMOOTH,
                              UNITE_CUT,
                              UNITE_NORM,
                              DIRECTED_SMALL,
                              DIRECTED_BIG,
                              DIRECTED_TEMPLATE
                            };

	using HalfspaceVector = std::vector<Halfspace<Number>>;

  private:
	mutable HalfspaceVector mHPlanes;
	unsigned mDimension;

	// State flags
	mutable TRIBOOL mEmpty;
	mutable bool mNonRedundant;


  public:
  	/**
  	 * @brief Constructor for the universal H-polytope.
  	 */
	HPolytopeT();

	/**
	 * @brief Copy constructor.
	 *
	 * @param orig Original H-polytope.
	 */
	HPolytopeT( const HPolytopeT& orig );

	/**
	 * @brief Constructor from a vector of halfspaces.
	 * @details The resulting object is the intersection of the given halfspaces, i.e. the conjunction of the linear
	 * constraints representing the halfspaces.
	 *
	 * @param planes A vector of halfspaces.
	 */
	HPolytopeT( const HalfspaceVector& planes );

	/**
	 * @brief Constructor from a matrix and a vector.
	 * @details Each row in the matrix is considered to represent a constraint, i.e. a normal to a bounding hyperplane.
	 * Each corresponding entry in the passed vector is the offset of that plane, i.e. the constant part of the linear
	 * constraint.
	 *
	 * @param A A matrix.
	 * @param b A vector.
	 */
	HPolytopeT( const matrix_t<Number>& A, const vector_t<Number>& b );

	/**
	 * @brief Constructor from a matrix.
	 * @details Each row in the matrix is considered to represent a constraint, i.e. a normal to a bounding hyperplane.
	 * The offset of each plane is considered to be 0, i.e. if the number of affinely independent constraints
	 * is larger or equal to d+1, the resulting polytope represents the origin point.
	 *
	 * @param A A matrix.
	 */
	HPolytopeT( const matrix_t<Number>& A );

	/**
	 * @brief Constructor from a vector of points.
	 * @details Converts a vertex-representation into a hyperplanar representation, i.e. solving the plane enumeration
	 * problem.
	 *
	 * @param points A vector of points.
	 */
	HPolytopeT( const std::vector<Point<Number>>& points );

	/**
	 * @brief Destructor.
	 */
	~HPolytopeT();

	/*
	 * Getters and setters
	 */

	double sizeOfHPolytopeT(){
		return sizeof(*this) + this->mHPlanes.size()*this->mHPlanes.at(0).sizeOfHalfspace();
	}

	/**
	 * @brief Determines, if the polytope is empty.
	 * @details Checks for satisfiability of the given constraints.
	 * @return True, if the polytope is empty.
	 */
	bool empty() const;

	/**
	 * @brief Static method for construction of an empty H-polytope.
	 *
	 * @return An empty polytope.
	 */
	static HPolytopeT<Number, Converter> Empty();

	/**
	 * @brief Getter for the dimension of the polytope.
	 * @details Note that this only returns the space dimension and not the effective dimension of the polytope.
	 * @return The space dimension.
	 */
	std::size_t dimension() const;

	/**
	 * @brief Getter for the number of constraints.
	 * @return The number of constraints.
	 */
	std::size_t size() const;

	/**
	 * @brief Getter for the matrix representation of the constraints.
	 * @return A matrix.
	 */
	matrix_t<Number> matrix() const;

	/**
	 * @brief Getter for the vector of offsets of the bounding hyperplanes.
	 * @return A vector.
	 */
	vector_t<Number> vector() const;

	/**
	 * @brief Getter for the full description of the polytope as a matrix and a vector.
	 * @return A pair of a matrix and a vector.
	 */
	std::pair<matrix_t<Number>, vector_t<Number>> inequalities() const;

	///**
	// * @brief Getter for the polytopal fan of the current polytope.
	// * @return A fan.
	// */
	//const typename polytope::Fan<Number>& fan() const;

	/**
	 * @brief Getter for the vertices of the current polytope
	 * @details Solves the vertex enumeration problem, i.e. converts the hyperplanar description into a vertex description.
	 * @return A vector of points.
	 */
	typename std::vector<Point<Number>> vertices() const;

	/**
	 * @brief Getter for a number representing the supremum of the polytope.
	 * @return A number.
	 */
	Number supremum() const;

	/**
	 * @brief Inserts an additional bounding halfspace into the polytope.
	 *
	 * @param plane The plane to add.
	 */
	void insert( const Halfspace<Number>& plane );


	/**
	 * @brief Inserts a range of halfspaces into the current polytope.
	 *
	 * @param iterator Iterator pointing to the start of the range.
	 * @param iterator Iterator pointing to the end of the range.
	 */
	void insert( const typename HalfspaceVector::iterator begin, const typename HalfspaceVector::iterator end );

  	void erase( const unsigned index);

	const HalfspaceVector& constraints() const;
	bool hasConstraint( const Halfspace<Number>& hplane ) const;
	void removeRedundancy();

	HPolytopeT<Number, Converter> heuristic() const;
	HPolytopeT<Number, Converter> reduce(unsigned facet=1, unsigned facet2=0, REDUCTION_STRATEGY strat = REDUCTION_STRATEGY::DROP) const;
	HPolytopeT<Number, Converter> reduce_directed(std::vector<vector_t<Number>> directions, REDUCTION_STRATEGY strat = REDUCTION_STRATEGY::DIRECTED_SMALL) const;
	void reduceAssign(unsigned _steps = 1, REDUCTION_STRATEGY strat = REDUCTION_STRATEGY::DROP);

	bool isBounded(std::vector<vector_t<Number>>) const;
	//static std::vector<vector_t<Number>> computeTemplate(unsigned dimension, unsigned polytope);

	bool isExtremePoint( const vector_t<Number>& point ) const;
	bool isExtremePoint( const Point<Number>& point ) const;
	EvaluationResult<Number> evaluate( const vector_t<Number>& _direction ) const;

	std::vector<Point<Number>> vertexEnumeration() const;

	/*
	 * General interface
	 */

	std::pair<bool, HPolytopeT> satisfiesHalfspace( const vector_t<Number>& normal, const Number& offset ) const;
	std::pair<bool, HPolytopeT> satisfiesHalfspaces( const matrix_t<Number>& _mat, const vector_t<Number>& _vec ) const;
	HPolytopeT linearTransformation( const matrix_t<Number>& A, const vector_t<Number>& b ) const;
	HPolytopeT minkowskiSum( const HPolytopeT& rhs ) const;
	HPolytopeT intersect( const HPolytopeT& rhs ) const;
	HPolytopeT intersectHalfspace( const Halfspace<Number>& rhs ) const;
	HPolytopeT intersectHalfspaces( const matrix_t<Number>& _mat, const vector_t<Number>& _vec ) const;
	bool contains( const Point<Number>& point ) const;
	bool contains( const vector_t<Number>& vec ) const;
	bool contains( const HPolytopeT<Number, Converter>& rhs ) const;
	HPolytopeT unite( const HPolytopeT& rhs ) const;

	void clear();
	void print() const;

	/*
	 * Operators
	 */

	//const Halfspace<Number>& operator[]( size_t i ) const;
	//Halfspace<Number>& operator[]( size_t i ) ;
	HPolytopeT& operator=( const HPolytopeT<Number, Converter>& rhs );

	friend std::ostream& operator<<( std::ostream& lhs, const HPolytopeT<Number, Converter>& rhs ) {
		if ( rhs.constraints().size() > 0 ) {
			lhs << "[ ";
			for ( unsigned i = 0; i < rhs.constraints().size() - 1; ++i ) {
				lhs << rhs.constraints()[i] << "," << std::endl;
			}
			lhs << rhs.constraints()[rhs.constraints().size() - 1] << " ]";
		}
		return lhs;
	}

	friend void swap( HPolytopeT<Number, Converter>& a, HPolytopeT<Number, Converter>& b ) {
		unsigned tmpDim = a.mDimension;
		a.mDimension = b.mDimension;
		b.mDimension = tmpDim;
		swap( a.mHPlanes, b.mHPlanes );
	}

	void reduceNumberRepresentation(unsigned limit = fReach_DENOMINATOR) const;
  private:
	/*
	 * Auxiliary functions
	 */

	//void calculateFan() const;

	std::vector<std::vector<unsigned>> getMembersOfVertices(std::vector<Point<Number>> vertices) const;
	std::vector<unsigned> getNeighborsOfIndex(unsigned facet, std::vector<std::vector<unsigned>> membersOfvertices) const;

	std::vector<Point<Number>> getVerticesOfIndex(unsigned a, std::vector<Point<Number>> vertices, std::vector<std::vector<unsigned>> membersOfvertices) const;
	std::vector<Point<Number>> getVerticesOf2Indices(unsigned a, unsigned b, std::vector<Point<Number>> vertices, std::vector<std::vector<unsigned>> membersOfvertices) const;
	std::vector<Point<Number>> getVerticesOf2IndicesAround(unsigned a, unsigned b, std::vector<Point<Number>> vertices, std::vector<std::vector<unsigned>> membersOfvertices) const;
	std::vector<std::vector<vector_t<Number>>> getVerticesPermutationForFacet(unsigned a, unsigned b, std::vector<Point<Number>> vertices) const;

	vector_t<Number> computeNormal(std::vector<vector_t<Number>> vertices, vector_t<Number> check) const;

	Point<Number> getVertexForVector(vector_t<Number> vector, std::vector<Point<Number>> vertices) const;

};

}  // namespace

#include "HPolytope.tpp"
#include "reduction.tpp"


