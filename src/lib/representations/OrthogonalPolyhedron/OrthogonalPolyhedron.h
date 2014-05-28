/* 
 * @file    OrthogonalPolyhedron.h
 * @author  Sebastian Junges
 * @author  Benedikt Seidl
 *
 * @since   2014-03-17
 * @version 2014-05-28
 */

#pragma once

#include "../../datastructures/Point.h"
#include "../../datastructures/Vertex.h"
#include "../../datastructures/VertexContainer.h"
#include "../Box/Box.h"
#include "../GeometricObject.h"
#include "NeighbourhoodContainer.h"
#include "Grid.h"

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <fstream>

namespace hypro
{
    
    
    template<typename Number>
    class OrthogonalPolyhedron : hypro::GeometricObject<Number>
    {
    private:
        
        // the container of all vertices
        VertexContainer<Number> mVertices;
        
        // the grid the polyhedron is defined on
        Grid<Number> mGrid;
        
        // the neighbourhood container which maps points to a set of vertices
        NeighbourhoodContainer<Number> mNeighbourhood;
        
        // the cached boundary box
        Box<Number> mBox;
        bool mBoxUpToDate = false;
        
    public:
        
        /***********************************************************************
         * Constructors
         ***********************************************************************/
        
        /**
         * Empty constructor
         */
        OrthogonalPolyhedron() {}
        
        /**
         * Constructor initialising values.
         * @param vertices
         * @param induceGrid
         */
        OrthogonalPolyhedron(const VertexContainer<Number>& vertices, const bool induceGrid = true) : mVertices(vertices)
        {
            mGrid.reserveInducedGrid(vertices.variables());
            if (induceGrid)
            {
                mGrid.induceGrid(vertices);
            }
        }
        
        /**
         * Copy constructor
         * @param copy
         */
        OrthogonalPolyhedron(const OrthogonalPolyhedron<Number>& copy)
        {
            mVertices(copy.mVertices);
            mGrid(copy.mGrid);
            mNeighbourhood(copy.mNeighbourhood);
            mBox(copy.mBox);
            mBoxUpToDate = copy.mBoxUpToDate;
        }
        
        /***********************************************************************
         * Geometric Object functions
         ***********************************************************************/
        
        unsigned int dimension();		
        bool linearTransformation(OrthogonalPolyhedron<Number>& result);
        bool minkowskiSum(OrthogonalPolyhedron<Number>& result, const OrthogonalPolyhedron<Number>& rhs);
        bool intersect(OrthogonalPolyhedron<Number>& result, const OrthogonalPolyhedron<Number>& rhs);
        bool hull(OrthogonalPolyhedron<Number>& result);
        bool contains(const Point<Number>& point);
        bool unite(OrthogonalPolyhedron<Number>& result, const OrthogonalPolyhedron<Number>& rhs);
        
        /***********************************************************************
         * Other functions
         ***********************************************************************/
        
        bool empty() const;
        
    };
    
}//namespace

#include "OrthogonalPolyhedron.tpp"

