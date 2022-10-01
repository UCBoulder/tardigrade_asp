/**
  ******************************************************************************
  * \file traction_separation.cpp
  ******************************************************************************
  * The source file for an implementation of various traction separation laws
  * for use in ASP. We will start with linear laws and, if required, progress
  * from there.
  ******************************************************************************
  */

#include<traction_separation.h>

namespace tractionSeparation{

    errorOut computeCurrentDistance( const floatVector &Xi_1, const floatVector &Xi_2, const floatVector &D,
                                     const floatVector &F,    const floatVector &chi,  const floatVector &gradChi,
                                     floatVector &d ){
        /*!
         * Compute the distance in the current configuration where
         * 
         * \f$d_i = dx_i - \xi_i^1 + \xi_i^2 \f$
         * 
         * \f$dx_i = F_{iI} dX_I \f$
         * 
         * \f$\xi_i^1 = \chi_{iI} \Xi_I^1 \f$
         * 
         * \f$\xi_i^2 = \left(\chi_{iI} + \chi_{iI,J} dX_J \right) \Xi_I^2 \f$
         * 
         * \f$dX_I = \Xi_I^1 + D_I - \Xi_I^2 \f$
         * 
         * \param &Xi_1: The micro-position vector for the local particle
         * \param &Xi_2: The micro-position vector for the non-local particle
         * \param &D: The initial separation between the particles
         * \param &F: The deformation gradient \f$\frac{dx_i}{dX_I}\f$
         * \param &chi: The micro-deformation
         * \param &gradChi: the gradient of the micro deformation w.r.t. the reference spatial variable
         * \param &d: The current separation between the particles
         */

        floatVector dX = Xi_1 + D - Xi_2;

        floatVector dx( dX.size( ), 0 );
        floatVector chi_2 = chi;

        for ( unsigned int i = 0; i < dX.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                dx[ i ] += F[ dX.size( ) * i + I ] * dX[ I ];

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_2[ dX.size( ) * i + I ] += gradChi[ dX.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                }

            }

        }

        floatVector xi_1( dX.size( ), 0 );
        floatVector xi_2( dX.size( ), 0 );

