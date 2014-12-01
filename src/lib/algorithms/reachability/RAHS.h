/* 
 * File:   RAHS.h
 * Author: Jonathan Gan
 *
 * Created on June 22, 2014, 19:15 PM
 */


#ifndef RAHS_H
#define RAHS_H

#include <iostream>
#include <cmath>
#include "../../representations/Zonotope/Zonotope.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>
#include <algorithm>
#include "../../../../examples/TwoTank/hybridAutomaton/HybridAutomaton.h" // a temporary hack
#include <map>
#include "../../representations/Zonotope/ZUtility.h"

template<typename Number> 
class RAHS {

    private: // private attributes
        unsigned int mDimension;
        
        Eigen::Matrix<Number, Eigen::Dynamic, Eigen::Dynamic> exp_rA_;
        std::vector< Zonotope<Number> > sequence_zonQ_;
        std::map <unsigned int, std::vector< Zonotope<Number> > > intersect_zonotopes_;
        std::map <unsigned int, std::vector< Zonotope<Number> > > resulting_intersect_;
        std::map <unsigned int, Eigen::Matrix<Number, Eigen::Dynamic, Eigen::Dynamic> > min_intersect_;
        std::map <unsigned int, Eigen::Matrix<Number, Eigen::Dynamic, Eigen::Dynamic> > max_intersect_;
        
        std::map <unsigned int, Zonotope<Number> > pivotal_zonotopes_;
        bool mInitialized, mReadjusted;
        
        Eigen::Matrix<Number, Eigen::Dynamic, Eigen::Dynamic> A_;
        Eigen::Matrix<Number, Eigen::Dynamic, Eigen::Dynamic> bigB_;
        Eigen::Matrix<Number, Eigen::Dynamic, 1> smallb_;
        Number r_;
        
        Zonotope<Number> U_;
        Zonotope<Number> Q_;
        
        hypro::HybridAutomaton<Number> mHybridAutomaton;
        hypro::Location<Number> mCurrentLoc;
        
        enum state_t {
            START,
            NORMAL,
            JUMP         
        };
        
    private: // private functions
        
        void preclustering(const std::vector< Zonotope<Number> >& intersects, const Hyperplane<Number>& hp, Zonotope<Number>& finalIntersect, const ZUtility::Options& option);
        void postclustering(const std::vector< Zonotope<Number> >& resultingIntersects, Zonotope<Number>& finalIntersect);
        
        void overapproximateZonotope(Zonotope<Number>& z);
        void readjustMatrices();
        void readjust();
        void initializeRAHSComputation(Zonotope<Number>& res_V, Zonotope<Number>& res_S);
        void computeNextZonotope(  unsigned int order_reduction_threshold,
                                        Zonotope<Number>& Q, 
                                        Zonotope<Number>& V,
                                        Zonotope<Number>& S);
        bool checkGuardJumpCondition(hypro::Transition<Number>& transition_taken,
                                    const Zonotope<Number>& Q,
                                    Eigen::Matrix<Number, Eigen::Dynamic, Eigen::Dynamic>& minMaxOfLine,
                                    const ZUtility::Options& option);
        
        void loadNewState(hypro::Transition<Number>& transition, const Zonotope<Number>& intersect_zonotope);
        void overapproximatedConvexHull(Zonotope<Number>& Q, 
                                        const Eigen::Matrix<Number, Eigen::Dynamic, Eigen::Dynamic>& expMatrix);
        
         bool checkForIntersection(const Zonotope<Number>& inputZonotope, const Hyperplane<Number>& hp, 
                                    Zonotope<Number>& result,
                                    const ZUtility::IntersectionMethod_t& method);
         
        bool checkForIntersection(const Zonotope<Number>& inputZonotope, const Hyperplane<Number>& hp, 
                                    Zonotope<Number>& result,
                                    const ZUtility::IntersectionMethod_t& method,
                                    Eigen::Matrix<Number, Eigen::Dynamic, Eigen::Dynamic>& minMaxOfLine);
        
        
        
        void runReachabilityAnalysis(unsigned int numIterations, 
                                       unsigned int offset,
                                       Number r_scalar, 
                                       unsigned int order_reduction_threshold,
                                       Zonotope<Number>& res_V, Zonotope<Number>& res_S,
                                       const ZUtility::Options& option);
        
    public: // public functions
        RAHS(unsigned int dimension);
        
        virtual ~RAHS();
        
        
        /**
         * Dimensionality of system
         * @return dimension
         */
        unsigned int dimension() const;
        
        /**
         * Flowpipe generated by reachability analysis
         * @return A vector array of states reached by the algorithm
         */
        std::vector< Zonotope<Number> > flowpipe();
        
        /**
         * Set of states where an intersection occurs
         * @return map of iteration number to vector array of zonotopes
         */
        std::map <unsigned int, std::vector< Zonotope<Number> > > intersections();
        
        /**
         * Set of resulting zonotope intersections at each iteration where an intersection occurs
         * @return map of iteration number to vector array of zonotopes
         */
        std::map <unsigned int, std::vector< Zonotope<Number> > > resultingIntersections();
        
        std::map <unsigned int, Zonotope<Number> > pivotalZonotopes();
        
        /**
         * Current state loaded
         * @return Current state (a Zonotope)
         */
        Zonotope<Number> currentSet() const;
        
        /**
         * Initial input passed through the hybrid automata
         * @return Initial input (a Zonotope)
         */
        Zonotope<Number> initialInput() const;
        
        /**
         * Loads the Hybrid Automaton and initializes initial state, initial input,
         * dynamics matrix A, input matrix B and constant vector b
         * @param hybridAutomaton_ A hybrid automaton as defined in hypro library
         */
        void loadHybridAutomaton(hypro::HybridAutomaton<Number>* hybridAutomaton_);
        
        /**
         * Runs the algorithm
         * @param numIterations Number of iterations that the algorithm should run
         * @param r_scalar Time division e.g. 0.01
         * @param order_reduction_threshold The threshold number of generators, from which we will overapproximate the generators
         */
        void startReachabilityAnalysis(unsigned int numIterations, 
                                       unsigned int offset,
                                       Number r_scalar, 
                                       unsigned int order_reduction_threshold,
                                       const ZUtility::Options& option);
        
        
        
        
        
        
};

#include "RAHS.tpp"

#endif /* RAHS.h */

