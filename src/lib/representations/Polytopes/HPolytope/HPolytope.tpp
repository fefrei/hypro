#include "HPolytope.h"

namespace hypro {
template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT()
	: mHPlanes(), mDimension( 0 ), mEmpty(TRIBOOL::NSET), mNonRedundant(true) {
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT( const HalfspaceVector &planes )
	: mHPlanes(), mDimension( 0 ), mEmpty(TRIBOOL::NSET), mNonRedundant(false) {
	//std::cout << __func__ << ": construct from planes." << std::endl;
	if ( !planes.empty() ) {
		mDimension = planes.begin()->dimension();
		for ( const auto &plane : planes ) {
			mHPlanes.push_back( plane );
		}
		reduceNumberRepresentation();
	}
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT( const matrix_t<Number> &A, const vector_t<Number> &b )
	: mHPlanes(), mDimension( A.cols() ), mEmpty(TRIBOOL::NSET), mNonRedundant(false) {
	assert( A.rows() == b.rows() );
	for ( unsigned i = 0; i < A.rows(); ++i ) {
		mHPlanes.emplace_back( A.row( i ), b( i ) );
	}
	//std::cout << "Constructor from matrix, vector, size: " << this->size() << std::endl;
	reduceNumberRepresentation();
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT( const matrix_t<Number> &A )
	: mHPlanes(), mDimension( A.cols() ), mEmpty(TRIBOOL::NSET), mNonRedundant(false) {
	for ( unsigned i = 0; i < A.rows(); ++i ) {
		mHPlanes.push_back( Halfspace<Number>( A.row( i ), Number( 0 ) ) );
	}
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::HPolytopeT( const std::vector<Point<Number>>& points )
	: mHPlanes(), mDimension( 0 ), mEmpty(TRIBOOL::NSET), mNonRedundant(true) {
#ifdef HPOLY_DEBUG_MSG
	std::cout << __func__ << "Construct from vertices." << std::endl;
#endif
	/*
	if ( !points.empty() ) {
		mDimension = points.begin()->dimension();
		ConvexHull<Number> ch(points);
		ch.convexHullVertices();
		mHPlanes = ch.getHsv();
		//std::cout << "After CH there are " << mHPlanes.size() << " new hplanes." << std::endl;
		assert(ch.getCone().empty());
		assert(ch.getLinealtySpace().empty());

		//std::cout << "Object constructed from vertices: " << *this << std::endl;
		//std::cout << "Vertices: " << std::endl;
		//for(const auto& vertex : points) {
		//	std::cout << vertex << std::endl;
		//}
	}
	*/

	if ( !points.empty() ) {
		mDimension = points.begin()->dimension();
		// check affine independence - verify object dimension.
		std::vector<vector_t<Number>> coordinates;
		for(const auto& vertex : points){
			coordinates.emplace_back(vertex.rawCoordinates());
		}
		int effectiveDim = effectiveDimension(coordinates);
		assert(effectiveDim >= 0);

		// For debugging
		//std::cout << "effectiveDimension: " << effectiveDim << std::endl;
		//if(effectiveDim < mDimension){
		//	std::vector<Halfspace<Number>> boxConstraints = computeOrientedBox(points);
		//	HPolytopeT<Number,Converter> tmp(boxConstraints);
		//	std::cout << "Theoretical oriented Box: " << tmp << std::endl;
		//}

		// End for debugging

		mEmpty = TRIBOOL::FALSE;
		//if ( points.size() <= mDimension ) {
		if ( unsigned(effectiveDim) < mDimension ) {
			std::vector<Halfspace<Number>> boxConstraints = computeOrientedBox(points);
			for(const auto& constraint : boxConstraints){
				mHPlanes.emplace_back(constraint);
			}

			// Alternative version
			// We need a copy of the set of points since auxiliary points will be added
			//std::vector<Point<Number>> auxiliaryPoints(points);
			//mHPlanes = computeConstraintsForDegeneratedPolytope(auxiliaryPoints, mDimension - effectiveDim);


		} else {
			/*
			ConvexHull<Number> ch(points);
			ch.convexHullVertices();
			mHPlanes = ch.getHsv();
			*/

			std::vector<std::shared_ptr<Facet<Number>>> facets = convexHull( points ).first;
			for ( auto &facet : facets ) {
				assert(facet->halfspace().contains(points));
				mHPlanes.push_back( facet->halfspace() );
			}
			facets.clear();

		}
	}

}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter>::~HPolytopeT() {
}

/*
 * Getters and setters
 */

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::empty() const {
	//std::cout << __func__ << ": State: " << mEmpty << std::endl;
	if(mEmpty == TRIBOOL::TRUE)
		return true;
	if(mEmpty == TRIBOOL::FALSE)
		return false;

	if(mHPlanes.empty()){
		mEmpty = TRIBOOL::FALSE;
		return false;
	}

	//std::cout << __func__ << ": CALL TO Optimizer" << std::endl;

	Optimizer<Number> opt;
	opt.setMatrix(this->matrix());
	opt.setVector(this->vector());

	bool res = !opt.checkConsistency();
	mEmpty = (res == true ? TRIBOOL::TRUE : TRIBOOL::FALSE);
	return res;
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::Empty(){
	Halfspace<Number> a({Number(1)},Number(-1));
	Halfspace<Number> b({Number(-1)},Number(-1));
	HalfspaceVector v;
	v.emplace_back(a);
	v.emplace_back(b);
	HPolytopeT<Number, Converter> res(v);
	res.mEmpty=TRIBOOL::TRUE;
	res.mNonRedundant=true;
	return res;
}

template <typename Number, typename Converter>
std::size_t HPolytopeT<Number, Converter>::dimension() const {
	if(mHPlanes.empty()) return 0;
	return mDimension;
}

template <typename Number, typename Converter>
std::size_t HPolytopeT<Number, Converter>::size() const {
	return mHPlanes.size();
}

template <typename Number, typename Converter>
matrix_t<Number> HPolytopeT<Number, Converter>::matrix() const {
	matrix_t<Number> res( mHPlanes.size(), dimension() );
	for ( unsigned planeIndex = 0; planeIndex < mHPlanes.size(); ++planeIndex ) {
		//std::cout << "Add HPlane " << mHPlanes.at( planeIndex ) << " to matrix ( " << res.rows() << " x " << res.cols() << " )" << std::endl;
		res.row( planeIndex ) = mHPlanes.at( planeIndex ).normal();
	}
	return res;
}

template <typename Number, typename Converter>
vector_t<Number> HPolytopeT<Number, Converter>::vector() const {
	vector_t<Number> res( mHPlanes.size() );
	for ( unsigned planeIndex = 0; planeIndex < mHPlanes.size(); ++planeIndex ) {
		res( planeIndex ) = mHPlanes.at( planeIndex ).offset();
	}
	return res;
}

template <typename Number, typename Converter>
std::pair<matrix_t<Number>, vector_t<Number>> HPolytopeT<Number, Converter>::inequalities() const {
	matrix_t<Number> A( mHPlanes.size(), dimension() );
	vector_t<Number> b( mHPlanes.size() );
	for ( unsigned planeIndex = 0; planeIndex < mHPlanes.size(); ++planeIndex ) {
		A.row( planeIndex ) = mHPlanes.at( planeIndex ).normal().transpose();
		b( planeIndex ) = mHPlanes.at( planeIndex ).offset();
	}
	return std::make_pair( A, b );
}

template <typename Number, typename Converter>
typename std::vector<Point<Number>> HPolytopeT<Number, Converter>::vertices( const Location<Number>* ) const {
	#ifdef HPOLY_DEBUG_MSG
	std::cout << __func__ << " " << *this << std::endl;
	#endif

	typename std::vector<Point<Number>> vertices;
	if(!mHPlanes.empty()) {
		unsigned dim = this->dimension();

		Permutator permutator(mHPlanes.size(), dim);
		std::vector<unsigned> permutation;
		while(!permutator.end()) {
			permutation = permutator();

			matrix_t<Number> A( dim, dim );
			vector_t<Number> b( dim );
			unsigned pos = 0;
			//std::cout << "Permute planes ";
			for(auto planeIt = permutation.begin(); planeIt != permutation.end(); ++planeIt) {
				//std::cout << *planeIt << ", ";
				A.row(pos) = mHPlanes.at(*planeIt).normal().transpose();
				// std::cout << A.row(pos) << std::endl;
				b(pos) = mHPlanes.at(*planeIt).offset();
				// std::cout << b(pos) << std::endl;
				++pos;
			}
			//std::cout << std::endl;

			//std::cout << "Created first matrix" << std::endl;

			Eigen::FullPivLU<matrix_t<Number>> lu_decomp( A );
			if ( lu_decomp.rank() < A.rows() ) {
				continue;
			}
			#ifdef HPOLY_DEBUG_MSG
			std::cout << convert<Number,double>(A) << std::endl;
			std::cout << convert<Number,double>(b) << std::endl;
			#endif

			vector_t<Number> res = lu_decomp.solve( b );
			#ifdef HPOLY_DEBUG_MSG
			std::cout << "Vertex: " << convert<Number,double>(res).transpose() << std::endl;
			#endif

			// Check if the computed vertex is a real vertex
			bool outside = false;
			for(unsigned planePos = 0; planePos < mHPlanes.size(); ++planePos) {
				bool skip = false;
				for(unsigned permPos = 0; permPos < permutation.size(); ++permPos) {
					if(planePos == permutation.at(permPos)) {
						//std::cout << "Skip plane " << planePos << std::endl;
						skip = true;
						break;
					}
				}

				if(!skip) {
					if( mHPlanes.at(planePos).offset() - mHPlanes.at(planePos).normal().dot(res) < 0 ) {
						#ifdef HPOLY_DEBUG_MSG
						std::cout << "Drop vertex: " << convert<Number,double>(res).transpose() << " because of plane " << planePos << std::endl;
						#endif
						outside = true;
						break;
					}
				}
			}
			if(!outside) {
				// insert, if no duplicate
				Point<Number> tmp(res);
				if(std::find(vertices.begin(), vertices.end(), tmp) == vertices.end()) {
					vertices.push_back(tmp);
				}

				#ifdef HPOLY_DEBUG_MSG
				std::cout << "Final vertex: " << convert<Number,double>(res).transpose() << std::endl;
				#endif
			}
		}
	}
	return vertices;
/*
	VertexEnumeration<Number> ev = VertexEnumeration<Number>(mHPlanes);
	ev.enumerateVertices();
	std::cout << "Enumerate vertices of " << std::endl << *this << std::endl;
	assert(ev.getLinealtySpace().empty());

	if(!ev.getCones().empty()) {
		std::cout << "Computed cone: " << std::endl;
		for(const auto& cone : ev.getCones() ) {
			std::cout << convert<Number,double>(cone).transpose() << std::endl;
		}
	}

	assert(ev.getCones().empty());
	for (const auto& point : ev.getPoints() ) {
		bool found = false;
		for (const auto& vert : vertices ) {
			found=found||(point==vert);
		}
		assert(found);
	}
	for ( const auto &point : vertices ) {
		bool found = false;
		for ( const auto &vert : ev.getPoints() ) {
			found=found||(point==vert);
		}
		assert(found);
	}
	return ev.getPoints();
*/
}

template <typename Number, typename Converter>
Number HPolytopeT<Number, Converter>::supremum() const {
	Number max = 0;
	assert(!this->empty());
	assert(!this->vertices().empty());
	for ( const auto &point : this->vertices() ) {
		Number inftyNorm = Point<Number>::inftyNorm( point );
		max = max > inftyNorm ? max : inftyNorm;
	}
#ifdef HPOLY_DEBUG_MSG
	std::cout << __func__ << ": " << max << std::endl;
#endif
	return max;
}

//template <typename Number, typename Converter>
//void HPolytopeT<Number, Converter>::calculateFan() const {
//	std::vector<std::shared_ptr<Facet<Number>>> facets = convexHull( vertices() ).first;
//	std::set<Point<Number>> preresult;
//	for ( unsigned i = 0; i < facets.size(); i++ ) {
//		for ( unsigned j = 0; j < facets[i].vertices().size(); j++ ) {
//			preresult.insert( facets[i]->vertices().at( j ) );
//		}
//	}
//	polytope::Fan<Number> fan;
//	for ( auto &point : preresult ) {
//		polytope::Cone<Number> *cone = new polytope::Cone<Number>();
//		for ( unsigned i = 0; i < facets.size(); i++ ) {
//			for ( unsigned j = 0; j < facets[i]->vertices().size(); j++ ) {
//				if ( point == facets[i]->vertices().at( j ) ) {
//					std::vector<Ridge<Number>> ridges = getRidges( *facets[i] );
//					for ( unsigned m = 0; m < ridges.size(); m++ ) {
//						if ( checkInsideRidge( ridges[m], point ) ) {
//							std::vector<Facet<Number>> conefacets = shareRidge( facets, ridges[m] );
//
//							matrix_t<Number> matrix = matrix_t<Number>( conefacets.size(), point.size() );
//							for ( unsigned k = 1; k < conefacets.size(); k++ ) {
//								for ( unsigned l = 0; l < conefacets[k].getNormal().size(); l++ ) {
//									matrix( k, l ) = conefacets[k].getNormal()( l );
//								}
//							}
//
//							for ( unsigned j = 0; j < point.size(); j++ ) {
//								matrix( 0, j ) = 1;
//
//								if ( matrix.fullPivLu().rank() == point.size() ) {
//									break;
//								} else {
//									matrix( 0, j ) = 0;
//								}
//							}
//							vector_t<Number> b = vector_t<Number>::Zero( conefacets.size() );
//							b( 0 ) = 1;
//							vector_t<Number> result = matrix.fullPivHouseholderQr().solve( b );
//
//							cone->add( std::shared_ptr<Halfspace<Number>>(
//								  new Halfspace<Number>( result, result.dot( point.rawCoordinates() ) ) ) );
//							// cone->add(std::make_shared<Halfspace<Number>>(Halfspace<Number>(result,
//							// result.dot(point.rawCoordinates()))));
//						}
//					}
//				}
//			}
//		}
//		fan.add( cone );
//	}
//	mFanSet = true;
//	mFan = fan;
//}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::insert( const Halfspace<Number> &plane ) {
	assert( mDimension == 0 || mDimension == plane.dimension() );
	if ( mDimension == 0 ) {
		mDimension = plane.dimension();
		mHPlanes.push_back( plane );
		mEmpty = TRIBOOL::FALSE;
		mNonRedundant = true;
	} else {
		bool found = false;
		for(auto planeIt = mHPlanes.begin(); planeIt != mHPlanes.end(); ++planeIt) {
			if(*planeIt == plane){
				found = true;
				break;
			}
		}
		if(!found){
			mHPlanes.push_back( plane );
			mEmpty = TRIBOOL::NSET;
			mNonRedundant = false;
		}
	}
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::insert( const typename HalfspaceVector::iterator begin,
								const typename HalfspaceVector::iterator end ) {
	assert( mDimension == 0 || mDimension == begin->dimension() );
	if ( mDimension == 0 ) {
		mDimension = begin->dimension();
	}
	auto it = begin;
	while ( it != end ) {
		this->insert( *it );
		++it;
	}
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::erase( const unsigned index ) {
	assert(index < mHPlanes.size());
	mHPlanes.erase(mHPlanes.begin()+index);
	mEmpty = TRIBOOL::NSET;
}

template <typename Number, typename Converter>
const typename HPolytopeT<Number, Converter>::HalfspaceVector &HPolytopeT<Number, Converter>::constraints() const {
	return mHPlanes;
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::hasConstraint( const Halfspace<Number> &hplane ) const {
	for ( const auto &plane : mHPlanes ) {
		if ( hplane == plane ) return true;
	}
	return false;
}

template <typename Number, typename Converter>
const HPolytopeT<Number,Converter>& HPolytopeT<Number, Converter>::removeRedundancy() {
	//std::cout << __func__ << std::endl;
	if(!mNonRedundant && mHPlanes.size() > 1){
		//std::cout << "Not already reduced." << std::endl;
		Optimizer<Number> opt;
		opt.setMatrix(this->matrix());
		opt.setVector(this->vector());

		std::vector<std::size_t> redundant = opt.redundantConstraints();
		//std::cout << __func__ << ": found " << redundant.size() << " redundant constraints." << std::endl;

		if(!redundant.empty()){
			std::size_t cnt = mHPlanes.size()-1;
			for ( auto rIt = mHPlanes.rbegin(); rIt != mHPlanes.rend(); ++rIt ) {
				if(redundant.empty())
					break;

				if(redundant.back() == cnt){
					mHPlanes.erase( --(rIt.base()) );
					redundant.pop_back();
					//std::cout << "Erase plane " << cnt << std::endl;
				}
				--cnt;
			}
		}

		assert(redundant.empty());
	}
	mNonRedundant=true;
	return *this;
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::isExtremePoint( const vector_t<Number>& point ) const {
	unsigned cnt = 0;
	for ( const auto &plane : mHPlanes ) {
		Number val = plane.evaluate( point );
		if ( plane.offset() == val  ) {
			++cnt;
		} else if ( plane.offset() - val < 0 ) {
			return false;
		}
	}
	return cnt >= mDimension;
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::isExtremePoint( const Point<Number> &point ) const {
	return isExtremePoint( point.rawCoordinates() );
}

template <typename Number, typename Converter>
EvaluationResult<Number> HPolytopeT<Number, Converter>::evaluate( const vector_t<Number> &_direction ) const {
	if(mHPlanes.empty())
		return EvaluationResult<Number>( Number(1), INFTY );

	//reduceNumberRepresentation();

	//std::cout << "Constraints: " << convert<Number,double>(this->matrix()) << std::endl << "Constants: " << this->vector() << std::endl;

	Optimizer<Number> opt;
	opt.setMatrix(this->matrix());
	opt.setVector(this->vector());

	return opt.evaluate(_direction);
}

/*
 * General interface
 */

template<typename Number, typename Converter>
std::pair<bool, HPolytopeT<Number, Converter>> HPolytopeT<Number, Converter>::satisfiesHalfspace( const vector_t<Number>& normal, const Number& offset ) const {
	HPolytopeT<Number,Converter> tmp = this->intersectHalfspace(Halfspace<Number>(normal, offset));
	return std::make_pair(!(tmp).empty(), tmp);
}

template<typename Number, typename Converter>
std::pair<bool, HPolytopeT<Number, Converter>> HPolytopeT<Number, Converter>::satisfiesHalfspaces( const matrix_t<Number>& _mat, const vector_t<Number>& _vec ) const {
	assert(_mat.rows() == _vec.rows());
	if(_mat.rows() == 0) {
		return std::make_pair(true, *this);
	}
	HPolytopeT<Number,Converter> tmp = this->intersectHalfspaces(_mat, _vec);
	return std::make_pair(!(tmp).empty(),tmp);
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::linearTransformation( const matrix_t<Number> &A,
														   const vector_t<Number> &b ) const {
	//std::cout << __func__ << ": Number Planes: "<< mHPlanes.size() << ", matrix: " << std::endl << A << std::endl << "b: " << std::endl << b << std::endl;
	if(A.nonZeros() == 0) {
		std::vector<Point<Number>> points;
		points.emplace_back(b);
		return HPolytopeT<Number,Converter>(points);
	}
	//std::cout << __func__ << " of " << *this << std::endl;
	if(!this->empty() && !mHPlanes.empty()) {
		Eigen::FullPivLU<matrix_t<Number>> lu(A);
		// if A has full rank, we can simply re-transform, otherwise use v-representation.
		if(lu.rank() == A.rows()) {
			//std::cout << "Full rank, retransform!" << std::endl;
			std::pair<matrix_t<Number>, vector_t<Number>> inequalities = this->inequalities();
			//std::cout << "Matrix: " << convert<Number,double>(inequalities.first*A.inverse()) << std::endl << "Vector: " << convert<Number,double>(((inequalities.first*A.inverse()*b) + (inequalities.second))) << std::endl;
			assert( (HPolytopeT<Number, Converter>(inequalities.first*A.inverse(), inequalities.first*A.inverse()*b + inequalities.second).size() == this->size()) );
			return HPolytopeT<Number, Converter>(inequalities.first*A.inverse(), inequalities.first*A.inverse()*b + inequalities.second);
		} else {
#ifdef HPOLY_DEBUG_MSG
			std::cout << "Use V-Conversion for linear transformation." << std::endl;
#endif
			auto intermediate = Converter::toVPolytope( *this );
			intermediate = intermediate.linearTransformation( A, b );
			auto res = Converter::toHPolytope(intermediate);
			//std::cout << "Size before linear transformation: " << this->size() << ", size after linear transformation: " << res.size() << std::endl;

			//assert(res.size() <= this->size());
			res.setReduced();
			return res;
		}
	} else {
		return *this;
	}
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::minkowskiSum( const HPolytopeT &rhs, bool oneWay ) const {
	HPolytopeT<Number, Converter> res;
	Number result;

#ifdef HPOLY_DEBUG_MSG
	std::cout << __func__ << " of " << *this << " and " << rhs << std::endl;
#endif

	// evaluation of rhs in directions of lhs
	//std::cout << "evaluation of rhs in directions of lhs" << std::endl;
	for ( unsigned i = 0; i < mHPlanes.size(); ++i ) {
		EvaluationResult<Number> evalRes = rhs.evaluate( mHPlanes.at( i ).normal() );
		if ( evalRes.errorCode == INFTY ) {
			//std::cout << __func__ << " Evaluated against " <<
			// mHPlanes.at(i).normal() << std::endl;
			//std::cout << "INFTY" << std::endl;
			// Do nothing - omit inserting plane.
		} else if ( evalRes.errorCode == INFEAS ) {
			// std::cout << "EMPTY" << std::endl;
			return Empty();
		} else {
			result = mHPlanes.at( i ).offset() + evalRes.supportValue;
			res.insert( Halfspace<Number>( mHPlanes.at( i ).normal(), result ) );
			// std::cout << __func__ << " Evaluated against " <<
			// mHPlanes.at(i).normal() << " results in a distance " << evalRes.supportValue << std::endl;
			// std::cout << "Old distance: " << carl::toDouble(mHPlanes.at(i).offset()) << ", new distance: " << carl::toDouble(result) << std::endl;
		}
	}

	if(!oneWay) {
		// evaluation of lhs in directions of rhs
		//std::cout << "evaluation of lhs in directions of rhs" << std::endl;
		for ( unsigned i = 0; i < rhs.constraints().size(); ++i ) {
			EvaluationResult<Number> evalRes = this->evaluate( rhs.constraints().at( i ).normal() );
			if ( evalRes.errorCode == INFTY ) {
				// std::cout << __func__ << " Evaluated against " <<
				// rhs.constraints().at( i ).normal() << std::endl;
				// std::cout << "INFTY" << std::endl;
				// Do nothing - omit inserting plane.
			} else if ( evalRes.errorCode == INFEAS ) {
				// std::cout << "EMPTY" << std::endl;
				return Empty();
			} else {
				result = rhs.constraints().at( i ).offset() + evalRes.supportValue;
				res.insert( Halfspace<Number>( rhs.constraints().at( i ).normal(), result ) );
				// std::cout << __func__ << " Evaluated against " <<
				// rhs.constraints().at( i ).normal() << " results in a distance " << evalRes.supportValue << std::endl;
			}
		}
	}
#ifdef HPOLY_DEBUG_MSG
	std::cout << "Result: " << res << std::endl;
#endif
	return res;
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::intersect( const HPolytopeT &rhs ) const {
	if ( rhs.empty() || this->empty() ) {
		return HPolytopeT<Number, Converter>::Empty();
	} else {
		HPolytopeT<Number, Converter> res;
		for ( const auto &plane : mHPlanes ) {
			res.insert( plane );
		}
		for ( const auto &plane : rhs.constraints() ) {
			res.insert( plane );
		}

		return res;
	}
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::intersectHalfspace( const Halfspace<Number> &rhs ) const {
	HPolytopeT<Number, Converter> res( *this );
	// only insert the new Halfspace, if it is not already redundant.
	if(res.evaluate(rhs.normal()) > rhs.offset())
		res.insert( Halfspace<Number>( rhs ) );

	return res;
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::intersectHalfspaces( const matrix_t<Number> &_mat,
														   const vector_t<Number> &_vec ) const {
	assert( _mat.rows() == _vec.rows() );
	HPolytopeT<Number, Converter> res( *this );
	for ( unsigned i = 0; i < _mat.rows(); ++i ) {
		res.insert( Halfspace<Number>( _mat.row(i), _vec( i ) ) );
	}
	//std::cout << "After intersection: " << res << std::endl;
	res.removeRedundancy();
	//std::cout << "After removing redundancy: " << res << std::endl;
	return res;
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::contains( const Point<Number> &point ) const {
	std::cout << __func__ << " of " << point << std::endl;
	return this->contains( point.rawCoordinates() );
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::contains( const vector_t<Number> &vec ) const {
	for ( const auto &plane : mHPlanes ) {
		// The 2's complement check for equality is required to ensure double compatibility.
		if (!carl::AlmostEqual2sComplement(plane.normal().dot( vec ), plane.offset(), 128) && plane.normal().dot( vec ) > plane.offset()) {
			//std::cout << __func__ << " False, " << plane.normal().dot( vec ) << " > " << plane.offset() << " Almost equal: " << carl::AlmostEqual2sComplement(plane.normal().dot( vec ), plane.offset(), 128) << std::endl;
			return false;
		}
	}
	return true;
}

template <typename Number, typename Converter>
bool HPolytopeT<Number, Converter>::contains( const HPolytopeT<Number, Converter> &rhs ) const {
	//std::cout << __func__ << " : " << *this << " contains " << rhs << std::endl;
	if(this->empty()){
		return false;
	}
	if(rhs.empty()) {
		return true;
	}

	for ( const auto &plane : rhs.constraints() ) {
		EvaluationResult<Number> evalRes = this->evaluate( plane.normal() );

		//std::cout << __func__ << ": plane " << plane << " -> " << evalRes.supportValue  << " orig offset: " << plane.offset() << "\t" ;
		if ( evalRes.errorCode == INFEAS ) {
			//std::cout << "INFEAS" << std::endl;
			assert(false);
			return false;  // empty!
		} else if ( evalRes.errorCode == INFTY ) {
			//std::cout << "INFTY" << std::endl;
			continue;
		} else if ( evalRes.supportValue < plane.offset() ) {
			assert(evalRes.errorCode == FEAS);
			//std::cout << "Rhs offset is larger than result" << std::endl;
			return false;
		}
	}
	return true;
}

template <typename Number, typename Converter>
HPolytopeT<Number, Converter> HPolytopeT<Number, Converter>::unite( const HPolytopeT &_rhs ) const {
	if ( _rhs.empty() && !this->empty()) {
		return *this;
	} else if (this->empty() && !_rhs.empty()) {
		return _rhs;
	} else { // none is empty

		std::vector<Point<Number>> vertices = this->vertices();
		std::vector<Point<Number>> rhsVertices = _rhs.vertices();
		vertices.insert(vertices.end(), rhsVertices.begin(), rhsVertices.end());
		HPolytopeT<Number,Converter> result(vertices);

		/*
		auto lhs = Converter::toVPolytope( *this );
		std::cout << "this as vpoly: " << lhs << std::endl;
		auto tmpRes = lhs.unite( Converter::toVPolytope( _rhs ) );
		std::cout << "rhs as vpoly : " << Converter::toVPolytope( _rhs ) << std::endl;
		HPolytopeT<Number,Converter> result = Converter::toHPolytope( tmpRes );
		*/

		//std::cout << "size after union: " << result.size() << std::endl;

		//assert(result.contains(*this));
		//assert(result.contains(_rhs));
		//std::cout << __func__ << " : tmpres " << tmpRes << std::endl;

		/*
		std::cout << __func__ << " BEGIN." << std::endl;
		std::vector<Point<Number>> unitedVertices(this->vertices());
		for(const auto& vertex : _rhs.vertices()) {
			unitedVertices.emplace_back(vertex);
		}


		for(const auto vertex : unitedVertices) {
			std::cout << "Vertex to unite: " << vertex << std::endl;
		}


		ConvexHull<Number> ch = ConvexHull<Number>(unitedVertices);
		ch.convexHullVertices();
		HPolytopeT<Number,Converter> result = HPolytopeT<Number,Converter>(ch.getHsv());
		assert(ch.getCone().empty());
		assert(ch.getLinealtySpace().empty());
		*/
		/*
		for(const auto& vertex : owenTmpRes.vertices()) {
			std::cout << "Result vertex " << convert<Number,double>(vertex.rawCoordinates()).transpose() << std::endl;
			//assert(std::find(unitedVertices.begin(), unitedVertices.end(), vertex) != unitedVertices.end());
		}
		std::cout << __func__ << " END." << std::endl;
		*/
		return result;
	}
}

template<typename Number, typename Converter>
HPolytopeT<Number,Converter> HPolytopeT<Number,Converter>::unite( const std::vector<HPolytopeT>& rhs ) const {
	std::vector<Point<Number>> vertices = this->vertices();
	for(const auto& poly : rhs) {
		vertices.insert(vertices.end(), poly.vertices().begin(), poly.vertices().end());
	}
	return HPolytopeT(vertices);
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::clear() {
	mHPlanes.clear();
	mDimension = 0;
	mEmpty = FALSE;
	mNonRedundant = true;
}

template <typename Number, typename Converter>
void HPolytopeT<Number, Converter>::print() const {
	//std::cout << *this << std::endl;
}

/*
 * Auxiliary functions
 */


template <typename Number, typename Converter>
typename HPolytopeT<Number, Converter>::HalfspaceVector HPolytopeT<Number, Converter>::computeConstraintsForDegeneratedPolytope(std::vector<Point<Number>>& points, unsigned degeneratedDimensions) const {
	if(degeneratedDimensions == 0) {
		return std::move(HPolytopeT<Number, Converter>(points).mHPlanes);
	}
	assert(!points.empty());
	Halfspace<Number> h; // TODO set h to some hyperplane holding all points, i.e., a*p=b for all points p
	points.emplace_back(points.begin()->rawCoordinates() + h.normal());
	// The effective dimension of the convex hull of points is now increased by one, i.e.,
	// the number of degenerated dimensions is decreased by one.
	// Furthermore, one of the faces of the convex hull of points is the polytope we are looking for.
	// This is exactly the face we get when we intersect the convex hull with the halfspace h
	HalfspaceVector result = commputeConstraintsForDegeneratedPolytope(points, degeneratedDimensions - 1);
	result.push_back(std::move(h)); // decreases the effective dimension again
	return result;
}

}  // namespace hypro
