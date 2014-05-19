#ifndef QUANT_PDE_MODULES_IMPLICIT_EUROPEAN_SOLVER
#define QUANT_PDE_MODULES_IMPLICIT_EUROPEAN_SOLVER

namespace QuantPDE {

namespace Modules {

/*
class ImplicitEuropeanSolver final : public Solver1 {

	Vector v;
	const Grid *grid;
	PiecewiseLinear1 interpolation;

	void _BlackScholesEquation(const Constraint &constraint, Real now,
			Real next) {

		const BlackScholesEquation *bse =
				dynamic_cast<const BlackScholesEquation *>(
				&constraint);

		// TODO: Stuff

	}

protected:

	virtual void onStart() {
		// TODO: Smooth solution
	}

	virtual void beforeConstraints() {
		// Turn solution into vector
		v = grid.image(solution);
	}

	virtual void afterConstraints() {
		// Turn vector into solution
		solution = interpolation;
	}

public:

	ImplicitEuropeanSolver(const Problem1 &problem, const Grid1 &grid)
			noexcept : Solver1(problem), grid(&grid),
			interpolation(grid, v) {

		QUANT_PDE_REGISTER_ROUTINE(BlackScholesEquation);

	}

};
*/

} // Modules

} // QuantPDE

#endif

/*
template <typename T>
class ImplicitStep : public Event {

	const Function &r, &v, &q;

public:

	ImplicitStep(double previousTime, double nextTime,
			const Function &interest, const Function &volatility,
			const Function &dividends)
			: Event(previousTime, nextTime), r(interest),
			v(volatility), q(dividends) {
	}

	// TODO: Copy constructor

	virtual Vector advance(const Grid &G, const Vector &solution) const {
		const Axis &S = G(0);
		Index n = S.size();

		double dt = nextTime() - previousTime();

		// Sparse matrix with 3 nonzeros per column
		Matrix M = G.matrix();
		M.reserve(Eigen::VectorXi::Constant(n, 3));
		auto M_G = G.accessor(M);

		// Interior points
		// alpha_i dt V_{i-1}^{n+1} + (1 + (alpha_i + beta_i + r) dt)
		// 		V_i^{n+1} + beta_i dt V_{i+1}^{n+1} = V_i^n

		for(Index i = 1; i < n - 1; i++) {

			double r_i = r(nextTime(), S(i));
			double v_i = v(nextTime(), S(i));
			double q_i = q(nextTime(), S(i));

			double
				dSb = S(i)   - S(i-1),
				dSc = S(i+1) - S(i-1),
				dSf = S(i+1) - S(i)
			;

			double alpha_common = v_i * v_i * S(i) * S(i) / dSb
					/ dSc;
			double  beta_common = v_i * v_i * S(i) * S(i) / dSf
					/ dSc;

			// Central
			double alpha_i = alpha_common - (r_i - q_i) * S(i)
					/ dSc;
			double beta_i  =  beta_common + (r_i - q_i) * S(i)
					/ dSc;
			if(alpha_i < 0) {
				// Forward
				alpha_i = alpha_common;
				beta_i  =  beta_common + (r_i - q_i) * S(i)
						/ dSf;
			} else if(beta_i < 0) {
				// Backward
				alpha_i = alpha_common - (r_i - q_i) * S(i)
						/ dSb;
				beta_i  =  beta_common;
			}

			M_G(i, i - 1) = -alpha_i * dt;
			M_G(i, i)     = 1. + (alpha_i + beta_i + r_i) * dt;
			M_G(i, i + 1) = -beta_i * dt;

		}

		// Boundaries
		// Left:  (1 + r dt) V_i^{n+1} = V_i^n
		// Right:            V_i^{n+1} = V_i^n

		M_G(0, 0)         = 1. + r(nextTime(), S(0)) * dt;
		M_G(n - 1, n - 1) = 1.;

		M.makeCompressed();

		T solver;
		solver.compute(M);
		assert(solver.info() == Eigen::Success);

		Vector s = solver.solve(solution);
		assert(solver.info() == Eigen::Success);

		return s;
	}

};
*/
