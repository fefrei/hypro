/*
 * This file contains the basic implementation of support functions of polyhedra (template polyhedra) and their
 *evaluation.
 * @file PolytopeSupportFunction.h
 *
 * @author Norman Hansen
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 * @author Simon Froitzheim
 *
 * @version	2015-02-27
 */

#pragma once

#include "../../config.h"
#include "../../datastructures/Point.h"
#include "../../datastructures/Hyperplane.h"
#include "../../util/Optimizer.h"
#include "util.h"

//#define PPOLYTOPESUPPORTFUNCTION_VERBOSE
#define PPOLYTOPESUPPORTFUNCTION_VERIFY

namespace hypro {

/*
* This class defines a support Function object representing a polytope (might not be closed)
* SupportFunctions can be evaluated in a specified direction l and return a correspondent evaluationResult
*/
template <typename Number>
class PolytopeSupportFunction {
  private:

	matrix_t<Number> mConstraints;
	vector_t<Number> mConstraintConstants;

	unsigned mDimension;

  public:
	PolytopeSupportFunction( matrix_t<Number> constraints, vector_t<Number> constraintConstants );
	PolytopeSupportFunction( const std::vector<Hyperplane<Number>>& _planes );
	PolytopeSupportFunction( const PolytopeSupportFunction<Number>& _origin );
	~PolytopeSupportFunction();

        PolytopeSupportFunction<Number>& operator=(const PolytopeSupportFunction& _orig);

	/**
	 * Returns the dimension of the object.
	 * @return
	 */
	unsigned dimension() const;

	SF_TYPE type() const;

	matrix_t<Number> constraints() const;
	vector_t<Number> constants() const;

	/**
	 * Evaluates the support function in the given direction.
	 * @param l
	 * @return
	 */
	evaluationResult<Number> evaluate( const vector_t<Number>& l ) const;

	/**
	 * @brief Evaluates the support function in the directions given in the passed matrix.
	 * @details Uses the standard evaluate method for multiple directions.
	 *
	 * @param _A Matrix holding the directions in which to evaluate.
	 * @return Vector of support values.
	 */
	std::vector<evaluationResult<Number>> multiEvaluate( const matrix_t<Number>& _A ) const;

	/**
	 * @brief Check if point is contained in the support function.
	 * @details To check if the given point is contained in the support function we evaluate it in all its directions
	 *(uses multiEvaluate).
	 *
	 * @param _point The point to check.
	 * @return True, if the point is inside the support function.
	 */
	bool contains( const Point<Number>& _point ) const;

	/**
	 * @brief Check if point is contained in the support function.
	 * @details To check if the given point is contained in the support function we evaluate it in all its directions
	 *(uses multiEvaluate).
	 *
	 * @param _point The point to check.
	 * @return True, if the point is inside the support function.
	 */
	bool contains( const vector_t<Number>& _point ) const;

	bool empty() const;

    void print() const;
};
}  // namespace
#include "PolytopeSupportFunction.tpp"
