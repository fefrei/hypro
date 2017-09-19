/*
 * @file HPolytope.h
 * \example example_HPolytope.cpp
 */

#pragma once

#ifndef INCL_FROM_GOHEADER
	static_assert(false, "This file may only be included indirectly by GeometricObject.h");
#endif

#include "../../../util/convexHull.h"
#include "../../../util/Permutator.h"
#include "../../../util/pca.h"
#include "../../../util/templateDirections.h"
#include "../../../util/linearOptimization/Optimizer.h"
#include "../../../algorithms/convexHull/ConvexHull.h"

#include <algorithm>
#include <cassert>

#define REDUCE_NUMBERS
#define AVOID_CONVERSION
//#define HPOLY_DEBUG_MSG

namespace hypro {

template<typename Number>
class Location;

/**
 * @brief      Class for a H-polytope.
 * @tparam     Number     The used number type.
 * @tparam     Converter  The used converter.
 * \ingroup geoState @{
 */
template <typename Number, typename Converter>
class HPolytopeT : public GeometricObject<Number, HPolytopeT<Number,Converter>> {
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
	std::size_t mDimension;

	// State flags
	mutable TRIBOOL mEmpty = TRIBOOL::NSET;
	mutable bool mNonRedundant;


  public:
  	/**
  	 * @brief Constructor for the universal H-polytope.
  	 */
	HPolytopeT();

	/**
	 * @brief Copy constructor.
	 * @param orig Original H-polytope.
	 */
	HPolytopeT( const HPolytopeT& orig ) = default;

	/**
	 * @brief Constructor from a vector of halfspaces.
	 * @details The resulting object is the intersection of the given halfspaces, i.e. the conjunction of the linear
	 * constraints representing the halfspaces.
	 * @param planes A vector of halfspaces.
	 */
	HPolytopeT( const HalfspaceVector& planes );

	/**
	 * @brief Constructor from a matrix and a vector.
	 * @details Each row in the matrix is considered to represent a constraint, i.e. a normal to a bounding hyperplane.
	 * Each corresponding entry in the passed vector is the offset of that plane, i.e. the constant part of the linear
	 * constraint.
	 * @param A A matrix.
	 * @param b A vector.
	 */
	HPolytopeT( const matrix_t<Number>& A, const vector_t<Number>& b );

	/**
	 * @brief Constructor from a matrix.
	 * @details Each row in the matrix is considered to represent a constraint, i.e. a normal to a bounding hyperplane.
	 * The offset of each plane is considered to be 0, i.e. if the number of affinely independent constraints
	 * is larger or equal to d+1, the resulting polytope represents the origin point.
	 * @param A A matrix.
	 */
	HPolytopeT( const matrix_t<Number>& A );

	/**
	 * @brief Constructor from a vector of points.
	 * @details Converts a vertex-representation into a hyperplanar representation, i.e. solving the plane enumeration
	 * problem.
	 * @param points A vector of points.
	 */
	HPolytopeT( const std::vector<Point<Number>>& points );

	/**
	 * @brief Destructor.
	 */
	~HPolytopeT();

	/**
	 * @brief Approximates the storage usage of the current polytope.
	 * @details [long description]
	 * @return The size.
	 */
	double sizeOfHPolytopeT(){
		return sizeof(*this) + this->mHPlanes.size()*this->mHPlanes.at(0).sizeOfHalfspace();
	}

	/**
	 * @brief Determines, if the polytope is empty.
	 * @details Checks for satisfiability of the given constraints.
	 * @return True if the polytope is empty, false otherwise.
	 */
	bool empty() const;

	/**
	 * @brief Static method for construction of an empty H-polytope.
	 * @details An empty H-polytope is constructed by creating two hyperplanes which falsify each other.
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

	static representation_name type() { return representation_name::polytope_h; }

	/**
	 * @brief Resets the information if the current polytope contains redundant constraints.
	 * @details This function may be used to allow a manual call to the redundancy removal method.
	 */
	void unreduce() const { mNonRedundant = false;}

	/**
	 * @brief Manually sets the polytope to behave as if it did not contain redundant constraints.
	 * @details This function can avoid an expensive redundancy removal call in case the user already knows that this property holds.
	 */
	void setReduced() const { mNonRedundant = true; }

	/**
	 * @brief Returns, whether the current polytope contains redundant constraints.
	 * @details Only reads the stored flag without.
	 * @return True, if there are no redundant constraints, false otherwise.
	 */
	bool isNonRedundant() const { return mNonRedundant; }

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
	 * @brief Getter for the full description of the polytope as a pair of a matrix and a vector.
	 * @return A pair of a matrix and a vector.
	 */
	std::pair<matrix_t<Number>, vector_t<Number>> inequalities() const;

	/**
	 * @brief Getter for the vertices of the current polytope
	 * @details Solves the vertex enumeration problem, i.e. converts the hyperplanar description into a vertex description.
	 * @return A vector of points.
	 */
	std::vector<Point<Number>> vertices( const matrix_t<Number>& = matrix_t<Number>::Zero(0,0) ) const;

