#pragma once
#include "../IHandler.h"
#include "../../../../datastructures/HybridAutomaton/State.h"

namespace hypro
{
    template<class Representation,typename Number>
	class ltiTimeEvolutionHandler : public ITimeEvolutionHandler {	
	protected:	
		State_t<Number>*  mState; // A state containing the first segment for each subspace
		matrix_t<Number> mTrafo;
		vector_t<Number> mTranslation;

		size_t mIndex;
		tNumber mTimeStep;
	public:

		ltiTimeEvolutionHandler() = delete;
		ltiTimeEvolutionHandler(State_t<Number>* state, size_t index, tNumber timeStep, matrix_t<Number> trafo,vector_t<Number> translation){
			mState = state;
			mIndex = index;
			mTimeStep = timeStep;
			mTrafo = trafo;
			mTranslation = translation;
		}

		void handle();
		const char* handlerName() {return "ltiTimeEvolutionHandler";}

	};
} // hypro

#include "ltiTimeEvolutionHandler.tpp"
