// Ceres Solver - A fast non-linear least squares minimizer
// Copyright 2012 Google Inc. All rights reserved.
// http://code.google.com/p/ceres-solver/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of Google Inc. nor the names of its contributors may be
//   used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Author: sameeragarwal@google.com (Sameer Agarwal)
//
// NIST non-linear regression problems solved using Ceres.
//
// The data was obtained from
// http://www.itl.nist.gov/div898/strd/nls/nls_main.shtml, where more
// background on these problems can also be found.
//
// Currently not all problems are solved successfully. Some of the
// failures are due to convergence to a local minimum, and some fail
// because of numerical issues.
//
// TODO(sameeragarwal): Fix numerical issues so that all the problems
// converge and then look at convergence to the wrong solution issues.

#include <iostream>
#include <fstream>
#include "ceres/ceres.h"
#include "ceres/split.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "Eigen/Core"

DEFINE_string(nist_data_dir, "", "Directory containing the NIST non-linear"
              "regression examples");
DEFINE_string(trust_region_strategy, "levenberg_marquardt",
              "Options are: levenberg_marquardt, dogleg");
DEFINE_string(dogleg, "traditional_dogleg",
              "Options are: traditional_dogleg, subspace_dogleg");
DEFINE_string(linear_solver, "dense_qr", "Options are: "
              "sparse_cholesky, dense_qr, dense_normal_cholesky and"
              "cgnr");
DEFINE_string(preconditioner, "jacobi", "Options are: "
              "identity, jacobi");
DEFINE_int32(num_iterations, 10000, "Number of iterations");
DEFINE_bool(nonmonotonic_steps, false, "Trust region algorithm can use"
            " nonmonotic steps");
DEFINE_double(initial_trust_region_radius, 1e4, "Initial trust region radius");

using Eigen::Dynamic;
using Eigen::RowMajor;
typedef Eigen::Matrix<double, Dynamic, 1> Vector;
typedef Eigen::Matrix<double, Dynamic, Dynamic, RowMajor> Matrix;

bool GetAndSplitLine(std::ifstream& ifs, std::vector<std::string>* pieces) {
  pieces->clear();
  char buf[256];
  ifs.getline(buf, 256);
  ceres::SplitStringUsing(std::string(buf), " ", pieces);
  return true;
}

void SkipLines(std::ifstream& ifs, int num_lines) {
  char buf[256];
  for (int i = 0; i < num_lines; ++i) {
    ifs.getline(buf, 256);
  }
}

bool IsSuccessfulTermination(ceres::SolverTerminationType status) {
  return
      (status == ceres::FUNCTION_TOLERANCE) ||
      (status == ceres::GRADIENT_TOLERANCE) ||
      (status == ceres::PARAMETER_TOLERANCE) ||
      (status == ceres::USER_SUCCESS);
}

