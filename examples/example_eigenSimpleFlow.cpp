/**
 * Sandbox for the Eigendecomposition project.
 */

#include "representations/GeometricObject.h"
#include "util/Plotter.h"
#include <Eigen/Eigenvalues>
#include <Eigen/Dense>
//#define PTS_DEBUG          //NO USE
//#include <Eigen/LU>
using namespace hypro;
using Number = double;
using Matrix = matrix_t<Number>;
using Vector = vector_t<Number>;
using DiagonalMatrix = Eigen::DiagonalMatrix<Number,Eigen::Dynamic>;
using BoolMatrix = matrix_t<bool>;
typedef struct flags {
    int n; //<--- DIMENSION --->
    std::size_t _DIM_PLOT_;      //0..n-1 for the plot
    bool _ORIGINAL_SYS_PLOT_; //plotting in original system or transformed?
    bool _HULLCONSTRUCTION_;
    bool _TRAJECTORY_;
    bool _ERROR_PLOT_;
} flags;
typedef struct initEquation {
    Matrix A;
    Vector x0;
    Vector x0_2;
    Vector b;
} initEquation;
typedef struct invariants {
    Matrix iA;
    Vector b;
    //?? VA ?? b_VA ??
} invariants;
typedef struct tmpFlowSeg { 
    std::vector<Vector> upper;
    std::vector<Vector> lower;
} tmpFlowSeg;
typedef struct const_solCalc {
    Matrix xhomconst;
    DiagonalMatrix D;
    Matrix xinhomconst;
    std::size_t delta;
    std::size_t deltalimit;
} const_solCalc;
typedef struct const_V_n {
    Matrix V;
    int n;
} const_V_n;

void mark_x0isMin(Matrix& x_tr, int n);
void swap_x0isMax(Matrix& x_tr, int n);
void initialize (const Matrix& xhomconst, const DiagonalMatrix& D, BoolMatrix& directmaxmin,
                    Matrix& derivFactormaxmin, std::vector<Vector>& ptsxmax_tr,
                    std::vector<Vector>& ptsxmin_tr, int n, Matrix& x_tr, const Matrix& V,
                    std::size_t _DIM_PLOT_, bool _ORIGINAL_SYS_PLOT_);
void loop (std::vector<VPolytope<Number>>& flowpipe, std::size_t deltalimit, const BoolMatrix& directmaxmin, const DiagonalMatrix& D, 
            const Matrix& xhomconstmaxmin, Number delta, int n, 
            std::vector<Vector>& ptsxmax_tr, std::vector<Vector>& ptsxmin_tr, 
            Matrix& derivFactormaxmin,Matrix& x_tr, const Vector& xinhomconst, const Matrix& V,
            std::size_t _DIM_PLOT_, bool _ORIGINAL_SYS_PLOT_);
void trajectory_plot(std::vector<VPolytope<Number>>& exactflowpipe, std::size_t deltalimit, 
            const DiagonalMatrix& D, Number delta, int n, 
            const Vector& xinhomconst, const Matrix& V,
            Matrix& traj_tr, const Matrix& traj_homconst, std::size_t traj_scale,
            std::size_t _DIM_PLOT_, bool _ORIGINAL_SYS_PLOT_);
void plot_addTimeSegment(std::size_t traj_time, const Matrix& traj_tr, const Matrix& V, 
        int n, std::vector<Vector>& plot_upper, std::vector<Vector>& plot_lower, 
        std::size_t _DIM_PLOT_, bool _ORIGINAL_SYS_PLOT_);
void addSegment(std::vector<VPolytope<Number>>& flowpipe, std::vector<Vector>& ptsxmax_tr,
        std::vector<Vector>& ptsxmin_tr, std::size_t colorUpper, std::size_t colorLower);