	/**
	 * @brief Getter for a number representing the supremum according to the infinity norm of the polytope.
	 * @return A number.
	 */
	Number supremum() const;

	/**
	 * @brief Inserts an additional bounding halfspace into the polytope.
	 * @param plane The plane to add.
	 */
	void insert( const Halfspace<Number>& plane );


	/**
	 * @brief Inserts a range of halfspaces into the current polytope.
	 * @param iterator Iterator pointing to the start of the range.
	 * @param iterator Iterator pointing to the end of the range.
	 */
	void insert( const typename HalfspaceVector::iterator begin, const typename HalfspaceVector::iterator end );

	/**
	 * @brief Removes the constraint at position index.
	 * @param index An index.
	 */
  	void erase( const unsigned index);

	const HalfspaceVector& constraints() const;
	bool hasConstraint( const Halfspace<Number>& hplane ) const;
	const HPolytopeT<Number,Converter>& removeRedundancy();

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

	std::pair<bool, HPolytopeT> satisfiesHalfspace( const Halfspace<Number>& rhs ) const;
	std::pair<bool, HPolytopeT> satisfiesHalfspaces( const matrix_t<Number>& _mat, const vector_t<Number>& _vec ) const;
	HPolytopeT project(const std::vector<std::size_t>& dimensions) const;
	HPolytopeT linearTransformation( const matrix_t<Number>& A ) const;
	HPolytopeT affineTransformation( const matrix_t<Number>& A, const vector_t<Number>& b ) const;
	HPolytopeT minkowskiSum( const HPolytopeT& rhs ) const;
	HPolytopeT intersect( const HPolytopeT& rhs ) const;
	HPolytopeT intersectHalfspace( const Halfspace<Number>& rhs ) const;
	HPolytopeT intersectHalfspaces( const matrix_t<Number>& _mat, const vector_t<Number>& _vec ) const;
	bool contains( const Point<Number>& point ) const;
	bool contains( const vector_t<Number>& vec ) const;
	bool contains( const HPolytopeT<Number, Converter>& rhs ) const;
	HPolytopeT unite( const HPolytopeT& rhs ) const;
	static HPolytopeT unite( const std::vector<HPolytopeT>& rhs );

	void clear();
	void print() const;