class NISTProblem {
 public:
  explicit NISTProblem(const std::string& filename) {
    std::ifstream ifs(filename.c_str(), std::ifstream::in);

    std::vector<std::string> pieces;
    SkipLines(ifs, 24);
    GetAndSplitLine(ifs, &pieces);
    const int kNumResponses = std::atoi(pieces[1].c_str());

    GetAndSplitLine(ifs, &pieces);
    const int kNumPredictors = std::atoi(pieces[0].c_str());

    GetAndSplitLine(ifs, &pieces);
    const int kNumObservations = std::atoi(pieces[0].c_str());

    SkipLines(ifs, 4);
    GetAndSplitLine(ifs, &pieces);
    const int kNumParameters = std::atoi(pieces[0].c_str());
    SkipLines(ifs, 8);

    // Get the first line of initial and final parameter values to
    // determine the number of tries.
    GetAndSplitLine(ifs, &pieces);
    const int kNumTries = pieces.size() - 4;

    predictor_.resize(kNumObservations, kNumPredictors);
    response_.resize(kNumObservations, kNumResponses);
    initial_parameters_.resize(kNumTries, kNumParameters);
    final_parameters_.resize(1, kNumParameters);

    // Parse the line for parameter b1.
    int parameter_id = 0;
    for (int i = 0; i < kNumTries; ++i) {
      initial_parameters_(i, parameter_id) = std::atof(pieces[i + 2].c_str());
    }
    final_parameters_(0, parameter_id) = std::atof(pieces[2 + kNumTries].c_str());

    // Parse the remaining parameter lines.
    for (int parameter_id = 1; parameter_id < kNumParameters; ++parameter_id) {
     GetAndSplitLine(ifs, &pieces);
     // b2, b3, ....
     for (int i = 0; i < kNumTries; ++i) {
       initial_parameters_(i, parameter_id) = std::atof(pieces[i + 2].c_str());
     }
     final_parameters_(0, parameter_id) = std::atof(pieces[2 + kNumTries].c_str());
    }

    // Certfied cost
    SkipLines(ifs, 1);
    GetAndSplitLine(ifs, &pieces);
    certified_cost_ = std::atof(pieces[4].c_str()) / 2.0;

    // Read the observations.
    SkipLines(ifs, 18 - kNumParameters);
    for (int i = 0; i < kNumObservations; ++i) {
      GetAndSplitLine(ifs, &pieces);
      // Response.
      for (int j = 0; j < kNumResponses; ++j) {
        response_(i, j) =  std::atof(pieces[j].c_str());
      }

      // Predictor variables.
      for (int j = 0; j < kNumPredictors; ++j) {
        predictor_(i, j) =  std::atof(pieces[j + kNumResponses].c_str());
      }
    }
  }

  Matrix initial_parameters(int start) const { return initial_parameters_.row(start); }
  Matrix final_parameters() const  { return final_parameters_; }
  Matrix predictor()        const { return predictor_;         }
  Matrix response()         const { return response_;          }
  int predictor_size()      const { return predictor_.cols();  }
  int num_observations()    const { return predictor_.rows();  }
  int response_size()       const { return response_.cols();   }
  int num_parameters()      const { return initial_parameters_.cols(); }
  int num_starts()          const { return initial_parameters_.rows(); }
  double certified_cost()   const { return certified_cost_; }

 private:
  Matrix predictor_;
  Matrix response_;
  Matrix initial_parameters_;
  Matrix final_parameters_;
  double certified_cost_;
};