int main()
{
    int n = 2;                  //<--- DIMENSION --->
    std::size_t _DIM_PLOT_ = 0;        //0..n-1 for the plot
    bool _ORIGINAL_SYS_PLOT_ = true; //plotting in original system or transformed?
    bool _HULLCONSTRUCTION_ = true;
    bool _TRAJECTORY_ = false;
	Matrix A = Matrix(n,n);
    Matrix AV = Matrix(n,n);
    Vector b = Vector(n);
    Vector x0 = Vector(n);
    Vector x0_2 = Vector(n);
    Vector b_tr = Vector(n);      //transformed
    Matrix x_tr = Matrix::Zero(n,3);
    x_tr.col(2).array() = 0;
    Matrix xhomconstmaxmin = Matrix::Zero(2,2);

    Vector b_AV = Vector(n);

    Vector xinhomconst = Vector(n);
    Vector factor = Vector(n);
    Vector xvalue = Vector(n);
    Matrix derivFactormaxmin = Matrix(n,2);
    BoolMatrix directmaxmin = BoolMatrix(2,2);
    directmaxmin.setConstant(0);
    std::size_t traj_scale = 1;
    //even if not plotting functions requires these
    Matrix traj_tr = Matrix::Zero(n,2);
    Matrix traj_homconst = Matrix::Zero(n,2);
    Number tend = 20;
    Number delta = 0.1; //segment stepping time
    Matrix V = Matrix(n,n);
    Matrix Vinv = Matrix(n,n);
	DiagonalMatrix D = DiagonalMatrix(2); //type Number size 2
    //*************** RESULT *******************
    std::vector<VPolytope<Number>> safeflowpipe;
    std::vector<VPolytope<Number>> exactflowpipe;
    //######   d/dx = A*x + b  ######
	A << 	0.001, 1,
			0.001, -0.002;
    b <<    0, -9.81;
    x0<<    100, 0;        //we require x0 > x0_2 elementwise
    x0_2 << 10, 0;          //else we swap values
    //######   invariants      ######
    b_AV.array() = 0;

	std::cout << "d/dx = A*x+b, A:"<< std::endl << A << std::endl;
	std::cout << "b: "<< std::endl << b << std::endl;
    std::cout << "x0: "<< std::endl << x0 << std::endl;
    std::cout << "x0_2: "<< std::endl << x0_2 << std::endl;
    //decompose directly + constructor
    Eigen::EigenSolver<Matrix> es(A);

    V << es.eigenvectors().real();
    D.diagonal() << es.eigenvalues().real();
    Vinv = V.inverse();
    std::cout << "Eigenvectors: "<< std::endl << V << std::endl;
	std::cout << "Vinverse: "<< std::endl << Vinv << std::endl;
	std::cout << "Eigenvalues: "<< std::endl << D.diagonal() << std::endl;
    //stop on bad conditioning ??

    //invariants+transformed system
    b_tr = Vinv*b;
    x_tr.col(0) = Vinv*x0;
    x_tr.col(1) = Vinv*x0_2;
    traj_tr.col(0) = x_tr.col(0);
    traj_tr.col(1) = x_tr.col(1);
    std::cout << "traj_tr: " << std::endl << traj_tr << std::endl;
    AV = A*V;
    std::cout << "Invariants: A*V="<<std::endl << AV;
    //std::cout << "x_tr_bef_maxtrafo: "<< std::endl << x_tr << std::endl;
    mark_x0isMin(x_tr, n); 
    swap_x0isMax(x_tr, n);
    //std::cout << "x_tr_after_maxtrafo: "<< std::endl << x_tr << std::endl;
    xinhomconst = b_tr.array() / D.diagonal().array();
    traj_homconst.col(0) = xinhomconst.array() + traj_tr.col(0).array();
    traj_homconst.col(1) = xinhomconst.array() + traj_tr.col(1).array();
    xhomconstmaxmin.col(0) = xinhomconst.array() + x_tr.col(0).array();
    xhomconstmaxmin.col(1) = xinhomconst.array() + x_tr.col(1).array();
    std::cout << "xhomconstmaxmin: "<< std::endl << xhomconstmaxmin;
    //plotting structures
    Plotter<Number>& plotter = Plotter<Number>::getInstance();
    std::vector<Vector> ptsxmax_tr;
    std::vector<Vector> ptsxmin_tr;
    //function pointers + 1.step(derivative+value)
    //initialize (const Matrix& a); //
    initialize (xhomconstmaxmin, D, directmaxmin, derivFactormaxmin, ptsxmax_tr, ptsxmin_tr, 
                n, x_tr, V, _DIM_PLOT_, _ORIGINAL_SYS_PLOT_);

    //initial value calculation+derivative, possible trajectory value
    //starting on delta: rational numbers might be better here
    std::size_t deltalimit = std::ceil( (tend/delta) );
    //traj_tr, traj_homconst, traj_scale
    if (_TRAJECTORY_) 
        trajectory_plot(exactflowpipe, deltalimit, D, delta, n, xinhomconst, V, traj_tr, traj_homconst, 
                        traj_scale, _DIM_PLOT_, _ORIGINAL_SYS_PLOT_);
    if (_HULLCONSTRUCTION_) {
    loop (safeflowpipe, deltalimit, directmaxmin, D, xhomconstmaxmin, delta, n, ptsxmax_tr, ptsxmin_tr,
          derivFactormaxmin, x_tr, xinhomconst, V, _DIM_PLOT_, _ORIGINAL_SYS_PLOT_);
    }
    //checking invariants for jump
    //std::pair<bool,VPolytope> res = Segment.satisfiesHalfspace(AV,b_AV);
    plotter.plot2d();
    plotter.plotGen();

    //std::cout << "tlimit: " << tlimit << std::endl; //better:rationals

    // /* compute points of exact solution ... E_points
    // #ifndef NDEBUG
    // for( const auto& p : ePoints) {
    // 		bool contained = false;
    // 		for( const auto& set : sets) {
    // 			if(set.contains(p)) {
    // 				contained = true;
    // 			}
    // 		}
    // 		assert(contained);
    // }
    // #endif
    //
    //
    //Halfspace<Number> hsp = Halfspace<Number>({-1,1},0);
    //plotter.plotTex();
    //plotter.plotEps();
	return 0;
}

