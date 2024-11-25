/**
  ******************************************************************************
  * \file surfaceIntegration.h
  ******************************************************************************
  * The header file for an implementation of various functions required for
  * surface integrations for use in ASP.
  ******************************************************************************
  */

#ifndef SURFACEINTEGRATION_H
#define SURFACEINTEGRATION_H

#define USE_EIGEN
#include<vector_tools.h>
#include<error_tools.h>
#include<constitutive_tools.h>

namespace surfaceIntegration{

    typedef constitutiveTools::floatType floatType; //!< Define the float values type.
    typedef constitutiveTools::floatVector floatVector; //!< Define a vector of floats
    typedef constitutiveTools::floatMatrix floatMatrix; //!< Define a matrix of floats

    void decomposeSphere( const floatType &radius, const unsigned int &elementCount,
                          floatVector &points, std::vector<unsigned int> &connectivity );

    void buildSurfacePoints( const floatType &x0, const floatType &y0, const floatType &z0,
                             const floatType &dx, const floatType &dy,
                             const unsigned int n_points_x, const unsigned int n_points_y,
                             floatVector &points );

    void rotatePoints( const floatVector &points,
                       const floatType &thetaX, const floatType &thetaY, const floatType &thetaZ,
                       floatVector &rotatedPoints );

    void formSurfaceConnectivity( const std::vector< unsigned int > &surfaceIDs,
                                  const unsigned int &n_elements_x, const unsigned int &n_elements_y,
                                  unsigned int &index, std::vector< unsigned int > &connectivity );

    void formBaseCubePoints( const unsigned int &elementCount, floatVector &points );

    void formCubeConnectivity( const unsigned int &elementCount, std::vector< unsigned int > &connectivity );

    void evaluateQuadraticShapeFunctions( const floatType &xi, const floatType &eta, floatVector &shapeFunctions );

    void evaluateGradQuadraticShapeFunctions( const floatType &xi, const floatType &eta, floatMatrix &gradShapeFunctions );

    void interpolateFunction( const floatType &xi, const floatType &eta, const floatMatrix &nodalValues, floatVector &answer );

    void localGradientFunction( const floatType &xi, const floatType &eta, const floatMatrix &nodalValues, floatMatrix &answer );

    void localJacobian( const floatType &xi, const floatType &eta, const floatMatrix &nodalPositions, floatType &jacobian );

    void integrateFunction( const floatMatrix &nodalPositions, const floatMatrix &nodalValues, floatVector &answer );

    void integrateMesh( const floatVector &nodalPositions, const std::vector< unsigned int > &connectivity,
                        const floatVector &nodalValues, floatVector &answer );

}

#endif