#define NIST_BEGIN(CostFunctionName) \
  struct CostFunctionName { \
    CostFunctionName(const double* const x, \
                     const double* const y) \
        : x_(*x), y_(*y) {} \
    double x_; \
    double y_; \
    template <typename T> \
    bool operator()(const T* const b, T* residual) const { \
    const T y(y_); \
    const T x(x_); \
      residual[0] = y - (

#define NIST_END ); return true; }};

// y = b1 * (b2+x)**(-1/b3)  +  e
NIST_BEGIN(Bennet5)
  b[0] * pow(b[1] + x, T(-1.0) / b[2])
NIST_END

// y = b1*(1-exp[-b2*x])  +  e
NIST_BEGIN(BoxBOD)
  b[0] * (T(1.0) - exp(-b[1] * x))
NIST_END

// y = exp[-b1*x]/(b2+b3*x)  +  e
NIST_BEGIN(Chwirut)
  exp(-b[0] * x) / (b[1] + b[2] * x)
NIST_END

// y  = b1*x**b2  +  e
NIST_BEGIN(DanWood)
  b[0] * pow(x, b[1])
NIST_END

// y = b1*exp( -b2*x ) + b3*exp( -(x-b4)**2 / b5**2 )
//     + b6*exp( -(x-b7)**2 / b8**2 ) + e
NIST_BEGIN(Gauss)
  b[0] * exp(-b[1] * x) +
  b[2] * exp(-pow((x - b[3])/b[4], 2)) +
  b[5] * exp(-pow((x - b[6])/b[7],2))
NIST_END

// y = b1*exp(-b2*x) + b3*exp(-b4*x) + b5*exp(-b6*x)  +  e
NIST_BEGIN(Lanczos)
  b[0] * exp(-b[1] * x) + b[2] * exp(-b[3] * x) + b[4] * exp(-b[5] * x)
NIST_END

// y = (b1+b2*x+b3*x**2+b4*x**3) /
//     (1+b5*x+b6*x**2+b7*x**3)  +  e
NIST_BEGIN(Hahn1)
  (b[0] + b[1] * x + b[2] * x * x + b[3] * x * x * x) /
  (T(1.0) + b[4] * x + b[5] * x * x + b[6] * x * x * x)
NIST_END

// y = (b1 + b2*x + b3*x**2) /
//    (1 + b4*x + b5*x**2)  +  e
NIST_BEGIN(Kirby2)
  (b[0] + b[1] * x + b[2] * x * x) /
  (T(1.0) + b[3] * x + b[4] * x * x)
NIST_END

// y = b1*(x**2+x*b2) / (x**2+x*b3+b4)  +  e
NIST_BEGIN(MGH09)
  b[0] * (x * x + x * b[1]) / (x * x + x * b[2] + b[3])
NIST_END

// y = b1 * exp[b2/(x+b3)]  +  e
NIST_BEGIN(MGH10)
  b[0] * exp(b[1] / (x + b[2]))
NIST_END

// y = b1 + b2*exp[-x*b4] + b3*exp[-x*b5]
NIST_BEGIN(MGH17)
  b[0] + b[1] * exp(-x * b[3]) + b[2] * exp(-x * b[4])
NIST_END

// y = b1*(1-exp[-b2*x])  +  e
NIST_BEGIN(Misra1a)
  b[0] * (T(1.0) - exp(-b[1] * x))
NIST_END

// y = b1 * (1-(1+b2*x/2)**(-2))  +  e
NIST_BEGIN(Misra1b)
  b[0] * (T(1.0) - T(1.0)/ ((T(1.0) + b[1] * x / 2.0) * (T(1.0) + b[1] * x / 2.0)))
NIST_END

// y = b1 * (1-(1+2*b2*x)**(-.5))  +  e
NIST_BEGIN(Misra1c)
  b[0] * (T(1.0) - pow(T(1.0) + T(2.0) * b[1] * x, -0.5))
NIST_END

// y = b1*b2*x*((1+b2*x)**(-1))  +  e
NIST_BEGIN(Misra1d)
  b[0] * b[1] * x / (T(1.0) + b[1] * x)
NIST_END

const double kPi = 3.141592653589793238462643383279;
// pi = 3.141592653589793238462643383279E0
// y =  b1 - b2*x - arctan[b3/(x-b4)]/pi  +  e
NIST_BEGIN(Roszman1)
  b[0] - b[1] * x - atan2(b[2], (x - b[3]))/T(kPi)
NIST_END

// y = b1 / (1+exp[b2-b3*x])  +  e
NIST_BEGIN(Rat42)
  b[0] / (T(1.0) + exp(b[1] - b[2] * x))
NIST_END

// y = b1 / ((1+exp[b2-b3*x])**(1/b4))  +  e
NIST_BEGIN(Rat43)
  b[0] / pow(T(1.0) + exp(b[1] - b[2] * x), T(1.0) / b[3])
NIST_END

// y = (b1 + b2*x + b3*x**2 + b4*x**3) /
//    (1 + b5*x + b6*x**2 + b7*x**3)  +  e
NIST_BEGIN(Thurber)
  (b[0] + b[1] * x + b[2] * x * x  + b[3] * x * x * x) /
  (T(1.0) + b[4] * x + b[5] * x * x + b[6] * x * x * x)
NIST_END

// y = b1 + b2*cos( 2*pi*x/12 ) + b3*sin( 2*pi*x/12 )
//        + b5*cos( 2*pi*x/b4 ) + b6*sin( 2*pi*x/b4 )
//        + b8*cos( 2*pi*x/b7 ) + b9*sin( 2*pi*x/b7 )  + e
NIST_BEGIN(ENSO)
  b[0] + b[1] * cos(T(2.0 * kPi) * x / T(12.0)) +
         b[2] * sin(T(2.0 * kPi) * x / T(12.0)) +
         b[4] * cos(T(2.0 * kPi) * x / b[3]) +
         b[5] * sin(T(2.0 * kPi) * x / b[3]) +
         b[7] * cos(T(2.0 * kPi) * x / b[6]) +
         b[8] * sin(T(2.0 * kPi) * x / b[6])
NIST_END

// y = (b1/b2) * exp[-0.5*((x-b3)/b2)**2]  +  e
NIST_BEGIN(Eckerle4)
  b[0] / b[1] * exp(T(-0.5) * pow((x - b[2])/b[1], 2))
NIST_END

struct Nelson {
 public:
  Nelson(const double* const x, const double* const y)
      : x1_(x[0]), x2_(x[1]), y_(y[0]) {}

  template <typename T>
  bool operator()(const T* const b, T* residual) const {
    // log[y] = b1 - b2*x1 * exp[-b3*x2]  +  e
    residual[0] = T(log(y_)) - (b[0] - b[1] * T(x1_) * exp(-b[2] * T(x2_)));
    return true;
  }

 private:
  double x1_;
  double x2_;
  double y_;
};

template <typename Model, int num_residuals, int num_parameters>
int RegressionDriver(const std::string& filename,
                      const ceres::Solver::Options& options) {
  NISTProblem nist_problem(FLAGS_nist_data_dir + filename);
  CHECK_EQ(num_residuals, nist_problem.response_size());
  CHECK_EQ(num_parameters, nist_problem.num_parameters());

  Matrix predictor = nist_problem.predictor();
  Matrix response = nist_problem.response();
  Matrix final_parameters = nist_problem.final_parameters();
  std::vector<ceres::Solver::Summary> summaries(nist_problem.num_starts() + 1);
  std::cerr << filename << std::endl;

  // Each NIST problem comes with multiple starting points, so we
  // construct the problem from scratch for each case and solve it.
  for (int start = 0; start < nist_problem.num_starts(); ++start) {
    Matrix initial_parameters = nist_problem.initial_parameters(start);

    ceres::Problem problem;
    for (int i = 0; i < nist_problem.num_observations(); ++i) {
      problem.AddResidualBlock(
          new ceres::AutoDiffCostFunction<Model, num_residuals, num_parameters>(
              new Model(predictor.data() + nist_problem.predictor_size() * i,
                        response.data() + nist_problem.response_size() * i)),
          NULL,
          initial_parameters.data());
    }

    Solve(options, &problem, &summaries[start]);
  }

  const double certified_cost = nist_problem.certified_cost();

  int num_success = 0;
  const int kMinNumMatchingDigits = 4;
  for (int start = 0; start < nist_problem.num_starts(); ++start) {
    const ceres::Solver::Summary& summary = summaries[start];

    int num_matching_digits = 0;
    if (IsSuccessfulTermination(summary.termination_type)
        && summary.final_cost < certified_cost) {
      num_matching_digits = kMinNumMatchingDigits + 1;
    } else {
      num_matching_digits =
          -std::log10(fabs(summary.final_cost - certified_cost) / certified_cost);
    }

    std::cerr << "start " << start + 1 << " " ;
    if (num_matching_digits <= kMinNumMatchingDigits) {
      std::cerr <<  "FAILURE";
    } else {
      std::cerr <<  "SUCCESS";
      ++num_success;
    }
    std::cerr << " summary: "
              << summary.BriefReport()
              << " Certified cost: " << certified_cost
              << std::endl;

  }

  return num_success;
}

void SetMinimizerOptions(ceres::Solver::Options* options) {
  CHECK(ceres::StringToLinearSolverType(FLAGS_linear_solver,
                                        &options->linear_solver_type));
  CHECK(ceres::StringToPreconditionerType(FLAGS_preconditioner,
                                          &options->preconditioner_type));
  CHECK(ceres::StringToTrustRegionStrategyType(
            FLAGS_trust_region_strategy,
            &options->trust_region_strategy_type));
  CHECK(ceres::StringToDoglegType(FLAGS_dogleg, &options->dogleg_type));

  options->max_num_iterations = FLAGS_num_iterations;
  options->use_nonmonotonic_steps = FLAGS_nonmonotonic_steps;
  options->initial_trust_region_radius = FLAGS_initial_trust_region_radius;
  options->function_tolerance = 1e-18;
  options->gradient_tolerance = 1e-18;
  options->parameter_tolerance = 1e-18;
}

void SolveNISTProblems() {
  if (FLAGS_nist_data_dir.empty()) {
    LOG(FATAL) << "Must specify the directory containing the NIST problems";
  }

  ceres::Solver::Options options;
  SetMinimizerOptions(&options);

  std::cerr << "Lower Difficulty\n";
  int easy_success = 0;
  easy_success += RegressionDriver<Misra1a,  1, 2>("Misra1a.dat",  options);
  easy_success += RegressionDriver<Chwirut,  1, 3>("Chwirut1.dat", options);
  easy_success += RegressionDriver<Chwirut,  1, 3>("Chwirut2.dat", options);
  easy_success += RegressionDriver<Lanczos,  1, 6>("Lanczos3.dat", options);
  easy_success += RegressionDriver<Gauss,    1, 8>("Gauss1.dat",   options);
  easy_success += RegressionDriver<Gauss,    1, 8>("Gauss2.dat",   options);
  easy_success += RegressionDriver<DanWood,  1, 2>("DanWood.dat",  options);
  easy_success += RegressionDriver<Misra1b,  1, 2>("Misra1b.dat",  options);

  std::cerr << "\nMedium Difficulty\n";
  int medium_success = 0;
  medium_success += RegressionDriver<Kirby2,   1, 5>("Kirby2.dat",   options);
  medium_success += RegressionDriver<Hahn1,    1, 7>("Hahn1.dat",    options);
  medium_success += RegressionDriver<Nelson,   1, 3>("Nelson.dat",   options);
  medium_success += RegressionDriver<MGH17,    1, 5>("MGH17.dat",    options);
  medium_success += RegressionDriver<Lanczos,  1, 6>("Lanczos1.dat", options);
  medium_success += RegressionDriver<Lanczos,  1, 6>("Lanczos2.dat", options);
  medium_success += RegressionDriver<Gauss,    1, 8>("Gauss3.dat",   options);
  medium_success += RegressionDriver<Misra1c,  1, 2>("Misra1c.dat",  options);
  medium_success += RegressionDriver<Misra1d,  1, 2>("Misra1d.dat",  options);
  medium_success += RegressionDriver<Roszman1, 1, 4>("Roszman1.dat", options);
  medium_success += RegressionDriver<ENSO,     1, 9>("ENSO.dat",     options);

  std::cerr << "\nHigher Difficulty\n";
  int hard_success = 0;
  hard_success += RegressionDriver<MGH09,    1, 4>("MGH09.dat",    options);
  hard_success += RegressionDriver<Thurber,  1, 7>("Thurber.dat",  options);
  hard_success += RegressionDriver<BoxBOD,   1, 2>("BoxBOD.dat",   options);
  hard_success += RegressionDriver<Rat42,    1, 3>("Rat42.dat",    options);
  hard_success += RegressionDriver<MGH10,    1, 3>("MGH10.dat",    options);

  hard_success += RegressionDriver<Eckerle4, 1, 3>("Eckerle4.dat", options);
  hard_success += RegressionDriver<Rat43,    1, 4>("Rat43.dat",    options);
  hard_success += RegressionDriver<Bennet5,  1, 3>("Bennett5.dat", options);

  std::cerr << "\n";
  std::cerr << "Easy    : " << easy_success << "/16\n";
  std::cerr << "Medium  : " << medium_success << "/22\n";
  std::cerr << "Hard    : " << hard_success << "/16\n";
  std::cerr << "Total   : " << easy_success + medium_success + hard_success << "/54\n";
}

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  SolveNISTProblems();
  return 0;
};