//mark if in transformed system x0<x0_2 in 3rd column
void mark_x0isMin(Matrix& x_tr, const int n) {
    for(int i=0; i<n; ++i) {   //check if x0_tr >= x0_2_tr
        if(x_tr(i,0) < x_tr(i,1)) {
            x_tr(i,2) = 1; //mark second column to recognize later
        }
    }
}
//check if in transformed system x0<x0_2 and swap if needed
void swap_x0isMax(Matrix& x_tr, const int n) {
    //std::cout << "x_tr beforeback: "<< std::endl << x_tr << std::endl;
    Vector tmp = Vector(n);
    for(int i=0; i<n; ++i) {
        if(x_tr(i,2) == 1) { 
            tmp(i)    = x_tr(i,0);
            x_tr(i,0) = x_tr(i,1);
            x_tr(i,1) = tmp(i);
        }
    }
    //std::cout << "x_tr afterback: "<< std::endl << x_tr << std::endl;
}
//1. assign e-Function behavior 2. calculate initial values
    //divergence: D>0
    //convergence D<0, 1 or 2 points
    //remind we assume x0 > x0_2 element wise and set addresses accordingly
            //case1:  e^x      --- D>0, xhomconst>0
            //case2: -e^(-x)   --- etc
            //case3: -e^(x)    --- D>0, xhomconst<0
            //case4:  e^(-x)   --- etc