        for ( unsigned int i = 0; i < dX.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                xi_1[ i ] += chi[ dX.size( ) * i + I ] * Xi_1[ I ];
                xi_2[ i ] += chi_2[ dX.size( ) * i + I ] * Xi_2[ I ];

            }

        }

        d = dx - xi_1 + xi_2;

        return NULL;

    }

    errorOut computeCurrentDistance( const floatVector &Xi_1, const floatVector &Xi_2, const floatVector &D,
                                     const floatVector &F,    const floatVector &chi,  const floatVector &gradChi,
                                     floatVector &d,
                                     floatMatrix &dddXi_1, floatMatrix &dddXi_2, floatMatrix &dddD,
                                     floatMatrix &dddF, floatMatrix &dddChi, floatMatrix &dddGradChi ){
        /*!
         * Compute the distance in the current configuration where
         * 
         * \f$d_i = dx_i - \xi_i^1 + \xi_i^2 \f$
         * 
         * \f$dx_i = F_{iI} dX_I \f$
         * 
         * \f$\xi_i^1 = \chi_{iI} \Xi_I^1 \f$
         * 
         * \f$\xi_i^2 = \left(\chi_{iI} + \chi_{iI,J} dX_J \right) \Xi_I^2 \f$
         * 
         * \f$dX_I = \Xi_I^1 + D_I - \Xi_I^2 \f$
         * 
         * \param &Xi_1: The micro-position vector for the local particle
         * \param &Xi_2: The micro-position vector for the non-local particle
         * \param &D: The initial separation between the particles
         * \param &F: The deformation gradient \f$\frac{dx_i}{dX_I}\f$
         * \param &chi: The micro-deformation
         * \param &gradChi: the gradient of the micro deformation w.r.t. the reference spatial variable
         * \param &d: The current separation between the particles
         * \param &dddXi_1: The gradient of the separation w.r.t. the local reference micro-position vector
         * \param &dddXi_2: The gradient of the separation w.r.t. the non-local reference micro-position vector
         * \param &dddD: The gradient of the separation w.r.t. the local reference separation distance
         * \param &dddF: The gradient of the separation w.r.t. the deformation gradient
         * \param &dddChi: The gradient of the separation w.r.t. the micro deformation
         * \param &dddGradChi: The gradient of the separation w.r.t. the gradient of the micro deformation
         */

        floatVector dX = Xi_1 + D - Xi_2;

        floatVector dx( dX.size( ), 0 );
        floatVector chi_2 = chi;

        floatMatrix dxdF( dX.size( ), floatVector( F.size( ), 0 ) );
        floatMatrix dxdX( dX.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix dchi_2dGradChi( chi.size( ), floatVector( gradChi.size( ), 0 ) );
        floatMatrix dchi_2dX( chi.size( ), floatVector( dX.size( ), 0 ) );

        floatVector eye( dX.size( ) * dX.size( ), 0 );
        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < dX.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                dx[ i ] += F[ dX.size( ) * i + I ] * dX[ I ];

                dxdX[ i ][ I ] += F[ dX.size( ) * i + I ];

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_2[ dX.size( ) * i + I ] += gradChi[ dX.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                    dxdF[ i ][ dX.size( ) * I + J ] += eye[ dX.size( ) * i + I ] * dX[ J ];

                    dchi_2dX[ dX.size( ) * i + I ][ J ] += gradChi[ dX.size( ) * dX.size( ) * i + dX.size( ) * I + J ];

                    for ( unsigned int k = 0; k < dX.size( ); k++ ){

                        for ( unsigned int K = 0; K < dX.size( ); K++ ){

                            dchi_2dGradChi[ dX.size( ) * i + I ][ dX.size( ) * dX.size( ) * k + dX.size( ) * K + J ] += eye[ dX.size( ) * i + k ] * eye[ dX.size( ) * I + K ] * dX[ J ];

                        } 

                    }

                }

            }

        }

        floatVector xi_1( dX.size( ), 0 );
        floatVector xi_2( dX.size( ), 0 );

        floatMatrix dxi_1dChi( dX.size( ), floatVector( chi.size( ), 0 ) );
        floatMatrix dxi_2dChi_2( dX.size( ), floatVector( chi.size( ), 0 ) );

        floatMatrix dxi_1dXi_1( dX.size( ), floatVector( Xi_1.size( ), 0 ) );
        floatMatrix dxi_2dXi_2( dX.size( ), floatVector( Xi_2.size( ), 0 ) );

        for ( unsigned int i = 0; i < dX.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                xi_1[ i ] += chi[ dX.size( ) * i + I ] * Xi_1[ I ];
                xi_2[ i ] += chi_2[ dX.size( ) * i + I ] * Xi_2[ I ];

                dxi_1dXi_1[ i ][ I ] += chi[ dX.size( ) * i + I ];
                dxi_2dXi_2[ i ][ I ] += chi_2[ dX.size( ) * i + I ];

                for ( unsigned int K = 0; K < dX.size( ); K++ ){

                    dxi_1dChi[ i ][ dX.size( ) * I + K ] += eye[ dX.size( ) * i + I ] * Xi_1[ K ];
                    dxi_2dChi_2[ i ][ dX.size( ) * I + K ] += eye[ dX.size( ) * i + I ] * Xi_2[ K ];

                }

            }

        }

        d = dx - xi_1 + xi_2;

        dddF = dxdF;

        dddChi = -dxi_1dChi + dxi_2dChi_2;

        dddGradChi = vectorTools::dot( dxi_2dChi_2, dchi_2dGradChi );

        dddXi_1 = dxdX - dxi_1dXi_1 + vectorTools::dot( dxi_2dChi_2, dchi_2dX );

        dddXi_2 =  -dxdX + dxi_2dXi_2 - vectorTools::dot( dxi_2dChi_2, dchi_2dX );

        dddD    =  dxdX + vectorTools::dot( dxi_2dChi_2, dchi_2dX );

        return NULL;

    }

    errorOut computeCurrentDistance( const floatVector &Xi_1, const floatVector &Xi_2, const floatVector &D,
                                     const floatVector &F,    const floatVector &chi,  const floatVector &gradChi,
                                     floatVector &d,
                                     floatMatrix &dddXi_1, floatMatrix &dddXi_2, floatMatrix &dddD,
                                     floatMatrix &dddF, floatMatrix &dddChi, floatMatrix &dddGradChi,
                                     floatMatrix &d2ddFdXi_1,       floatMatrix &d2ddFdXi_2,       floatMatrix &d2ddFdD,
                                     floatMatrix &d2ddChidXi_1,     floatMatrix &d2ddChidXi_2,     floatMatrix &d2ddChidD,
                                     floatMatrix &d2ddGradChidXi_1, floatMatrix &d2ddGradChidXi_2, floatMatrix &d2ddGradChidD ){
        /*!
         * Compute the distance in the current configuration where
         * 
         * \f$d_i = dx_i - \xi_i^1 + \xi_i^2 \f$
         * 
         * \f$dx_i = F_{iI} dX_I \f$
         * 
         * \f$\xi_i^1 = \chi_{iI} \Xi_I^1 \f$
         * 
         * \f$\xi_i^2 = \left(\chi_{iI} + \chi_{iI,J} dX_J \right) \Xi_I^2 \f$
         * 
         * \f$dX_I = \Xi_I^1 + D_I - \Xi_I^2 \f$
         * 
         * \param &Xi_1: The micro-position vector for the local particle
         * \param &Xi_2: The micro-position vector for the non-local particle
         * \param &D: The initial separation between the particles
         * \param &F: The deformation gradient \f$\frac{dx_i}{dX_I}\f$
         * \param &chi: The micro-deformation
         * \param &gradChi: the gradient of the micro deformation w.r.t. the reference spatial variable
         * \param &d: The current separation between the particles
         * \param &dddF: The gradient of the separation w.r.t. the deformation gradient
         * \param &dddChi: The gradient of the separation w.r.t. the micro deformation
         * \param &dddGradChi: The gradient of the separation w.r.t. the gradient of the micro deformation
         * \param &d2ddFdXi_1: The second derivative of d w.r.t. F and \f$\Xi_1\f$
         * \param &d2ddFdXi_2: The second derivative of d w.r.t. F and \f$\Xi_2\f$
         * \param &d2ddFdD: The second derivative of d w.r.t. F and D
         * \param &d2ddChidXi_1: The second derivative of d w.r.t. \f$\chi\f$ and \f$\Xi_1\f$
         * \param &d2ddChidXi_2: The second derivative of d w.r.t. \f$\chi\f$ and \f$\Xi_2\f$
         * \param &d2ddChidD: The second derivative of d w.r.t. \f$\chi\f$ and D
         * \param &d2ddGradChidXi_1: The second derivative of d w.r.t. \f$\nabla_X \chi\f$ and \f$\Xi_1\f$
         * \param &d2ddGradChidXi_2: The second derivative of d w.r.t. \f$\nabla_X \chi\f$ and \f$\Xi_2\f$
         * \param &d2ddGradChidD: The second derivative of d w.r.t. \f$\nabla_X \chi\f$ and D
         */

        floatVector dX = Xi_1 + D - Xi_2;

        floatVector dx( dX.size( ), 0 );
        floatVector chi_2 = chi;

        floatMatrix dxdF( dX.size( ), floatVector( F.size( ), 0 ) );
        floatMatrix dxdX( dX.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix d2xdFdX( dX.size( ), floatVector( F.size( ) * dX.size( ), 0 ) );

        floatMatrix dchi_2dGradChi( chi.size( ), floatVector( gradChi.size( ), 0 ) );
        floatMatrix dchi_2dX( chi.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix d2chi_2dGradChidX( chi.size( ), floatVector( gradChi.size( ) * dX.size( ), 0 ) );

        floatVector eye( dX.size( ) * dX.size( ), 0 );
        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < dX.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                dx[ i ] += F[ dX.size( ) * i + I ] * dX[ I ];

                dxdX[ i ][ I ] += F[ dX.size( ) * i + I ];

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_2[ dX.size( ) * i + I ] += gradChi[ dX.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                    dxdF[ i ][ dX.size( ) * I + J ] += eye[ dX.size( ) * i + I ] * dX[ J ];

                    dchi_2dX[ dX.size( ) * i + I ][ J ] += gradChi[ dX.size( ) * dX.size( ) * i + dX.size( ) * I + J ];

                    for ( unsigned int k = 0; k < dX.size( ); k++ ){

                        d2xdFdX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + k ] += eye[ dX.size( ) * i + I ] * eye[ dX.size( ) * J + k ];

                        for ( unsigned int K = 0; K < dX.size( ); K++ ){

                            dchi_2dGradChi[ dX.size( ) * i + I ][ dX.size( ) * dX.size( ) * k + dX.size( ) * K + J ] += eye[ dX.size( ) * i + k ] * eye[ dX.size( ) * I + K ] * dX[ J ];

                            for ( unsigned int L = 0; L < dX.size( ); L++ ){

                                d2chi_2dGradChidX[ dX.size( ) * i + I ][ dX.size( ) * dX.size( ) * dX.size( ) * k + dX.size( ) * dX.size( ) * K + dX.size( ) * J + L ]
                                    += eye[ dX.size( ) * i + k ] * eye[ dX.size( ) * I + K ] * eye[ dX.size( ) * J + L ];

                            }

                        } 

                    }

                }

            }

        }

        floatVector xi_1( dX.size( ), 0 );
        floatVector xi_2( dX.size( ), 0 );

        floatMatrix dxi_1dChi( dX.size( ), floatVector( chi.size( ), 0 ) );
        floatMatrix dxi_2dChi_2( dX.size( ), floatVector( chi.size( ), 0 ) );

        floatMatrix d2xi_1dChidXi_1( dX.size( ), floatVector( chi.size( ) * Xi_1.size( ), 0 ) );
        floatMatrix d2xi_2dChi_2dXi_2( dX.size( ), floatVector( chi.size( ) * Xi_1.size( ), 0 ) );

        floatMatrix dxi_1dXi_1( dX.size( ), floatVector( Xi_1.size( ), 0 ) );
        floatMatrix dxi_2dXi_2( dX.size( ), floatVector( Xi_2.size( ), 0 ) );

        for ( unsigned int i = 0; i < dX.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                xi_1[ i ] += chi[ dX.size( ) * i + I ] * Xi_1[ I ];
                xi_2[ i ] += chi_2[ dX.size( ) * i + I ] * Xi_2[ I ];

                dxi_1dXi_1[ i ][ I ] += chi[ dX.size( ) * i + I ];
                dxi_2dXi_2[ i ][ I ] += chi_2[ dX.size( ) * i + I ];

                for ( unsigned int K = 0; K < dX.size( ); K++ ){

                    dxi_1dChi[ i ][ dX.size( ) * I + K ] += eye[ dX.size( ) * i + I ] * Xi_1[ K ];
                    dxi_2dChi_2[ i ][ dX.size( ) * I + K ] += eye[ dX.size( ) * i + I ] * Xi_2[ K ];

                    for ( unsigned int L = 0; L < dX.size( ); L++ ){

                        d2xi_1dChidXi_1[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * K + L ] += eye[ dX.size( ) * i + I ] * eye[ dX.size( ) * K + L ];
                        d2xi_2dChi_2dXi_2[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * K + L ] += eye[ dX.size( ) * i + I ] * eye[ dX.size( ) * K + L ];

                    }

                }

            }

        }

        d = dx - xi_1 + xi_2;

        dddF = dxdF;

        dddChi = -dxi_1dChi + dxi_2dChi_2;

        dddGradChi = vectorTools::dot( dxi_2dChi_2, dchi_2dGradChi );

        dddXi_1 = dxdX - dxi_1dXi_1 + vectorTools::dot( dxi_2dChi_2, dchi_2dX );

        dddXi_2 =  -dxdX + dxi_2dXi_2 - vectorTools::dot( dxi_2dChi_2, dchi_2dX );

        dddD    =  dxdX + vectorTools::dot( dxi_2dChi_2, dchi_2dX );

        d2ddFdXi_1       = d2xdFdX;
        d2ddChidXi_1     = -d2xi_1dChidXi_1;
        d2ddGradChidXi_1 = vectorTools::dot( dxi_2dChi_2, d2chi_2dGradChidX );

        d2ddFdXi_2       = -d2xdFdX;
        d2ddChidXi_2     = d2xi_2dChi_2dXi_2;
        d2ddGradChidXi_2 = -vectorTools::dot( dxi_2dChi_2, d2chi_2dGradChidX );

        d2ddFdD          = d2xdFdX;
        d2ddChidD        = floatMatrix( d.size( ), floatVector( chi.size( ) * D.size( ), 0 ) );
        d2ddGradChidD    = vectorTools::dot( dxi_2dChi_2, d2chi_2dGradChidX );

        for ( unsigned int i = 0; i < dX.size( ); i++ ){
            for ( unsigned int j = 0; j < dX.size( ); j++ ){
                for ( unsigned int J = 0; J < dX.size( ); J++ ){
                    for ( unsigned int K = 0; K < dX.size( ); K++ ){
                        for ( unsigned int L = 0; L < dX.size( ); L++ ){
                            for ( unsigned int a = 0; a < dX.size( ); a++ ){
                                for ( unsigned int A = 0; A < dX.size( ); A++ ){
                                     d2ddGradChidXi_2[ i ][ dX.size( ) * dX.size( ) * dX.size( ) * j + dX.size( ) * dX.size( ) * J + dX.size( ) * K + L ]
                                         += d2xi_2dChi_2dXi_2[ i ][ dX.size( ) * dX.size( ) * a + dX.size( ) * A + L ] * dchi_2dGradChi[ dX.size( ) * a + A ][ dX.size( ) * dX.size( ) * j + dX.size( ) * J + K ];
                                }

                            }

                        }

                    }

                }

            }

        }

        return NULL;

    }

    errorOut decomposeVector( const floatVector &d, const floatVector &n,
                              floatVector &dn, floatVector &dt ){
        /*!
         * Decompose a vector into a normal part and a tangential part via
         * 
         * \f$ d^n_i = d_a n_a n_i \f$
         * 
         * \f$ d^t_i = d_i - d^n_i \f$
         * 
         * \param &d: The vector to decompose
         * \param &n: The normal vector ( assumed to be unit length ).
         * \param &dn: The part of the vector in the normal direction
         * \param &dt: The tangential part of the vector
         */

        if ( !vectorTools::fuzzyEquals( vectorTools::l2norm( n ), 1. ) ){
            return new errorNode( __func__, "The normal vector isn't a unit vector!" );
        }

        dn = vectorTools::dot( d, n ) * n;

        dt = d - dn;

        return NULL;

    }

    errorOut decomposeVector( const floatVector &d, const floatVector &n,
                              floatVector &dn, floatVector &dt,
                              floatMatrix &ddndd, floatMatrix &ddndn,
                              floatMatrix &ddtdd, floatMatrix &ddtdn ){
        /*!
         * Decompose a vector into a normal part and a tangential part via
         * 
         * \f$ d^n_i = d_a n_a n_i \f$
         * 
         * \f$ d^t_i = d_i - d^n_i \f$
         * 
         * \param &d: The vector to decompose
         * \param &n: The normal vector ( assumed to be unit length ).
         * \param &dn: The part of the vector in the normal direction
         * \param &dt: The tangential part of the vector
         * \param &ddndd: The derivative of the normal part of the vector w.r.t. the original vector
         * \param &ddndn: The derivative of the normal part of the vector w.r.t. the normal vector
         * \param &ddtdd: The derivative of the tangential part of the vector w.r.t. the original vector
         * \param &ddtdn: The derivative of the tangential part of the vector w.r.t. the normal vector
         */

        if ( !vectorTools::fuzzyEquals( vectorTools::l2norm( n ), 1. ) ){
            return new errorNode( __func__, "The normal vector isn't a unit vector!" );
        }

        dn = vectorTools::dot( d, n ) * n;

        ddndd = vectorTools::dyadic( n, n );

        ddndn = vectorTools::dyadic( n, d ) + vectorTools::dot( d, n ) * vectorTools::eye<floatType>( d.size( ) );

        dt = d - dn;

        ddtdd = vectorTools::eye<floatType>( d.size( ) ) - ddndd;

        ddtdn = -ddndn;

        return NULL;

    }

    errorOut decomposeVector( const floatVector &d, const floatVector &n,
                              floatVector &dn, floatVector &dt,
                              floatMatrix &ddndd, floatMatrix &ddndn,
                              floatMatrix &ddtdd, floatMatrix &ddtdn,
                              floatMatrix &d2dndddd, floatMatrix &d2dndddn,
                              floatMatrix &d2dndndn,
                              floatMatrix &d2dtdddd, floatMatrix &d2dtdddn,
                              floatMatrix &d2dtdndn ){
        /*!
         * Decompose a vector into a normal part and a tangential part via
         * 
         * \f$ d^n_i = d_a n_a n_i \f$
         * 
         * \f$ d^t_i = d_i - d^n_i \f$
         * 
         * \param &d: The vector to decompose
         * \param &n: The normal vector ( assumed to be unit length ).
         * \param &dn: The part of the vector in the normal direction
         * \param &dt: The tangential part of the vector
         * \param &ddndd: The derivative of the normal part of the vector w.r.t. the original vector
         * \param &ddndn: The derivative of the normal part of the vector w.r.t. the normal vector
         * \param &ddtdd: The derivative of the tangential part of the vector w.r.t. the original vector
         * \param &ddtdn: The derivative of the tangential part of the vector w.r.t. the normal vector
         * \param &d2dndddd: The second derivative of the normal part of the vector w.r.t. the original vector
         * \param &d2dndddn: The second derivative of the normal part of the vector w.r.t. the original vector and the normal vector
         * \param &d2dndndn: The second derivative of the normal part of the vector w.r.t. the normal vector
         * \param &d2dtdddd: The second derivative of the tangential part of the vector w.r.t. the original vector
         * \param &d2dtdddn: The second derivative of the tangential part of the vector w.r.t. the original vector and the normal vector
         * \param &d2dtdndn: The second derivative of the tangential part of the vector w.r.t. the normal vector
         */

        if ( !vectorTools::fuzzyEquals( vectorTools::l2norm( n ), 1. ) ){
            return new errorNode( __func__, "The normal vector isn't a unit vector!" );
        }

        dn = vectorTools::dot( d, n ) * n;

        ddndd = vectorTools::dyadic( n, n );

        ddndn = vectorTools::dyadic( n, d ) + vectorTools::dot( d, n ) * vectorTools::eye<floatType>( d.size( ) );

        dt = d - dn;

        ddtdd = vectorTools::eye<floatType>( d.size( ) ) - ddndd;

        ddtdn = -ddndn;

        d2dndddd = floatMatrix( d.size( ), floatVector( d.size( ) * d.size( ), 0 ) );
        d2dndddn = floatMatrix( d.size( ), floatVector( d.size( ) * d.size( ), 0 ) );
        d2dndndn = floatMatrix( d.size( ), floatVector( d.size( ) * d.size( ), 0 ) );

        d2dtdddd = floatMatrix( d.size( ), floatVector( d.size( ) * d.size( ), 0 ) );
        d2dtdddn = floatMatrix( d.size( ), floatVector( d.size( ) * d.size( ), 0 ) );
        d2dtdndn = floatMatrix( d.size( ), floatVector( d.size( ) * d.size( ), 0 ) );

        floatVector eye( d.size( ) * d.size( ) );
        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < d.size( ); i++ ){

            for ( unsigned int j = 0; j < d.size( ); j++ ){

                for ( unsigned int k = 0; k < d.size( ); k++ ){

                    d2dndddn[ i ][ d.size( ) * j + k ] += eye[ d.size( ) * i + k ] * n[ j ] + n[ i ] * eye[ d.size( ) * j + k ];
                    d2dndndn[ i ][ d.size( ) * j + k ] += eye[ d.size( ) * i + k ] * d[ j ] + d[ k ] * eye[ d.size( ) * i + j ];

                }

            }

        }

        d2dtdddn = -d2dndddn;
        d2dtdndn = -d2dndndn;

        return NULL;

    }

    errorOut computeLinearTractionEnergy( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
                                          const floatVector &parameters, floatType &energy ){
        /*!
         * Compute the linear traction-separation energy
         * 
         * \f$ e^t = \frac{1}{2} \left[ E^n d^n_i d^n_i + E^t d^t_i d^t_i\right]\f$
         * 
         * \param &normalDeformationMeasure: The normal deformation measure \f$ d^n_i \f$
         * \param &tangentialDeformationMeasure: The tangential deformation measure \f$ d^t_i \f$
         * \param &parameters: The material parameters \f$ E^n \f$ and \f$ E^t \f$.
         * \param &energy: The returned energy value \f$ e^t \f$
         */

        if ( parameters.size( ) != 2 ){

            return new errorNode( __func__, "Two parameters are required for the traction separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        }

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        energy = 0.5 * ( En * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ) + Et * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) );

        return NULL;

    }

    errorOut computeLinearTractionEnergy( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
                                          const floatVector &parameters, floatType &energy,
                                          floatVector &denergyddn, floatVector &denergyddt ){
        /*!
         * Compute the linear traction-separation energy
         * 
         * \f$ e^t = \frac{1}{2} \left[ E^n d^n_i d^n_i + E^t d^t_i d^t_i\right]\f$
         * 
         * \param &normalDeformationMeasure: The normal deformation measure \f$ d^n_i \f$
         * \param &tangentialDeformationMeasure: The tangential deformation measure \f$ d^t_i \f$
         * \param &parameters: The material parameters \f$ E^n \f$ and \f$ E^t \f$.
         * \param &energy: The returned energy value \f$ e^t \f$
         * \param &denergyddn: The derivative of the energy w.r.t. the normal deformation measure
         * \param &denergyddt: The derivative of the energy w.r.t. the tangential deformation measure
         */

        if ( parameters.size( ) != 2 ){

            return new errorNode( __func__, "Two parameters are required for the traction separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        }

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        energy = 0.5 * ( En * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ) + Et * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) );

        denergyddn = En * normalDeformationMeasure;

        denergyddt = Et * tangentialDeformationMeasure;

        return NULL;

    }

    errorOut computeLinearTractionEnergy( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
                                          const floatVector &parameters, floatType &energy,
                                          floatVector &denergyddn, floatVector &denergyddt,
                                          floatVector &d2energyddnddn, floatVector &d2energyddnddt,
                                          floatVector &d2energyddtddt ){
        /*!
         * Compute the linear traction-separation energy
         * 
         * \f$ e^t = \frac{1}{2} \left[ E^n d^n_i d^n_i + E^t d^t_i d^t_i\right]\f$
         * 
         * \param &normalDeformationMeasure: The normal deformation measure \f$ d^n_i \f$
         * \param &tangentialDeformationMeasure: The tangential deformation measure \f$ d^t_i \f$
         * \param &parameters: The material parameters \f$ E^n \f$ and \f$ E^t \f$.
         * \param &energy: The returned energy value \f$ e^t \f$
         * \param &denergyddn: The derivative of the energy w.r.t. the normal deformation measure
         * \param &denergyddt: The derivative of the energy w.r.t. the tangential deformation measure
         * \param &d2energyddnddn: The second derivative of the energy w.r.t. the normal deformation measure
         * \param &d2energyddnddt: The second derivative of the energy w.r.t. the normal and tangential deformation measures
         * \param &d2energyddtddt: The second derivative of the energy w.r.t. the tangential deformation measure
         */

        if ( parameters.size( ) != 2 ){

            return new errorNode( __func__, "Two parameters are required for the traction separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        }

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        energy = 0.5 * ( En * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ) + Et * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) );

        denergyddn = En * normalDeformationMeasure;

        denergyddt = Et * tangentialDeformationMeasure;

        floatVector eye( normalDeformationMeasure.size( ) * normalDeformationMeasure.size( ) );

        vectorTools::eye( eye );

        d2energyddnddn = En * eye;

        d2energyddnddt = floatVector( eye.size( ), 0 );

        d2energyddtddt = Et * eye;

        return NULL;

    }

    errorOut computeLinearTractionEnergy( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
                                          const floatVector &parameters, floatType &energy,
                                          floatVector &denergyddn, floatVector &denergyddt, floatVector &denergydParameters ){
        /*!
         * Compute the linear traction-separation energy
         * 
         * \f$ e^t = \frac{1}{2} \left[ E^n d^n_i d^n_i + E^t d^t_i d^t_i\right]\f$
         * 
         * \param &normalDeformationMeasure: The normal deformation measure \f$ d^n_i \f$
         * \param &tangentialDeformationMeasure: The tangential deformation measure \f$ d^t_i \f$
         * \param &parameters: The material parameters \f$ E^n \f$ and \f$ E^t \f$.
         * \param &energy: The returned energy value \f$ e^t \f$
         * \param &denergyddn: The derivative of the energy w.r.t. the normal deformation measure
         * \param &denergyddt: The derivative of the energy w.r.t. the tangential deformation measure
         * \param &denergydParameters: The derivative of the energy w.r.t. the parameters
         */

        if ( parameters.size( ) != 2 ){

            return new errorNode( __func__, "Two parameters are required for the traction separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        }

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        energy = 0.5 * ( En * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ) + Et * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) );

        denergyddn = En * normalDeformationMeasure;

        denergyddt = Et * tangentialDeformationMeasure;

        denergydParameters = { 0.5 * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ),
                               0.5 * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) };

        return NULL;
    }

    errorOut computeLinearTractionEnergy( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
                                          const floatVector &parameters, floatType &energy,
                                          floatVector &denergyddn, floatVector &denergyddt, floatVector &denergydParameters,
                                          floatVector &d2energyddnddn, floatVector &d2energyddnddt, floatVector &d2energyddndParameters,
                                          floatVector &d2energyddtddt, floatVector &d2energyddtdParameters,
                                          floatVector &d2energydParametersdParameters ){
        /*!
         * Compute the linear traction-separation energy
         * 
         * \f$ e^t = \frac{1}{2} \left[ E^n d^n_i d^n_i + E^t d^t_i d^t_i\right]\f$
         * 
         * \param &normalDeformationMeasure: The normal deformation measure \f$ d^n_i \f$
         * \param &tangentialDeformationMeasure: The tangential deformation measure \f$ d^t_i \f$
         * \param &parameters: The material parameters \f$ E^n \f$ and \f$ E^t \f$.
         * \param &energy: The returned energy value \f$ e^t \f$
         * \param &denergyddn: The derivative of the energy w.r.t. the normal deformation measure
         * \param &denergyddt: The derivative of the energy w.r.t. the tangential deformation measure
         * \param &denergydParameters: The derivative of the energy w.r.t. the parameters
         * \param &d2energyddnddn: The second derivative of the energy w.r.t. the normal deformation measure
         * \param &d2energyddnddt: The second derivative of the energy w.r.t. the normal and tangential deformation measures
         * \param &d2energyddnddt: The second derivative of the energy w.r.t. the normal deformation measure and the parameters
         * \param &d2energyddtdParameters: The second derivative of the energy w.r.t. the tangential deformation measure and the parameters
         * \param &d2energyddtdParameters: The second derivative of the energy w.r.t. the parameters
         */

        if ( parameters.size( ) != 2 ){

            return new errorNode( __func__, "Two parameters are required for the traction separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        }

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        energy = 0.5 * ( En * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ) + Et * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) );

        denergyddn = En * normalDeformationMeasure;

        denergyddt = Et * tangentialDeformationMeasure;

        denergydParameters = { 0.5 * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ),
                               0.5 * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) };

        floatVector eye( normalDeformationMeasure.size( ) * normalDeformationMeasure.size( ) );

        vectorTools::eye( eye );

        d2energyddnddn = En * eye;

        d2energyddnddt = floatVector( eye.size( ), 0 );

        d2energyddndParameters = floatVector( normalDeformationMeasure.size( ) * parameters.size( ), 0 );

        for ( unsigned int i = 0; i < normalDeformationMeasure.size( ); i++ ){

            d2energyddndParameters[ parameters.size( ) * i ] += normalDeformationMeasure[ i ];

        }

        d2energyddtddt = Et * eye;

        d2energyddtdParameters = floatVector( tangentialDeformationMeasure.size( ) * parameters.size( ), 0 );

        for ( unsigned int i = 0; i < tangentialDeformationMeasure.size( ); i++ ){

            d2energyddtdParameters[ parameters.size( ) * i + 1 ] += tangentialDeformationMeasure[ i ];

        }

        d2energydParametersdParameters = floatVector( parameters.size( ) * parameters.size( ), 0 );

        return NULL;

    }

}
