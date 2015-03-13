/*
 * This file contains the basic implementation of support functions of polyhedra (template polyhedra) and their evaluation.
 * @file PolytopeSupportFunction.h
 * 
 * @author Norman Hansen
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 * 
 * @version	2015-02-27
 */
 
#pragma once 

#include "../../config.h"
 #include "../../datastructures/Point.h"
#include "util.h"
#include <glpk.h>

#define PPOLYTOPESUPPORTFUNCTION_VERBOSE

namespace hypro
{
	
	/*
	* This class defines a support Function object representing a polytope (might not be closed)
	* SupportFunctions can be evaluated in a specified direction l and return a correspondent evaluationResult
	*/
	template<typename Number>
	class PolytopeSupportFunction{
	
	private:
		
		glp_prob *lp;
		int* ia;
		int* ja;
		double* ar;
		
		void createArrays(unsigned size);
		void deleteArrays();
		
		unsigned mDimension;
		
		/**
		 * Initializes the class by setting up the glpk problem with the given parameters.
		 * @param constraints
		 * @param constraintConstants
		 */
		void initialize(matrix_t<Number> constraints, vector_t<Number> constraintConstants);
			
	   public:
		
		PolytopeSupportFunction(matrix_t<Number> constraints, vector_t<Number> constraintConstants);
		~PolytopeSupportFunction();
		
		/**
		 * Returns the dimension of the object.
		 * @return 
		 */
		unsigned dimension() const;
		
		/**
		 * Evaluates the support function in the given direction.
		 * @param l
		 * @return
		 */
		evaluationResult<Number> evaluate(const vector_t<Number>& l) const;
		
		/**
		 * @brief Evaluates the support function in the directions given in the passed matrix.
		 * @details Uses the standard evaluate method for multiple directions.
		 * 
		 * @param _A Matrix holding the directions in which to evaluate.
		 * @return Vector of support values.
		 */
		vector_t<Number> multiEvaluate(const matrix_t<Number>& _A) const;

		/**
		 * @brief Check if point is contained in the support function.
		 * @details To check if the given point is contained in the support function we evaluate it in all its directions (uses multiEvaluate).
		 * 
		 * @param _point The point to check.
		 * @return True, if the point is inside the support function.
		 */
		bool contains(const Point<Number>& _point) const;

		bool empty() const;
	};
} // namespace
#include "PolytopeSupportFunction.tpp"