void initialize (const  Matrix& xhomconst, const DiagonalMatrix& D, BoolMatrix& directmaxmin,
                        Matrix& derivFactormaxmin, std::vector<Vector>& ptsxmax_tr,
                        std::vector<Vector>& ptsxmin_tr, int n, Matrix& x_tr, const Matrix& V,
                        std::size_t _DIM_PLOT_, bool _ORIGINAL_SYS_PLOT_) {
    Vector plot_vector = Vector(n);
    for(int i=0; i<n; ++i) {
        if (D.diagonal()(i)>0) { //divergence
            if (xhomconst(i,0) >= 0) {  //
                directmaxmin(i,0) = true;      //xmax(i) directLine
                if (xhomconst(i,1) > 0) {
                    //xmin(i) derivLine
                } else {    //xmin: -e^x
                    directmaxmin(i,1) = true;  //unusual!! xmin(i) directLine
                    std::cout<<"set evolves as opposite diverging trajectoris"<<std::endl;
                }
            } else {    //xmax: -e^x
                    //xmax(i) derivLine
                if(xhomconst(i,1) > 0) {
                    std::cout<<"ERROR upwards and downwards"<<std::endl;
                    //ERROR upwards and downwards
                } else {
                    directmaxmin(i,1) = true;  //xmin(i) directLine
                }
            }
        } else { //e^(-x) and -e^(-x) whereas -x fixed here
        //convergence
            if (xhomconst(i,0) <= 0) {  //xmax:-e^(-x)
                directmaxmin(i,0) = true;      //xmax(i) directLine
                if (xhomconst(i,1) < 0) {
                    //xmin(i) derivLine
                } else {
                    directmaxmin(i,1) = true;  //xmin(i) directLine
                    std::cout<<"convergence point between initial set"<<std::endl;
                }
            } else {    //xmax:e^(-x)
                //xmax(i) derivLine
                if (xhomconst(i,1) >=0) {
                    directmaxmin(i,1) = true;   //xmin(i) directLine
                } else {
                    //xmin(i) derivLine
                    std::cout<<"2 convergence points, check if intended"<<std::endl;
                }
            }
        }
    }
    //std::cout<<"directmaxmin:"<<std::endl<<directmaxmin;
    //std::cout << "xhomconst: "<< std::endl << x0_2 << std::endl;
    //1.step calculation + inserting
    //e^0 = 1
    derivFactormaxmin.col(0) = xhomconst.col(0).array()*D.diagonal().array();
    derivFactormaxmin.col(1) = xhomconst.col(1).array()*D.diagonal().array();
    if (_ORIGINAL_SYS_PLOT_) {
        swap_x0isMax(x_tr, n);
        plot_addTimeSegment(0, x_tr, V, n, ptsxmax_tr, ptsxmin_tr, 
        _DIM_PLOT_, _ORIGINAL_SYS_PLOT_);
        swap_x0isMax(x_tr, n);
    } else {
        plot_addTimeSegment(0, x_tr, V, n, ptsxmax_tr, ptsxmin_tr, 
        _DIM_PLOT_, _ORIGINAL_SYS_PLOT_);
    }

    //STARTING POINT
}
    //1.linear value + derivative OR direct value
    //1.1 possible trajectory calculation pushing writing deleting
    //2.pushing,writing,deleting   [maybe flushing in between??]

void loop (std::vector<VPolytope<Number>>& flowpipe, std::size_t deltalimit, const BoolMatrix& directmaxmin,
        const DiagonalMatrix& D, const Matrix& xhomconst, Number delta,
        int n, std::vector<Vector>& ptsxmax_tr, std::vector<Vector>& ptsxmin_tr, 
        Matrix& derivFactormaxmin, Matrix& x_tr, const Vector& xinhomconst, 
        const Matrix& V, std::size_t _DIM_PLOT_, bool _ORIGINAL_SYS_PLOT_) {
    //bool _ERROR_PLOT_, 
    Vector factor = Vector::Zero(n);
    //Matrix x_tr_error = Matrix(n,2);
    //WE ASSUME we always want to check 1 timestep
    for(std::size_t j = 1; j <= deltalimit;  ++j) {
    	//std::cout << "Time: " << j*delta << std::endl;
        for (int i=0; i<n; ++i) {
            factor(i) = std::exp(D.diagonal()(i) *j*delta);

            if(directmaxmin(i,0)) {
                //e^x OR -e^(-x)
                x_tr(i,0) = xhomconst(i,0)*factor(i) - xinhomconst(i);
            } else {
                //e^(-x) OR -e^x
                x_tr(i,0) = x_tr(i,0) + derivFactormaxmin(i,0)*delta;
                derivFactormaxmin(i,0) = xhomconst(i,0)*D.diagonal()(i)*factor(i);
            }
            if(directmaxmin(i,1)) {
                x_tr(i,1) = xhomconst(i,1)*factor(i) - xinhomconst(i);
            } else {
                x_tr(i,1) = x_tr(i,1) + derivFactormaxmin(i,1)*delta;
                derivFactormaxmin(i,1) = xhomconst(i,1)*D.diagonal()(i)*factor(i);
            }
        }
        //x_tr_error.column(0).array() = xhomconst.column(0).array()*factor.array()-xinhomconst.array();
        //x_tr_error.column(1).array() = xhomconst.column(1).array()*factor.array()-xinhomconst.array();
        //x_tr_error.column(0) = x_tr.column(0) - x_tr_error;
        //x_tr_error.column(1) -= x_tr.column(1);
        if (_ORIGINAL_SYS_PLOT_) {
            swap_x0isMax(x_tr, n);
            plot_addTimeSegment(j, x_tr, V, n, ptsxmax_tr, ptsxmin_tr, 
            _DIM_PLOT_, _ORIGINAL_SYS_PLOT_);
            swap_x0isMax(x_tr, n);
        } else {
            plot_addTimeSegment(j, x_tr, V, n, ptsxmax_tr, ptsxmin_tr, 
            _DIM_PLOT_, _ORIGINAL_SYS_PLOT_);
        }
        addSegment(flowpipe, ptsxmax_tr, ptsxmin_tr, plotting::colors[plotting::red], 
            plotting::colors[plotting::blue]);
    }
}