	/*
	 * Operators
	 */
	HPolytopeT& operator=( const HPolytopeT<Number, Converter>& rhs ) = default;

#ifdef HYPRO_LOGGING
	friend std::ostream& operator<<( std::ostream& lhs, const HPolytopeT<Number, Converter>& rhs ) {
		if ( rhs.constraints().size() > 0 ) {
			lhs << "[ ";
			for ( unsigned i = 0; i < rhs.constraints().size() - 1; ++i ) {
				lhs << convert<Number,double>(rhs.constraints()[i]) << "," << std::endl;
			}
			lhs << convert<Number,double>(rhs.constraints()[rhs.constraints().size() - 1]) << " ]";
		}
#else
	friend std::ostream& operator<<( std::ostream& lhs, const HPolytopeT<Number, Converter>& ) {
#endif
		return lhs;
	}

	bool operator==(const HPolytopeT<Number,Converter>& rhs) const {
		if(this->dimension() != rhs.dimension()) {
			return false;
		}

		return this->constraints() == rhs.constraints();
	}

	friend void swap( HPolytopeT<Number, Converter>& a, HPolytopeT<Number, Converter>& b ) {
		std::size_t tmpDim = a.mDimension;
		a.mDimension = b.mDimension;
		b.mDimension = tmpDim;
		swap( a.mHPlanes, b.mHPlanes );
	}

	template<typename N = Number, carl::DisableIf< std::is_same<N, double> > = carl::dummy>
	void reduceNumberRepresentation(const std::vector<Point<Number>>& _vertices = std::vector<Point<Number>>(), unsigned limit = fReach_DENOMINATOR) const {
		#ifdef REDUCE_NUMBERS
		TRACE("hypro.hPolytope","Attempt to reduce numbers.");
		std::vector<Point<Number>> originalVertices;
		if(_vertices.empty()) {
			TRACE("hypro.hPolytope","No passed vertices, computed vertices.");
			originalVertices = this->vertices();
		} else {
			TRACE("hypro.hPolytope","Use passed vertices.");
			originalVertices = _vertices;
		}
		TRACE("hypro.hPolytope","Vertices empty: " << originalVertices.empty());

		if(!this->empty()){
			// normal reduction
			for(unsigned planeIndex = 0; planeIndex < mHPlanes.size(); ++planeIndex){
				#ifdef HPOLY_DEBUG_MSG
				std::cout << "Original: " << mHPlanes.at(planeIndex) << std::endl;
				#endif
				// find maximal value
				Number largest = Number(0);
				mHPlanes.at(planeIndex).makeInteger();
				#ifdef HPOLY_DEBUG_MSG
				std::cout << "As Integer: " << mHPlanes.at(planeIndex) << std::endl;
				#endif
				largest = carl::abs(mHPlanes.at(planeIndex).offset());
				for(unsigned i = 0; i < mDimension; ++i){
					if(carl::abs(mHPlanes.at(planeIndex).normal()(i)) > largest){
						largest = carl::abs(mHPlanes.at(planeIndex).normal()(i));
					}
				}

				// reduce, if reduction is required
				if(largest > (limit*limit)) {
					#ifdef HPOLY_DEBUG_MSG
					std::cout << "Actual reduction" << std::endl;
					#endif
					vector_t<Number> newNormal(mDimension);
					for(unsigned i = 0; i < mDimension; ++i){
						newNormal(i) = carl::floor(Number((mHPlanes.at(planeIndex).normal()(i)/largest)*Number(limit)));
						assert(carl::abs(Number(mHPlanes.at(planeIndex).normal()(i)/largest)) <= Number(1));
						assert(carl::isInteger(newNormal(i)));
						assert(newNormal(i) <= Number(limit));
					}
					mHPlanes.at(planeIndex).setNormal(newNormal);
					Number newOffset = mHPlanes.at(planeIndex).offset();
					newOffset = carl::ceil(Number((newOffset/largest)*Number(limit)));
					for(const auto& vertex : originalVertices) {
						Number tmp = newNormal.dot(vertex.rawCoordinates());
						if(tmp > newOffset){
							newOffset = newOffset + (tmp-newOffset);
							assert(newNormal.dot(vertex.rawCoordinates()) <= newOffset);
						}
						assert(Halfspace<Number>(newNormal,newOffset).contains(vertex));
					}
					newOffset = carl::ceil(newOffset);
					#ifdef HPOLY_DEBUG_MSG
					std::cout << "Reduced to " << convert<Number,double>(newNormal).transpose() << " <= " << carl::toDouble(newOffset) << std::endl;
					#endif
					mHPlanes.at(planeIndex).setOffset(newOffset);
				}
				#ifdef HPOLY_DEBUG_MSG
				std::cout << "Reduced: " << mHPlanes.at(planeIndex) << std::endl;
				#endif
			}
			#ifdef HPOLY_DEBUG_MSG
			//std::cout << "After Reduction: " << *this << std::endl;
			#endif
		}
		#endif
	}

	template<typename N = Number, carl::EnableIf< std::is_same<N, double> > = carl::dummy>
	void reduceNumberRepresentation(const std::vector<Point<double>>& = std::vector<Point<double>>(), unsigned = fReach_DENOMINATOR) const {}

  private:
	/*
	 * Auxiliary functions
	 */

	//void calculateFan() const;

	/*
     * Computes a set of constraints that correpsonds to the convex hull of the points
	 * @param points The set of points. Note that auxilarry points might be added to this vector.
	 * @param degeneratedDimensions the number of dimensions in which the convex hull of the points is degenerated, i.e., spaceDimension - effectiveDimension
	 */
	HalfspaceVector computeConstraintsForDegeneratedPolytope(std::vector<Point<Number>>& points, unsigned degeneratedDimensions) const;

	std::vector<std::vector<unsigned>> getMembersOfVertices(std::vector<Point<Number>> vertices) const;
	std::vector<unsigned> getNeighborsOfIndex(unsigned facet, std::vector<std::vector<unsigned>> membersOfvertices) const;

	std::vector<Point<Number>> getVerticesOfIndex(unsigned a, std::vector<Point<Number>> vertices, std::vector<std::vector<unsigned>> membersOfvertices) const;
	std::vector<Point<Number>> getVerticesOf2Indices(unsigned a, unsigned b, std::vector<Point<Number>> vertices, std::vector<std::vector<unsigned>> membersOfvertices) const;
	std::vector<Point<Number>> getVerticesOf2IndicesAround(unsigned a, unsigned b, std::vector<Point<Number>> vertices, std::vector<std::vector<unsigned>> membersOfvertices) const;
	std::vector<std::vector<vector_t<Number>>> getVerticesPermutationForFacet(unsigned a, unsigned b, std::vector<Point<Number>> vertices) const;

	vector_t<Number> computeNormal(std::vector<vector_t<Number>> vertices, vector_t<Number> check) const;

	Point<Number> getVertexForVector(vector_t<Number> vector, std::vector<Point<Number>> vertices) const;

	/**
	 * @brief      Extends the constraint to a higher dimension. This effectively unbounds the polytope in that direction. The passed set of
	 * existing dimensions is used to allow adding dimensions in between. Note that both vectors have to be ordered ascending.
	 * @param[in]  existingDimensions  The existing dimensions.
	 * @param[in]  newDimensions       The new dimensions.
	 */
	void insertEmptyDimensions(const std::vector<std::size_t>& existingDimensions, const std::vector<std::size_t>& newDimensions);

};
/** @} */

template<typename From, typename To, typename Converter>
HPolytopeT<To,Converter> convert(const HPolytopeT<From,Converter>& in) {
	return HPolytopeT<To,Converter>(convert<From,To>(in.matrix()), convert<From,To>(in.vector()));
}

}  // namespace hypro

#include "HPolytope.tpp"
#include "reduction.tpp"