void trajectory_plot(std::vector<VPolytope<Number>>& exactflowpipe, std::size_t deltalimit, const DiagonalMatrix& D, Number delta, 
            int n, const Vector& xinhomconst, const Matrix& V, Matrix& traj_tr, 
            const Matrix& traj_homconst, std::size_t traj_scale, std::size_t _DIM_PLOT_, 
            bool _ORIGINAL_SYS_PLOT_) {
    std::vector<Vector> plot_traj_upper;
    std::vector<Vector> plot_traj_lower;
    Vector factor = Vector::Zero(n);
    plot_addTimeSegment(0, traj_tr, V, n, plot_traj_upper, plot_traj_lower, 
        _DIM_PLOT_, _ORIGINAL_SYS_PLOT_);
    for(std::size_t traj_time = 1; traj_time<=traj_scale*deltalimit; traj_time+=1) {
        for (int i=0; i<n; ++i) {
            factor(i) = std::exp(D.diagonal()(i)*traj_time*delta/traj_scale);
        }
        //std::cout<<"traj_time"<<traj_time<<" factor "<<std::endl<<factor<<std::endl;
        //depends what we want to plot here
        traj_tr.col(0) = traj_homconst.col(0).array()*factor.array()  - xinhomconst.array();
        traj_tr.col(1) = traj_homconst.col(1).array()*factor.array()  - xinhomconst.array();
        plot_addTimeSegment(traj_time, traj_tr, V, n, plot_traj_upper, plot_traj_lower, 
        _DIM_PLOT_, _ORIGINAL_SYS_PLOT_);

        addSegment(exactflowpipe, plot_traj_upper, plot_traj_lower, plotting::colors[plotting::green], 
            plotting::colors[plotting::green]);
    }
}
void plot_addTimeSegment(std::size_t traj_time, const Matrix& traj_tr, const Matrix& V, 
        int n, std::vector<Vector>& plot_upper, std::vector<Vector>& plot_lower, 
        std::size_t _DIM_PLOT_, bool _ORIGINAL_SYS_PLOT_) {
    Vector plot_vector = Vector(n);
    plot_vector(0) = traj_time;
    if(_ORIGINAL_SYS_PLOT_) {
        plot_vector(1) = (V*traj_tr.col(0))(_DIM_PLOT_);
        plot_upper.push_back(plot_vector);
        plot_vector(1) = (V*traj_tr.col(1))(_DIM_PLOT_);
    } else {
        plot_vector(1) = (traj_tr.col(0))(_DIM_PLOT_);
        plot_upper.push_back(plot_vector);
        plot_vector(1) = (traj_tr.col(1))(_DIM_PLOT_);
    }
    plot_lower.push_back(plot_vector);
}
void addSegment(std::vector<VPolytope<Number>>& flowpipe, std::vector<Vector>& ptsxmax_tr,
        std::vector<Vector>& ptsxmin_tr, std::size_t colorUpper, std::size_t colorLower) { 
    Plotter<Number>& plotter = Plotter<Number>::getInstance();
    VPolytope<Number> vpoly_upper = VPolytope<Number>(ptsxmax_tr);
    VPolytope<Number> vpoly_lower = VPolytope<Number>(ptsxmin_tr);
    flowpipe.push_back(vpoly_upper.unite(vpoly_lower));
    unsigned v = plotter.addObject(vpoly_upper.vertices());
    plotter.setObjectColor(v, colorUpper);
    unsigned w = plotter.addObject(vpoly_lower.vertices());
    plotter.setObjectColor(w, colorLower);
    ptsxmax_tr.erase( ptsxmax_tr.begin() );
    ptsxmin_tr.erase( ptsxmin_tr.begin() );
}
