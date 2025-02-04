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

    void computeCurrentDistanceGeneral( const floatVector &Xi_1, const floatVector &Xi_2, const floatVector &D,
                                            const floatVector &F,    const floatVector &chi,  const floatVector &chiNL,
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
         * \param &Xi_1: The relative micro-position vector for the local particle
         * \param &Xi_2: The relative micro-position vector for the non-local particle
         * \param &D: The initial separation between the particles
         * \param &F: The deformation gradient \f$\frac{dx_i}{dX_I}\f$
         * \param &chi: The micro-deformation
         * \param &chiNL: the non-local micro-deformation at \f$\Xi_2\f$
         * \param &d: The current separation between the particles
         */

        floatVector dX = Xi_1 + D - Xi_2;

        floatVector dx( dX.size( ), 0 );

        floatVector xi_1( dX.size( ), 0 );

        floatVector xi_2( dX.size( ), 0 );

        for ( unsigned int i = 0; i < dX.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                dx[ i ] += F[ dX.size( ) * i + I ] * dX[ I ];

                xi_1[ i ] += chi[ dX.size( ) * i + I ] * Xi_1[ I ];

                xi_2[ i ] += chiNL[ dX.size( ) * i + I ] * Xi_2[ I ];

            }

        }

        d = dx - xi_1 + xi_2;

        return;

    }

    void computeCurrentDistanceGeneral( const floatVector &Xi_1, const floatVector &Xi_2, const floatVector &D,
                                            const floatVector &F,    const floatVector &chi,  const floatVector &chiNL,
                                            floatVector &d,
                                            floatMatrix &dddXi_1, floatMatrix &dddXi_2, floatMatrix &dddD,
                                            floatMatrix &dddF, floatMatrix &dddchi, floatMatrix &dddchiNL ){
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
         * \param &Xi_1: The reference micro-position vector for the local particle
         * \param &Xi_2: The reference micro-position vector for the non-local particle
         * \param &D: The initial separation between the particles
         * \param &F: The deformation gradient \f$\frac{dx_i}{dX_I}\f$
         * \param &chi: The micro-deformation
         * \param &chiNL: the non-local micro-deformation at \f$\Xi_2\f$
         * \param &d: The current separation between the particles
         * \param &dddXi_1: The gradient of d w.r.t. the local reference relative position vector
         * \param &dddXi_2: The gradient of d w.r.t. the non-local reference relative position vector
         * \param &dddD: The gradient of d w.r.t. the reference distance vector
         * \param &dddF: The gradient of d w.r.t. the deformation gradient
         * \param &dddchi: The gradient of d w.r.t. the local micro-deformation tensor
         * \param &dddchiNL: The gradient of d w.r.t. the non-local micro-deformation tensor
         */
        floatVector dX = Xi_1 + D - Xi_2;

        floatVector dx( dX.size( ), 0 );

        floatVector xi_1( dX.size( ), 0 );

        floatVector xi_2( dX.size( ), 0 );

        dddXi_1  = floatMatrix( dX.size( ), floatVector( dX.size( ), 0 ) );

        dddXi_2  = floatMatrix( dX.size( ), floatVector( dX.size( ), 0 ) );

        dddD     = floatMatrix( dX.size( ), floatVector( dX.size( ), 0 ) );

        dddF     = floatMatrix( dX.size( ), floatVector( dX.size( ) * dX.size( ), 0 ) );

        dddchi   = floatMatrix( dX.size( ), floatVector( dX.size( ) * dX.size( ), 0 ) );

        dddchiNL = floatMatrix( dX.size( ), floatVector( dX.size( ) * dX.size( ), 0 ) );

        floatVector eye( dX.size( ) * dX.size( ), 0 );

        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < dX.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                dx[ i ] += F[ dX.size( ) * i + I ] * dX[ I ];

                xi_1[ i ] += chi[ dX.size( ) * i + I ] * Xi_1[ I ];

                xi_2[ i ] += chiNL[ dX.size( ) * i + I ] * Xi_2[ I ];

                dddXi_1[ i ][ I ] =  F[ dX.size( ) * i + I ] - chi[ dX.size( ) * i + I ];

                dddXi_2[ i ][ I ] = -F[ dX.size( ) * i + I ] + chiNL[ dX.size( ) * i + I ];

                dddD[ i ][ I ] = F[ dX.size( ) * i + I ];

                for ( unsigned int A = 0; A < dX.size( ); A++ ){

                    dddF[ i ][ dX.size( ) * I + A ]     =  eye[ dX.size( ) * i + I ] * dX[ A ];

                    dddchi[ i ][ dX.size( ) * I + A ]   = -eye[ dX.size( ) * i + I ] * Xi_1[ A ];

                    dddchiNL[ i ][ dX.size( ) * I + A ] =  eye[ dX.size( ) * i + I ] * Xi_2[ A ];

                }

            }

        }

        d = dx - xi_1 + xi_2;

        return;

    }

    void computeCurrentDistanceGeneral( const floatVector &Xi_1, const floatVector &Xi_2, const floatVector &D,
                                            const floatVector &F,    const floatVector &chi,  const floatVector &chiNL,
                                            floatVector &d,
                                            floatMatrix &dddXi_1, floatMatrix &dddXi_2, floatMatrix &dddD,
                                            floatMatrix &dddF, floatMatrix &dddchi, floatMatrix &dddchiNL,
                                            floatMatrix &d2ddFdXi_1, floatMatrix &d2ddchidXi_1,
                                            floatMatrix &d2ddFdXi_2, floatMatrix &d2ddchiNLdXi_2,
                                            floatMatrix &d2ddFdD ){
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
         * \param &Xi_1: The reference micro-position vector for the local particle
         * \param &Xi_2: The reference micro-position vector for the non-local particle
         * \param &D: The initial separation between the particles
         * \param &F: The deformation gradient \f$\frac{dx_i}{dX_I}\f$
         * \param &chi: The micro-deformation
         * \param &chiNL: the non-local micro-deformation at \f$\Xi_2\f$
         * \param &d: The current separation between the particles
         * \param &dddXi_1: The gradient of d w.r.t. the local reference relative position vector
         * \param &dddXi_2: The gradient of d w.r.t. the non-local reference relative position vector
         * \param &dddD: The gradient of d w.r.t. the reference distance vector
         * \param &dddF: The gradient of d w.r.t. the deformation gradient
         * \param &dddchi: The gradient of d w.r.t. the local micro-deformation tensor
         * \param &dddchiNL: The gradient of d w.r.t. the non-local micro-deformation tensor,
         * \param &d2ddFdXi_1: The second gradient of d w.r.t. the deformation gradient and the local reference relative position vector
         * \param &d2ddchidXi_1: The second gradient of d w.r.t. the local micro-deformation tensor and the local reference relative position vector
         * \param &d2ddFdXi_2: The second gradient of d w.r.t. the deformation gradient and the non-local reference relative position vector
         * \param &d2ddchiNLdXi_2: The second gradient of d w.r.t. the non-local micro-deformation tensor and the non-local reference relative position vector
         * \param &d2ddFdD: The second gradient of d w.r.t. the deformation gradient and the non-local reference distance vector
         */
        floatVector dX = Xi_1 + D - Xi_2;

        floatVector dx( dX.size( ), 0 );

        floatVector xi_1( dX.size( ), 0 );

        floatVector xi_2( dX.size( ), 0 );

        dddXi_1  = floatMatrix( dX.size( ), floatVector( dX.size( ), 0 ) );

        dddXi_2  = floatMatrix( dX.size( ), floatVector( dX.size( ), 0 ) );

        dddD     = floatMatrix( dX.size( ), floatVector( dX.size( ), 0 ) );

        dddF     = floatMatrix( dX.size( ), floatVector( dX.size( ) * dX.size( ), 0 ) );

        dddchi   = floatMatrix( dX.size( ), floatVector( dX.size( ) * dX.size( ), 0 ) );

        dddchiNL = floatMatrix( dX.size( ), floatVector( dX.size( ) * dX.size( ), 0 ) );

        d2ddFdXi_1     = floatMatrix( dX.size( ), floatVector( dX.size( ) * F.size( ), 0 ) );

        d2ddchidXi_1   = floatMatrix( dX.size( ), floatVector( dX.size( ) * chi.size( ), 0 ) );

        d2ddFdXi_2     = floatMatrix( dX.size( ), floatVector( dX.size( ) * F.size( ), 0 ) );

        d2ddchiNLdXi_2 = floatMatrix( dX.size( ), floatVector( dX.size( ) * chi.size( ), 0 ) );

        d2ddFdD        = floatMatrix( dX.size( ), floatVector( dX.size( ) * F.size( ), 0 ) );

        floatVector eye( dX.size( ) * dX.size( ), 0 );

        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < dX.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                dx[ i ] += F[ dX.size( ) * i + I ] * dX[ I ];

                xi_1[ i ] += chi[ dX.size( ) * i + I ] * Xi_1[ I ];

                xi_2[ i ] += chiNL[ dX.size( ) * i + I ] * Xi_2[ I ];

                dddXi_1[ i ][ I ] =  F[ dX.size( ) * i + I ] - chi[ dX.size( ) * i + I ];

                dddXi_2[ i ][ I ] = -F[ dX.size( ) * i + I ] + chiNL[ dX.size( ) * i + I ];

                dddD[ i ][ I ] = F[ dX.size( ) * i + I ];

                for ( unsigned int A = 0; A < dX.size( ); A++ ){

                    dddF[ i ][ dX.size( ) * I + A ]     =  eye[ dX.size( ) * i + I ] * dX[ A ];

                    dddchi[ i ][ dX.size( ) * I + A ]   = -eye[ dX.size( ) * i + I ] * Xi_1[ A ];

                    dddchiNL[ i ][ dX.size( ) * I + A ] =  eye[ dX.size( ) * i + I ] * Xi_2[ A ];

                    for ( unsigned int a = 0; a < dX.size( ); a++ ){

                        d2ddFdXi_1[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * A + a ]     +=  eye[ dX.size( ) * i + I ] * eye[ dX.size( ) * A + a ];

                        d2ddchidXi_1[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * A + a ]   += -eye[ dX.size( ) * i + I ] * eye[ dX.size( ) * A + a ];

                        d2ddFdXi_2[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * A + a ]     += -eye[ dX.size( ) * i + I ] * eye[ dX.size( ) * A + a ];

                        d2ddchiNLdXi_2[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * A + a ] += eye[ dX.size( ) * i + I ] * eye[ dX.size( ) * A + a ];

                        d2ddFdD[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * A + a ]        +=  eye[ dX.size( ) * i + I ] * eye[ dX.size( ) * A + a ];

                    }

                }

            }

        }

        d = dx - xi_1 + xi_2;

        return;

    }

    void computeCurrentDistance( const floatVector &Xi_1, const floatVector &Xi_2, const floatVector &D,
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

        return;

    }

    void computeCurrentDistance( const floatVector &Xi_1, const floatVector &Xi_2, const floatVector &D,
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

        return;

    }

    void computeCurrentDistance( const floatVector &Xi_1, const floatVector &Xi_2, const floatVector &D,
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
         * \param &dddXi_1: The gradient of the separation w.r.t. the local reference micro-vector
         * \param &dddXi_2: The gradient of the separation w.r.t. the non-local reference micro-vector
         * \param &dddD: The gradient of the separation w.r.t. the initial separation between the particles
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

        return;

    }

    void decomposeVector( const floatVector &d, const floatVector &n,
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

        TARDIGRADE_ERROR_TOOLS_CHECK( vectorTools::fuzzyEquals( vectorTools::l2norm( n ), 1. ), "The normal vector isn't a unit vector!" );

        dn = vectorTools::dot( d, n ) * n;

        dt = d - dn;

        return;

    }

    void decomposeVector( const floatVector &d, const floatVector &n,
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

        TARDIGRADE_ERROR_TOOLS_CHECK( vectorTools::fuzzyEquals( vectorTools::l2norm( n ), 1. ), "The normal vector isn't a unit vector!" );

        dn = vectorTools::dot( d, n ) * n;

        ddndd = vectorTools::dyadic( n, n );

        ddndn = vectorTools::dyadic( n, d ) + vectorTools::dot( d, n ) * vectorTools::eye<floatType>( d.size( ) );

        dt = d - dn;

        ddtdd = vectorTools::eye<floatType>( d.size( ) ) - ddndd;

        ddtdn = -ddndn;

        return;

    }

    void decomposeVector( const floatVector &d, const floatVector &n,
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

        TARDIGRADE_ERROR_TOOLS_CHECK( vectorTools::fuzzyEquals( vectorTools::l2norm( n ), 1. ), "The normal vector isn't a unit vector!" );

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

        return;

    }

    void computeLinearTraction( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
                                    const floatVector &parameters, floatVector &traction ){
        /*!
         * Compute the linear traction
         * 
         * \f$ = E^n d^n_i + E^t d^t_i\f$
         * 
         * \param &normalDeformationMeasure: The normal deformation measure \f$ d^n_i \f$
         * \param &tangentialDeformationMeasure: The tangential deformation measure \f$ d^t_i \f$
         * \param &parameters: The material parameters \f$ E^n \f$ and \f$ E^t \f$.
         * \param &traction: The resulting traction
         */

        TARDIGRADE_ERROR_TOOLS_CHECK( parameters.size( ) == 2, "Two parameters are required for the traction-separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        traction = En * normalDeformationMeasure + Et * tangentialDeformationMeasure;

        return;

    }

    void computeLinearTraction( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
                                    const floatVector &parameters, floatVector &traction,
                                    floatMatrix &dtractionddn, floatMatrix &dtractionddt, floatMatrix &dtractiondp ){
        /*!
         * Compute the linear traction
         * 
         * \f$ = E^n d^n_i + E^t d^t_i\f$
         * 
         * \param &normalDeformationMeasure: The normal deformation measure \f$ d^n_i \f$
         * \param &tangentialDeformationMeasure: The tangential deformation measure \f$ d^t_i \f$
         * \param &parameters: The material parameters \f$ E^n \f$ and \f$ E^t \f$.
         * \param &traction: The resulting traction
         * \param &dtractionddn: The derivative of the traction w.r.t. the normal deformation measure
         * \param &dtractionddt: The derivative of the traction w.r.t. the tangential deformation measure
         * \param &dtractiondp: The derivative of the traction w.r.t. the parameters
         */

        TARDIGRADE_ERROR_TOOLS_CHECK( parameters.size( ) == 2, "Two parameters are required for the traction-separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        traction = En * normalDeformationMeasure + Et * tangentialDeformationMeasure;

        dtractionddn = floatMatrix( traction.size( ), floatVector( normalDeformationMeasure.size( ), 0 ) );

        dtractionddt = floatMatrix( traction.size( ), floatVector( tangentialDeformationMeasure.size( ), 0 ) );

        dtractiondp = floatMatrix( traction.size( ), floatVector( parameters.size( ), 0 ) );

        for ( unsigned int i = 0; i < traction.size( ); i++ ){

            dtractionddn[ i ][ i ] = En;

            dtractionddt[ i ][ i ] = Et;

            dtractiondp[ i ] = { normalDeformationMeasure[ i ], tangentialDeformationMeasure[ i ] };

        }

        return;

    }

    void computeLinearTraction( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
                                    const floatVector &parameters, floatVector &traction,
                                    floatMatrix &dtractionddn, floatMatrix &dtractionddt, floatMatrix &dtractiondp,
                                    floatMatrix &d2tractionddndp, floatMatrix &d2tractionddtdp ){
        /*!
         * Compute the linear traction
         * 
         * \f$ = E^n d^n_i + E^t d^t_i\f$
         * 
         * \param &normalDeformationMeasure: The normal deformation measure \f$ d^n_i \f$
         * \param &tangentialDeformationMeasure: The tangential deformation measure \f$ d^t_i \f$
         * \param &parameters: The material parameters \f$ E^n \f$ and \f$ E^t \f$.
         * \param &traction: The resulting traction
         * \param &dtractionddn: The derivative of the traction w.r.t. the normal deformation measure
         * \param &dtractionddt: The derivative of the traction w.r.t. the tangential deformation measure
         * \param &dtractiondp: The derivative of the traction w.r.t. the parameters
         * \param &d2tractionddndp: The second derivative of the traction w.r.t. the normal deformation measure and the parameters
         * \param &d2tractionddtdp: The second derivative of the traction w.r.t. the tangential deformation measure and the parameters
         */

        TARDIGRADE_ERROR_TOOLS_CHECK( parameters.size( ) == 2, "Two parameters are required for the traction-separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        traction = En * normalDeformationMeasure + Et * tangentialDeformationMeasure;

        dtractionddn = floatMatrix( traction.size( ), floatVector( normalDeformationMeasure.size( ), 0 ) );

        dtractionddt = floatMatrix( traction.size( ), floatVector( tangentialDeformationMeasure.size( ), 0 ) );

        dtractiondp = floatMatrix( traction.size( ), floatVector( parameters.size( ), 0 ) );

        d2tractionddndp = floatMatrix( traction.size( ), floatVector( normalDeformationMeasure.size( ) * parameters.size( ), 0 ) );

        d2tractionddtdp = floatMatrix( traction.size( ), floatVector( tangentialDeformationMeasure.size( ) * parameters.size( ), 0 ) );

        for ( unsigned int i = 0; i < traction.size( ); i++ ){

            dtractionddn[ i ][ i ] = En;

            dtractionddt[ i ][ i ] = Et;

            dtractiondp[ i ] = { normalDeformationMeasure[ i ], tangentialDeformationMeasure[ i ] };

            d2tractionddndp[ i ][ parameters.size( ) * i + 0 ] = 1.;

            d2tractionddtdp[ i ][ parameters.size( ) * i + 1 ] = 1.;

        }

        return;

    }

    void computeLinearTractionEnergy( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
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

        TARDIGRADE_ERROR_TOOLS_CHECK( parameters.size( ) == 2, "Two parameters are required for the traction-separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        energy = 0.5 * ( En * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ) + Et * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) );

        return;

    }

    void computeLinearTractionEnergy( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
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

        TARDIGRADE_ERROR_TOOLS_CHECK( parameters.size( ) == 2, "Two parameters are required for the traction-separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        energy = 0.5 * ( En * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ) + Et * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) );

        denergyddn = En * normalDeformationMeasure;

        denergyddt = Et * tangentialDeformationMeasure;

        return;

    }

    void computeLinearTractionEnergy( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
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

        TARDIGRADE_ERROR_TOOLS_CHECK( parameters.size( ) == 2, "Two parameters are required for the traction-separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

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

        return;

    }

    void computeLinearTractionEnergy( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
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

        TARDIGRADE_ERROR_TOOLS_CHECK( parameters.size( ) == 2, "Two parameters are required for the traction-separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

        floatType En = parameters[ 0 ];

        floatType Et = parameters[ 1 ];

        energy = 0.5 * ( En * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ) + Et * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) );

        denergyddn = En * normalDeformationMeasure;

        denergyddt = Et * tangentialDeformationMeasure;

        denergydParameters = { 0.5 * vectorTools::dot( normalDeformationMeasure, normalDeformationMeasure ),
                               0.5 * vectorTools::dot( tangentialDeformationMeasure, tangentialDeformationMeasure ) };

        return;
    }

    void computeLinearTractionEnergy( const floatVector &normalDeformationMeasure, const floatVector &tangentialDeformationMeasure,
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
         * \param &d2energyddndParameters: The second derivative of the energy w.r.t. the normal deformation measure and the parameters
         * \param &d2energyddtddt: The second derivative of the energy w.r.t. the tangential deformation measures
         * \param &d2energyddtdParameters: The second derivative of the energy w.r.t. the tangential deformation measure and the parameters
         * \param &d2energydParametersdParameters: The second derivative of the energy w.r.t. the parameters
         */

        TARDIGRADE_ERROR_TOOLS_CHECK( parameters.size( ) == 2, "Two parameters are required for the traction-separation law. " + std::to_string( parameters.size( ) ) + " are provided." );

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

        return;

    }

    void computeNansonsRelation( const floatVector &deformationGradient, const floatVector &dAN, floatVector &dan ){
        /*!
         * Compute Nanson's relation
         * 
         * \f$ da n_i = J dA N_I F_{Ii}^{-1} \f$
         * 
         * where \f$ J \f$ is the determinant of the deformation gradient \f$ F \f$
         * 
         * \param &deformationGradient: The deformation gradient \f$ F \f$
         * \param &dAN: The product of the reference area \f$ dA \f$ and the reference normal \f$ N \f$
         * \param &dan: The mapping of \f$ dA N_I \f$ to the current configuration.
         */

        TARDIGRADE_ERROR_TOOLS_CHECK( deformationGradient.size( ) == ( dAN.size( ) * dAN.size( ) ), "The deformation gradient must have " + std::to_string( dAN.size( ) * dAN.size( ) ) + " terms and has " + std::to_string( deformationGradient.size( ) ) );

        floatVector Finv = vectorTools::inverse( deformationGradient, dAN.size( ), dAN.size( ) );

        floatType J = vectorTools::determinant( deformationGradient, dAN.size( ), dAN.size( ) );

        dan = floatVector( dAN.size( ), 0 );

        for ( unsigned int i = 0; i < dAN.size( ); i++ ){

            for ( unsigned int I = 0; I < dAN.size( ); I++ ){

                dan[ i ] += J * dAN[ I ] * Finv[ dAN.size( ) * I + i ];

            }

        }

        return;

    }

    void computeNansonsRelation( const floatVector &deformationGradient, const floatVector &dAN, floatVector &dan,
                                     floatMatrix &ddandF, floatMatrix &ddanddAN ){
        /*!
         * Compute Nanson's relation
         * 
         * \f$ da n_i = J dA N_I F_{Ii}^{-1} \f$
         * 
         * where \f$ J \f$ is the determinant of the deformation gradient \f$ F \f$
         * 
         * \param &deformationGradient: The deformation gradient \f$ F \f$
         * \param &dAN: The product of the reference area \f$ dA \f$ and the reference normal \f$ N \f$
         * \param &dan: The mapping of \f$ dA N_I \f$ to the current configuration.
         * \param &ddandF: The gradient of the current surface area w.r.t. the deformation gradient
         * \param &ddanddAN: The gradient of the current surface area w.r.t. the reference surface area
         */

        TARDIGRADE_ERROR_TOOLS_CHECK( deformationGradient.size( ) == ( dAN.size( ) * dAN.size( ) ), "The deformation gradient must have " + std::to_string( dAN.size( ) * dAN.size( ) ) + " terms and has " + std::to_string( deformationGradient.size( ) ) );

        floatVector Finv = vectorTools::inverse( deformationGradient, dAN.size( ), dAN.size( ) );

        floatType J = vectorTools::determinant( deformationGradient, dAN.size( ), dAN.size( ) );

        dan = floatVector( dAN.size( ), 0 );

        ddandF = floatMatrix( dan.size( ), floatVector( deformationGradient.size( ), 0 ) );

        ddanddAN = floatMatrix( dan.size( ), floatVector( dAN.size( ), 0 ) );

        for ( unsigned int i = 0; i < dAN.size( ); i++ ){

            for ( unsigned int I = 0; I < dAN.size( ); I++ ){

                dan[ i ] += J * dAN[ I ] * Finv[ dAN.size( ) * I + i ];

                ddanddAN[ i ][ I ] += J * Finv[ dAN.size( ) * I + i ];

                for ( unsigned int a = 0; a < dAN.size( ); a++ ){

                    for ( unsigned int A = 0; A < dAN.size( ); A++ ){

                        ddandF[ i ][ dAN.size( ) * a + A ] += J * dAN[ I ] * ( Finv[ dAN.size( ) * I + i ] * Finv[ dAN.size( ) * A + a ] - Finv[ dAN.size( ) * A + i ] * Finv[ dAN.size( ) * I + a ] );

                    }

                }

            }

        }

        return;

    }

    void computeNansonsRelation( const floatVector &deformationGradient, const floatVector &dAN, floatVector &dan,
                                     floatMatrix &ddandF, floatMatrix &ddanddAN,
                                     floatMatrix &d2dandFdF, floatMatrix &d2dandFddAN ){
        /*!
         * Compute Nanson's relation
         * 
         * \f$ da n_i = J dA N_I F_{Ii}^{-1} \f$
         * 
         * where \f$ J \f$ is the determinant of the deformation gradient \f$ F \f$
         * 
         * \param &deformationGradient: The deformation gradient \f$ F \f$
         * \param &dAN: The product of the reference area \f$ dA \f$ and the reference normal \f$ N \f$
         * \param &dan: The mapping of \f$ dA N_I \f$ to the current configuration.
         * \param &ddandF: The gradient of the current surface area w.r.t. the deformation gradient
         * \param &ddanddAN: The gradient of the current surface area w.r.t. the reference surface area
         * \param &d2dandFdF: The second derivative of the current surface area w.r.t. the deformation gradient
         * \param &d2dandFddAN: The second derivative of the current surface area w.r.t. the deformation gradient and reference surface area
         */

        TARDIGRADE_ERROR_TOOLS_CHECK( deformationGradient.size( ) == ( dAN.size( ) * dAN.size( ) ), "The deformation gradient must have " + std::to_string( dAN.size( ) * dAN.size( ) ) + " terms and has " + std::to_string( deformationGradient.size( ) ) );

        floatVector Finv = vectorTools::inverse( deformationGradient, dAN.size( ), dAN.size( ) );

        floatType J = vectorTools::determinant( deformationGradient, dAN.size( ), dAN.size( ) );

        dan = floatVector( dAN.size( ), 0 );

        ddandF = floatMatrix( dan.size( ), floatVector( deformationGradient.size( ), 0 ) );

        ddanddAN = floatMatrix( dan.size( ), floatVector( dAN.size( ), 0 ) );

        d2dandFdF = floatMatrix( dan.size( ), floatVector( deformationGradient.size( ) * deformationGradient.size( ), 0 ) );

        d2dandFddAN = floatMatrix( dan.size( ), floatVector( deformationGradient.size( ) * dAN.size( ), 0 ) );

        for ( unsigned int i = 0; i < dAN.size( ); i++ ){

            for ( unsigned int I = 0; I < dAN.size( ); I++ ){

                dan[ i ] += J * dAN[ I ] * Finv[ dAN.size( ) * I + i ];

                ddanddAN[ i ][ I ] += J * Finv[ dAN.size( ) * I + i ];

                for ( unsigned int a = 0; a < dAN.size( ); a++ ){

                    for ( unsigned int A = 0; A < dAN.size( ); A++ ){

                        ddandF[ i ][ dAN.size( ) * a + A ] += J * dAN[ I ] * ( Finv[ dAN.size( ) * I + i ] * Finv[ dAN.size( ) * A + a ] - Finv[ dAN.size( ) * A + i ] * Finv[ dAN.size( ) * I + a ] );

                        d2dandFddAN[ i ][ dAN.size( ) * dAN.size( ) * a + dAN.size( ) * A + I ] += J * ( Finv[ dAN.size( ) * I + i ] * Finv[ dAN.size( ) * A + a ] - Finv[ dAN.size( ) * A + i ] * Finv[ dAN.size( ) * I + a ] );

                        for ( unsigned int b = 0; b < dAN.size( ); b++ ){

                            for ( unsigned int B = 0; B < dAN.size( ); B++ ){

                                d2dandFdF[ i ][ dAN.size( ) * dAN.size( ) * dAN.size( ) * a + dAN.size( ) * dAN.size( ) * A + dAN.size( ) * b + B ]
                                    += J * dAN[ I ] * Finv[ dAN.size( ) * B + b ] *  ( Finv[ dAN.size( ) * I + i ] * Finv[ dAN.size( ) * A + a ] - Finv[ dAN.size( ) * A + i ] * Finv[ dAN.size( ) * I + a ] )
                                     - J * dAN[ I ] * ( Finv[ dAN.size( ) * I + b ] * Finv[ dAN.size( ) * B + i ] * Finv[ dAN.size( ) * A + a ] + Finv[ dAN.size( ) * I + i ] * Finv[ dAN.size( ) * A + b ] * Finv[ dAN.size( ) * B + a ]
                                                      - Finv[ dAN.size( ) * A + b ] * Finv[ dAN.size( ) * B + i ] * Finv[ dAN.size( ) * I + a ] - Finv[ dAN.size( ) * A + i ] * Finv[ dAN.size( ) * I + b ] * Finv[ dAN.size( ) * B + a ] );

                            }

                        }

                    }

                }

            }

        }

        return;

    }

    void computeParticleOverlap( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                     const floatVector &F,    const floatVector &chi,  const floatVector &gradChi,
                                     floatVector &overlap ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I = \left(\chi_{iI} + \chi_{iI,J} dX_J\right) \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &gradChi: The gradient of the micro deformation tensor w.r.t. the reference spatial position
         * \param &overlap: The overlap vector
         */

        TARDIGRADE_ERROR_TOOLS_CHECK( chi.size( ) == ( Xi_1.size( ) * Xi_1.size( ) ), "The incoming chi vector has an inconsistent size with the micro-position vector\n  size is " + std::to_string( chi.size( ) ) + " and must be " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) );

        TARDIGRADE_ERROR_TOOLS_CHECK( gradChi.size( ) == ( Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ), "The gradient of the micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( gradChi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ) );

        // Compute the non-local micro-deformation tensor
        floatVector chi_nl = chi;

        for ( unsigned int i = 0; i < Xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_nl[ Xi_1.size( ) * i + I ] += gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                }

            }

        }

        TARDIGRADE_ERROR_TOOLS_CATCH( computeParticleOverlapChi_nl( Xi_1, dX, R_nl, F, chi, chi_nl, overlap ) );

        return;

    }

    void computeParticleOverlap( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                     const floatVector &F,    const floatVector &chi, const floatVector &chi_nl_basis, const floatVector &gradChi,
                                     floatVector &overlap ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I = \left(\chi_{iI}^{NL,basis} + \chi_{iI,J} dX_J\right) \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &chi_nl_basis: The non-local micro deformation tensor basis
         * \param &gradChi: The gradient of the micro deformation tensor w.r.t. the reference spatial position
         * \param &overlap: The overlap vector
         */

        TARDIGRADE_ERROR_TOOLS_CHECK( chi.size( ) == ( Xi_1.size( ) * Xi_1.size( ) ), "The incoming chi vector has an inconsistent size with the micro-position vector\n  size is " + std::to_string( chi.size( ) ) + " and must be " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) );

        TARDIGRADE_ERROR_TOOLS_CHECK( chi_nl_basis.size( ) == ( Xi_1.size( ) * Xi_1.size( ) ), "The incoming chi non-local basis vector has an inconsistent size with the micro-position vector\n  size is " + std::to_string( chi.size( ) ) + " and must be " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) );

        TARDIGRADE_ERROR_TOOLS_CHECK( gradChi.size( ) == ( Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ), "The gradient of the micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( gradChi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ) );

        // Compute the non-local micro-deformation tensor
        floatVector chi_nl = chi_nl_basis;

        for ( unsigned int i = 0; i < Xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_nl[ Xi_1.size( ) * i + I ] += gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                }

            }

        }

        TARDIGRADE_ERROR_TOOLS_CATCH( computeParticleOverlapChi_nl( Xi_1, dX, R_nl, F, chi, chi_nl, overlap ) );

        return;

    }

    void computeParticleOverlapChi_nl( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                           const floatVector &F,    const floatVector &chi,  const floatVector &chi_nl,
                                           floatVector &overlap ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &chi_nl: The non-local micro deformation tensor
         * \param &overlap: The overlap vector
         */

        TARDIGRADE_ERROR_TOOLS_CHECK( Xi_1.size( ) == dX.size( ), "The local micro relative position vector and the inter-particle spacing should have the same dimension\n\tXi_1: " + std::to_string( Xi_1.size( ) ) + "\n\tdX: " + std::to_string( dX.size( ) ) );

        TARDIGRADE_ERROR_TOOLS_CHECK( F.size( ) == ( dX.size( ) * dX.size( ) ), "The deformation gradient is not the expected dimension.\n\tF: " + std::to_string( F.size( ) ) + "\n\texpected: " + std::to_string( dX.size( ) * dX.size( ) ) );

        TARDIGRADE_ERROR_TOOLS_CHECK( chi.size( ) == ( Xi_1.size( ) * Xi_1.size( ) ), "The micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) );

        TARDIGRADE_ERROR_TOOLS_CHECK( chi_nl.size( ) == ( Xi_1.size( ) * Xi_1.size( ) ), "The non-local micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi_nl.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) );

        // Compute the current configuration local relative position vector and inter-particle spacing
        floatVector xi_1( Xi_1.size( ), 0 );

        for ( unsigned int i = 0; i < xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                xi_1[ i ] += chi[ Xi_1.size( ) * i + I ] * Xi_1[ I ];

            }

        }

        floatVector dx( dX.size( ), 0 );
        for ( unsigned int i = 0; i < dx.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                dx[ i ] += F[ dx.size( ) * i + I ] * dX[ I ];

            }

        }

        // Compute the representation of xi_1 in the non-local particle's reference frame
        floatVector xi_t = xi_1 - dx;

        // Compute the inverse of the non-local micro-deformation tensor
        TARDIGRADE_ERROR_TOOLS_CHECK( tardigradeVectorTools::determinant( chi_nl, xi_t.size( ), Xi_1.size( ) ) > 0, "The non-local micro deformation gradient has a determinant less than or equal to zero" );

        floatVector inv_chi_nl = vectorTools::inverse( chi_nl, xi_t.size( ), Xi_1.size( ) );

        floatVector Xi_t( Xi_1.size( ), 0 );

        for ( unsigned int I = 0; I < Xi_t.size( ); I++ ){

            for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

                Xi_t[ I ] += inv_chi_nl[ Xi_t.size( ) * I + i ] * xi_t[ i ];

            }

        }  

        if ( vectorTools::dot( Xi_t, Xi_t ) > ( R_nl * R_nl ) ){

            overlap = floatVector( xi_t.size( ), 0 );

        }
        else{

            TARDIGRADE_ERROR_TOOLS_CATCH( solveOverlapDistance( chi_nl, xi_t, R_nl, overlap ) );

        }

        return;

    }
    
    void computeParticleOverlap( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                     const floatVector &F,    const floatVector &chi,  const floatVector &gradChi,
                                     floatVector &overlap,
                                     floatMatrix &dOverlapdXi_1, floatMatrix &dOverlapddX, floatVector &dOverlapdR_nl,
                                     floatMatrix &dOverlapdF, floatMatrix &dOverlapdChi, floatMatrix &dOverlapdGradChi ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I = \left(\chi_{iI} + \chi_{iI,J} dX_J\right) \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &gradChi: The gradient of the micro deformation tensor w.r.t. the reference spatial position
         * \param &overlap: The overlap vector
         * \param &dOverlapdXi_1: The gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &dOverlapddX: The gradient of the overlap w.r.t. the local reference center-to-center vector
         * \param &dOverlapdR_nl: The gradient of the overlap w.r.t. the non-local reference radius
         * \param &dOverlapdF: The gradient of the overlap w.r.t. the deformation gradient
         * \param &dOverlapdChi: The gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &dOverlapdGradChi: The gradient of the overlap w.r.t. the reference spatial gradient of the micro-deformation tensor
         */

        if ( chi.size( ) != ( Xi_1.size( ) * Xi_1.size( ) ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The incoming chi vector has an inconsistent size with the micro-position vector\n  size is " + std::to_string( chi.size( ) ) + " and must be " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( gradChi.size( ) != Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The gradient of the micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( gradChi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ) ) );

        }

        // Compute the non-local micro-deformation tensor
        floatVector chi_nl = chi;

        floatMatrix dchi_nldchi( chi_nl.size( ), floatVector( chi.size( ), 0 ) );

        floatMatrix dchi_nlddX( chi.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix dchi_nldGradChi( chi.size( ), floatVector( gradChi.size( ), 0 ) );

        floatVector eye( Xi_1.size( ) * Xi_1.size( ) );
        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < Xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                dchi_nldchi[ Xi_1.size( ) * i + I ][ Xi_1.size( ) * i + I ] = 1.;

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_nl[ Xi_1.size( ) * i + I ] += gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                    dchi_nlddX[ Xi_1.size( ) * i + I ][ J ] = gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ];

                    for ( unsigned int K = 0; K < Xi_1.size( ); K++ ){

                        for ( unsigned int L = 0; L < dX.size( ); L++ ){

                            dchi_nldGradChi[ Xi_1.size( ) * i + I ][ Xi_1.size( ) * dX.size( ) * J + dX.size( ) * K + L ] = eye[ Xi_1.size( ) * i + J ] * eye[ Xi_1.size( ) * I + K ] * dX[ L ];

                        }

                    }

                }

            }

        }

        floatMatrix dOverlapdChi_nl;

        ERROR_TOOLS_CATCH( computeParticleOverlapChi_nl( Xi_1, dX, R_nl, F, chi, chi_nl, overlap,
                                                         dOverlapdXi_1, dOverlapddX, dOverlapdR_nl,
                                                         dOverlapdF, dOverlapdChi, dOverlapdChi_nl ) );

        dOverlapddX  += vectorTools::dot( dOverlapdChi_nl, dchi_nlddX );

        dOverlapdChi += vectorTools::dot( dOverlapdChi_nl, dchi_nldchi );

        dOverlapdGradChi = vectorTools::dot( dOverlapdChi_nl, dchi_nldGradChi );

        return;

    }

    void computeParticleOverlap( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                     const floatVector &F,    const floatVector &chi, const floatVector &chi_nl_basis, const floatVector &gradChi,
                                     floatVector &overlap,
                                     floatMatrix &dOverlapdXi_1, floatMatrix &dOverlapddX, floatVector &dOverlapdR_nl,
                                     floatMatrix &dOverlapdF, floatMatrix &dOverlapdChi, floatMatrix &dOverlapdChi_NL_B, floatMatrix &dOverlapdGradChi ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I = \left(\chi_{iI}^{NL,basis} + \chi_{iI,J} dX_J\right) \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &chi_nl_basis: The non-local micro deformation tensor basis
         * \param &gradChi: The gradient of the micro deformation tensor w.r.t. the reference spatial position
         * \param &overlap: The overlap vector
         * \param &dOverlapdXi_1: The gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &dOverlapddX: The gradient of the overlap w.r.t. the local reference center-to-center vector
         * \param &dOverlapdR_nl: The gradient of the overlap w.r.t. the non-local reference radius
         * \param &dOverlapdF: The gradient of the overlap w.r.t. the deformation gradient
         * \param &dOverlapdChi: The gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &dOverlapdChi_NL_B: The gradient of the overlap w.r.t. the non-local micro-deformation tensor basis
         * \param &dOverlapdGradChi: The gradient of the overlap w.r.t. the reference spatial gradient of the micro-deformation tensor
         */

        if ( chi.size( ) != ( Xi_1.size( ) * Xi_1.size( ) ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The incoming chi vector has an inconsistent size with the micro-position vector\n  size is " + std::to_string( chi.size( ) ) + " and must be " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( chi_nl_basis.size( ) != ( Xi_1.size( ) * Xi_1.size( ) ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The incoming chi non-local basis vector has an inconsistent size with the micro-position vector\n  size is " + std::to_string( chi_nl_basis.size( ) ) + " and must be " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( gradChi.size( ) != Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The gradient of the micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( gradChi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ) ) );

        }

        // Compute the non-local micro-deformation tensor
        floatVector chi_nl = chi_nl_basis;

        floatMatrix dchi_nlddX( chi.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix dchi_nldGradChi( chi.size( ), floatVector( gradChi.size( ), 0 ) );

        floatVector eye( Xi_1.size( ) * Xi_1.size( ) );
        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < Xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_nl[ Xi_1.size( ) * i + I ] += gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                    dchi_nlddX[ Xi_1.size( ) * i + I ][ J ] = gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ];

                    for ( unsigned int K = 0; K < Xi_1.size( ); K++ ){

                        for ( unsigned int L = 0; L < dX.size( ); L++ ){

                            dchi_nldGradChi[ Xi_1.size( ) * i + I ][ Xi_1.size( ) * dX.size( ) * J + dX.size( ) * K + L ] = eye[ Xi_1.size( ) * i + J ] * eye[ Xi_1.size( ) * I + K ] * dX[ L ];

                        }

                    }

                }

            }

        }

        floatMatrix dOverlapdChi_nl;

        ERROR_TOOLS_CATCH( computeParticleOverlapChi_nl( Xi_1, dX, R_nl, F, chi, chi_nl, overlap,
                                                         dOverlapdXi_1, dOverlapddX, dOverlapdR_nl,
                                                         dOverlapdF, dOverlapdChi, dOverlapdChi_nl ) );

        dOverlapddX  += vectorTools::dot( dOverlapdChi_nl, dchi_nlddX );

        dOverlapdChi_NL_B = dOverlapdChi_nl;

        dOverlapdGradChi = vectorTools::dot( dOverlapdChi_nl, dchi_nldGradChi );

        return;

    }

    void computeParticleOverlapChi_nl( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                           const floatVector &F,    const floatVector &chi,  const floatVector &chi_nl,
                                           floatVector &overlap,
                                           floatMatrix &dOverlapdXi_1, floatMatrix &dOverlapddX, floatVector &dOverlapdR_nl,
                                           floatMatrix &dOverlapdF, floatMatrix &dOverlapdChi, floatMatrix &dOverlapdChi_nl ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &chi_nl: The non-local micro deformation tensor
         * \param &overlap: The overlap vector
         * \param &dOverlapdXi_1: The gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &dOverlapddX: The gradient of the overlap w.r.t. the local reference center-to-center vector
         * \param &dOverlapdR_nl: The gradient of the overlap w.r.t. the non-local reference radius
         * \param &dOverlapdF: The gradient of the overlap w.r.t. the deformation gradient
         * \param &dOverlapdChi: The gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &dOverlapdChi_nl: The gradient of the overlap w.r.t. the non-local micro-deformation tensor
         */

        if ( Xi_1.size( ) != dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The local micro relative position vector and the inter-particle spacing should have the same dimension\n\tXi_1: " + std::to_string( Xi_1.size( ) ) + "\n\tdX: " + std::to_string( dX.size( ) ) ) );

        }

        if ( F.size( ) != dX.size( ) * dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The deformation gradient is not the expected dimension.\n\tF: " + std::to_string( F.size( ) ) + "\n\texpected: " + std::to_string( dX.size( ) * dX.size( ) ) ) );

        }

        if ( chi.size( ) != Xi_1.size( ) * Xi_1.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( chi_nl.size( ) != Xi_1.size( ) * Xi_1.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The non-local micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi_nl.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        // Compute the current configuration local relative position vector and inter-particle spacing
        floatVector xi_1( Xi_1.size( ), 0 );

        floatVector eye( Xi_1.size( ) * Xi_1.size( ), 0 );
        vectorTools::eye( eye );

        floatMatrix dxi_1dXi_1( xi_1.size( ), floatVector( Xi_1.size( ), 0 ) );

        floatMatrix dxi_1dChi( xi_1.size( ), floatVector( chi.size( ), 0 ) );

        for ( unsigned int i = 0; i < xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                xi_1[ i ] += chi[ Xi_1.size( ) * i + I ] * Xi_1[ I ];

                dxi_1dXi_1[ i ][ I ] += chi[ Xi_1.size( ) * i + I ];

                for ( unsigned int J = 0; J < Xi_1.size( ); J++ ){

                    dxi_1dChi[ i ][ Xi_1.size( ) * I + J ] += eye[ Xi_1.size( ) * i + I ] * Xi_1[ J ];

                }

            }

        }

        floatVector dx( dX.size( ), 0 );

        floatMatrix ddxddX( dx.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix ddxdF( dx.size( ), floatVector( F.size( ), 0 ) );

        for ( unsigned int i = 0; i < dx.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                dx[ i ] += F[ dx.size( ) * i + I ] * dX[ I ];

                ddxddX[ i ][ I ] += F[ dx.size( ) * i + I ];

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    ddxdF[ i ][ dX.size( ) * I + J ] += eye[ dX.size( ) * i + I ] * dX[ J ];

                }

            }

        }

        // Compute the representation of xi_1 in the non-local particle's reference frame
        floatVector xi_t = xi_1 - dx;

        floatMatrix dxi_tdXi_1 = dxi_1dXi_1;

        floatMatrix dxi_tddX = -ddxddX;

        floatMatrix dxi_tdchi = dxi_1dChi;

        floatMatrix dxi_tdF = -ddxdF;

        // Compute the inverse of the non-local micro-deformation tensor
        if ( vectorTools::determinant( chi_nl, xi_t.size( ), Xi_1.size( ) ) <= 0 ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The non-local micro deformation gradient has a determinant less than or equal to zero" ) );

        }

        floatVector inv_chi_nl = vectorTools::inverse( chi_nl, xi_t.size( ), Xi_1.size( ) );

        floatVector Xi_t( Xi_1.size( ), 0 );

        for ( unsigned int I = 0; I < Xi_t.size( ); I++ ){

            for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

                Xi_t[ I ] += inv_chi_nl[ Xi_t.size( ) * I + i ] * xi_t[ i ];

            }

        }  

        if ( vectorTools::dot( Xi_t, Xi_t ) > ( R_nl * R_nl ) ){

            overlap = floatVector( xi_t.size( ), 0 );

            dOverlapdXi_1 = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ), 0 ) );

            dOverlapddX = floatMatrix( overlap.size( ), floatVector( dX.size( ), 0 ) );

            dOverlapdR_nl = floatVector( overlap.size( ), 0 );

            dOverlapdF = floatMatrix( overlap.size( ), floatVector( F.size( ), 0 ) );

            dOverlapdChi = floatMatrix( overlap.size( ), floatVector( chi.size( ), 0 ) );

            dOverlapdChi_nl = floatMatrix( overlap.size( ), floatVector( chi_nl.size( ), 0 ) );

        }
        else{

            floatMatrix dOverlapdxi_t;

            ERROR_TOOLS_CATCH( solveOverlapDistance( chi_nl, xi_t, R_nl, overlap, dOverlapdChi_nl, dOverlapdxi_t, dOverlapdR_nl ) );

            dOverlapdXi_1 = vectorTools::dot( dOverlapdxi_t, dxi_tdXi_1 );

            dOverlapddX   = vectorTools::dot( dOverlapdxi_t, dxi_tddX );

            dOverlapdF    = vectorTools::dot( dOverlapdxi_t, dxi_tdF );

            dOverlapdChi  = vectorTools::dot( dOverlapdxi_t, dxi_tdchi );

        }

        return;

    }

    void computeParticleOverlap( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                     const floatVector &F,    const floatVector &chi,  const floatVector &gradChi,
                                     floatVector &overlap,
                                     floatMatrix &dOverlapdXi_1, floatMatrix &dOverlapddX, floatVector &dOverlapdR_nl,
                                     floatMatrix &dOverlapdF, floatMatrix &dOverlapdChi, floatMatrix &dOverlapdGradChi,
                                     floatMatrix &d2OverlapdXi_1dXi_1, floatMatrix &d2OverlapdXi_1ddX, floatMatrix &d2OverlapdXi_1dR_nl, floatMatrix &d2OverlapdXi_1dF, floatMatrix &d2OverlapdXi_1dChi, floatMatrix &d2OverlapdXi_1dGradChi,
                                     floatMatrix &d2OverlapddXddX, floatMatrix &d2OverlapddXdR_nl, floatMatrix &d2OverlapddXdF, floatMatrix &d2OverlapddXdChi, floatMatrix &d2OverlapddXdGradChi,
                                     floatVector &d2OverlapdR_nldR_nl, floatMatrix &d2OverlapdR_nldF, floatMatrix &d2OverlapdR_nldChi, floatMatrix &d2OverlapdR_nldGradChi,
                                     floatMatrix &d2OverlapdFdF, floatMatrix &d2OverlapdFdChi, floatMatrix &d2OverlapdFdGradChi,
                                     floatMatrix &d2OverlapdChidChi, floatMatrix &d2OverlapdChidGradChi,
                                     floatMatrix &d2OverlapdGradChidGradChi ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I = \left(\chi_{iI} + \chi_{iI,J} dX_J\right) \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &gradChi: The gradient of the micro deformation tensor w.r.t. the reference spatial position
         * \param &overlap: The overlap vector
         * \param &dOverlapdXi_1: The gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &dOverlapddX: The gradient of the overlap w.r.t. the local reference center-to-center vector
         * \param &dOverlapdR_nl: The gradient of the overlap w.r.t. the non-local reference radius
         * \param &dOverlapdF: The gradient of the overlap w.r.t. the deformation gradient
         * \param &dOverlapdChi: The gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &dOverlapdGradChi: The gradient of the overlap w.r.t. the reference spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdXi_1dXi_1: The second gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &d2OverlapdXi_1ddX: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the particle reference center-to-center vector
         * \param &d2OverlapdXi_1dR_nl: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local reference radius
         * \param &d2OverlapdXi_1dF: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the deformation gradient
         * \param &d2OverlapdXi_1dChi: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the micro-deformation tensor
         * \param &d2OverlapdXi_1dGradChi: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapddXddX: The second gradient of the overlap w.r.t. the particle reference center-to-center vector
         * \param &d2OverlapddXdR_nl: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the non-local reference radius
         * \param &d2OverlapddXdF: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the deformation gradient
         * \param &d2OverlapddXdChi: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the micro-deformation tensor
         * \param &d2OverlapddXdGradChi: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdR_nldR_nl: The second gradient of the overlap w.r.t. the non-local reference radius
         * \param &d2OverlapdR_nldF: The second gradient of the overlap w.r.t. the non-local reference radius and the deformation gradient
         * \param &d2OverlapdR_nldChi: The second gradient of the overlap w.r.t. the non-local reference radius and the micro-deformation tensor
         * \param &d2OverlapdR_nldGradChi: The second gradient of the overlap w.r.t. the non-local reference radius and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdFdF: The second gradient of the overlap w.r.t. the deformation gradient
         * \param &d2OverlapdFdChi: The second gradient of the overlap w.r.t. the deformation gradient and the micro-deformation tensor
         * \param &d2OverlapdFdGradChi: The second gradient of the overlap w.r.t. the deformation gradient and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdChidChi: The second gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &d2OverlapdChidGradChi: The second gradient of the overlap w.r.t. the micro-deformation tensor and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdGradChidGradChi: The second gradient of the overlap w.r.t. the spatial gradient of the micro-deformation tensor
         */

        if ( chi.size( ) != Xi_1.size( ) * Xi_1.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( gradChi.size( ) != Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The gradient of the micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( gradChi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ) ) );

        }

        // Compute the non-local micro-deformation tensor
        floatVector chi_nl = chi;

        floatMatrix dchi_nldchi( chi_nl.size( ), floatVector( chi.size( ), 0 ) );

        floatMatrix dchi_nlddX( chi_nl.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix dchi_nldGradChi( chi_nl.size( ), floatVector( gradChi.size( ), 0 ) );

        floatMatrix d2chi_nlddXdGradChi( chi_nl.size( ), floatVector( dX.size( ) * gradChi.size( ), 0 ) );

        floatVector eye( Xi_1.size( ) * Xi_1.size( ) );
        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < Xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                dchi_nldchi[ Xi_1.size( ) * i + I ][ Xi_1.size( ) * i + I ] = 1.;

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_nl[ Xi_1.size( ) * i + I ] += gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                    dchi_nlddX[ Xi_1.size( ) * i + I ][ J ] = gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ];

                    for ( unsigned int K = 0; K < Xi_1.size( ); K++ ){

                        for ( unsigned int L = 0; L < dX.size( ); L++ ){

                            for ( unsigned int M = 0; M < dX.size( ); M++ ){

                                dchi_nldGradChi[ Xi_1.size( ) * i + I ][ Xi_1.size( ) * dX.size( ) * J + dX.size( ) * K + L ] = eye[ Xi_1.size( ) * i + J ] * eye[ Xi_1.size( ) * I + K ] * dX[ L ];

                                d2chi_nlddXdGradChi[ Xi_1.size( ) * i + I ][ gradChi.size( ) * J + Xi_1.size( ) * Xi_1.size( ) * K + Xi_1.size( ) * L + M ] = eye[ Xi_1.size( ) * i + K ] * eye[ Xi_1.size( ) * I + L ] * eye[ Xi_1.size( ) * J + M ];

                            }

                        }

                    }

                }

            }

        }

        floatMatrix dOverlapdChi_nl;

        floatMatrix d2OverlapdXi_1dChi_nl, d2OverlapddXdChi_nl, d2OverlapdR_nldChi_nl, d2OverlapdFdChi_nl, d2OverlapdChidChi_nl,
                    d2OverlapdChi_nldChi_nl;

        ERROR_TOOLS_CATCH( computeParticleOverlapChi_nl( Xi_1, dX, R_nl, F, chi, chi_nl, overlap,
                                                         dOverlapdXi_1, dOverlapddX, dOverlapdR_nl,
                                                         dOverlapdF, dOverlapdChi, dOverlapdChi_nl,
                                                         d2OverlapdXi_1dXi_1, d2OverlapdXi_1ddX, d2OverlapdXi_1dR_nl, d2OverlapdXi_1dF, d2OverlapdXi_1dChi, d2OverlapdXi_1dChi_nl,
                                                         d2OverlapddXddX, d2OverlapddXdR_nl, d2OverlapddXdF, d2OverlapddXdChi, d2OverlapddXdChi_nl,
                                                         d2OverlapdR_nldR_nl, d2OverlapdR_nldF, d2OverlapdR_nldChi, d2OverlapdR_nldChi_nl,
                                                         d2OverlapdFdF, d2OverlapdFdChi, d2OverlapdFdChi_nl,
                                                         d2OverlapdChidChi, d2OverlapdChidChi_nl,
                                                         d2OverlapdChi_nldChi_nl ) );

        d2OverlapdXi_1dGradChi    = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * gradChi.size( ), 0 ) );

        d2OverlapddXdGradChi      = floatMatrix( overlap.size( ), floatVector( dX.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdFdGradChi       = floatMatrix( overlap.size( ), floatVector( F.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdChidGradChi     = floatMatrix( overlap.size( ), floatVector( chi.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdGradChidGradChi = floatMatrix( overlap.size( ), floatVector( gradChi.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdR_nldGradChi    = floatMatrix( overlap.size( ), floatVector( gradChi.size( ), 0 ) );

        dOverlapddX  += vectorTools::dot( dOverlapdChi_nl, dchi_nlddX );

        dOverlapdChi += vectorTools::dot( dOverlapdChi_nl, dchi_nldchi );

        dOverlapdGradChi = vectorTools::dot( dOverlapdChi_nl, dchi_nldGradChi );

        for ( unsigned int i = 0; i < overlap.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdXi_1ddX[ i ][ dX.size( ) * I + J ] += d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                    }

                }

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdXi_1dChi[ i ][ chi.size( ) * I + J ] += d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ];

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdXi_1dGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                    }

                }

            }

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXddX[ i ][ dX.size( ) * I + J ] += d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ]
                                                                    + d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapddXddX[ i ][ dX.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];

                        }

                    }

                }

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d2OverlapddXdR_nl[ i ][ I ] += d2OverlapdR_nldChi_nl[ i ][ a ] * dchi_nlddX[ a ][ I ];

                }

                for ( unsigned int J = 0; J < F.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdF[ i ][ chi.size( ) * I + J ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ];

                    }

                }

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdChi[ i ][ chi.size( ) * I + J ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ]
                                                                      + d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapddXdChi[ i ][ chi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldchi[ b ][ J ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ]
                                                                              + dOverlapdChi_nl[ i ][ a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + J ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapddXdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < F.size( ); I++ ){

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdFdChi[ i ][ chi.size( ) * I + J ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ];

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdFdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                    }

                }

            }

            for ( unsigned int I = 0; I < chi.size( ); I++ ){

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d2OverlapdR_nldChi[ i ][ I ] += d2OverlapdR_nldChi_nl[ i ][ a ] * dchi_nldchi[ a ][ I ];

                }

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdChidChi[ i ][ chi.size( ) * I + J ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ]
                                                                       + d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ I ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapdChidChi[ i ][ chi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ I ] * dchi_nldchi[ b ][ J ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdChidGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapdChidGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < gradChi.size( ); I++ ){

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d2OverlapdR_nldGradChi[ i ][ I ] += d2OverlapdR_nldChi_nl[ i ][ a ] * dchi_nldGradChi[ a ][ I ];

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapdGradChidGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                        }

                    }

                }

            }

        }

        return;

    }

    void computeParticleOverlap( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                     const floatVector &F,    const floatVector &chi, const floatVector &chi_nl_basis, const floatVector &gradChi,
                                     floatVector &overlap,
                                     floatMatrix &dOverlapdXi_1, floatMatrix &dOverlapddX, floatVector &dOverlapdR_nl,
                                     floatMatrix &dOverlapdF, floatMatrix &dOverlapdChi, floatMatrix &dOverlapdChi_NL_B, floatMatrix &dOverlapdGradChi,
                                     floatMatrix &d2OverlapdXi_1dXi_1, floatMatrix &d2OverlapdXi_1ddX, floatMatrix &d2OverlapdXi_1dR_nl, floatMatrix &d2OverlapdXi_1dF, floatMatrix &d2OverlapdXi_1dChi, floatMatrix &d2OverlapdXi_1dChi_NL_B, floatMatrix &d2OverlapdXi_1dGradChi,
                                     floatMatrix &d2OverlapddXddX, floatMatrix &d2OverlapddXdR_nl, floatMatrix &d2OverlapddXdF, floatMatrix &d2OverlapddXdChi, floatMatrix &d2OverlapddXdChi_NL_B, floatMatrix &d2OverlapddXdGradChi,
                                     floatVector &d2OverlapdR_nldR_nl, floatMatrix &d2OverlapdR_nldF, floatMatrix &d2OverlapdR_nldChi, floatMatrix &d2OverlapdR_nldChi_NL_B, floatMatrix &d2OverlapdR_nldGradChi,
                                     floatMatrix &d2OverlapdFdF, floatMatrix &d2OverlapdFdChi, floatMatrix &d2OverlapdFdChi_NL_B, floatMatrix &d2OverlapdFdGradChi,
                                     floatMatrix &d2OverlapdChidChi, floatMatrix &d2OverlapdChidChi_NL_B, floatMatrix &d2OverlapdChidGradChi,
                                     floatMatrix &d2OverlapdChi_NL_BdChi_NL_B, floatMatrix &d2OverlapdChi_NL_BdGradChi,
                                     floatMatrix &d2OverlapdGradChidGradChi ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I = \left(\chi_{iI}^{NL,basis} + \chi_{iI,J} dX_J\right) \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &chi_nl_basis: The non-local micro deformation tensor basis
         * \param &gradChi: The gradient of the micro deformation tensor w.r.t. the reference spatial position
         * \param &overlap: The overlap vector
         * \param &dOverlapdXi_1: The gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &dOverlapddX: The gradient of the overlap w.r.t. the local reference center-to-center vector
         * \param &dOverlapdR_nl: The gradient of the overlap w.r.t. the non-local reference radius
         * \param &dOverlapdF: The gradient of the overlap w.r.t. the deformation gradient
         * \param &dOverlapdChi: The gradient of the overlap w.r.t. the micro-deformation tensor.
         * \param &dOverlapdChi_NL_B: The gradient of the overlap w.r.t. the non-local micro-deformation tensor basis
         * \param &dOverlapdGradChi: The gradient of the overlap w.r.t. the reference spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdXi_1dXi_1: The second gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &d2OverlapdXi_1ddX: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the particle reference center-to-center vector
         * \param &d2OverlapdXi_1dR_nl: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local reference radius
         * \param &d2OverlapdXi_1dF: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the deformation gradient
         * \param &d2OverlapdXi_1dChi: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the micro-deformation tensor
         * \param &d2OverlapdXi_1dChi_NL_B: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local micro-deformation tensor basis
         * \param &d2OverlapdXi_1dGradChi: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapddXddX: The second gradient of the overlap w.r.t. the particle reference center-to-center vector
         * \param &d2OverlapddXdR_nl: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the non-local reference radius
         * \param &d2OverlapddXdF: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the deformation gradient
         * \param &d2OverlapddXdChi: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the micro-deformation tensor
         * \param &d2OverlapddXdChi_NL_B: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the non-local micro-deformation tensor basis
         * \param &d2OverlapddXdGradChi: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdR_nldR_nl: The second gradient of the overlap w.r.t. the non-local reference radius
         * \param &d2OverlapdR_nldF: The second gradient of the overlap w.r.t. the non-local reference radius and the deformation gradient
         * \param &d2OverlapdR_nldChi: The second gradient of the overlap w.r.t. the non-local reference radius and the micro-deformation tensor
         * \param &d2OverlapdR_nldChi_NL_B: The second gradient of the overlap w.r.t. the non-local reference radius and the non-local micro-deformation tensor basis
         * \param &d2OverlapdR_nldGradChi: The second gradient of the overlap w.r.t. the non-local reference radius and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdFdF: The second gradient of the overlap w.r.t. the deformation gradient
         * \param &d2OverlapdFdChi: The second gradient of the overlap w.r.t. the deformation gradient and the micro-deformation tensor
         * \param &d2OverlapdFdChi_NL_B: The second gradient of the overlap w.r.t. the deformation gradient and the non-local micro-deformation tensor basis
         * \param &d2OverlapdFdGradChi: The second gradient of the overlap w.r.t. the deformation gradient and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdChidChi: The second gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &d2OverlapdChidChi_NL_B: The second gradient of the overlap w.r.t. the non-local micro-deformation tensor basis
         * \param &d2OverlapdChidGradChi: The second gradient of the overlap w.r.t. the micro-deformation tensor and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdChi_NL_BdChi_NL_B: The second gradient of the overlap w.r.t. the non-local micro-deformation tensor basis
         * \param &d2OverlapdChi_NL_BdGradChi: The second gradient of the overlap w.r.t. the non-local micro-deformation tensor basis and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdGradChidGradChi: The second gradient of the overlap w.r.t. the spatial gradient of the micro-deformation tensor
         */

        if ( chi.size( ) != Xi_1.size( ) * Xi_1.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( chi_nl_basis.size( ) != ( Xi_1.size( ) * Xi_1.size( ) ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The incoming chi non-local basis vector has an inconsistent size with the micro-position vector\n  size is " + std::to_string( chi_nl_basis.size( ) ) + " and must be " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( gradChi.size( ) != Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The gradient of the micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( gradChi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ) ) );

        }

        // Compute the non-local micro-deformation tensor
        floatVector chi_nl = chi_nl_basis;

        floatMatrix dchi_nlddX( chi_nl.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix dchi_nldGradChi( chi_nl.size( ), floatVector( gradChi.size( ), 0 ) );

        floatMatrix d2chi_nlddXdGradChi( chi_nl.size( ), floatVector( dX.size( ) * gradChi.size( ), 0 ) );

        floatVector eye( Xi_1.size( ) * Xi_1.size( ) );
        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < Xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_nl[ Xi_1.size( ) * i + I ] += gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                    dchi_nlddX[ Xi_1.size( ) * i + I ][ J ] = gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ];

                    for ( unsigned int K = 0; K < Xi_1.size( ); K++ ){

                        for ( unsigned int L = 0; L < dX.size( ); L++ ){

                            for ( unsigned int M = 0; M < dX.size( ); M++ ){

                                dchi_nldGradChi[ Xi_1.size( ) * i + I ][ Xi_1.size( ) * dX.size( ) * J + dX.size( ) * K + L ] = eye[ Xi_1.size( ) * i + J ] * eye[ Xi_1.size( ) * I + K ] * dX[ L ];

                                d2chi_nlddXdGradChi[ Xi_1.size( ) * i + I ][ gradChi.size( ) * J + Xi_1.size( ) * Xi_1.size( ) * K + Xi_1.size( ) * L + M ] = eye[ Xi_1.size( ) * i + K ] * eye[ Xi_1.size( ) * I + L ] * eye[ Xi_1.size( ) * J + M ];

                            }

                        }

                    }

                }

            }

        }

        floatMatrix dOverlapdChi_nl;

        floatMatrix d2OverlapdXi_1dChi_nl, d2OverlapddXdChi_nl, d2OverlapdR_nldChi_nl, d2OverlapdFdChi_nl, d2OverlapdChidChi_nl,
                    d2OverlapdChi_nldChi_nl;

        ERROR_TOOLS_CATCH( computeParticleOverlapChi_nl( Xi_1, dX, R_nl, F, chi, chi_nl, overlap,
                                                         dOverlapdXi_1, dOverlapddX, dOverlapdR_nl,
                                                         dOverlapdF, dOverlapdChi, dOverlapdChi_nl,
                                                         d2OverlapdXi_1dXi_1, d2OverlapdXi_1ddX, d2OverlapdXi_1dR_nl, d2OverlapdXi_1dF, d2OverlapdXi_1dChi, d2OverlapdXi_1dChi_nl,
                                                         d2OverlapddXddX, d2OverlapddXdR_nl, d2OverlapddXdF, d2OverlapddXdChi, d2OverlapddXdChi_nl,
                                                         d2OverlapdR_nldR_nl, d2OverlapdR_nldF, d2OverlapdR_nldChi, d2OverlapdR_nldChi_nl,
                                                         d2OverlapdFdF, d2OverlapdFdChi, d2OverlapdFdChi_nl,
                                                         d2OverlapdChidChi, d2OverlapdChidChi_nl,
                                                         d2OverlapdChi_nldChi_nl ) );

        d2OverlapdXi_1dChi_NL_B     = d2OverlapdXi_1dChi_nl;

        d2OverlapdXi_1dGradChi      = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * gradChi.size( ), 0 ) );

        d2OverlapddXdChi_NL_B       = d2OverlapddXdChi_nl;

        d2OverlapddXdGradChi        = floatMatrix( overlap.size( ), floatVector( dX.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdR_nldChi_NL_B     = d2OverlapdR_nldChi_nl;

        d2OverlapdR_nldGradChi      = floatMatrix( overlap.size( ), floatVector( gradChi.size( ), 0 ) );

        d2OverlapdFdChi_NL_B        = d2OverlapdFdChi_nl;

        d2OverlapdFdGradChi         = floatMatrix( overlap.size( ), floatVector( F.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdChidChi_NL_B      = d2OverlapdChidChi_nl;

        d2OverlapdChidGradChi       = floatMatrix( overlap.size( ), floatVector( chi.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdChi_NL_BdChi_NL_B = d2OverlapdChi_nldChi_nl;

        d2OverlapdChi_NL_BdGradChi  = floatMatrix( overlap.size( ), floatVector( chi_nl_basis.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdGradChidGradChi   = floatMatrix( overlap.size( ), floatVector( gradChi.size( ) * gradChi.size( ), 0 ) );

        dOverlapddX  += vectorTools::dot( dOverlapdChi_nl, dchi_nlddX );

        dOverlapdChi_NL_B = dOverlapdChi_nl;

        dOverlapdGradChi = vectorTools::dot( dOverlapdChi_nl, dchi_nldGradChi );

        for ( unsigned int i = 0; i < overlap.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdXi_1ddX[ i ][ dX.size( ) * I + J ] += d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdXi_1dGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                    }

                }

            }

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXddX[ i ][ dX.size( ) * I + J ] += d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ]
                                                                    + d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapddXddX[ i ][ dX.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];

                        }

                    }

                }

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d2OverlapddXdR_nl[ i ][ I ] += d2OverlapdR_nldChi_nl[ i ][ a ] * dchi_nlddX[ a ][ I ];

                }

                for ( unsigned int J = 0; J < F.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdF[ i ][ chi.size( ) * I + J ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ];

                    }

                }

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdChi[ i ][ chi.size( ) * I + J ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ];

                    }

                }

                for ( unsigned int J = 0; J < chi_nl_basis.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdChi_NL_B[ i ][ chi_nl_basis.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + J ] * dchi_nlddX[ a ][ I ];


                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ]
                                                                              + dOverlapdChi_nl[ i ][ a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + J ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapddXdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < F.size( ); I++ ){

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdFdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                    }

                }

            }

            for ( unsigned int I = 0; I < chi.size( ); I++ ){

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdChidGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                    }

                }

            }

            for ( unsigned int I = 0; I < chi_nl_basis.size( ); I++ ){

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdChi_NL_BdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                    }

                }

            }

            for ( unsigned int I = 0; I < gradChi.size( ); I++ ){

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d2OverlapdR_nldGradChi[ i ][ I ] += d2OverlapdR_nldChi_nl[ i ][ a ] * dchi_nldGradChi[ a ][ I ];

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapdGradChidGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                        }

                    }

                }

            }

        }

        return;

    }

    void computeParticleOverlapChi_nl( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                           const floatVector &F,    const floatVector &chi,  const floatVector &chi_nl,
                                           floatVector &overlap,
                                           floatMatrix &dOverlapdXi_1, floatMatrix &dOverlapddX, floatVector &dOverlapdR_nl,
                                           floatMatrix &dOverlapdF, floatMatrix &dOverlapdChi, floatMatrix &dOverlapdChi_nl,
                                           floatMatrix &d2OverlapdXi_1dXi_1, floatMatrix &d2OverlapdXi_1ddX, floatMatrix &d2OverlapdXi_1dR_nl, floatMatrix &d2OverlapdXi_1dF, floatMatrix &d2OverlapdXi_1dChi, floatMatrix &d2OverlapdXi_1dChi_nl,
                                           floatMatrix &d2OverlapddXddX, floatMatrix &d2OverlapddXdR_nl, floatMatrix &d2OverlapddXdF, floatMatrix &d2OverlapddXdChi, floatMatrix &d2OverlapddXdChi_nl,
                                           floatVector &d2OverlapdR_nldR_nl, floatMatrix &d2OverlapdR_nldF, floatMatrix &d2OverlapdR_nldChi, floatMatrix &d2OverlapdR_nldChi_nl,
                                           floatMatrix &d2OverlapdFdF, floatMatrix &d2OverlapdFdChi, floatMatrix &d2OverlapdFdChi_nl,
                                           floatMatrix &d2OverlapdChidChi, floatMatrix &d2OverlapdChidChi_nl,
                                           floatMatrix &d2OverlapdChi_nldChi_nl ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I = \left(\chi_{iI} + \chi_{iI,J} dX_J\right) \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &chi_nl: The non-local micro deformation tensor
         * \param &overlap: The overlap vector
         * \param &dOverlapdXi_1: The gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &dOverlapddX: The gradient of the overlap w.r.t. the local reference center-to-center vector
         * \param &dOverlapdR_nl: The gradient of the overlap w.r.t. the non-local reference radius
         * \param &dOverlapdF: The gradient of the overlap w.r.t. the deformation gradient
         * \param &dOverlapdChi: The gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &dOverlapdChi_nl: The gradient of the overlap w.r.t. the non-local micro-deformation tensor
         * \param &d2OverlapdXi_1dXi_1: The second gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &d2OverlapdXi_1ddX: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the particle reference center-to-center vector
         * \param &d2OverlapdXi_1dR_nl: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local reference radius
         * \param &d2OverlapdXi_1dF: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the deformation gradient
         * \param &d2OverlapdXi_1dChi: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the micro-deformation tensor
         * \param &d2OverlapdXi_1dChi_nl: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local micro-deformation tensor
         * \param &d2OverlapddXddX: The second gradient of the overlap w.r.t. the particle reference center-to-center vector
         * \param &d2OverlapddXdR_nl: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the non-local reference radius
         * \param &d2OverlapddXdF: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the deformation gradient
         * \param &d2OverlapddXdChi: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the micro-deformation tensor
         * \param &d2OverlapddXdChi_nl: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the non-local micro-deformation tensor
         * \param &d2OverlapdR_nldR_nl: The second gradient of the overlap w.r.t. the non-local reference radius
         * \param &d2OverlapdR_nldF: The second gradient of the overlap w.r.t. the non-local reference radius and the deformation gradient
         * \param &d2OverlapdR_nldChi: The second gradient of the overlap w.r.t. the non-local reference radius and the micro-deformation tensor
         * \param &d2OverlapdR_nldChi_nl: The second gradient of the overlap w.r.t. the non-local reference radius and the non-local micro-deformation tensor
         * \param &d2OverlapdFdF: The second gradient of the overlap w.r.t. the deformation gradient
         * \param &d2OverlapdFdChi: The second gradient of the overlap w.r.t. the deformation gradient and the micro-deformation tensor
         * \param &d2OverlapdFdChi_nl: The second gradient of the overlap w.r.t. the deformation gradient and the non-local micro-deformation tensor
         * \param &d2OverlapdChidChi: The second gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &d2OverlapdChidChi_nl: The second gradient of the overlap w.r.t. the micro-deformation tensor and the non-local micro-deformation tensor
         * \param &d2OverlapdChi_nldChi_nl: The second gradient of the overlap w.r.t. the non-local micro-deformation tensor
         */

        if ( Xi_1.size( ) != dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The local micro relative position vector and the inter-particle spacing should have the same dimension\n\tXi_1: " + std::to_string( Xi_1.size( ) ) + "\n\tdX: " + std::to_string( dX.size( ) ) ) );

        }

        if ( F.size( ) != dX.size( ) * dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The deformation gradient is not the expected dimension.\n\tF: " + std::to_string( F.size( ) ) + "\n\texpected: " + std::to_string( dX.size( ) * dX.size( ) ) ) );

        }

        if ( chi.size( ) != Xi_1.size( ) * Xi_1.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( chi_nl.size( ) != Xi_1.size( ) * Xi_1.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The non-local micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi_nl.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        // Compute the current configuration local relative position vector and inter-particle spacing
        floatVector xi_1( Xi_1.size( ), 0 );

        floatVector eye( Xi_1.size( ) * Xi_1.size( ), 0 );
        vectorTools::eye( eye );

        floatMatrix dxi_1dXi_1( xi_1.size( ), floatVector( Xi_1.size( ), 0 ) );

        floatMatrix dxi_1dChi( xi_1.size( ), floatVector( chi.size( ), 0 ) );

        floatMatrix d2xi_1dXi_1dChi( xi_1.size( ), floatVector( Xi_1.size( ) * chi.size( ), 0 ) );

        for ( unsigned int i = 0; i < xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                xi_1[ i ] += chi[ Xi_1.size( ) * i + I ] * Xi_1[ I ];

                dxi_1dXi_1[ i ][ I ] += chi[ Xi_1.size( ) * i + I ];

                for ( unsigned int J = 0; J < Xi_1.size( ); J++ ){

                    dxi_1dChi[ i ][ Xi_1.size( ) * I + J ] += eye[ Xi_1.size( ) * i + I ] * Xi_1[ J ];

                    for ( unsigned int K = 0; K < Xi_1.size( ); K++ ){

                        d2xi_1dXi_1dChi[ i ][ chi.size( ) * I + Xi_1.size( ) * J + K ] += eye[ Xi_1.size( ) * i + J ] * eye[ Xi_1.size( ) * I + K ];

                    }

                }

            }

        }

        floatVector dx( dX.size( ), 0 );

        floatMatrix ddxddX( dx.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix ddxdF( dx.size( ), floatVector( F.size( ), 0 ) );

        floatMatrix d2dxddXdF( dx.size( ), floatVector( dX.size( ) * F.size( ), 0 ) );

        for ( unsigned int i = 0; i < dx.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                dx[ i ] += F[ dx.size( ) * i + I ] * dX[ I ];

                ddxddX[ i ][ I ] += F[ dx.size( ) * i + I ];

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    ddxdF[ i ][ dX.size( ) * I + J ] += eye[ dX.size( ) * i + I ] * dX[ J ];

                    for ( unsigned int K = 0; K < dX.size( ); K++ ){

                        d2dxddXdF[ i ][ F.size( ) * I + dX.size( ) * J + K ] += eye[ dx.size( ) * i + J ] * eye[ dx.size( ) * I + K ];

                    }

                }

            }

        }

        // Compute the representation of xi_1 in the non-local particle's reference frame
        floatVector xi_t = xi_1 - dx;

        floatMatrix dxi_tdXi_1 = dxi_1dXi_1;

        floatMatrix dxi_tddX = -ddxddX;

        floatMatrix dxi_tdchi = dxi_1dChi;

        floatMatrix dxi_tdF = -ddxdF;

        floatMatrix d2xi_tdXi_1dChi = d2xi_1dXi_1dChi;

        floatMatrix d2xi_tddXdF = -d2dxddXdF;

        // Compute the inverse of the non-local micro-deformation tensor
        if ( vectorTools::determinant( chi_nl, xi_t.size( ), Xi_1.size( ) ) <= 0 ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The non-local micro deformation gradient has a determinant less than or equal to zero" ) );

        }

        floatVector inv_chi_nl = vectorTools::inverse( chi_nl, xi_t.size( ), Xi_1.size( ) );

        floatVector Xi_t( Xi_1.size( ), 0 );

        for ( unsigned int I = 0; I < Xi_t.size( ); I++ ){

            for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

                Xi_t[ I ] += inv_chi_nl[ Xi_t.size( ) * I + i ] * xi_t[ i ];

            }

        }  

        d2OverlapdXi_1dXi_1    = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * Xi_1.size( ), 0 ) );

        d2OverlapdXi_1ddX      = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * dX.size( ), 0 ) );

        d2OverlapdXi_1dR_nl    = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ), 0 ) );

        d2OverlapdXi_1dF       = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * F.size( ), 0 ) );

        d2OverlapdXi_1dChi     = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * chi.size( ), 0 ) );

        d2OverlapdXi_1dChi_nl  = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * chi_nl.size( ), 0 ) );

        d2OverlapddXddX        = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * dX.size( ), 0 ) );

        d2OverlapddXdR_nl      = floatMatrix( xi_t.size( ), floatVector( dX.size( ), 0 ) );

        d2OverlapddXdF         = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * F.size( ), 0 ) );

        d2OverlapddXdChi       = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * chi.size( ), 0 ) );

        d2OverlapddXdChi_nl    = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * chi_nl.size( ), 0 ) );

        d2OverlapdR_nldR_nl    = floatVector( xi_t.size( ), 0 );

        d2OverlapdR_nldF       = floatMatrix( xi_t.size( ), floatVector( F.size( ), 0 ) );

        d2OverlapdR_nldChi     = floatMatrix( xi_t.size( ), floatVector( chi.size( ), 0 ) );

        d2OverlapdR_nldChi_nl  = floatMatrix( xi_t.size( ), floatVector( chi_nl.size( ), 0 ) );

        d2OverlapdFdF       = floatMatrix( xi_t.size( ), floatVector( F.size( ) * F.size( ), 0 ) );

        d2OverlapdFdChi     = floatMatrix( xi_t.size( ), floatVector( F.size( ) * chi.size( ), 0 ) );

        d2OverlapdFdChi_nl  = floatMatrix( xi_t.size( ), floatVector( F.size( ) * chi_nl.size( ), 0 ) );

        d2OverlapdChidChi     = floatMatrix( xi_t.size( ), floatVector( chi.size( ) * chi.size( ), 0 ) );

        d2OverlapdChidChi_nl  = floatMatrix( xi_t.size( ), floatVector( chi.size( ) * chi_nl.size( ), 0 ) );

        d2OverlapdChi_nldChi_nl = floatMatrix( xi_t.size( ), floatVector( chi_nl.size( ) * chi_nl.size( ), 0 ) );

        if ( vectorTools::dot( Xi_t, Xi_t ) > ( R_nl * R_nl ) ){

            overlap = floatVector( xi_t.size( ), 0 );

            dOverlapdXi_1 = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ), 0 ) );

            dOverlapddX = floatMatrix( overlap.size( ), floatVector( dX.size( ), 0 ) );

            dOverlapdR_nl = floatVector( overlap.size( ), 0 );

            dOverlapdF = floatMatrix( overlap.size( ), floatVector( F.size( ), 0 ) );

            dOverlapdChi = floatMatrix( overlap.size( ), floatVector( chi.size( ), 0 ) );

            dOverlapdChi_nl = floatMatrix( overlap.size( ), floatVector( chi_nl.size( ), 0 ) );

        }
        else{

            floatMatrix dOverlapdxi_t;

            floatMatrix d2Overlapdchi_nldxi_t,
                        d2Overlapdxi_tdxi_t, d2Overlapdxi_tdR_nl;

            ERROR_TOOLS_CATCH( solveOverlapDistance( chi_nl, xi_t, R_nl, overlap, dOverlapdChi_nl, dOverlapdxi_t, dOverlapdR_nl,
                                                     d2OverlapdChi_nldChi_nl, d2Overlapdchi_nldxi_t, d2OverlapdR_nldChi_nl,
                                                     d2Overlapdxi_tdxi_t, d2Overlapdxi_tdR_nl,
                                                     d2OverlapdR_nldR_nl ) );

            dOverlapdXi_1 = vectorTools::dot( dOverlapdxi_t, dxi_tdXi_1 );

            dOverlapddX   = vectorTools::dot( dOverlapdxi_t, dxi_tddX );

            dOverlapdF    = vectorTools::dot( dOverlapdxi_t, dxi_tdF );

            dOverlapdChi  = vectorTools::dot( dOverlapdxi_t, dxi_tdchi );

            d2OverlapdXi_1dR_nl = vectorTools::dot( d2Overlapdxi_tdR_nl, dxi_tdXi_1 );

            d2OverlapddXdR_nl = vectorTools::dot( d2Overlapdxi_tdR_nl, dxi_tddX );

            d2OverlapdR_nldF = vectorTools::dot( d2Overlapdxi_tdR_nl, dxi_tdF );

            d2OverlapdR_nldChi = vectorTools::dot( d2Overlapdxi_tdR_nl, dxi_tdchi );

            d2OverlapddXdF = vectorTools::dot( dOverlapdxi_t, d2xi_tddXdF );

            d2OverlapdXi_1dChi = vectorTools::dot( dOverlapdxi_t, d2xi_tdXi_1dChi );

            for ( unsigned int i = 0; i < overlap.size( ); i++ ){

                for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                    for ( unsigned int J = 0; J < Xi_1.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdXi_1dXi_1[ i ][ Xi_1.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdXi_1[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < dX.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdXi_1ddX[ i ][ dX.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tddX[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < F.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdXi_1dF[ i ][ F.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdF[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdXi_1dChi[ i ][ chi.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdchi[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi_nl.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + J ] += d2Overlapdchi_nldxi_t[ i ][ xi_t.size( ) * J + a ] * dxi_tdXi_1[ a ][ I ];

                        }

                    }

                }

                for ( unsigned int I = 0; I < dX.size( ); I++ ){

                    for ( unsigned int J = 0; J < dX.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapddXddX[ i ][ dX.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tddX[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < F.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapddXdF[ i ][ F.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tdF[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapddXdChi[ i ][ chi.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tdchi[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi_nl.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + J ] += d2Overlapdchi_nldxi_t[ i ][ xi_t.size( ) * J + a ] * dxi_tddX[ a ][ I ];

                        }


                    }

                }

                for ( unsigned int I = 0; I < F.size( ); I++ ){

                    for ( unsigned int J = 0; J < F.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdFdF[ i ][ F.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdF[ a ][ I ] * dxi_tdF[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdFdChi[ i ][ chi.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdF[ a ][ I ] * dxi_tdchi[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi_nl.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * I + J ] += d2Overlapdchi_nldxi_t[ i ][ xi_t.size( ) * J + a ] * dxi_tdF[ a ][ I ];

                        }

                    }

                }

                for ( unsigned int I = 0; I < chi.size( ); I++ ){

                    for ( unsigned int J = 0; J < chi.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdChidChi[ i ][ chi.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdchi[ a ][ I ] * dxi_tdchi[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi_nl.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * I + J ] += d2Overlapdchi_nldxi_t[ i ][ xi_t.size( ) * J + a ] * dxi_tdchi[ a ][ I ];

                        }

                    }

                }

            }

        }

        return;

    }

    void computeParticleOverlap( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                     const floatVector &F,    const floatVector &chi,  const floatVector &gradChi,
                                     floatVector &overlap,
                                     floatMatrix &dOverlapdXi_1, floatMatrix &dOverlapddX, floatVector &dOverlapdR_nl,
                                     floatMatrix &dOverlapdF, floatMatrix &dOverlapdChi, floatMatrix &dOverlapdGradChi,
                                     floatMatrix &d2OverlapdXi_1dXi_1, floatMatrix &d2OverlapdXi_1ddX, floatMatrix &d2OverlapdXi_1dR_nl, floatMatrix &d2OverlapdXi_1dF, floatMatrix &d2OverlapdXi_1dChi, floatMatrix &d2OverlapdXi_1dGradChi,
                                     floatMatrix &d2OverlapddXddX, floatMatrix &d2OverlapddXdR_nl, floatMatrix &d2OverlapddXdF, floatMatrix &d2OverlapddXdChi, floatMatrix &d2OverlapddXdGradChi,
                                     floatVector &d2OverlapdR_nldR_nl, floatMatrix &d2OverlapdR_nldF, floatMatrix &d2OverlapdR_nldChi, floatMatrix &d2OverlapdR_nldGradChi,
                                     floatMatrix &d2OverlapdFdF, floatMatrix &d2OverlapdFdChi, floatMatrix &d2OverlapdFdGradChi,
                                     floatMatrix &d2OverlapdChidChi, floatMatrix &d2OverlapdChidGradChi,
                                     floatMatrix &d2OverlapdGradChidGradChi,
                                     floatMatrix &d3OverlapdXi_1dXi_1dXi_1, floatMatrix &d3OverlapdXi_1dXi_1ddX, floatMatrix &d3OverlapdXi_1dXi_1dR_nl, floatMatrix &d3OverlapdXi_1dXi_1dF, floatMatrix &d3OverlapdXi_1dXi_1dChi, floatMatrix &d3OverlapdXi_1dXi_1dGradChi,
                                     floatMatrix &d3OverlapdXi_1ddXddX, floatMatrix &d3OverlapdXi_1ddXdR_nl, floatMatrix &d3OverlapdXi_1ddXdF, floatMatrix &d3OverlapdXi_1ddXdChi, floatMatrix &d3OverlapdXi_1ddXdGradChi,
                                     floatMatrix &d3OverlapdXi_1dR_nldR_nl, floatMatrix &d3OverlapdXi_1dR_nldF, floatMatrix &d3OverlapdXi_1dR_nldChi, floatMatrix &d3OverlapdXi_1dR_nldGradChi,
                                     floatMatrix &d3OverlapdXi_1dFdF, floatMatrix &d3OverlapdXi_1dFdChi, floatMatrix &d3OverlapdXi_1dFdGradChi,
                                     floatMatrix &d3OverlapdXi_1dChidChi, floatMatrix &d3OverlapdXi_1dChidGradChi,
                                     floatMatrix &d3OverlapdXi_1dGradChidGradChi,
                                     floatMatrix &d3OverlapddXddXddX, floatMatrix &d3OverlapddXddXdR_nl, floatMatrix &d3OverlapddXddXdF, floatMatrix &d3OverlapddXddXdChi, floatMatrix &d3OverlapddXddXdGradChi,
                                     floatMatrix &d3OverlapddXdR_nldR_nl, floatMatrix &d3OverlapddXdR_nldF, floatMatrix &d3OverlapddXdR_nldChi, floatMatrix &d3OverlapddXdR_nldGradChi,
                                     floatMatrix &d3OverlapddXdFdF, floatMatrix &d3OverlapddXdFdChi, floatMatrix &d3OverlapddXdFdGradChi,
                                     floatMatrix &d3OverlapddXdChidChi, floatMatrix &d3OverlapddXdChidGradChi,
                                     floatMatrix &d3OverlapddXdGradChidGradChi,
                                     floatVector &d3OverlapdR_nldR_nldR_nl, floatMatrix &d3OverlapdR_nldR_nldF, floatMatrix &d3OverlapdR_nldR_nldChi, floatMatrix &d3OverlapdR_nldR_nldGradChi,
                                     floatMatrix &d3OverlapdR_nldFdF, floatMatrix &d3OverlapdR_nldFdChi, floatMatrix &d3OverlapdR_nldFdGradChi,
                                     floatMatrix &d3OverlapdR_nldChidChi, floatMatrix &d3OverlapdR_nldChidGradChi,
                                     floatMatrix &d3OverlapdR_nldGradChidGradChi,
                                     floatMatrix &d3OverlapdFdFdF, floatMatrix &d3OverlapdFdFdChi, floatMatrix &d3OverlapdFdFdGradChi,
                                     floatMatrix &d3OverlapdFdChidChi, floatMatrix &d3OverlapdFdChidGradChi,
                                     floatMatrix &d3OverlapdFdGradChidGradChi,
                                     floatMatrix &d3OverlapdChidChidChi, floatMatrix &d3OverlapdChidChidGradChi,
                                     floatMatrix &d3OverlapdChidGradChidGradChi,
                                     floatMatrix &d3OverlapdGradChidGradChidGradChi ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I = \left(\chi_{iI} + \chi_{iI,J} dX_J\right) \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &gradChi: The gradient of the micro deformation tensor w.r.t. the reference spatial position
         * \param &overlap: The overlap vector
         * \param &dOverlapdXi_1: The gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &dOverlapddX: The gradient of the overlap w.r.t. the local reference center-to-center vector
         * \param &dOverlapdR_nl: The gradient of the overlap w.r.t. the non-local reference radius
         * \param &dOverlapdF: The gradient of the overlap w.r.t. the deformation gradient
         * \param &dOverlapdChi: The gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &dOverlapdGradChi: The gradient of the overlap w.r.t. the reference spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdXi_1dXi_1: The second gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &d2OverlapdXi_1ddX: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the particle reference center-to-center vector
         * \param &d2OverlapdXi_1dR_nl: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local reference radius
         * \param &d2OverlapdXi_1dF: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the deformation gradient
         * \param &d2OverlapdXi_1dChi: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the micro-deformation tensor
         * \param &d2OverlapdXi_1dGradChi: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapddXddX: The second gradient of the overlap w.r.t. the particle reference center-to-center vector
         * \param &d2OverlapddXdR_nl: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the non-local reference radius
         * \param &d2OverlapddXdF: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the deformation gradient
         * \param &d2OverlapddXdChi: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the micro-deformation tensor
         * \param &d2OverlapddXdGradChi: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdR_nldR_nl: The second gradient of the overlap w.r.t. the non-local reference radius
         * \param &d2OverlapdR_nldF: The second gradient of the overlap w.r.t. the non-local reference radius and the deformation gradient
         * \param &d2OverlapdR_nldChi: The second gradient of the overlap w.r.t. the non-local reference radius and the micro-deformation tensor
         * \param &d2OverlapdR_nldGradChi: The second gradient of the overlap w.r.t. the non-local reference radius and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdFdF: The second gradient of the overlap w.r.t. the deformation gradient
         * \param &d2OverlapdFdChi: The second gradient of the overlap w.r.t. the deformation gradient and the micro-deformation tensor
         * \param &d2OverlapdFdGradChi: The second gradient of the overlap w.r.t. the deformation gradient and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdChidChi: The second gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &d2OverlapdChidGradChi: The second gradient of the overlap w.r.t. the micro-deformation tensor and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdGradChidGradChi: The second gradient of the overlap w.r.t. the spatial gradient of the micro-deformation tensor
         * 
         * \param &d3OverlapdXi_1dXi_1dXi_1: The third gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &d3OverlapdXi_1dXi_1ddX: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the reference center-to-center vector
         * \param &d3OverlapdXi_1dXi_1dR_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the non-local reference radius
         * \param &d3OverlapdXi_1dXi_1dF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the deformation gradient
         * \param &d3OverlapdXi_1dXi_1dChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the micro-deformation tensor
         * \param &d3OverlapdXi_1dXi_1dGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1ddXddX: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the reference center-to-center vector twice
         * \param &d3OverlapdXi_1ddXdR_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the non-local reference radius
         * \param &d3OverlapdXi_1ddXdF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the deformation gradient
         * \param &d3OverlapdXi_1ddXdChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the micro-deformation tensor
         * \param &d3OverlapdXi_1ddXdGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1dR_nldR_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local reference radius
         * \param &d3OverlapdXi_1dR_nldF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the deformation gradient
         * \param &d3OverlapdXi_1dR_nldChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the micro-deformation tensor
         * \param &d3OverlapdXi_1dR_nldGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1dFdF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the deformation gradient twice
         * \param &d3OverlapdXi_1dFdChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the deformation gradient, and the micro-deformation tensor
         * \param &d3OverlapdXi_1dFdGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the deformation gradient, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1dChidChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the micro-deformation tensor twice
         * \param &d3OverlapdXi_1dChidGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the micro-deformation tensor, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1dGradChidGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapddXddXddX: The third gradient of the overlap w.r.t. the reference center-to-center vector
         * \param &d3OverlapddXddXdR_nl: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the non-local reference radius
         * \param &d3OverlapddXddXdF: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the deformation gradient
         * \param &d3OverlapddXddXdChi: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the micro-deformation tensor
         * \param &d3OverlapddXddXdGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapddXdR_nldR_nl: The third gradient of the overlap w.r.t. the local center-to-center vector and the non-local reference radius twice
         * \param &d3OverlapddXdR_nldF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the deformation gradient
         * \param &d3OverlapddXdR_nldChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the non-local reference radius, and the micro-deformation tensor
         * \param &d3OverlapddXdR_nldGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the non-local reference radius, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapddXdFdF: The third gradient of the overlap w.r.t. the local center-to-center vector and the deformation gradient twice
         * \param &d3OverlapddXdFdChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the deformation gradient, and the micro-deformation tensor
         * \param &d3OverlapddXdFdGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the deformation gradient, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapddXdChidChi: The third gradient of the overlap w.r.t. the local center-to-center vector and the micro-deformation tensor twice
         * \param &d3OverlapddXdChidGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the micro-deformation tensor, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapddXdGradChidGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapdR_nldR_nldR_nl: The third gradient of the overlap w.r.t. the non-local reference radius
         * \param &d3OverlapdR_nldR_nldF: The third gradient of the overlap w.r.t. the the non-local reference radius twice and the deformation gradient
         * \param &d3OverlapdR_nldR_nldChi: The third gradient of the overlap w.r.t. the non-local reference radius twice and the micro-deformation tensor
         * \param &d3OverlapdR_nldR_nldGradChi: The third gradient of the overlap w.r.t. the non-local reference radius twice and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdR_nldFdF: The third gradient of the overlap w.r.t. the non-local reference radius and the deformation gradient twice
         * \param &d3OverlapdR_nldFdChi: The third gradient of the overlap w.r.t. the non-local reference radius, the deformation gradient, and the micro-deformation tensor
         * \param &d3OverlapdR_nldFdGradChi: The third gradient of the overlap w.r.t. the non-local reference radius, the deformation gradient, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdR_nldChidChi: The third gradient of the overlap w.r.t. the non-local reference radius and the micro-deformation tensor twice
         * \param &d3OverlapdR_nldChidGradChi: The third gradient of the overlap w.r.t. the non-local reference radius, the micro-deformation tensor, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdR_nldGradChidGradChi: The third gradient of the overlap w.r.t. the non-local reference radius and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapdFdFdF: The third gradient of the overlap w.r.t. the deformation gradient
         * \param &d3OverlapdFdFdChi: The third gradient of the overlap w.r.t. the deformation gradient twice and the micro-deformation tensor
         * \param &d3OverlapdFdFdGradChi: The third gradient of the overlap w.r.t. the deformation gradient twice and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdFdChidChi: The third gradient of the overlap w.r.t. the deformation gradient and the micro-deformation tensor twice
         * \param &d3OverlapdFdChidGradChi: The third gradient of the overlap w.r.t. the deformation gradient, the micro-deformation tensor, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdFdGradChidGradChi: The third gradient of the overlap w.r.t. the deformation gradient and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapdChidChidChi: The third gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &d3OverlapdChidChidGradChi: The third gradient of the overlap w.r.t. the micro-deformation tensor twice and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdChidGradChidGradChi: The third gradient of the overlap w.r.t. the micro-deformation tensor and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapdGradChidGradChidGradChi: The third gradient of the overlap w.r.t. the spatial gradient of the micro-deformation tensor
         */

        if ( chi.size( ) != Xi_1.size( ) * Xi_1.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( gradChi.size( ) != Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The gradient of the micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( gradChi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ) ) );

        }

        // Compute the non-local micro-deformation tensor
        floatVector chi_nl = chi;

        floatMatrix dchi_nldchi( chi_nl.size( ), floatVector( chi.size( ), 0 ) );

        floatMatrix dchi_nlddX( chi.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix dchi_nldGradChi( chi.size( ), floatVector( gradChi.size( ), 0 ) );

        floatMatrix d2chi_nlddXdGradChi( chi.size( ), floatVector( dX.size( ) * gradChi.size( ), 0 ) );

        floatVector eye( Xi_1.size( ) * Xi_1.size( ) );
        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < Xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                dchi_nldchi[ Xi_1.size( ) * i + I ][ Xi_1.size( ) * i + I ] = 1.;

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_nl[ Xi_1.size( ) * i + I ] += gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                    dchi_nlddX[ Xi_1.size( ) * i + I ][ J ] = gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ];

                    for ( unsigned int K = 0; K < Xi_1.size( ); K++ ){

                        for ( unsigned int L = 0; L < dX.size( ); L++ ){

                            dchi_nldGradChi[ Xi_1.size( ) * i + I ][ Xi_1.size( ) * dX.size( ) * J + dX.size( ) * K + L ] = eye[ Xi_1.size( ) * i + J ] * eye[ Xi_1.size( ) * I + K ] * dX[ L ];

                            for ( unsigned int M = 0; M < dX.size( ); M++ ){

                                d2chi_nlddXdGradChi[ Xi_1.size( ) * i + I ][ gradChi.size( ) * J + Xi_1.size( ) * Xi_1.size( ) * K + Xi_1.size( ) * L + M ] = eye[ Xi_1.size( ) * i + K ] * eye[ Xi_1.size( ) * I + L ] * eye[ Xi_1.size( ) * J + M ];

                            }

                        }

                    }

                }

            }

        }

        floatMatrix dOverlapdChi_nl, d2OverlapdXi_1dChi_nl, d2OverlapddXdChi_nl, d2OverlapdR_nldChi_nl, d2OverlapdFdChi_nl,
                    d2OverlapdChidChi_nl, d2OverlapdChi_nldChi_nl;

        floatMatrix d3OverlapdXi_1dXi_1dChi_nl, d3OverlapdXi_1ddXdChi_nl, d3OverlapdXi_1dR_nldChi_nl, d3OverlapdXi_1dFdChi_nl,
                    d3OverlapdXi_1dChidChi_nl, d3OverlapdXi_1dChi_nldChi_nl, d3OverlapddXddXdChi_nl, d3OverlapddXdR_nldChi_nl,
                    d3OverlapddXdFdChi_nl, d3OverlapddXdChidChi_nl, d3OverlapddXdChi_nldChi_nl, d3OverlapdR_nldR_nldChi_nl,
                    d3OverlapdR_nldFdChi_nl, d3OverlapdR_nldChidChi_nl, d3OverlapdR_nldChi_nldChi_nl, d3OverlapdFdFdChi_nl,
                    d3OverlapdFdChidChi_nl, d3OverlapdFdChi_nldChi_nl, d3OverlapdChidChidChi_nl, d3OverlapdChidChi_nldChi_nl,
                    d3OverlapdChi_nldChi_nldChi_nl; 

        ERROR_TOOLS_CATCH( computeParticleOverlapChi_nl( Xi_1, dX,  R_nl,
                                                         F,    chi,  chi_nl,
                                                         overlap,
                                                         dOverlapdXi_1, dOverlapddX, dOverlapdR_nl,
                                                         dOverlapdF, dOverlapdChi, dOverlapdChi_nl,
                                                         d2OverlapdXi_1dXi_1, d2OverlapdXi_1ddX, d2OverlapdXi_1dR_nl, d2OverlapdXi_1dF, d2OverlapdXi_1dChi, d2OverlapdXi_1dChi_nl,
                                                         d2OverlapddXddX, d2OverlapddXdR_nl, d2OverlapddXdF, d2OverlapddXdChi, d2OverlapddXdChi_nl,
                                                         d2OverlapdR_nldR_nl, d2OverlapdR_nldF, d2OverlapdR_nldChi, d2OverlapdR_nldChi_nl,
                                                         d2OverlapdFdF, d2OverlapdFdChi, d2OverlapdFdChi_nl,
                                                         d2OverlapdChidChi, d2OverlapdChidChi_nl,
                                                         d2OverlapdChi_nldChi_nl,
                                                         d3OverlapdXi_1dXi_1dXi_1, d3OverlapdXi_1dXi_1ddX, d3OverlapdXi_1dXi_1dR_nl, d3OverlapdXi_1dXi_1dF, d3OverlapdXi_1dXi_1dChi, d3OverlapdXi_1dXi_1dChi_nl,
                                                         d3OverlapdXi_1ddXddX, d3OverlapdXi_1ddXdR_nl, d3OverlapdXi_1ddXdF, d3OverlapdXi_1ddXdChi, d3OverlapdXi_1ddXdChi_nl,
                                                         d3OverlapdXi_1dR_nldR_nl, d3OverlapdXi_1dR_nldF, d3OverlapdXi_1dR_nldChi, d3OverlapdXi_1dR_nldChi_nl,
                                                         d3OverlapdXi_1dFdF, d3OverlapdXi_1dFdChi, d3OverlapdXi_1dFdChi_nl,
                                                         d3OverlapdXi_1dChidChi, d3OverlapdXi_1dChidChi_nl,
                                                         d3OverlapdXi_1dChi_nldChi_nl,
                                                         d3OverlapddXddXddX, d3OverlapddXddXdR_nl, d3OverlapddXddXdF, d3OverlapddXddXdChi, d3OverlapddXddXdChi_nl,
                                                         d3OverlapddXdR_nldR_nl, d3OverlapddXdR_nldF, d3OverlapddXdR_nldChi, d3OverlapddXdR_nldChi_nl,
                                                         d3OverlapddXdFdF, d3OverlapddXdFdChi, d3OverlapddXdFdChi_nl,
                                                         d3OverlapddXdChidChi, d3OverlapddXdChidChi_nl,
                                                         d3OverlapddXdChi_nldChi_nl,
                                                         d3OverlapdR_nldR_nldR_nl, d3OverlapdR_nldR_nldF, d3OverlapdR_nldR_nldChi, d3OverlapdR_nldR_nldChi_nl,
                                                         d3OverlapdR_nldFdF, d3OverlapdR_nldFdChi, d3OverlapdR_nldFdChi_nl,
                                                         d3OverlapdR_nldChidChi, d3OverlapdR_nldChidChi_nl,
                                                         d3OverlapdR_nldChi_nldChi_nl,
                                                         d3OverlapdFdFdF, d3OverlapdFdFdChi, d3OverlapdFdFdChi_nl,
                                                         d3OverlapdFdChidChi, d3OverlapdFdChidChi_nl,
                                                         d3OverlapdFdChi_nldChi_nl,
                                                         d3OverlapdChidChidChi, d3OverlapdChidChidChi_nl,
                                                         d3OverlapdChidChi_nldChi_nl,
                                                         d3OverlapdChi_nldChi_nldChi_nl ) );

        d2OverlapdXi_1dGradChi    = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * gradChi.size( ), 0 ) );

        d2OverlapddXdGradChi      = floatMatrix( overlap.size( ), floatVector( dX.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdFdGradChi       = floatMatrix( overlap.size( ), floatVector( F.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdChidGradChi     = floatMatrix( overlap.size( ), floatVector( chi.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdGradChidGradChi = floatMatrix( overlap.size( ), floatVector( gradChi.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdR_nldGradChi    = floatMatrix( overlap.size( ), floatVector( gradChi.size( ), 0 ) );

        d3OverlapdXi_1dXi_1dGradChi       = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * Xi_1.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1ddXdGradChi         = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * dX.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1dR_nldGradChi       = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1dFdGradChi          = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * F.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1dChidGradChi        = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * chi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1dGradChidGradChi    = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXddXdGradChi           = floatMatrix( overlap.size( ), floatVector( dX.size( ) * dX.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXdR_nldGradChi         = floatMatrix( overlap.size( ), floatVector( dX.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXdFdGradChi            = floatMatrix( overlap.size( ), floatVector( dX.size( ) * F.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXdChidGradChi          = floatMatrix( overlap.size( ), floatVector( dX.size( ) * chi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXdGradChidGradChi      = floatMatrix( overlap.size( ), floatVector( dX.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdR_nldR_nldGradChi       = floatMatrix( overlap.size( ), floatVector( gradChi.size( ), 0 ) );

        d3OverlapdR_nldFdGradChi          = floatMatrix( overlap.size( ), floatVector( F.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdR_nldChidGradChi        = floatMatrix( overlap.size( ), floatVector( chi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdR_nldGradChidGradChi    = floatMatrix( overlap.size( ), floatVector( gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdFdFdGradChi             = floatMatrix( overlap.size( ), floatVector( F.size( ) * F.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdFdChidGradChi           = floatMatrix( overlap.size( ), floatVector( F.size( ) * chi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdFdGradChidGradChi       = floatMatrix( overlap.size( ), floatVector( F.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdChidChidGradChi         = floatMatrix( overlap.size( ), floatVector( chi.size( ) * chi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdChidGradChidGradChi     = floatMatrix( overlap.size( ), floatVector( chi.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdGradChidGradChidGradChi = floatMatrix( overlap.size( ), floatVector( gradChi.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        dOverlapddX  += vectorTools::dot( dOverlapdChi_nl, dchi_nlddX );

        dOverlapdChi += vectorTools::dot( dOverlapdChi_nl, dchi_nldchi );

        dOverlapdGradChi = vectorTools::dot( dOverlapdChi_nl, dchi_nldGradChi );

        for ( unsigned int i = 0; i < overlap.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                for ( unsigned int J = 0; J < Xi_1.size( ); J++ ){

                    for ( unsigned int K = 0; K < dX.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdXi_1dXi_1ddX[ i ][ Xi_1.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapdXi_1dXi_1dChi_nl[ i ][ Xi_1.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ K ];

                        }

                    }

                    for ( unsigned int K = 0; K < chi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdXi_1dXi_1dChi[ i ][ Xi_1.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdXi_1dXi_1dChi_nl[ i ][ Xi_1.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ K ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdXi_1dXi_1dGradChi[ i ][ Xi_1.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dXi_1dChi_nl[ i ][ Xi_1.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }


                }

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdXi_1ddX[ i ][ dX.size( ) * I + J ] += d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                        for ( unsigned int K = 0; K < dX.size( ); K++ ){

                            d3OverlapdXi_1ddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapdXi_1ddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ]
                                                                                                           + d3OverlapdXi_1ddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdXi_1ddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ J ] * dchi_nlddX[ b ][ K ];

                            }

                        }

                        d3OverlapdXi_1ddXdR_nl[ i ][ dX.size( ) * I + J ] += d3OverlapdXi_1dR_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                        for ( unsigned int K = 0; K < F.size( ); K++ ){

                            d3OverlapdXi_1ddXdF[ i ][ dX.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3OverlapdXi_1dFdChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ];

                        }

                        for ( unsigned int K = 0; K < chi.size( ); K++ ){

                            d3OverlapdXi_1ddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdXi_1dChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ]
                                                                                                              + d3OverlapdXi_1ddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdXi_1ddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ J ] * dchi_nldchi[ b ][ K ];

                            }

                        }

                        for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                            d3OverlapdXi_1ddXdGradChi[ i ][ dX.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdXi_1dChi_nl[ i ][ chi.size( ) * I + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * J + K ]
                                                                                                                          + d3OverlapdXi_1ddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdXi_1ddXdGradChi[ i ][ dX.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

                for ( unsigned int J = 0; J < F.size( ); J++ ){

                    for ( unsigned int K = 0; K < chi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdXi_1dFdChi[ i ][ F.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdXi_1dFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ K ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdXi_1dFdGradChi[ i ][ F.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdXi_1dChi[ i ][ chi.size( ) * I + J ] += d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ];

                        d3OverlapdXi_1dR_nldChi[ i ][ chi.size( ) * I + J ] += d3OverlapdXi_1dR_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ];

                        for ( unsigned int K = 0; K < chi.size( ); K++ ){

                            d3OverlapdXi_1dChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdXi_1dChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nldchi[ a ][ J ]
                                                                                                                + d3OverlapdXi_1dChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdXi_1dChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ J ] * dchi_nldchi[ b ][ K ];

                            }

                        }

                        for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                            d3OverlapdXi_1dChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];
 
                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdXi_1dChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdXi_1dGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        d3OverlapdXi_1dR_nldGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdXi_1dR_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdXi_1dGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXddX[ i ][ dX.size( ) * I + J ] += d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ]
                                                                    + d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                        d3OverlapddXddXdR_nl[ i ][ dX.size( ) * I + J ] += d3OverlapddXdR_nldChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ]
                                                                         + d3OverlapddXdR_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                        for ( unsigned int K = 0; K < dX.size( ); K++ ){

                            d3OverlapddXddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapddXddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ]
                                                                                                         + d3OverlapddXddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ]
                                                                                                         + d3OverlapddXddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ K ];

                        }

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapddXddX[ i ][ dX.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];

                            d3OverlapddXddXdR_nl[ i ][ dX.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];

                            for ( unsigned int K = 0; K < dX.size( ); K++ ){

                                d3OverlapddXddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ J ] * dchi_nlddX[ b ][ K ]
                                                                                                             + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ K ]
                                                                                                             + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * K + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];
 
                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){
    
                                    d3OverlapddXddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ] * dchi_nlddX[ c ][ K ];
    
                                }
    
                            }

                        }

                    }

                    for ( unsigned int K = 0; K < F.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXddXdF[ i ][ dX.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3OverlapddXdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ]
                                                                                                      + d3OverlapddXdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXddXdF[ i ][ dX.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * K + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int K = 0; K < chi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapddXdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ]
                                                                                                            + d3OverlapddXdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ]
                                                                                                            + d3OverlapddXddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * K + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ]
                                                                                                                + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldchi[ b ][ K ]
                                                                                                                + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ J ] * dchi_nldchi[ b ][ K ];

                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapddXddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ] * dchi_nldchi[ c ][ K ];

                                }

                            }

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXddXdGradChi[ i ][ dX.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * J + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ]
                                                                                                                        + d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * J + K ]
                                                                                                                        + d3OverlapddXddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXddXdGradChi[ i ][ dX.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * ( d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ] * dchi_nlddX[ b ][ J ] + dchi_nlddX[ a ][ I ] * d2chi_nlddXdGradChi[ b ][ gradChi.size( ) * J + K ] )
                                                                                                                            + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ K ]
                                                                                                                            + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapddXddXdGradChi[ i ][ dX.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ] * dchi_nldGradChi[ c ][ K ];

                                }

                            }

                        }

                    }

                }

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d2OverlapddXdR_nl[ i ][ I ] += d2OverlapdR_nldChi_nl[ i ][ a ] * dchi_nlddX[ a ][ I ];

                    d3OverlapddXdR_nldR_nl[ i ][ I ] += d3OverlapdR_nldR_nldChi_nl[ i ][ a ] * dchi_nlddX[ a ][ I ];

                }

                for ( unsigned int J = 0; J < F.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdF[ i ][ chi.size( ) * I + J ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ];

                        d3OverlapddXdR_nldF[ i ][ F.size( ) * I + J ] += d3OverlapdR_nldFdChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ];

                        for ( unsigned int K = 0; K < F.size( ); K++ ){

                            d3OverlapddXdFdF[ i ][ F.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3OverlapdFdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ];

                        }

                        for ( unsigned int K = 0; K < chi.size( ); K++ ){

                            d3OverlapddXdFdChi[ i ][ F.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdFdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ]
                                                                                                          + d3OverlapddXdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXdFdChi[ i ][ F.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldchi[ b ][ K ];

                            }

                        }

                        for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                            d3OverlapddXdFdGradChi[ i ][ F.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * J + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ]
                                                                                                                      + d3OverlapddXdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXdFdGradChi[ i ][ F.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdChi[ i ][ chi.size( ) * I + J ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ]
                                                                      + d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ];

                        d3OverlapddXdR_nldChi[ i ][ chi.size( ) * I + J ] += d3OverlapdR_nldChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ]
                                                                           + d3OverlapddXdR_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapddXdChi[ i ][ chi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldchi[ b ][ J ];

                            d3OverlapddXdR_nldChi[ i ][ chi.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldchi[ b ][ J ];

                        }

                    }

                    for ( unsigned int K = 0; K < chi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXdChidChi[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3OverlapdChidChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ]
                                                                                                                             + d3OverlapddXdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nldchi[ a ][ J ]
                                                                                                                             + d3OverlapddXdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXdChidChi[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * K + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldchi[ b ][ J ]
                                                                                                                                 + d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldchi[ b ][ K ]
                                                                                                                                 + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ J ] * dchi_nldchi[ b ][ K ];

                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapddXdChidChi[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nlddX[ a ][ I ] * dchi_nldchi[ b ][ J ] * dchi_nldchi[ c ][ K ];

                                }

                            }

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXdChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ]
                                                                                                                          + d3OverlapddXdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXdChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ] * dchi_nldchi[ b ][ J ]
                                                                                                                              + d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ K ]
                                                                                                                              + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapddXdChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nlddX[ a ][ I ] * dchi_nldchi[ b ][ J ] * dchi_nldGradChi[ c ][ K ];

                                }

                            }

                        }

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ]
                                                                              + dOverlapdChi_nl[ i ][ a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + J ];

                        d3OverlapddXdR_nldGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapddXdR_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ]
                                                                                   + d2OverlapdR_nldChi_nl[ i ][ a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + J ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapddXdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                            d3OverlapddXdR_nldGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                    d3OverlapddXdGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ] * dchi_nldGradChi[ b ][ J ]
                                                                                                                                          + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ J ] * dchi_nldGradChi[ b ][ K ]
                                                                                                                                          + d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + J ] * dchi_nldGradChi[ b ][ K ];
    
                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapddXdGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ J ] * dchi_nldGradChi[ c ][ K ];
        
                                }
    
                            }

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < F.size( ); I++ ){

                for ( unsigned int J = 0; J < F.size( ); J++ ){

                    for ( unsigned int K = 0; K < chi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdFdFdChi[ i ][ F.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdFdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ K ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdFdFdGradChi[ i ][ F.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdFdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdFdChi[ i ][ chi.size( ) * I + J ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ];

                        d3OverlapdR_nldFdChi[ i ][ chi.size( ) * I + J ] += d3OverlapdR_nldFdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ];

                    }

                    for ( unsigned int K = 0; K < chi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdFdChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdFdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nldchi[ a ][ J ]
                                                                                                             + d3OverlapdFdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdFdChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ J ] * dchi_nldchi[ b ][ K ];

                            }

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdFdChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdFdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdFdChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdFdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        d3OverlapdR_nldFdGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdR_nldFdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdFdGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < chi.size( ); I++ ){

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d2OverlapdR_nldChi[ i ][ I ] += d2OverlapdR_nldChi_nl[ i ][ a ] * dchi_nldchi[ a ][ I ];

                    d3OverlapdR_nldR_nldChi[ i ][ I ] += d3OverlapdR_nldR_nldChi_nl[ i ][ a ] * dchi_nldchi[ a ][ I ];

                }

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdChidChi[ i ][ chi.size( ) * I + J ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ]
                                                                       + d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ I ];

                        d3OverlapdR_nldChidChi[ i ][ chi.size( ) * I + J ] += d3OverlapdR_nldChidChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldchi[ a ][ J ]
                                                                            + d3OverlapdR_nldChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ I ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapdChidChi[ i ][ chi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ I ] * dchi_nldchi[ b ][ J ];

                            d3OverlapdR_nldChidChi[ i ][ chi.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ I ] * dchi_nldchi[ b ][ J ];

                        }

                    }

                    for ( unsigned int K = 0; K < chi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdChidChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdChidChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nldchi[ a ][ I ]
                                                                                                               + d3OverlapdChidChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nldchi[ a ][ J ]
                                                                                                               + d3OverlapdChidChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldchi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdChidChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * K + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ I ] * dchi_nldchi[ b ][ J ]
                                                                                                                   + d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ I ] * dchi_nldchi[ b ][ K ]
                                                                                                                   + d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ J ] * dchi_nldchi[ b ][ K ];

                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapdChidChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nldchi[ a ][ I ] * dchi_nldchi[ b ][ J ] * dchi_nldchi[ c ][ K ];

                                }

                            }

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdChidChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChidChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdChidChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ I ] * dchi_nldGradChi[ b ][ K ]
                                                                                                                               + d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapdChidChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nldchi[ a ][ I ] * dchi_nldchi[ b ][ J ] * dchi_nldGradChi[ c ][ K ];

                                }

                            }

                        }

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdChidGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        d3OverlapdR_nldChidGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdR_nldChidChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapdChidGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                            d3OverlapdR_nldChidGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldchi[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdChidGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapdChidGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nldchi[ a ][ I ] * dchi_nldGradChi[ b ][ J ] * dchi_nldGradChi[ c ][ K ];

                                }

                            }

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < gradChi.size( ); I++ ){

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d2OverlapdR_nldGradChi[ i ][ I ] += d2OverlapdR_nldChi_nl[ i ][ a ] * dchi_nldGradChi[ a ][ I ];

                    d3OverlapdR_nldR_nldGradChi[ i ][ I ] += d3OverlapdR_nldR_nldChi_nl[ i ][ a ] * dchi_nldGradChi[ a ][ I ];

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapdGradChidGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                            d3OverlapdR_nldGradChidGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapdGradChidGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nldGradChi[ a ][ I ] * dchi_nldGradChi[ b ][ J ] * dchi_nldGradChi[ c ][ K ];

                                }

                            }

                        }

                    }

                }

            }

        }

        return;

    }

    void computeParticleOverlap( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                     const floatVector &F,    const floatVector &chi, const floatVector &chi_nl_basis, const floatVector &gradChi,
                                     floatVector &overlap,
                                     floatMatrix &dOverlapdXi_1, floatMatrix &dOverlapddX, floatVector &dOverlapdR_nl,
                                     floatMatrix &dOverlapdF, floatMatrix &dOverlapdChi, floatMatrix &dOverlapdChi_NL_B, floatMatrix &dOverlapdGradChi,
                                     floatMatrix &d2OverlapdXi_1dXi_1, floatMatrix &d2OverlapdXi_1ddX, floatMatrix &d2OverlapdXi_1dR_nl, floatMatrix &d2OverlapdXi_1dF, floatMatrix &d2OverlapdXi_1dChi, floatMatrix &d2OverlapdXi_1dChi_NL_B, floatMatrix &d2OverlapdXi_1dGradChi,
                                     floatMatrix &d2OverlapddXddX, floatMatrix &d2OverlapddXdR_nl, floatMatrix &d2OverlapddXdF, floatMatrix &d2OverlapddXdChi, floatMatrix &d2OverlapddXdChi_NL_B, floatMatrix &d2OverlapddXdGradChi,
                                     floatVector &d2OverlapdR_nldR_nl, floatMatrix &d2OverlapdR_nldF, floatMatrix &d2OverlapdR_nldChi, floatMatrix &d2OverlapdR_nldChi_NL_B, floatMatrix &d2OverlapdR_nldGradChi,
                                     floatMatrix &d2OverlapdFdF, floatMatrix &d2OverlapdFdChi, floatMatrix &d2OverlapdFdChi_NL_B, floatMatrix &d2OverlapdFdGradChi,
                                     floatMatrix &d2OverlapdChidChi, floatMatrix &d2OverlapdChidChi_NL_B, floatMatrix &d2OverlapdChidGradChi,
                                     floatMatrix &d2OverlapdChi_NL_BdChi_NL_B, floatMatrix &d2OverlapdChi_NL_BdGradChi,
                                     floatMatrix &d2OverlapdGradChidGradChi,
                                     floatMatrix &d3OverlapdXi_1dXi_1dXi_1, floatMatrix &d3OverlapdXi_1dXi_1ddX, floatMatrix &d3OverlapdXi_1dXi_1dR_nl, floatMatrix &d3OverlapdXi_1dXi_1dF, floatMatrix &d3OverlapdXi_1dXi_1dChi, floatMatrix &d3OverlapdXi_1dXi_1dChi_NL_B, floatMatrix &d3OverlapdXi_1dXi_1dGradChi,
                                     floatMatrix &d3OverlapdXi_1ddXddX, floatMatrix &d3OverlapdXi_1ddXdR_nl, floatMatrix &d3OverlapdXi_1ddXdF, floatMatrix &d3OverlapdXi_1ddXdChi, floatMatrix &d3OverlapdXi_1ddXdChi_NL_B, floatMatrix &d3OverlapdXi_1ddXdGradChi,
                                     floatMatrix &d3OverlapdXi_1dR_nldR_nl, floatMatrix &d3OverlapdXi_1dR_nldF, floatMatrix &d3OverlapdXi_1dR_nldChi, floatMatrix &d3OverlapdXi_1dR_nldChi_NL_B, floatMatrix &d3OverlapdXi_1dR_nldGradChi,
                                     floatMatrix &d3OverlapdXi_1dFdF, floatMatrix &d3OverlapdXi_1dFdChi, floatMatrix &d3OverlapdXi_1dFdChi_NL_B, floatMatrix &d3OverlapdXi_1dFdGradChi,
                                     floatMatrix &d3OverlapdXi_1dChidChi, floatMatrix &d3OverlapdXi_1dChidChi_NL_B, floatMatrix &d3OverlapdXi_1dChidGradChi,
                                     floatMatrix &d3OverlapdXi_1dChi_NL_BdChi_NL_B, floatMatrix &d3OverlapdXi_1dChi_NL_BdGradChi,
                                     floatMatrix &d3OverlapdXi_1dGradChidGradChi,
                                     floatMatrix &d3OverlapddXddXddX, floatMatrix &d3OverlapddXddXdR_nl, floatMatrix &d3OverlapddXddXdF, floatMatrix &d3OverlapddXddXdChi, floatMatrix &d3OverlapddXddXdChi_NL_B, floatMatrix &d3OverlapddXddXdGradChi,
                                     floatMatrix &d3OverlapddXdR_nldR_nl, floatMatrix &d3OverlapddXdR_nldF, floatMatrix &d3OverlapddXdR_nldChi, floatMatrix &d3OverlapddXdR_nldChi_NL_B, floatMatrix &d3OverlapddXdR_nldGradChi,
                                     floatMatrix &d3OverlapddXdFdF, floatMatrix &d3OverlapddXdFdChi, floatMatrix &d3OverlapddXdFdChi_NL_B, floatMatrix &d3OverlapddXdFdGradChi,
                                     floatMatrix &d3OverlapddXdChidChi, floatMatrix &d3OverlapddXdChidChi_NL_B, floatMatrix &d3OverlapddXdChidGradChi,
                                     floatMatrix &d3OverlapddXdChi_NL_BdChi_NL_B, floatMatrix &d3OverlapddXdChi_NL_BdGradChi,
                                     floatMatrix &d3OverlapddXdGradChidGradChi,
                                     floatVector &d3OverlapdR_nldR_nldR_nl, floatMatrix &d3OverlapdR_nldR_nldF, floatMatrix &d3OverlapdR_nldR_nldChi, floatMatrix &d3OverlapdR_nldR_nldChi_NL_B, floatMatrix &d3OverlapdR_nldR_nldGradChi,
                                     floatMatrix &d3OverlapdR_nldFdF, floatMatrix &d3OverlapdR_nldFdChi, floatMatrix &d3OverlapdR_nldFdChi_NL_B, floatMatrix &d3OverlapdR_nldFdGradChi,
                                     floatMatrix &d3OverlapdR_nldChidChi, floatMatrix &d3OverlapdR_nldChidChi_NL_B, floatMatrix &d3OverlapdR_nldChidGradChi,
                                     floatMatrix &d3OverlapdR_nldChi_NL_BdChi_NL_B, floatMatrix &d3OverlapdR_nldChi_NL_BdGradChi,
                                     floatMatrix &d3OverlapdR_nldGradChidGradChi,
                                     floatMatrix &d3OverlapdFdFdF, floatMatrix &d3OverlapdFdFdChi, floatMatrix &d3OverlapdFdFdChi_NL_B, floatMatrix &d3OverlapdFdFdGradChi,
                                     floatMatrix &d3OverlapdFdChidChi, floatMatrix &d3OverlapdFdChidChi_NL_B, floatMatrix &d3OverlapdFdChidGradChi,
                                     floatMatrix &d3OverlapdFdChi_NL_BdChi_NL_B, floatMatrix &d3OverlapdFdChi_NL_BdGradChi,
                                     floatMatrix &d3OverlapdFdGradChidGradChi,
                                     floatMatrix &d3OverlapdChidChidChi, floatMatrix &d3OverlapdChidChidChi_NL_B, floatMatrix &d3OverlapdChidChidGradChi,
                                     floatMatrix &d3OverlapdChidChi_NL_BdChi_NL_B, floatMatrix &d3OverlapdChidChi_NL_BdGradChi,
                                     floatMatrix &d3OverlapdChidGradChidGradChi,
                                     floatMatrix &d3OverlapdChi_NL_BdChi_NL_BdChi_NL_B, floatMatrix &d3OverlapdChi_NL_BdChi_NL_BdGradChi,
                                     floatMatrix &d3OverlapdChi_NL_BdGradChidGradChi,
                                     floatMatrix &d3OverlapdGradChidGradChidGradChi ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I = \left(\chi_{iI}^{NL,Basis} + \chi_{iI,J} dX_J\right) \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &chi_nl_basis: The non-local micro deformation tensor basis
         * \param &gradChi: The gradient of the micro deformation tensor w.r.t. the reference spatial position
         * \param &overlap: The overlap vector
         * \param &dOverlapdXi_1: The gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &dOverlapddX: The gradient of the overlap w.r.t. the local reference center-to-center vector
         * \param &dOverlapdR_nl: The gradient of the overlap w.r.t. the non-local reference radius
         * \param &dOverlapdF: The gradient of the overlap w.r.t. the deformation gradient
         * \param &dOverlapdChi: The gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &dOverlapdChi_NL_B: The gradient of the overlap w.r.t. the non-local micro-deformation tensor basis
         * \param &dOverlapdGradChi: The gradient of the overlap w.r.t. the reference spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdXi_1dXi_1: The second gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &d2OverlapdXi_1ddX: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the particle reference center-to-center vector
         * \param &d2OverlapdXi_1dR_nl: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local reference radius
         * \param &d2OverlapdXi_1dF: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the deformation gradient
         * \param &d2OverlapdXi_1dChi: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the micro-deformation tensor
         * \param &d2OverlapdXi_1dChi_NL_B: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local micro-deformation tensor basis
         * \param &d2OverlapdXi_1dGradChi: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapddXddX: The second gradient of the overlap w.r.t. the particle reference center-to-center vector
         * \param &d2OverlapddXdR_nl: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the non-local reference radius
         * \param &d2OverlapddXdF: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the deformation gradient
         * \param &d2OverlapddXdChi: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the micro-deformation tensor
         * \param &d2OverlapddXdChi_NL_B: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the non-local micro-deformation tensor basis
         * \param &d2OverlapddXdGradChi: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdR_nldR_nl: The second gradient of the overlap w.r.t. the non-local reference radius
         * \param &d2OverlapdR_nldF: The second gradient of the overlap w.r.t. the non-local reference radius and the deformation gradient
         * \param &d2OverlapdR_nldChi: The second gradient of the overlap w.r.t. the non-local reference radius and the micro-deformation tensor
         * \param &d2OverlapdR_nldChi_NL_B: The second gradient of the overlap w.r.t. the non-local reference radius and the non-local micro-deformation tensor basis
         * \param &d2OverlapdR_nldGradChi: The second gradient of the overlap w.r.t. the non-local reference radius and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdFdF: The second gradient of the overlap w.r.t. the deformation gradient
         * \param &d2OverlapdFdChi: The second gradient of the overlap w.r.t. the deformation gradient and the micro-deformation tensor
         * \param &d2OverlapdFdChi_NL_B: The second gradient of the overlap w.r.t. the deformation gradient and the non-local micro-deformation tensor basis
         * \param &d2OverlapdFdGradChi: The second gradient of the overlap w.r.t. the deformation gradient and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdChidChi: The second gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &d2OverlapdChidChi_NL_B: The second gradient of the overlap w.r.t. the micro-deformation tensor and the non-local micro-deformation tensor basis
         * \param &d2OverlapdChidGradChi: The second gradient of the overlap w.r.t. the micro-deformation tensor and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdChi_NL_BdChi_NL_B: The second gradient of the overlap w.r.t. the non-local micro-deformation tensor basis
         * \param &d2OverlapdChi_NL_BdGradChi: The second gradient of the overlap w.r.t. the non-local micro-deformation tensor basis and the spatial gradient of the micro-deformation tensor
         * \param &d2OverlapdGradChidGradChi: The second gradient of the overlap w.r.t. the spatial gradient of the micro-deformation tensor
         * 
         * \param &d3OverlapdXi_1dXi_1dXi_1: The third gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &d3OverlapdXi_1dXi_1ddX: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the reference center-to-center vector
         * \param &d3OverlapdXi_1dXi_1dR_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the non-local reference radius
         * \param &d3OverlapdXi_1dXi_1dF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the deformation gradient
         * \param &d3OverlapdXi_1dXi_1dChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the micro-deformation tensor
         * \param &d3OverlapdXi_1dXi_1dChi_NL_B: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the non-local micro-deformation tensor basis
         * \param &d3OverlapdXi_1dXi_1dGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1ddXddX: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the reference center-to-center vector twice
         * \param &d3OverlapdXi_1ddXdR_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the non-local reference radius
         * \param &d3OverlapdXi_1ddXdF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the deformation gradient
         * \param &d3OverlapdXi_1ddXdChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the micro-deformation tensor
         * \param &d3OverlapdXi_1ddXdChi_NL_B: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdXi_1ddXdGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1dR_nldR_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local reference radius
         * \param &d3OverlapdXi_1dR_nldF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the deformation gradient
         * \param &d3OverlapdXi_1dR_nldChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the micro-deformation tensor
         * \param &d3OverlapdXi_1dR_nldChi_NL_B: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdXi_1dR_nldGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1dFdF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the deformation gradient twice
         * \param &d3OverlapdXi_1dFdChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the deformation gradient, and the micro-deformation tensor
         * \param &d3OverlapdXi_1dFdChi_NL_B: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the deformation gradient, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdXi_1dFdGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the deformation gradient, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1dChidChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the micro-deformation tensor twice
         * \param &d3OverlapdXi_1dChidChi_NL_B: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the micro-deformation tensor, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdXi_1dChidGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the micro-deformation tensor, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1dChi_NL_BdChi_NL_B: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local micro-deformation tensor basis, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdXi_1dChi_NL_BdGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local micro-deformation tensor basis, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdXi_1dGradChidGradChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapddXddXddX: The third gradient of the overlap w.r.t. the reference center-to-center vector
         * \param &d3OverlapddXddXdR_nl: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the non-local reference radius
         * \param &d3OverlapddXddXdF: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the deformation gradient
         * \param &d3OverlapddXddXdChi: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the micro-deformation tensor
         * \param &d3OverlapddXddXdChi_NL_B: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the non-local micro-deformation tensor basis
         * \param &d3OverlapddXddXdGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapddXdR_nldR_nl: The third gradient of the overlap w.r.t. the local center-to-center vector and the non-local reference radius twice
         * \param &d3OverlapddXdR_nldF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the deformation gradient
         * \param &d3OverlapddXdR_nldChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the non-local reference radius, and the micro-deformation tensor
         * \param &d3OverlapddXdR_nldChi_NL_B: The third gradient of the overlap w.r.t. the local center-to-center vector, the non-local reference radius, and the non-local micro-deformation tensor basis
         * \param &d3OverlapddXdR_nldGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the non-local reference radius, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapddXdFdF: The third gradient of the overlap w.r.t. the local center-to-center vector and the deformation gradient twice
         * \param &d3OverlapddXdFdChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the deformation gradient, and the micro-deformation tensor
         * \param &d3OverlapddXdFdChi_NL_B: The third gradient of the overlap w.r.t. the local center-to-center vector, the deformation gradient, and the non-local micro-deformation tensor basis
         * \param &d3OverlapddXdFdGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the deformation gradient, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapddXdChidChi: The third gradient of the overlap w.r.t. the local center-to-center vector and the micro-deformation tensor twice
         * \param &d3OverlapddXdChidChi_NL_B: The third gradient of the overlap w.r.t. the local center-to-center vector, the micro-deformation tensor, and the non-local micro-deformation tensor basis
         * \param &d3OverlapddXdChidGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the micro-deformation tensor, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapddXdChi_NL_BdChi_NL_B: The third gradient of the overlap w.r.t. the local center-to-center vector, the non-local micro-deformation tensor basis, and the non-local micro-deformation tensor basis
         * \param &d3OverlapddXdChi_NL_BdGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the non-local micro-deformation tensor basis, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapddXdGradChidGradChi: The third gradient of the overlap w.r.t. the local center-to-center vector and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapdR_nldR_nldR_nl: The third gradient of the overlap w.r.t. the non-local reference radius
         * \param &d3OverlapdR_nldR_nldF: The third gradient of the overlap w.r.t. the the non-local reference radius twice and the deformation gradient
         * \param &d3OverlapdR_nldR_nldChi: The third gradient of the overlap w.r.t. the non-local reference radius twice and the micro-deformation tensor
         * \param &d3OverlapdR_nldR_nldChi_NL_B: The third gradient of the overlap w.r.t. the non-local reference radius twice and the non-local micro-deformation tensor basis
         * \param &d3OverlapdR_nldR_nldGradChi: The third gradient of the overlap w.r.t. the non-local reference radius twice and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdR_nldFdF: The third gradient of the overlap w.r.t. the non-local reference radius and the deformation gradient twice
         * \param &d3OverlapdR_nldFdChi: The third gradient of the overlap w.r.t. the non-local reference radius, the deformation gradient, and the micro-deformation tensor
         * \param &d3OverlapdR_nldFdChi_NL_B: The third gradient of the overlap w.r.t. the non-local reference radius, the deformation gradient, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdR_nldFdGradChi: The third gradient of the overlap w.r.t. the non-local reference radius, the deformation gradient, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdR_nldChidChi: The third gradient of the overlap w.r.t. the non-local reference radius and the micro-deformation tensor twice
         * \param &d3OverlapdR_nldChidChi_NL_B: The third gradient of the overlap w.r.t. the non-local reference radius, the micro-deformation tensor, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdR_nldChidGradChi: The third gradient of the overlap w.r.t. the non-local reference radius, the micro-deformation tensor, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdR_nldChi_NL_BdChi_NL_B: The third gradient of the overlap w.r.t. the non-local reference radius, the non-local micro-deformation tensor basis, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdR_nldChi_NL_BdGradChi: The third gradient of the overlap w.r.t. the non-local reference radius, the non-local micro-deformation tensor basis, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdR_nldGradChidGradChi: The third gradient of the overlap w.r.t. the non-local reference radius and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapdFdFdF: The third gradient of the overlap w.r.t. the deformation gradient
         * \param &d3OverlapdFdFdChi: The third gradient of the overlap w.r.t. the deformation gradient twice and the micro-deformation tensor
         * \param &d3OverlapdFdFdChi_NL_B: The third gradient of the overlap w.r.t. the deformation gradient twice and the non-local micro-deformation tensor basis
         * \param &d3OverlapdFdFdGradChi: The third gradient of the overlap w.r.t. the deformation gradient twice and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdFdChidChi: The third gradient of the overlap w.r.t. the deformation gradient and the micro-deformation tensor twice
         * \param &d3OverlapdFdChidChi_NL_B: The third gradient of the overlap w.r.t. the deformation gradient, the micro-deformation tensor, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdFdChidGradChi: The third gradient of the overlap w.r.t. the deformation gradient, the micro-deformation tensor, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdFdChi_NL_BdChi_NL_B: The third gradient of the overlap w.r.t. the deformation gradient, the non-local micro-deformation tensor basis, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdFdChi_NL_BdGradChi: The third gradient of the overlap w.r.t. the deformation gradient, the non-local micro-deformation tensor basis, and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdFdGradChidGradChi: The third gradient of the overlap w.r.t. the deformation gradient and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapdChidChidChi: The third gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &d3OverlapdChidChidChi_NL_B: The third gradient of the overlap w.r.t. the micro-deformation tensor twice and the non-local micro-deformation tensor basis
         * \param &d3OverlapdChidChidGradChi: The third gradient of the overlap w.r.t. the micro-deformation tensor twice and the spatial gradient of the micro-deformation tensor
         * \param &d3OverlapdChidChi_NL_BdChi_NL_B: The third gradient of the overlap w.r.t. the micro-deformation tensor, the non-local micro-deformation tensor basis, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdChidChi_NL_BdGradChi: The third gradient of the overlap w.r.t. the micro-deformation tensor, the non-local micro-deformation tensor, and the gradient of the micro-deformation tensor basis
         * \param &d3OverlapdChidGradChidGradChi: The third gradient of the overlap w.r.t. the micro-deformation tensor and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapdChi_NL_BdChi_NL_BdChi_NL_B: The third gradient of the overlap w.r.t. the non-local micro-deformation tensor basis, the non-local micro-deformation tensor basis, and the non-local micro-deformation tensor basis
         * \param &d3OverlapdChi_NL_BdChi_NL_BdGradChi: The third gradient of the overlap w.r.t. the non-local micro-deformation tensor basis, the non-local micro-deformation tensor basis, and the gradient of the micro-deformation tensor basis
         * \param &d3OverlapdChi_NL_BdGradChidGradChi: The third gradient of the overlap w.r.t. the non-local micro-deformation tensor basis and the spatial gradient of the micro-deformation tensor twice
         * \param &d3OverlapdGradChidGradChidGradChi: The third gradient of the overlap w.r.t. the spatial gradient of the micro-deformation tensor
         */

        if ( chi.size( ) != Xi_1.size( ) * Xi_1.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( chi_nl_basis.size( ) != ( Xi_1.size( ) * Xi_1.size( ) ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The incoming chi non-local basis vector has an inconsistent size with the micro-position vector\n  size is " + std::to_string( chi_nl_basis.size( ) ) + " and must be " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( gradChi.size( ) != Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The gradient of the micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( gradChi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) * dX.size( ) ) ) );

        }

        // Compute the non-local micro-deformation tensor
        floatVector chi_nl = chi_nl_basis;

        floatMatrix dchi_nlddX( chi.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix dchi_nldGradChi( chi.size( ), floatVector( gradChi.size( ), 0 ) );

        floatMatrix d2chi_nlddXdGradChi( chi.size( ), floatVector( dX.size( ) * gradChi.size( ), 0 ) );

        floatVector eye( Xi_1.size( ) * Xi_1.size( ) );
        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < Xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    chi_nl[ Xi_1.size( ) * i + I ] += gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ] * dX[ J ];

                    dchi_nlddX[ Xi_1.size( ) * i + I ][ J ] = gradChi[ Xi_1.size( ) * dX.size( ) * i + dX.size( ) * I + J ];

                    for ( unsigned int K = 0; K < Xi_1.size( ); K++ ){

                        for ( unsigned int L = 0; L < dX.size( ); L++ ){

                            dchi_nldGradChi[ Xi_1.size( ) * i + I ][ Xi_1.size( ) * dX.size( ) * J + dX.size( ) * K + L ] = eye[ Xi_1.size( ) * i + J ] * eye[ Xi_1.size( ) * I + K ] * dX[ L ];

                            for ( unsigned int M = 0; M < dX.size( ); M++ ){

                                d2chi_nlddXdGradChi[ Xi_1.size( ) * i + I ][ gradChi.size( ) * J + Xi_1.size( ) * Xi_1.size( ) * K + Xi_1.size( ) * L + M ] = eye[ Xi_1.size( ) * i + K ] * eye[ Xi_1.size( ) * I + L ] * eye[ Xi_1.size( ) * J + M ];

                            }

                        }

                    }

                }

            }

        }

        floatMatrix dOverlapdChi_nl, d2OverlapdXi_1dChi_nl, d2OverlapddXdChi_nl, d2OverlapdR_nldChi_nl, d2OverlapdFdChi_nl,
                    d2OverlapdChidChi_nl, d2OverlapdChi_nldChi_nl;

        floatMatrix d3OverlapdXi_1dXi_1dChi_nl, d3OverlapdXi_1ddXdChi_nl, d3OverlapdXi_1dR_nldChi_nl, d3OverlapdXi_1dFdChi_nl,
                    d3OverlapdXi_1dChidChi_nl, d3OverlapdXi_1dChi_nldChi_nl, d3OverlapddXddXdChi_nl, d3OverlapddXdR_nldChi_nl,
                    d3OverlapddXdFdChi_nl, d3OverlapddXdChidChi_nl, d3OverlapddXdChi_nldChi_nl, d3OverlapdR_nldR_nldChi_nl,
                    d3OverlapdR_nldFdChi_nl, d3OverlapdR_nldChidChi_nl, d3OverlapdR_nldChi_nldChi_nl, d3OverlapdFdFdChi_nl,
                    d3OverlapdFdChidChi_nl, d3OverlapdFdChi_nldChi_nl, d3OverlapdChidChidChi_nl, d3OverlapdChidChi_nldChi_nl,
                    d3OverlapdChi_nldChi_nldChi_nl; 

        ERROR_TOOLS_CATCH( computeParticleOverlapChi_nl( Xi_1, dX,  R_nl,
                                                         F,    chi,  chi_nl,
                                                         overlap,
                                                         dOverlapdXi_1, dOverlapddX, dOverlapdR_nl,
                                                         dOverlapdF, dOverlapdChi, dOverlapdChi_nl,
                                                         d2OverlapdXi_1dXi_1, d2OverlapdXi_1ddX, d2OverlapdXi_1dR_nl, d2OverlapdXi_1dF, d2OverlapdXi_1dChi, d2OverlapdXi_1dChi_nl,
                                                         d2OverlapddXddX, d2OverlapddXdR_nl, d2OverlapddXdF, d2OverlapddXdChi, d2OverlapddXdChi_nl,
                                                         d2OverlapdR_nldR_nl, d2OverlapdR_nldF, d2OverlapdR_nldChi, d2OverlapdR_nldChi_nl,
                                                         d2OverlapdFdF, d2OverlapdFdChi, d2OverlapdFdChi_nl,
                                                         d2OverlapdChidChi, d2OverlapdChidChi_nl,
                                                         d2OverlapdChi_nldChi_nl,
                                                         d3OverlapdXi_1dXi_1dXi_1, d3OverlapdXi_1dXi_1ddX, d3OverlapdXi_1dXi_1dR_nl, d3OverlapdXi_1dXi_1dF, d3OverlapdXi_1dXi_1dChi, d3OverlapdXi_1dXi_1dChi_nl,
                                                         d3OverlapdXi_1ddXddX, d3OverlapdXi_1ddXdR_nl, d3OverlapdXi_1ddXdF, d3OverlapdXi_1ddXdChi, d3OverlapdXi_1ddXdChi_nl,
                                                         d3OverlapdXi_1dR_nldR_nl, d3OverlapdXi_1dR_nldF, d3OverlapdXi_1dR_nldChi, d3OverlapdXi_1dR_nldChi_nl,
                                                         d3OverlapdXi_1dFdF, d3OverlapdXi_1dFdChi, d3OverlapdXi_1dFdChi_nl,
                                                         d3OverlapdXi_1dChidChi, d3OverlapdXi_1dChidChi_nl,
                                                         d3OverlapdXi_1dChi_nldChi_nl,
                                                         d3OverlapddXddXddX, d3OverlapddXddXdR_nl, d3OverlapddXddXdF, d3OverlapddXddXdChi, d3OverlapddXddXdChi_nl,
                                                         d3OverlapddXdR_nldR_nl, d3OverlapddXdR_nldF, d3OverlapddXdR_nldChi, d3OverlapddXdR_nldChi_nl,
                                                         d3OverlapddXdFdF, d3OverlapddXdFdChi, d3OverlapddXdFdChi_nl,
                                                         d3OverlapddXdChidChi, d3OverlapddXdChidChi_nl,
                                                         d3OverlapddXdChi_nldChi_nl,
                                                         d3OverlapdR_nldR_nldR_nl, d3OverlapdR_nldR_nldF, d3OverlapdR_nldR_nldChi, d3OverlapdR_nldR_nldChi_nl,
                                                         d3OverlapdR_nldFdF, d3OverlapdR_nldFdChi, d3OverlapdR_nldFdChi_nl,
                                                         d3OverlapdR_nldChidChi, d3OverlapdR_nldChidChi_nl,
                                                         d3OverlapdR_nldChi_nldChi_nl,
                                                         d3OverlapdFdFdF, d3OverlapdFdFdChi, d3OverlapdFdFdChi_nl,
                                                         d3OverlapdFdChidChi, d3OverlapdFdChidChi_nl,
                                                         d3OverlapdFdChi_nldChi_nl,
                                                         d3OverlapdChidChidChi, d3OverlapdChidChidChi_nl,
                                                         d3OverlapdChidChi_nldChi_nl,
                                                         d3OverlapdChi_nldChi_nldChi_nl ) );

        d2OverlapdXi_1dChi_NL_B     = d2OverlapdXi_1dChi_nl;

        d2OverlapdXi_1dGradChi      = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * gradChi.size( ), 0 ) );

        d2OverlapddXdChi_NL_B       = d2OverlapddXdChi_nl;

        d2OverlapddXdGradChi        = floatMatrix( overlap.size( ), floatVector( dX.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdR_nldChi_NL_B     = d2OverlapdR_nldChi_nl;

        d2OverlapdR_nldGradChi      = floatMatrix( overlap.size( ), floatVector( gradChi.size( ), 0 ) );

        d2OverlapdFdChi_NL_B        = d2OverlapdFdChi_nl;

        d2OverlapdFdGradChi         = floatMatrix( overlap.size( ), floatVector( F.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdChidChi_NL_B      = d2OverlapdChidChi_nl;

        d2OverlapdChidGradChi       = floatMatrix( overlap.size( ), floatVector( chi.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdChi_NL_BdChi_NL_B = d2OverlapdChi_nldChi_nl;

        d2OverlapdChi_NL_BdGradChi  = floatMatrix( overlap.size( ), floatVector( chi_nl_basis.size( ) * gradChi.size( ), 0 ) );

        d2OverlapdGradChidGradChi   = floatMatrix( overlap.size( ), floatVector( gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1dXi_1dChi_NL_B         = d3OverlapdXi_1dXi_1dChi_nl;

        d3OverlapdXi_1dXi_1dGradChi          = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * Xi_1.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1ddXdChi_NL_B           = d3OverlapdXi_1ddXdChi_nl;

        d3OverlapdXi_1ddXdGradChi            = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * dX.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1dR_nldChi_NL_B         = d3OverlapdXi_1dR_nldChi_nl;

        d3OverlapdXi_1dR_nldGradChi          = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1dFdChi_NL_B            = d3OverlapdXi_1dFdChi_nl;

        d3OverlapdXi_1dFdGradChi             = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * F.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1dChidChi_NL_B          = d3OverlapdXi_1dChidChi_nl;

        d3OverlapdXi_1dChidGradChi           = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * chi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1dChi_NL_BdChi_NL_B     = d3OverlapdXi_1dChi_nldChi_nl;

        d3OverlapdXi_1dChi_NL_BdGradChi      = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * chi_nl_basis.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdXi_1dGradChidGradChi       = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXddXdChi_NL_B             = d3OverlapddXddXdChi_nl;

        d3OverlapddXddXdGradChi              = floatMatrix( overlap.size( ), floatVector( dX.size( ) * dX.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXdR_nldChi_NL_B           = d3OverlapddXdR_nldChi_nl;

        d3OverlapddXdR_nldGradChi            = floatMatrix( overlap.size( ), floatVector( dX.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXdFdChi_NL_B              = d3OverlapddXdFdChi_nl;

        d3OverlapddXdFdGradChi               = floatMatrix( overlap.size( ), floatVector( dX.size( ) * F.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXdChidChi_NL_B            = d3OverlapddXdChidChi_nl;

        d3OverlapddXdChidGradChi             = floatMatrix( overlap.size( ), floatVector( dX.size( ) * chi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXdChi_NL_BdChi_NL_B       = d3OverlapddXdChi_nldChi_nl;

        d3OverlapddXdChi_NL_BdGradChi        = floatMatrix( overlap.size( ), floatVector( dX.size( ) * chi_nl_basis.size( ) * gradChi.size( ), 0 ) );

        d3OverlapddXdGradChidGradChi         = floatMatrix( overlap.size( ), floatVector( dX.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdR_nldR_nldChi_NL_B         = d3OverlapdR_nldR_nldChi_nl;

        d3OverlapdR_nldR_nldGradChi          = floatMatrix( overlap.size( ), floatVector( gradChi.size( ), 0 ) );

        d3OverlapdR_nldFdChi_NL_B            = d3OverlapdR_nldFdChi_nl;

        d3OverlapdR_nldFdGradChi             = floatMatrix( overlap.size( ), floatVector( F.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdR_nldChidChi_NL_B          = d3OverlapdR_nldChidChi_nl;

        d3OverlapdR_nldChidGradChi           = floatMatrix( overlap.size( ), floatVector( chi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdR_nldChi_NL_BdChi_NL_B     = d3OverlapdR_nldChi_nldChi_nl;

        d3OverlapdR_nldChi_NL_BdGradChi      = floatMatrix( overlap.size( ), floatVector( chi_nl_basis.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdR_nldGradChidGradChi       = floatMatrix( overlap.size( ), floatVector( gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdFdFdChi_NL_B               = d3OverlapdFdFdChi_nl;

        d3OverlapdFdFdGradChi                = floatMatrix( overlap.size( ), floatVector( F.size( ) * F.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdFdChidChi_NL_B             = d3OverlapdFdChidChi_nl;

        d3OverlapdFdChidGradChi              = floatMatrix( overlap.size( ), floatVector( F.size( ) * chi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdFdChi_NL_BdChi_NL_B        = d3OverlapdFdChi_nldChi_nl;

        d3OverlapdFdChi_NL_BdGradChi         = floatMatrix( overlap.size( ), floatVector( F.size( ) * chi_nl_basis.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdFdGradChidGradChi          = floatMatrix( overlap.size( ), floatVector( F.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdChidChidChi_NL_B           = d3OverlapdChidChidChi_nl;

        d3OverlapdChidChidGradChi            = floatMatrix( overlap.size( ), floatVector( chi.size( ) * chi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdChidChi_NL_BdChi_NL_B      = d3OverlapdChidChi_nldChi_nl;

        d3OverlapdChidChi_NL_BdGradChi       = floatMatrix( overlap.size( ), floatVector( chi.size( ) * chi_nl_basis.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdChidGradChidGradChi        = floatMatrix( overlap.size( ), floatVector( chi.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdChi_NL_BdChi_NL_BdChi_NL_B = d3OverlapdChi_nldChi_nldChi_nl;

        d3OverlapdChi_NL_BdChi_NL_BdGradChi  = floatMatrix( overlap.size( ), floatVector( chi_nl_basis.size( ) * chi_nl_basis.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdChi_NL_BdGradChidGradChi   = floatMatrix( overlap.size( ), floatVector( chi_nl_basis.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        d3OverlapdGradChidGradChidGradChi    = floatMatrix( overlap.size( ), floatVector( gradChi.size( ) * gradChi.size( ) * gradChi.size( ), 0 ) );

        dOverlapddX  += vectorTools::dot( dOverlapdChi_nl, dchi_nlddX );

        dOverlapdChi_NL_B = dOverlapdChi_nl;

        dOverlapdGradChi = vectorTools::dot( dOverlapdChi_nl, dchi_nldGradChi );

        for ( unsigned int i = 0; i < overlap.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                for ( unsigned int J = 0; J < Xi_1.size( ); J++ ){

                    for ( unsigned int K = 0; K < dX.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdXi_1dXi_1ddX[ i ][ Xi_1.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapdXi_1dXi_1dChi_nl[ i ][ Xi_1.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ K ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdXi_1dXi_1dGradChi[ i ][ Xi_1.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dXi_1dChi_nl[ i ][ Xi_1.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdXi_1ddX[ i ][ dX.size( ) * I + J ] += d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                        d3OverlapdXi_1ddXdR_nl[ i ][ dX.size( ) * I + J ] += d3OverlapdXi_1dR_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                        for ( unsigned int K = 0; K < F.size( ); K++ ){

                            d3OverlapdXi_1ddXdF[ i ][ dX.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3OverlapdXi_1dFdChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ];

                        }

                        for ( unsigned int K = 0; K < chi.size( ); K++ ){

                            d3OverlapdXi_1ddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdXi_1dChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ];

                        }

                        for ( unsigned int K = 0; K < chi_nl_basis.size( ); K++ ){

                            d3OverlapdXi_1ddXdChi_NL_B[ i ][ dX.size( ) * chi_nl_basis.size( ) * I + chi_nl_basis.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ];

                        }

                        for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                            d3OverlapdXi_1ddXdGradChi[ i ][ dX.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdXi_1dChi_nl[ i ][ chi.size( ) * I + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * J + K ]
                                                                                                                          + d3OverlapdXi_1ddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdXi_1ddXdGradChi[ i ][ dX.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                    for ( unsigned int K = 0; K < dX.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdXi_1ddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapdXi_1ddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ K ]
                                                                                                           + d3OverlapdXi_1ddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdXi_1ddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ J ] * dchi_nlddX[ b ][ K ];

                            }

                        }

                    }

                }

                for ( unsigned int J = 0; J < F.size( ); J++ ){

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdXi_1dFdGradChi[ i ][ F.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }


                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                            d3OverlapdXi_1dChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < chi_nl_basis.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                            d3OverlapdXi_1dChi_NL_BdGradChi[ i ][ chi_nl_basis.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }


                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdXi_1dGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        d3OverlapdXi_1dR_nldGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdXi_1dR_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdXi_1dGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d3OverlapddXdR_nldR_nl[ i ][ I ] += d3OverlapdR_nldR_nldChi_nl[ i ][ a ] * dchi_nlddX[ a ][ I ];

                }

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXddX[ i ][ dX.size( ) * I + J ] += d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ]
                                                                    + d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                        d3OverlapddXddXdR_nl[ i ][ dX.size( ) * I + J ] += d3OverlapddXdR_nldChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ]
                                                                         + d3OverlapddXdR_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nlddX[ a ][ J ];

                        for ( unsigned int K = 0; K < dX.size( ); K++ ){

                            d3OverlapddXddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapddXddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ]
                                                                                                         + d3OverlapddXddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ]
                                                                                                         + d3OverlapddXddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ K ];

                        }

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapddXddX[ i ][ dX.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];

                            d3OverlapddXddXdR_nl[ i ][ dX.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];

                            for ( unsigned int K = 0; K < dX.size( ); K++ ){

                                d3OverlapddXddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ J ] * dchi_nlddX[ b ][ K ]
                                                                                                             + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ K ]
                                                                                                             + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * K + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];
 
                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){
    
                                    d3OverlapddXddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ] * dchi_nlddX[ c ][ K ];
    
                                }
    
                            }

                        }

                    }

                    for ( unsigned int K = 0; K < F.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXddXdF[ i ][ dX.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3OverlapddXdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ]
                                                                                                      + d3OverlapddXdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXddXdF[ i ][ dX.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * K + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int K = 0; K < chi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapddXdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ]
                                                                                                            + d3OverlapddXdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * K + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int K = 0; K < chi_nl_basis.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXddXdChi_NL_B[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapddXdChi_nldChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ]
                                                                                                                 + d3OverlapddXdChi_nldChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ J ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXddXdChi_NL_B[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * K + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ];

                            }

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXddXdGradChi[ i ][ dX.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * J + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ]
                                                                                                                        + d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * J + K ]
                                                                                                                        + d3OverlapddXddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXddXdGradChi[ i ][ dX.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * ( d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ] * dchi_nlddX[ b ][ J ] + dchi_nlddX[ a ][ I ] * d2chi_nlddXdGradChi[ b ][ gradChi.size( ) * J + K ] )
                                                                                                                            + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ K ]
                                                                                                                            + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapddXddXdGradChi[ i ][ dX.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nlddX[ a ][ I ] * dchi_nlddX[ b ][ J ] * dchi_nldGradChi[ c ][ K ];

                                }

                            }

                        }

                    }

                }

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d2OverlapddXdR_nl[ i ][ I ] += d2OverlapdR_nldChi_nl[ i ][ a ] * dchi_nlddX[ a ][ I ];

                }

                for ( unsigned int J = 0; J < F.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdF[ i ][ chi.size( ) * I + J ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ];

                        d3OverlapddXdR_nldF[ i ][ chi.size( ) * I + J ] += d3OverlapdR_nldFdChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ];

                        for ( unsigned int K = 0; K < F.size( ); K++ ){

                            d3OverlapddXdFdF[ i ][ F.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3OverlapdFdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ];

                        }

                        for ( unsigned int K = 0; K < chi.size( ); K++ ){

                            d3OverlapddXdFdChi[ i ][ F.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3OverlapdFdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ];

                        }

                        for ( unsigned int K = 0; K < chi_nl_basis.size( ); K++ ){

                            d3OverlapddXdFdChi_NL_B[ i ][ F.size( ) * chi_nl_basis.size( ) * I + chi_nl_basis.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ];

                        }

                        for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                            d3OverlapddXdFdGradChi[ i ][ F.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * J + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ]
                                                                                                                      + d3OverlapddXdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXdFdGradChi[ i ][ F.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdChi[ i ][ chi.size( ) * I + J ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ];

                        d3OverlapddXdR_nldChi[ i ][ chi.size( ) * I + J ] += d3OverlapdR_nldChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * dchi_nlddX[ a ][ I ];

                    }

                    for ( unsigned int K = 0; K < chi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXdChidChi[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3OverlapdChidChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ];

                        }

                    }

                    for ( unsigned int K = 0; K < chi_nl_basis.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXdChidChi_NL_B[ i ][ chi.size( ) * chi_nl_basis.size( ) * I + chi_nl_basis.size( ) * J + K ] += d3OverlapdChidChi_nldChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXdChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * J + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ]
                                                                                                                          + d3OverlapddXdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXdChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

                for ( unsigned int J = 0; J < chi_nl_basis.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdChi_NL_B[ i ][ chi_nl_basis.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + J ] * dchi_nlddX[ a ][ I ];

                        d3OverlapddXdR_nldChi_NL_B[ i ][ chi_nl_basis.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * a + J ] * dchi_nlddX[ a ][ I ];

                    }

                    for ( unsigned int K = 0; K < chi_nl_basis.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXdChi_NL_BdChi_NL_B[ i ][ chi.size( ) * chi_nl_basis.size( ) * I + chi_nl_basis.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * K + a ] * dchi_nlddX[ a ][ I ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapddXdChi_NL_BdGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * J + a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ]
                                                                                                                               + d3OverlapddXdChi_nldChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapddXdChi_NL_BdGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapddXdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ]
                                                                              + dOverlapdChi_nl[ i ][ a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + J ];

                        d3OverlapddXdR_nldGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapddXdR_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ]
                                                                                   + d2OverlapdR_nldChi_nl[ i ][ a ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + J ];

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapddXdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                            d3OverlapddXdR_nldGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                    d3OverlapddXdGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + K ] * dchi_nldGradChi[ b ][ J ]
                                                                                                                                          + d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ J ] * dchi_nldGradChi[ b ][ K ]
                                                                                                                                          + d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * d2chi_nlddXdGradChi[ a ][ gradChi.size( ) * I + J ] * dchi_nldGradChi[ b ][ K ];
    
                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapddXdGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nlddX[ a ][ I ] * dchi_nldGradChi[ b ][ J ] * dchi_nldGradChi[ c ][ K ];
        
                                }
    
                            }

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < F.size( ); I++ ){

                for ( unsigned int J = 0; J < F.size( ); J++ ){

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdFdFdGradChi[ i ][ F.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdFdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdFdChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdFdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < chi_nl_basis.size( ); J++ ){

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdFdChi_NL_BdGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdFdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        d3OverlapdR_nldFdGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdR_nldFdChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdFdGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdFdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < chi.size( ); I++ ){

                for ( unsigned int J = 0; J < chi.size( ); J++ ){

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdChidChidGradChi[ i ][ chi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChidChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < chi_nl_basis.size( ); J++ ){

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdChidChi_NL_BdGradChi[ i ][ chi_nl_basis.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChidChi_nldChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdChidGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        d3OverlapdR_nldChidGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdR_nldChidChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdChidGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < chi_nl_basis.size( ); I++ ){

                for ( unsigned int J = 0; J < chi_nl_basis.size( ); J++ ){

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            d3OverlapdChi_NL_BdChi_NL_BdGradChi[ i ][ chi_nl_basis.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + a ] * dchi_nldGradChi[ a ][ K ];

                        }

                    }

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        d2OverlapdChi_NL_BdGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                        d3OverlapdR_nldChi_NL_BdGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * I + a ] * dchi_nldGradChi[ a ][ J ];

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                d3OverlapdChi_NL_BdGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ J ] * dchi_nldGradChi[ b ][ K ];

                            }

                        }

                    }

                }

            }

            for ( unsigned int I = 0; I < gradChi.size( ); I++ ){

                for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                    d2OverlapdR_nldGradChi[ i ][ I ] += d2OverlapdR_nldChi_nl[ i ][ a ] * dchi_nldGradChi[ a ][ I ];

                    d3OverlapdR_nldR_nldGradChi[ i ][ I ] += d3OverlapdR_nldR_nldChi_nl[ i ][ a ] * dchi_nldGradChi[ a ][ I ];

                }

                for ( unsigned int J = 0; J < gradChi.size( ); J++ ){

                    for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                        for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                            d2OverlapdGradChidGradChi[ i ][ gradChi.size( ) * I + J ] += d2OverlapdChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                            d3OverlapdR_nldGradChidGradChi[ i ][ gradChi.size( ) * I + J ] += d3OverlapdR_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * a + b ] * dchi_nldGradChi[ a ][ I ] * dchi_nldGradChi[ b ][ J ];

                        }

                    }

                    for ( unsigned int K = 0; K < gradChi.size( ); K++ ){

                        for ( unsigned int a = 0; a < chi_nl.size( ); a++ ){

                            for ( unsigned int b = 0; b < chi_nl.size( ); b++ ){

                                for ( unsigned int c = 0; c < chi_nl.size( ); c++ ){

                                    d3OverlapdGradChidGradChidGradChi[ i ][ gradChi.size( ) * gradChi.size( ) * I + gradChi.size( ) * J + K ] += d3OverlapdChi_nldChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * b + c ] * dchi_nldGradChi[ a ][ I ] * dchi_nldGradChi[ b ][ J ] * dchi_nldGradChi[ c ][ K ];

                                }

                            }

                        }

                    }

                }

            }

        }

        return;

    }

    void computeParticleOverlapChi_nl( const floatVector &Xi_1, const floatVector &dX, const floatType &R_nl,
                                           const floatVector &F,    const floatVector &chi,  const floatVector &chi_nl,
                                           floatVector &overlap,
                                           floatMatrix &dOverlapdXi_1, floatMatrix &dOverlapddX, floatVector &dOverlapdR_nl,
                                           floatMatrix &dOverlapdF, floatMatrix &dOverlapdChi, floatMatrix &dOverlapdChi_nl,
                                           floatMatrix &d2OverlapdXi_1dXi_1, floatMatrix &d2OverlapdXi_1ddX, floatMatrix &d2OverlapdXi_1dR_nl, floatMatrix &d2OverlapdXi_1dF, floatMatrix &d2OverlapdXi_1dChi, floatMatrix &d2OverlapdXi_1dChi_nl,
                                           floatMatrix &d2OverlapddXddX, floatMatrix &d2OverlapddXdR_nl, floatMatrix &d2OverlapddXdF, floatMatrix &d2OverlapddXdChi, floatMatrix &d2OverlapddXdChi_nl,
                                           floatVector &d2OverlapdR_nldR_nl, floatMatrix &d2OverlapdR_nldF, floatMatrix &d2OverlapdR_nldChi, floatMatrix &d2OverlapdR_nldChi_nl,
                                           floatMatrix &d2OverlapdFdF, floatMatrix &d2OverlapdFdChi, floatMatrix &d2OverlapdFdChi_nl,
                                           floatMatrix &d2OverlapdChidChi, floatMatrix &d2OverlapdChidChi_nl,
                                           floatMatrix &d2OverlapdChi_nldChi_nl,
                                           floatMatrix &d3OverlapdXi_1dXi_1dXi_1, floatMatrix &d3OverlapdXi_1dXi_1ddX, floatMatrix &d3OverlapdXi_1dXi_1dR_nl, floatMatrix &d3OverlapdXi_1dXi_1dF, floatMatrix &d3OverlapdXi_1dXi_1dChi, floatMatrix &d3OverlapdXi_1dXi_1dChi_nl,
                                           floatMatrix &d3OverlapdXi_1ddXddX, floatMatrix &d3OverlapdXi_1ddXdR_nl, floatMatrix &d3OverlapdXi_1ddXdF, floatMatrix &d3OverlapdXi_1ddXdChi, floatMatrix &d3OverlapdXi_1ddXdChi_nl,
                                           floatMatrix &d3OverlapdXi_1dR_nldR_nl, floatMatrix &d3OverlapdXi_1dR_nldF, floatMatrix &d3OverlapdXi_1dR_nldChi, floatMatrix &d3OverlapdXi_1dR_nldChi_nl,
                                           floatMatrix &d3OverlapdXi_1dFdF, floatMatrix &d3OverlapdXi_1dFdChi, floatMatrix &d3OverlapdXi_1dFdChi_nl,
                                           floatMatrix &d3OverlapdXi_1dChidChi, floatMatrix &d3OverlapdXi_1dChidChi_nl,
                                           floatMatrix &d3OverlapdXi_1dChi_nldChi_nl,
                                           floatMatrix &d3OverlapddXddXddX, floatMatrix &d3OverlapddXddXdR_nl, floatMatrix &d3OverlapddXddXdF, floatMatrix &d3OverlapddXddXdChi, floatMatrix &d3OverlapddXddXdChi_nl,
                                           floatMatrix &d3OverlapddXdR_nldR_nl, floatMatrix &d3OverlapddXdR_nldF, floatMatrix &d3OverlapddXdR_nldChi, floatMatrix &d3OverlapddXdR_nldChi_nl,
                                           floatMatrix &d3OverlapddXdFdF, floatMatrix &d3OverlapddXdFdChi, floatMatrix &d3OverlapddXdFdChi_nl,
                                           floatMatrix &d3OverlapddXdChidChi, floatMatrix &d3OverlapddXdChidChi_nl,
                                           floatMatrix &d3OverlapddXdChi_nldChi_nl,
                                           floatVector &d3OverlapdR_nldR_nldR_nl, floatMatrix &d3OverlapdR_nldR_nldF, floatMatrix &d3OverlapdR_nldR_nldChi, floatMatrix &d3OverlapdR_nldR_nldChi_nl,
                                           floatMatrix &d3OverlapdR_nldFdF, floatMatrix &d3OverlapdR_nldFdChi, floatMatrix &d3OverlapdR_nldFdChi_nl,
                                           floatMatrix &d3OverlapdR_nldChidChi, floatMatrix &d3OverlapdR_nldChidChi_nl,
                                           floatMatrix &d3OverlapdR_nldChi_nldChi_nl,
                                           floatMatrix &d3OverlapdFdFdF, floatMatrix &d3OverlapdFdFdChi, floatMatrix &d3OverlapdFdFdChi_nl,
                                           floatMatrix &d3OverlapdFdChidChi, floatMatrix &d3OverlapdFdChidChi_nl,
                                           floatMatrix &d3OverlapdFdChi_nldChi_nl,
                                           floatMatrix &d3OverlapdChidChidChi, floatMatrix &d3OverlapdChidChidChi_nl,
                                           floatMatrix &d3OverlapdChidChi_nldChi_nl,
                                           floatMatrix &d3OverlapdChi_nldChi_nldChi_nl ){
        /*!
         * Compute the amount that a point on the local particle overlaps with the non-local particle. For now, we assume
         * a micromorphic theory of degree 1 meaning that for the local particle
         * 
         * \f$ \xi_i = \chi_{iI} \Xi_I\f$
         * 
         * and for the non-local particle
         * 
         * \f$ \xi_i^{NL} = \chi_{iI}^{NL} \Xi_I = \left(\chi_{iI} + \chi_{iI,J} dX_J\right) \Xi_I\f$
         * 
         * where
         * 
         * \f$ dX_I = Xi_I^1 + D_I - Xi_I^2 \f$
         * 
         * So we first must determine if the particles are overlapped which can be done via computing the relative position vector
         * of \f$ \Xi^1 \f$ with respect to the non-local centroid and seeing if it's magnitude in the non-local reference configuration
         * is less than the non-local particle's radius. If so we will solve for the shortest distance between the overlapped local point
         * and the surface of the non-local particle.
         * 
         * \param &Xi_1: The local micro relative position vector to test.
         * \param &dX: The spacing between the local and non-local particle centroids in the reference configuration
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &F: The deformation gradient
         * \param &chi: The micro deformation tensor
         * \param &chi_nl: The non-local micro deformation tensor
         * \param &overlap: The overlap vector
         * \param &dOverlapdXi_1: The gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &dOverlapddX: The gradient of the overlap w.r.t. the local reference center-to-center vector
         * \param &dOverlapdR_nl: The gradient of the overlap w.r.t. the non-local reference radius
         * \param &dOverlapdF: The gradient of the overlap w.r.t. the deformation gradient
         * \param &dOverlapdChi: The gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &dOverlapdChi_nl: The gradient of the overlap w.r.t. the non-local micro-deformation tensor
         * \param &d2OverlapdXi_1dXi_1: The second gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &d2OverlapdXi_1ddX: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the particle reference center-to-center vector
         * \param &d2OverlapdXi_1dR_nl: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local reference radius
         * \param &d2OverlapdXi_1dF: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the deformation gradient
         * \param &d2OverlapdXi_1dChi: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the micro-deformation tensor
         * \param &d2OverlapdXi_1dChi_nl: The second gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local micro-deformation tensor
         * \param &d2OverlapddXddX: The second gradient of the overlap w.r.t. the particle reference center-to-center vector
         * \param &d2OverlapddXdR_nl: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the non-local reference radius
         * \param &d2OverlapddXdF: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the deformation gradient
         * \param &d2OverlapddXdChi: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the micro-deformation tensor
         * \param &d2OverlapddXdChi_nl: The second gradient of the overlap w.r.t. the particle reference center-to-center vector and the non-local micro-deformation tensor
         * \param &d2OverlapdR_nldR_nl: The second gradient of the overlap w.r.t. the non-local reference radius
         * \param &d2OverlapdR_nldF: The second gradient of the overlap w.r.t. the non-local reference radius and the deformation gradient
         * \param &d2OverlapdR_nldChi: The second gradient of the overlap w.r.t. the non-local reference radius and the micro-deformation tensor
         * \param &d2OverlapdR_nldChi_nl: The second gradient of the overlap w.r.t. the non-local reference radius and the non-local micro-deformation tensor
         * \param &d2OverlapdFdF: The second gradient of the overlap w.r.t. the deformation gradient
         * \param &d2OverlapdFdChi: The second gradient of the overlap w.r.t. the deformation gradient and the micro-deformation tensor
         * \param &d2OverlapdFdChi_nl: The second gradient of the overlap w.r.t. the deformation gradient and the non-local micro-deformation tensor
         * \param &d2OverlapdChidChi: The second gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &d2OverlapdChidChi_nl: The second gradient of the overlap w.r.t. the micro-deformation tensor and the non-local micro-deformation tensor
         * \param &d2OverlapdChi_nldChi_nl: The second gradient of the overlap w.r.t. the non-local micro-deformation tensor
         * 
         * \param &d3OverlapdXi_1dXi_1dXi_1: The third gradient of the overlap w.r.t. the local reference relative micro-position vector
         * \param &d3OverlapdXi_1dXi_1ddX: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the reference center-to-center vector
         * \param &d3OverlapdXi_1dXi_1dR_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the non-local reference radius
         * \param &d3OverlapdXi_1dXi_1dF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the deformation gradient
         * \param &d3OverlapdXi_1dXi_1dChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the micro-deformation tensor
         * \param &d3OverlapdXi_1dXi_1dChi_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector twice and the non-local micro-deformation tensor
         * \param &d3OverlapdXi_1ddXddX: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the reference center-to-center vector twice
         * \param &d3OverlapdXi_1ddXdR_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the non-local reference radius
         * \param &d3OverlapdXi_1ddXdF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the deformation gradient
         * \param &d3OverlapdXi_1ddXdChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the micro-deformation tensor
         * \param &d3OverlapdXi_1ddXdChi_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the reference center-to-center vector, and the non-local micro-deformation tensor
         * \param &d3OverlapdXi_1dR_nldR_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local reference radius
         * \param &d3OverlapdXi_1dR_nldF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the deformation gradient
         * \param &d3OverlapdXi_1dR_nldChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the micro-deformation tensor
         * \param &d3OverlapdXi_1dR_nldChi_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the non-local micro-deformation tensor
         * \param &d3OverlapdXi_1dFdF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the deformation gradient twice
         * \param &d3OverlapdXi_1dFdChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the deformation gradient, and the micro-deformation tensor
         * \param &d3OverlapdXi_1dFdChi_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the deformation gradient, and the non-local micro-deformation tensor
         * \param &d3OverlapdXi_1dChidChi: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the micro-deformation tensor twice
         * \param &d3OverlapdXi_1dChidChi_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the micro-deformation tensor, and the non-local micro-deformation tensor
         * \param &d3OverlapdXi_1dChi_nldChi_nl: The third gradient of the overlap w.r.t. the local reference relative micro-position vector and the non-local micro-deformation tensor twice
         * \param &d3OverlapddXddXddX: The third gradient of the overlap w.r.t. the reference center-to-center vector
         * \param &d3OverlapddXddXdR_nl: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the non-local reference radius
         * \param &d3OverlapddXddXdF: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the deformation gradient
         * \param &d3OverlapddXddXdChi: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the micro-deformation tensor
         * \param &d3OverlapddXddXdChi_nl: The third gradient of the overlap w.r.t. the local center-to-center vector twice and the non-local micro-deformation tensor
         * \param &d3OverlapddXdR_nldR_nl: The third gradient of the overlap w.r.t. the local center-to-center vector and the non-local reference radius twice
         * \param &d3OverlapddXdR_nldF: The third gradient of the overlap w.r.t. the local reference relative micro-position vector, the non-local reference radius, and the deformation gradient
         * \param &d3OverlapddXdR_nldChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the non-local reference radius, and the micro-deformation tensor
         * \param &d3OverlapddXdR_nldChi_nl: The third gradient of the overlap w.r.t. the local center-to-center vector, the non-local reference radius, and the non-local micro-deformation tensor
         * \param &d3OverlapddXdFdF: The third gradient of the overlap w.r.t. the local center-to-center vector and the deformation gradient twice
         * \param &d3OverlapddXdFdChi: The third gradient of the overlap w.r.t. the local center-to-center vector, the deformation gradient, and the micro-deformation tensor
         * \param &d3OverlapddXdFdChi_nl: The third gradient of the overlap w.r.t. the local center-to-center vector, the deformation gradient, and the non-local micro-deformation tensor
         * \param &d3OverlapddXdChidChi: The third gradient of the overlap w.r.t. the local center-to-center vector and the micro-deformation tensor twice
         * \param &d3OverlapddXdChidChi_nl: The third gradient of the overlap w.r.t. the local center-to-center vector, the micro-deformation tensor, and the non-local micro-deformation tensor
         * \param &d3OverlapddXdChi_nldChi_nl: The third gradient of the overlap w.r.t. the local center-to-center vector and the non-local micro-deformation tensor twice
         * \param &d3OverlapdR_nldR_nldR_nl: The third gradient of the overlap w.r.t. the non-local reference radius
         * \param &d3OverlapdR_nldR_nldF: The third gradient of the overlap w.r.t. the the non-local reference radius twice and the deformation gradient
         * \param &d3OverlapdR_nldR_nldChi: The third gradient of the overlap w.r.t. the non-local reference radius twice and the micro-deformation tensor
         * \param &d3OverlapdR_nldR_nldChi_nl: The third gradient of the overlap w.r.t. the non-local reference radius twice and the non-local micro-deformation tensor
         * \param &d3OverlapdR_nldFdF: The third gradient of the overlap w.r.t. the non-local reference radius and the deformation gradient twice
         * \param &d3OverlapdR_nldFdChi: The third gradient of the overlap w.r.t. the non-local reference radius, the deformation gradient, and the micro-deformation tensor
         * \param &d3OverlapdR_nldFdChi_nl: The third gradient of the overlap w.r.t. the non-local reference radius, the deformation gradient, and the non-local micro-deformation tensor
         * \param &d3OverlapdR_nldChidChi: The third gradient of the overlap w.r.t. the non-local reference radius and the micro-deformation tensor twice
         * \param &d3OverlapdR_nldChidChi_nl: The third gradient of the overlap w.r.t. the non-local reference radius, the micro-deformation tensor, and the non-local micro-deformation tensor
         * \param &d3OverlapdR_nldChi_nldChi_nl: The third gradient of the overlap w.r.t. the non-local reference radius and the non-local micro-deformation tensor twice
         * \param &d3OverlapdFdFdF: The third gradient of the overlap w.r.t. the deformation gradient
         * \param &d3OverlapdFdFdChi: The third gradient of the overlap w.r.t. the deformation gradient twice and the micro-deformation tensor
         * \param &d3OverlapdFdFdChi_nl: The third gradient of the overlap w.r.t. the deformation gradient twice and the non-local micro-deformation tensor
         * \param &d3OverlapdFdChidChi: The third gradient of the overlap w.r.t. the deformation gradient and the micro-deformation tensor twice
         * \param &d3OverlapdFdChidChi_nl: The third gradient of the overlap w.r.t. the deformation gradient, the micro-deformation tensor, and the non-local micro-deformation tensor
         * \param &d3OverlapdFdChi_nldChi_nl: The third gradient of the overlap w.r.t. the deformation gradient and the non-local micro-deformation tensor twice
         * \param &d3OverlapdChidChidChi: The third gradient of the overlap w.r.t. the micro-deformation tensor
         * \param &d3OverlapdChidChidChi_nl: The third gradient of the overlap w.r.t. the micro-deformation tensor twice and the non-local micro-deformation tensor
         * \param &d3OverlapdChidChi_nldChi_nl: The third gradient of the overlap w.r.t. the micro-deformation tensor and the non-local micro-deformation tensor twice
         * \param &d3OverlapdChi_nldChi_nldChi_nl: The third gradient of the overlap w.r.t. the non-local micro-deformation tensor
         */
        if ( Xi_1.size( ) != dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The local micro relative position vector and the inter-particle spacing should have the same dimension\n\tXi_1: " + std::to_string( Xi_1.size( ) ) + "\n\tdX: " + std::to_string( dX.size( ) ) ) );

        }

        if ( F.size( ) != dX.size( ) * dX.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The deformation gradient is not the expected dimension.\n\tF: " + std::to_string( F.size( ) ) + "\n\texpected: " + std::to_string( dX.size( ) * dX.size( ) ) ) );

        }

        if ( chi.size( ) != Xi_1.size( ) * Xi_1.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        if ( chi_nl.size( ) != Xi_1.size( ) * Xi_1.size( ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The non-local micro-deformation tensor is not the expected dimension.\n\tF: " + std::to_string( chi_nl.size( ) ) + "\n\texpected: " + std::to_string( Xi_1.size( ) * Xi_1.size( ) ) ) );

        }

        // Compute the current configuration local relative position vector and inter-particle spacing
        floatVector xi_1( Xi_1.size( ), 0 );

        floatVector eye( Xi_1.size( ) * Xi_1.size( ), 0 );
        vectorTools::eye( eye );

        floatMatrix dxi_1dXi_1( xi_1.size( ), floatVector( Xi_1.size( ), 0 ) );

        floatMatrix dxi_1dChi( xi_1.size( ), floatVector( chi.size( ), 0 ) );

        floatMatrix d2xi_1dXi_1dChi( xi_1.size( ), floatVector( Xi_1.size( ) * chi.size( ), 0 ) );

        for ( unsigned int i = 0; i < xi_1.size( ); i++ ){

            for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                xi_1[ i ] += chi[ Xi_1.size( ) * i + I ] * Xi_1[ I ];

                dxi_1dXi_1[ i ][ I ] += chi[ Xi_1.size( ) * i + I ];

                for ( unsigned int J = 0; J < Xi_1.size( ); J++ ){

                    dxi_1dChi[ i ][ Xi_1.size( ) * I + J ] += eye[ Xi_1.size( ) * i + I ] * Xi_1[ J ];

                    for ( unsigned int K = 0; K < Xi_1.size( ); K++ ){

                        d2xi_1dXi_1dChi[ i ][ chi.size( ) * I + Xi_1.size( ) * J + K ] += eye[ Xi_1.size( ) * i + J ] * eye[ Xi_1.size( ) * I + K ];

                    }

                }

            }

        }

        floatVector dx( dX.size( ), 0 );

        floatMatrix ddxddX( dx.size( ), floatVector( dX.size( ), 0 ) );

        floatMatrix ddxdF( dx.size( ), floatVector( F.size( ), 0 ) );

        floatMatrix d2dxddXdF( dx.size( ), floatVector( dX.size( ) * F.size( ), 0 ) );

        for ( unsigned int i = 0; i < dx.size( ); i++ ){

            for ( unsigned int I = 0; I < dX.size( ); I++ ){

                dx[ i ] += F[ dx.size( ) * i + I ] * dX[ I ];

                ddxddX[ i ][ I ] += F[ dx.size( ) * i + I ];

                for ( unsigned int J = 0; J < dX.size( ); J++ ){

                    ddxdF[ i ][ dX.size( ) * I + J ] += eye[ dX.size( ) * i + I ] * dX[ J ];

                    for ( unsigned int K = 0; K < dX.size( ); K++ ){

                        d2dxddXdF[ i ][ F.size( ) * I + dX.size( ) * J + K ] += eye[ dx.size( ) * i + J ] * eye[ dx.size( ) * I + K ];

                    }

                }

            }

        }

        // Compute the representation of xi_1 in the non-local particle's reference frame
        floatVector xi_t = xi_1 - dx;

        floatMatrix dxi_tdXi_1 = dxi_1dXi_1;

        floatMatrix dxi_tddX = -ddxddX;

        floatMatrix dxi_tdchi = dxi_1dChi;

        floatMatrix dxi_tdF = -ddxdF;

        floatMatrix d2xi_tdXi_1dChi = d2xi_1dXi_1dChi;

        floatMatrix d2xi_tddXdF = -d2dxddXdF;

        // Compute the inverse of the non-local micro-deformation tensor
        if ( vectorTools::determinant( chi_nl, xi_t.size( ), Xi_1.size( ) ) <= 0 ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The non-local micro deformation gradient has a determinant less than or equal to zero" ) );

        }

        floatVector inv_chi_nl = vectorTools::inverse( chi_nl, xi_t.size( ), Xi_1.size( ) );

        floatVector Xi_t( Xi_1.size( ), 0 );

        for ( unsigned int I = 0; I < Xi_t.size( ); I++ ){

            for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

                Xi_t[ I ] += inv_chi_nl[ Xi_t.size( ) * I + i ] * xi_t[ i ];

            }

        }  

        d2OverlapdXi_1dXi_1               = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * Xi_1.size( ), 0 ) );

        d2OverlapdXi_1ddX                 = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * dX.size( ), 0 ) );

        d2OverlapdXi_1dR_nl               = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ), 0 ) );

        d2OverlapdXi_1dF                  = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * F.size( ), 0 ) );

        d2OverlapdXi_1dChi                = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * chi.size( ), 0 ) );

        d2OverlapdXi_1dChi_nl             = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * chi_nl.size( ), 0 ) );

        d2OverlapddXddX                   = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * dX.size( ), 0 ) );

        d2OverlapddXdR_nl                 = floatMatrix( xi_t.size( ), floatVector( dX.size( ), 0 ) );

        d2OverlapddXdF                    = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * F.size( ), 0 ) );

        d2OverlapddXdChi                  = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * chi.size( ), 0 ) );

        d2OverlapddXdChi_nl               = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * chi_nl.size( ), 0 ) );

        d2OverlapdR_nldR_nl               = floatVector( xi_t.size( ), 0 );

        d2OverlapdR_nldF                  = floatMatrix( xi_t.size( ), floatVector( F.size( ), 0 ) );

        d2OverlapdR_nldChi                = floatMatrix( xi_t.size( ), floatVector( chi.size( ), 0 ) );

        d2OverlapdR_nldChi_nl             = floatMatrix( xi_t.size( ), floatVector( chi_nl.size( ), 0 ) );

        d2OverlapdFdF                     = floatMatrix( xi_t.size( ), floatVector( F.size( ) * F.size( ), 0 ) );

        d2OverlapdFdChi                   = floatMatrix( xi_t.size( ), floatVector( F.size( ) * chi.size( ), 0 ) );

        d2OverlapdFdChi_nl                = floatMatrix( xi_t.size( ), floatVector( F.size( ) * chi_nl.size( ), 0 ) );

        d2OverlapdChidChi                 = floatMatrix( xi_t.size( ), floatVector( chi.size( ) * chi.size( ), 0 ) );

        d2OverlapdChidChi_nl              = floatMatrix( xi_t.size( ), floatVector( chi.size( ) * chi_nl.size( ), 0 ) );

        d2OverlapdChi_nldChi_nl           = floatMatrix( xi_t.size( ), floatVector( chi_nl.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdXi_1dXi_1dXi_1          = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * Xi_1.size( ) * Xi_1.size( ), 0 ) );

        d3OverlapdXi_1dXi_1ddX            = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * Xi_1.size( ) * dX.size( ), 0 ) );

        d3OverlapdXi_1dXi_1dR_nl          = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * Xi_1.size( ), 0 ) );

        d3OverlapdXi_1dXi_1dF             = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * Xi_1.size( ) * F.size( ), 0 ) );

        d3OverlapdXi_1dXi_1dChi           = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * Xi_1.size( ) * chi.size( ), 0 ) );

        d3OverlapdXi_1dXi_1dChi_nl        = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * Xi_1.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdXi_1ddXddX              = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * dX.size( ) * dX.size( ), 0 ) );

        d3OverlapdXi_1ddXdR_nl            = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * dX.size( ), 0 ) );

        d3OverlapdXi_1ddXdF               = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * dX.size( ) * F.size( ), 0 ) );

        d3OverlapdXi_1ddXdChi             = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * dX.size( ) * chi.size( ), 0 ) );

        d3OverlapdXi_1ddXdChi_nl          = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * dX.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdXi_1dR_nldR_nl          = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ), 0 ) );

        d3OverlapdXi_1dR_nldF             = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * F.size( ), 0 ) );

        d3OverlapdXi_1dR_nldChi           = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * chi.size( ), 0 ) );

        d3OverlapdXi_1dR_nldChi_nl        = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdXi_1dFdF                = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * F.size( ) * F.size( ), 0 ) );

        d3OverlapdXi_1dFdChi              = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * F.size( ) * chi.size( ), 0 ) );

        d3OverlapdXi_1dFdChi_nl           = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * F.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdXi_1dChidChi            = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * chi.size( ) * chi.size( ), 0 ) );

        d3OverlapdXi_1dChidChi_nl         = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * chi.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdXi_1dChi_nldChi_nl      = floatMatrix( xi_t.size( ), floatVector( Xi_1.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapddXddXddX                = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * dX.size( ) * dX.size( ), 0 ) );

        d3OverlapddXddXdR_nl              = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * dX.size( ), 0 ) );

        d3OverlapddXddXdF                 = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * dX.size( ) * F.size( ), 0 ) );

        d3OverlapddXddXdChi               = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * dX.size( ) * chi.size( ), 0 ) );

        d3OverlapddXddXdChi_nl            = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * dX.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapddXdR_nldR_nl            = floatMatrix( xi_t.size( ), floatVector( dX.size( ), 0 ) );

        d3OverlapddXdR_nldF               = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * F.size( ), 0 ) );

        d3OverlapddXdR_nldChi             = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * chi.size( ), 0 ) );

        d3OverlapddXdR_nldChi_nl          = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapddXdFdF                  = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * F.size( ) * F.size( ), 0 ) );

        d3OverlapddXdFdChi                = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * F.size( ) * chi.size( ), 0 ) );

        d3OverlapddXdFdChi_nl             = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * F.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapddXdChidChi              = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * chi.size( ) * chi.size( ), 0 ) );

        d3OverlapddXdChidChi_nl           = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * chi.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapddXdChi_nldChi_nl        = floatMatrix( xi_t.size( ), floatVector( dX.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdR_nldR_nldR_nl          = floatVector( xi_t.size( ), 0 );

        d3OverlapdR_nldR_nldF             = floatMatrix( xi_t.size( ), floatVector( F.size( ), 0 ) );

        d3OverlapdR_nldR_nldChi           = floatMatrix( xi_t.size( ), floatVector( chi.size( ), 0 ) );

        d3OverlapdR_nldR_nldChi_nl        = floatMatrix( xi_t.size( ), floatVector( chi_nl.size( ), 0 ) );

        d3OverlapdR_nldFdF                = floatMatrix( xi_t.size( ), floatVector( F.size( ) * F.size( ), 0 ) );

        d3OverlapdR_nldFdChi              = floatMatrix( xi_t.size( ), floatVector( F.size( ) * chi.size( ), 0 ) );

        d3OverlapdR_nldFdChi_nl           = floatMatrix( xi_t.size( ), floatVector( F.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdR_nldChidChi            = floatMatrix( xi_t.size( ), floatVector( chi.size( ) * chi.size( ), 0 ) );

        d3OverlapdR_nldChidChi_nl         = floatMatrix( xi_t.size( ), floatVector( chi.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdR_nldChi_nldChi_nl      = floatMatrix( xi_t.size( ), floatVector( chi_nl.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdFdFdF                   = floatMatrix( xi_t.size( ), floatVector( F.size( ) * F.size( ) * F.size( ), 0 ) );

        d3OverlapdFdFdChi                 = floatMatrix( xi_t.size( ), floatVector( F.size( ) * F.size( ) * chi.size( ), 0 ) );

        d3OverlapdFdFdChi_nl              = floatMatrix( xi_t.size( ), floatVector( F.size( ) * F.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdFdChidChi               = floatMatrix( xi_t.size( ), floatVector( F.size( ) * chi.size( ) * chi.size( ), 0 ) );

        d3OverlapdFdChidChi_nl            = floatMatrix( xi_t.size( ), floatVector( F.size( ) * chi.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdFdChi_nldChi_nl         = floatMatrix( xi_t.size( ), floatVector( F.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdChidChidChi             = floatMatrix( xi_t.size( ), floatVector( chi.size( ) * chi.size( ) * chi.size( ), 0 ) );

        d3OverlapdChidChidChi_nl          = floatMatrix( xi_t.size( ), floatVector( chi.size( ) * chi.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdChidChi_nldChi_nl       = floatMatrix( xi_t.size( ), floatVector( chi.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 ) );

        d3OverlapdChi_nldChi_nldChi_nl    = floatMatrix( xi_t.size( ), floatVector( chi_nl.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 ) );

        if ( vectorTools::dot( Xi_t, Xi_t ) > ( R_nl * R_nl ) ){

            overlap = floatVector( xi_t.size( ), 0 );

            dOverlapdXi_1 = floatMatrix( overlap.size( ), floatVector( Xi_1.size( ), 0 ) );

            dOverlapddX = floatMatrix( overlap.size( ), floatVector( dX.size( ), 0 ) );

            dOverlapdR_nl = floatVector( overlap.size( ), 0 );

            dOverlapdF = floatMatrix( overlap.size( ), floatVector( F.size( ), 0 ) );

            dOverlapdChi = floatMatrix( overlap.size( ), floatVector( chi.size( ), 0 ) );

            dOverlapdChi_nl = floatMatrix( overlap.size( ), floatVector( chi_nl.size( ), 0 ) );

        }
        else{

            floatMatrix dOverlapdchi_nl, dOverlapdxi_t;

            floatMatrix d2Overlapdchi_nldxi_t,
                        d2Overlapdxi_tdxi_t, d2Overlapdxi_tdR_nl;

            floatMatrix d3Overlapdchi_nldchi_nldxi_t,
                        d3Overlapdchi_nldxi_tdxi_t, d3Overlapdchi_nldxi_tdR_nl,
                        d3Overlapdxi_tdxi_tdxi_t, d3Overlapdxi_tdxi_tdR_nl,
                        d3Overlapdxi_tdR_nldR_nl;

            ERROR_TOOLS_CATCH( solveOverlapDistance( chi_nl, xi_t, R_nl, overlap, dOverlapdChi_nl, dOverlapdxi_t, dOverlapdR_nl,
                                                     d2OverlapdChi_nldChi_nl, d2Overlapdchi_nldxi_t, d2OverlapdR_nldChi_nl,
                                                     d2Overlapdxi_tdxi_t, d2Overlapdxi_tdR_nl,
                                                     d2OverlapdR_nldR_nl,
                                                     d3OverlapdChi_nldChi_nldChi_nl, d3Overlapdchi_nldchi_nldxi_t, d3OverlapdR_nldChi_nldChi_nl,
                                                     d3Overlapdchi_nldxi_tdxi_t, d3Overlapdchi_nldxi_tdR_nl,
                                                     d3OverlapdR_nldR_nldChi_nl,
                                                     d3Overlapdxi_tdxi_tdxi_t, d3Overlapdxi_tdxi_tdR_nl,
                                                     d3Overlapdxi_tdR_nldR_nl,
                                                     d3OverlapdR_nldR_nldR_nl ) );

            dOverlapdXi_1 = vectorTools::dot( dOverlapdxi_t, dxi_tdXi_1 );

            dOverlapddX   = vectorTools::dot( dOverlapdxi_t, dxi_tddX );

            dOverlapdF    = vectorTools::dot( dOverlapdxi_t, dxi_tdF );

            dOverlapdChi  = vectorTools::dot( dOverlapdxi_t, dxi_tdchi );

            d2OverlapdXi_1dR_nl = vectorTools::dot( d2Overlapdxi_tdR_nl, dxi_tdXi_1 );

            d3OverlapdXi_1dR_nldR_nl = vectorTools::dot( d3Overlapdxi_tdR_nldR_nl, dxi_tdXi_1 );

            d2OverlapddXdR_nl = vectorTools::dot( d2Overlapdxi_tdR_nl, dxi_tddX );

            d3OverlapddXdR_nldF = vectorTools::dot( d2Overlapdxi_tdR_nl, d2xi_tddXdF );

            d3OverlapddXdR_nldR_nl = vectorTools::dot( d3Overlapdxi_tdR_nldR_nl, dxi_tddX );

            d2OverlapdR_nldF = vectorTools::dot( d2Overlapdxi_tdR_nl, dxi_tdF );

            d2OverlapdR_nldChi = vectorTools::dot( d2Overlapdxi_tdR_nl, dxi_tdchi );

            d2OverlapddXdF = vectorTools::dot( dOverlapdxi_t, d2xi_tddXdF );

            d2OverlapdXi_1dChi = vectorTools::dot( dOverlapdxi_t, d2xi_tdXi_1dChi );

            d3OverlapdR_nldR_nldF = vectorTools::dot( d3Overlapdxi_tdR_nldR_nl, dxi_tdF );

            d3OverlapdR_nldR_nldChi = vectorTools::dot( d3Overlapdxi_tdR_nldR_nl, dxi_tdchi );

            for ( unsigned int i = 0; i < overlap.size( ); i++ ){

                for ( unsigned int I = 0; I < Xi_1.size( ); I++ ){

                    for ( unsigned int J = 0; J < Xi_1.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdXi_1dXi_1[ i ][ Xi_1.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdXi_1[ b ][ J ];

                                for ( unsigned int K = 0; K < Xi_1.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdXi_1dXi_1dXi_1[ i ][ Xi_1.size( ) * Xi_1.size( ) * I + Xi_1.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdXi_1[ a ][ I ] * dxi_tdXi_1[ b ][ J ] * dxi_tdXi_1[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < dX.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdXi_1dXi_1ddX[ i ][ Xi_1.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdXi_1[ a ][ I ] * dxi_tdXi_1[ b ][ J ] * dxi_tddX[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < F.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdXi_1dXi_1dF[ i ][ Xi_1.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdXi_1[ a ][ I ] * dxi_tdXi_1[ b ][ J ] * dxi_tdF[ c ][ K ];

                                    }

                                }

                                d3OverlapdXi_1dXi_1dR_nl[ i ][ Xi_1.size( ) * I + J ] += d3Overlapdxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdXi_1[ b ][ J ];

                                for ( unsigned int K = 0; K < chi.size( ); K++ ){

                                    d3OverlapdXi_1dXi_1dChi[ i ][ Xi_1.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * d2xi_tdXi_1dChi[ a ][ chi.size( ) * I + K ] * dxi_tdXi_1[ b ][ J ]
                                                                                                                          + d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * d2xi_tdXi_1dChi[ b ][ chi.size( ) * J + K ];

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdXi_1dXi_1dChi[ i ][ Xi_1.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdXi_1[ a ][ I ] * dxi_tdXi_1[ b ][ J ] * dxi_tdchi[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                    d3OverlapdXi_1dXi_1dChi_nl[ i ][ Xi_1.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdXi_1[ b ][ J ];

                                }


                            }

                        }

                    }

                    for ( unsigned int J = 0; J < dX.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdXi_1ddX[ i ][ dX.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tddX[ b ][ J ];

                                for ( unsigned int K = 0; K < dX.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdXi_1ddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdXi_1[ a ][ I ] * dxi_tddX[ b ][ J ] * dxi_tddX[ c ][ K ];

                                    }

                                }

                                d3OverlapdXi_1ddXdR_nl[ i ][ dX.size( ) * I + J ] += d3Overlapdxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tddX[ b ][ J ];

                                for ( unsigned int K = 0; K < F.size( ); K++ ){

                                    d3OverlapdXi_1ddXdF[ i ][ dX.size( ) * F.size( ) * I + F.size( ) * J + K ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * d2xi_tddXdF[ b ][ F.size( ) * J + K ];

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdXi_1ddXdF[ i ][ dX.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdXi_1[ a ][ I ] * dxi_tddX[ b ][ J ] * dxi_tdF[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi.size( ); K++ ){

                                    d3OverlapdXi_1ddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * d2xi_tdXi_1dChi[ a ][ chi.size( ) * I + K ] * dxi_tddX[ b ][ J ];

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdXi_1ddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdXi_1[ a ][ I ] * dxi_tddX[ b ][ J ] * dxi_tdchi[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                    d3OverlapdXi_1ddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tddX[ b ][ J ];

                                }

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < F.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdXi_1dF[ i ][ F.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdF[ b ][ J ];

                                d3OverlapdXi_1dR_nldF[ i ][ F.size( ) * I + J ] += d3Overlapdxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdF[ b ][ J ];

                                for ( unsigned int K = 0; K < F.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdXi_1dFdF[ i ][ F.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdXi_1[ a ][ I ] * dxi_tdF[ b ][ J ] * dxi_tdF[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi.size( ); K++ ){

                                    d3OverlapdXi_1dFdChi[ i ][ F.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * d2xi_tdXi_1dChi[ a ][ chi.size( ) * I + K ] * dxi_tdF[ b ][ J ];

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdXi_1dFdChi[ i ][ F.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdXi_1[ a ][ I ] * dxi_tdF[ b ][ J ] * dxi_tdchi[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                    d3OverlapdXi_1dFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdF[ b ][ J ];

                                }

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            d3OverlapdXi_1dR_nldChi[ i ][ chi.size( ) * I + J ] += d2Overlapdxi_tdR_nl[ i ][ a ] * d2xi_tdXi_1dChi[ a ][ chi.size( ) * I + J ];

                            for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                d3OverlapdXi_1dChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d2Overlapdchi_nldxi_t[ i ][ xi_t.size( ) * K + a ] * d2xi_tdXi_1dChi[ a ][ chi.size( ) * I + J ];

                            }

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdXi_1dChi[ i ][ chi.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                d3OverlapdXi_1dR_nldChi[ i ][ chi.size( ) * I + J ] += d3Overlapdxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                for ( unsigned int K = 0; K < chi.size( ); K++ ){

                                    d3OverlapdXi_1dChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * d2xi_tdXi_1dChi[ a ][ chi.size( ) * I + K ] * dxi_tdchi[ b ][ J ]
                                                                                                                        + d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * d2xi_tdXi_1dChi[ a ][ chi.size( ) * I + J ] * dxi_tdchi[ b ][ K ];

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdXi_1dChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdXi_1[ a ][ I ] * dxi_tdchi[ b ][ J ] * dxi_tdchi[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                    d3OverlapdXi_1dChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * a + b ] * dxi_tdXi_1[ a ][ I ] * dxi_tdchi[ b ][ J ];
                                }

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi_nl.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            d2OverlapdXi_1dChi_nl[ i ][ chi_nl.size( ) * I + J ] += d2Overlapdchi_nldxi_t[ i ][ xi_t.size( ) * J + a ] * dxi_tdXi_1[ a ][ I ];

                            d3OverlapdXi_1dR_nldChi_nl[ i ][ chi_nl.size( ) * I + J ] += d3Overlapdchi_nldxi_tdR_nl[ i ][ xi_t.size( ) * J + a ] * dxi_tdXi_1[ a ][ I ];

                            for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){
    
                                d3OverlapdXi_1dChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldchi_nldxi_t[ i ][ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * K + a ] * dxi_tdXi_1[ a ][ I ];
    
                            }

                        }

                    }

                }

                for ( unsigned int I = 0; I < dX.size( ); I++ ){

                    for ( unsigned int J = 0; J < dX.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapddXddX[ i ][ dX.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tddX[ b ][ J ];

                                for ( unsigned int K = 0; K < dX.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapddXddXddX[ i ][ dX.size( ) * dX.size( ) * I + dX.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tddX[ a ][ I ] * dxi_tddX[ b ][ J ] * dxi_tddX[ c ][ K ];

                                    }

                                }

                                d3OverlapddXddXdR_nl[ i ][ dX.size( ) * I + J ] += d3Overlapdxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tddX[ b ][ J ];

                                for ( unsigned int K = 0; K < F.size( ); K++ ){

                                    d3OverlapddXddXdF[ i ][ dX.size( ) * F.size( ) * I + F.size( ) * J + K ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * d2xi_tddXdF[ a ][ F.size( ) * I + K ] * dxi_tddX[ b ][ J ]
                                                                                                              + d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * d2xi_tddXdF[ b ][ F.size( ) * J + K ];

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapddXddXdF[ i ][ dX.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tddX[ a ][ I ] * dxi_tddX[ b ][ J ] * dxi_tdF[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapddXddXdChi[ i ][ dX.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tddX[ a ][ I ] * dxi_tddX[ b ][ J ] * dxi_tdchi[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                    d3OverlapddXddXdChi_nl[ i ][ dX.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tddX[ b ][ J ];

                                }

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < F.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){
    
                                d3OverlapddXdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d2Overlapdchi_nldxi_t[ i ][ xi_t.size( ) * K + a ] * d2xi_tddXdF[ a ][ F.size( ) * I + J ];
    
                            }


                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapddXdF[ i ][ F.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tdF[ b ][ J ];

                                d3OverlapddXdR_nldF[ i ][ F.size( ) * I + J ] += d3Overlapdxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tdF[ b ][ J ];

                                for ( unsigned int K = 0; K < F.size( ); K++ ){

                                    d3OverlapddXdFdF[ i ][ F.size( ) * F.size( ) * I + F.size( ) * J + K ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * d2xi_tddXdF[ a ][ F.size( ) * I + K ] * dxi_tdF[ b ][ J ]
                                                                                                            + d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * d2xi_tddXdF[ a ][ F.size( ) * I + J ] * dxi_tdF[ b ][ K ];

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapddXdFdF[ i ][ F.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tddX[ a ][ I ] * dxi_tdF[ b ][ J ] * dxi_tdF[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi.size( ); K++ ){

                                    d3OverlapddXdFdChi[ i ][ F.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * d2xi_tddXdF[ a ][ F.size( ) * I + J ] * dxi_tdchi[ b ][ K ];

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapddXdFdChi[ i ][ F.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tddX[ a ][ I ] * dxi_tdF[ b ][ J ] * dxi_tdchi[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                    d3OverlapddXdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tdF[ b ][ J ];

                                }

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapddXdChi[ i ][ chi.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                d3OverlapddXdR_nldChi[ i ][ chi.size( ) * I + J ] += d3Overlapdxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                for ( unsigned int K = 0; K < chi.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapddXdChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tddX[ a ][ I ] * dxi_tdchi[ b ][ J ] * dxi_tdchi[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                    d3OverlapddXdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * a + b ] * dxi_tddX[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                }

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi_nl.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            d2OverlapddXdChi_nl[ i ][ chi_nl.size( ) * I + J ] += d2Overlapdchi_nldxi_t[ i ][ xi_t.size( ) * J + a ] * dxi_tddX[ a ][ I ];

                            d3OverlapddXdR_nldChi_nl[ i ][ chi_nl.size( ) * I + J ] += d3Overlapdchi_nldxi_tdR_nl[ i ][ xi_t.size( ) * J + a ] * dxi_tddX[ a ][ I ];

                            for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                d3OverlapddXdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldchi_nldxi_t[ i ][ xi_t.size( ) * chi_nl.size( ) * J + xi_t.size( ) * K + a ] * dxi_tddX[ a ][ I ];

                            }

                        }

                    }

                }

                for ( unsigned int I = 0; I < F.size( ); I++ ){

                    for ( unsigned int J = 0; J < F.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdFdF[ i ][ F.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdF[ a ][ I ] * dxi_tdF[ b ][ J ];

                                d3OverlapdR_nldFdF[ i ][ F.size( ) * I + J ] += d3Overlapdxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + b ] * dxi_tdF[ a ][ I ] * dxi_tdF[ b ][ J ];

                                for ( unsigned int K = 0; K < F.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdFdFdF[ i ][ F.size( ) * F.size( ) * I + F.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdF[ a ][ I ] * dxi_tdF[ b ][ J ] * dxi_tdF[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdFdFdChi[ i ][ F.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdF[ a ][ I ] * dxi_tdF[ b ][ J ] * dxi_tdchi[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                    d3OverlapdFdFdChi_nl[ i ][ F.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * a + b ] *  dxi_tdF[ a ][ I ] * dxi_tdF[ b ][ J ];

                                }

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdFdChi[ i ][ chi.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdF[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                d3OverlapdR_nldFdChi[ i ][ chi.size( ) * I + J ] += d3Overlapdxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + b ] * dxi_tdF[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                for ( unsigned int K = 0; K < chi.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdFdChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( )* a + xi_t.size( ) * b + c ] * dxi_tdF[ a ][ I ] * dxi_tdchi[ b ][ J ] * dxi_tdchi[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                    d3OverlapdFdChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * a + b ] * dxi_tdF[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                }

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi_nl.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            d2OverlapdFdChi_nl[ i ][ chi_nl.size( ) * I + J ] += d2Overlapdchi_nldxi_t[ i ][ xi_t.size( ) * J + a ] * dxi_tdF[ a ][ I ];

                            d3OverlapdR_nldFdChi_nl[ i ][ chi_nl.size( ) * I + J ] += d3Overlapdchi_nldxi_tdR_nl[ i ][ xi_t.size( ) * J + a ] * dxi_tdF[ a ][ I ];

                            for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                d3OverlapdFdChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldchi_nldxi_t[ i ][ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * K + a ] * dxi_tdF[ a ][ I ];

                            }

                        }

                    }

                }

                for ( unsigned int I = 0; I < chi.size( ); I++ ){

                    for ( unsigned int J = 0; J < chi.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                                d2OverlapdChidChi[ i ][ chi.size( ) * I + J ] += d2Overlapdxi_tdxi_t[ i ][ xi_t.size( ) * a + b ] * dxi_tdchi[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                d3OverlapdR_nldChidChi[ i ][ chi.size( ) * I + J ] += d3Overlapdxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + b ] * dxi_tdchi[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                for ( unsigned int K = 0; K < chi.size( ); K++ ){

                                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                        d3OverlapdChidChidChi[ i ][ chi.size( ) * chi.size( ) * I + chi.size( ) * J + K ] += d3Overlapdxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ] * dxi_tdchi[ a ][ I ] * dxi_tdchi[ b ][ J ] * dxi_tdchi[ c ][ K ];

                                    }

                                }

                                for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                    d3OverlapdChidChidChi_nl[ i ][ chi.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * a + b ] * dxi_tdchi[ a ][ I ] * dxi_tdchi[ b ][ J ];

                                }

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < chi_nl.size( ); J++ ){

                        for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                            d2OverlapdChidChi_nl[ i ][ chi_nl.size( ) * I + J ] += d2Overlapdchi_nldxi_t[ i ][ xi_t.size( ) * J + a ] * dxi_tdchi[ a ][ I ];

                            d3OverlapdR_nldChidChi_nl[ i ][ chi_nl.size( ) * I + J ] += d3Overlapdchi_nldxi_tdR_nl[ i ][ xi_t.size( ) * J + a ] * dxi_tdchi[ a ][ I ];

                            for ( unsigned int K = 0; K < chi_nl.size( ); K++ ){

                                d3OverlapdChidChi_nldChi_nl[ i ][ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + K ] += d3Overlapdchi_nldchi_nldxi_t[ i ][ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * K + a ] * dxi_tdchi[ a ][ I ];

                            }

                        }

                    }

                }

            }

        }

        return;

    }

    void computeOverlapDistanceLagrangian( const floatVector &X, const floatVector &chi_nl, const floatVector &xi_t, const floatType &R_nl, floatType &L ){
        /*!
         * Compute the Lagrangian for the computation of the amount that the point \f$\xi_t\f$ in the local particle
         * is overlapping it's non-local neighbor.
         * 
         * \f$ \mathcal{L} = \frac{1}{2} \left( \chi^{nl}_{iI} \Xi_I - \xi_i \right)\left( \chi^{nl}_{iJ} \Xi_J - \xi_i\right) - \lambda \left( \Xi_I \Xi_I - 1\right)\f$
         * 
         * \param &X: The vector of unknowns. The first values are the current estimate of \f$\Xi\f$ and the final value is the Lagrange multiplier \f$\lambda\f$.
         * \param &chi_nl: The non-local micro-deformation tensor
         * \param &xi_t: The target point in the local particle
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &L: The value of the Lagrangian
         */

        unsigned int Xsize = X.size( );

        if ( Xsize < ( xi_t.size( ) + 1 ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "X has a size of " + std::to_string( Xsize ) + " and should have a size of " + std::to_string( xi_t.size( ) + 1 ) ) );

        }

        if ( chi_nl.size( ) != ( xi_t.size( ) * xi_t.size( ) ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "chi_nl has a size of " + std::to_string( chi_nl.size( ) ) + " and should have a size of " + std::to_string( xi_t.size( ) * xi_t.size( ) ) ) );

        }

        floatVector Xi( X.begin( ), X.begin( ) + xi_t.size( ) );
        floatType lambda = X.back( );

        floatVector d = -xi_t;
        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                d[ i ] += chi_nl[ xi_t.size( ) * i + I ] * Xi[ I ];

            }

        }
        
        L = 0.5 * vectorTools::dot( d, d ) - lambda * ( vectorTools::dot( Xi, Xi ) - R_nl * R_nl );

        return;

    }

    void computeOverlapDistanceLagrangian( const floatVector &X, const floatVector &chi_nl, const floatVector &xi_t, const floatType &R_nl, floatType &L,
                                               floatVector &dLdX, floatVector &dLdchi_nl, floatVector &dLdxi_t, floatType &dLdR_nl ){
        /*!
         * Compute the Lagrangian for the computation of the amount that the point \f$\xi_t\f$ in the local particle
         * is overlapping it's non-local neighbor.
         * 
         * \f$ \mathcal{L} = \frac{1}{2} \left( \chi^{nl}_{iI} \Xi_I - \xi_i \right)\left( \chi^{nl}_{iJ} \Xi_J - \xi_i\right) - \lambda \left( \Xi_I \Xi_I - 1\right)\f$
         * 
         * \param &X: The vector of unknowns. The first values are the current estimate of \f$\Xi\f$ and the final value is the Lagrange multiplier \f$\lambda\f$.
         * \param &chi_nl: The non-local micro-deformation tensor
         * \param &xi_t: The target point in the local particle
         * \param &R_nl: The radius of the non-local particle in the reference configuration
         * \param &L: The value of the Lagrangian
         * \param &dLdX: The gradient of the Lagrangian w.r.t. the unknown vector
         * \param &dLdchi_nl: The gradient of the Lagrangian w.r.t. the non-local micro deformation tensor
         * \param &dLdxi_t: The gradient of the Lagrangian w.r.t. the target point
         * \param &dLdR_nl: The gradient of the Lagrangian w.r.t. the reference configuration non-local particle radius
         */

        unsigned int Xsize = X.size( );

        if ( Xsize < ( xi_t.size( ) + 1 ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "X has a size of " + std::to_string( Xsize ) + " and should have a size of " + std::to_string( xi_t.size( ) + 1 ) ) );

        }

        if ( chi_nl.size( ) != ( xi_t.size( ) * xi_t.size( ) ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "chi_nl has a size of " + std::to_string( chi_nl.size( ) ) + " and should have a size of " + std::to_string( xi_t.size( ) * xi_t.size( ) ) ) );

        }

        floatVector Xi( X.begin( ), X.begin( ) + xi_t.size( ) );
        floatType lambda = X.back( );

        floatVector d = -xi_t;
        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                d[ i ] += chi_nl[ xi_t.size( ) * i + I ] * Xi[ I ];

            }

        }
        
        L = 0.5 * vectorTools::dot( d, d ) - lambda * ( vectorTools::dot( Xi, Xi ) - R_nl * R_nl );

        dLdX = floatVector( X.size( ), 0 );

        dLdchi_nl = floatVector( chi_nl.size( ), 0 );

        dLdxi_t = -d;

        dLdR_nl = 2 * lambda * R_nl;

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                dLdX[ I ] += chi_nl[ xi_t.size( ) * i + I ] * d[ i ];

                dLdchi_nl[ xi_t.size( ) * i + I ] += Xi[ I ] * d[ i ];

            }

            dLdX[ i ] -= 2 * lambda * Xi[ i ];

        }

        dLdX[ Xi.size( ) ] -= ( vectorTools::dot( Xi, Xi ) - R_nl * R_nl );

        return;

    }

    void computeOverlapDistanceLagrangian( const floatVector &X, const floatVector &chi_nl, const floatVector &xi_t, const floatType &R_nl, floatType &L,
                                               floatVector &dLdX, floatVector &dLdchi_nl, floatVector &dLdxi_t, floatType &dLdR_nl,
                                               floatVector &d2LdXdX, floatVector &d2LdXdchi_nl, floatVector &d2LdXdxi_t, floatVector &d2LdXdR_nl,
                                               floatVector &d2Ldchi_nldchi_nl, floatVector &d2Ldchi_nldxi_t, floatVector &d2Ldchi_nldR_nl,
                                               floatVector &d2Ldxi_tdxi_t, floatVector &d2Ldxi_tdR_nl,
                                               floatType &d2LdR_nldR_nl ){
        /*!
         * Compute the Lagrangian for the computation of the amount that the point \f$\xi_t\f$ in the local particle
         * is overlapping it's non-local neighbor.
         * 
         * \f$ \mathcal{L} = \frac{1}{2} \left( \chi^{nl}_{iI} \Xi_I - \xi_i \right)\left( \chi^{nl}_{iJ} \Xi_J - \xi_i\right) - \lambda \left( \Xi_I \Xi_I - 1\right)\f$
         * 
         * \param &X: The vector of unknowns. The first values are the current estimate of \f$\Xi\f$ and the final value is the Lagrange multiplier \f$\lambda\f$.
         * \param &chi_nl: The non-local micro-deformation tensor
         * \param &xi_t: The target point in the local particle
         * \param &R_nl: The radius of the non-local particle in the reference configuration
         * \param &L: The value of the Lagrangian
         * \param &dLdX: The gradient of the Lagrangian w.r.t. the unknown vector
         * \param &dLdchi_nl: The gradient of the Lagrangian w.r.t. the non-local micro deformation tensor
         * \param &dLdxi_t: The gradient of the Lagrangian w.r.t. the target point
         * \param &dLdR_nl: The gradient of the Lagrangian w.r.t. the reference configuration non-local particle radius
         * \param &d2LdXdX: The second gradient of the Lagrangian w.r.t. the solution vector
         * \param &d2LdXdchi_nl: The second gradient of the Lagrangian w.r.t. the solution vector and the non-local micro deformation tensor
         * \param &d2LdXdxi_t: The second gradient of the Lagrangian w.r.t. the solution vector and the target point
         * \param &d2LdXdR_nl: The second gradient of the Lagrangina w.r.t. the solution vector and the non-local radius in the reference configuration
         * \param &d2Ldchi_nldchi_nl: The second gradient of the Lagrangian w.r.t. the non-local micro deformation tensor
         * \param &d2Ldchi_nldxi_t: The second gradient of the Lagrangian w.r.t. the non-local micro deformation tensor and the target point
         * \param &d2Ldchi_nldR_nl: The second gradient of the Lagrangina w.r.t. the non-local micro deformation tensor and the non-local radius in the reference configuration
         * \param &d2Ldxi_tdxi_t: The second gradient of the Lagrangian w.r.t. the target point
         * \param &d2Ldxi_tdR_nl: The second gradient of the Lagrangina w.r.t. the target point and the non-local radius in the reference configuration
         * \param &d2LdR_nldR_nl: The second gradient of the Lagrangina w.r.t. the non-local radius in the reference configuration
         */

        unsigned int Xsize = X.size( );

        if ( Xsize < ( xi_t.size( ) + 1 ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "X has a size of " + std::to_string( Xsize ) + " and should have a size of " + std::to_string( xi_t.size( ) + 1 ) ) );

        }

        if ( chi_nl.size( ) != ( xi_t.size( ) * xi_t.size( ) ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "chi_nl has a size of " + std::to_string( chi_nl.size( ) ) + " and should have a size of " + std::to_string( xi_t.size( ) * xi_t.size( ) ) ) );

        }

        floatVector Xi( X.begin( ), X.begin( ) + xi_t.size( ) );
        floatType lambda = X.back( );

        floatVector d = -xi_t;
        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                d[ i ] += chi_nl[ xi_t.size( ) * i + I ] * Xi[ I ];

            }

        }
        
        L = 0.5 * vectorTools::dot( d, d ) - lambda * ( vectorTools::dot( Xi, Xi ) - R_nl * R_nl );

        dLdX = floatVector( X.size( ), 0 );

        dLdchi_nl = floatVector( chi_nl.size( ), 0 );

        dLdxi_t = -d;

        dLdR_nl = 2 * lambda * R_nl;

        d2LdXdX = floatVector( X.size( ) * X.size( ), 0 );

        d2LdXdchi_nl = floatVector( X.size( ) * chi_nl.size( ), 0 );

        d2LdXdxi_t = floatVector( X.size( ) * xi_t.size( ), 0 );

        d2Ldchi_nldchi_nl = floatVector( chi_nl.size( ) * chi_nl.size( ), 0 );

        d2Ldchi_nldxi_t = floatVector( chi_nl.size( ) * xi_t.size( ), 0 );

        d2Ldxi_tdxi_t = floatVector( xi_t.size( ) * xi_t.size( ), 0 );

        d2LdXdR_nl = floatVector( X.size( ), 0 );

        d2Ldchi_nldR_nl = floatVector( chi_nl.size( ), 0 );

        d2Ldxi_tdR_nl = floatVector( xi_t.size( ), 0 );

        d2LdR_nldR_nl = 2 * lambda;

        floatVector eye( chi_nl.size( ), 0 );

        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            d2Ldxi_tdxi_t[ xi_t.size( ) * i + i ] = 1;

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                dLdX[ I ] += chi_nl[ xi_t.size( ) * i + I ] * d[ i ];

                dLdchi_nl[ xi_t.size( ) * i + I ] += Xi[ I ] * d[ i ];

                d2LdXdxi_t[ xi_t.size( ) * i + I ] -= chi_nl[ xi_t.size( ) * I + i ];

                for ( unsigned int J = 0; J < xi_t.size( ); J++ ){

                    d2LdXdX[ X.size( ) * I + J ] += chi_nl[ xi_t.size( ) * i + I ] * chi_nl[ xi_t.size( ) * i + J ];

                    d2LdXdchi_nl[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * i + J ] += d[ i ] * eye[ xi_t.size( ) * I + J ] + chi_nl[ xi_t.size( ) * i + I ] * Xi[ J ];

                    d2Ldchi_nldxi_t[ xi_t.size( ) * xi_t.size( ) * i + xi_t.size( ) * I + J ] -= Xi[ I ] * eye[ xi_t.size( ) * i + J ];

                    for ( unsigned int j = 0; j < xi_t.size( ); j++ ){

                        d2Ldchi_nldchi_nl[ xi_t.size( ) * xi_t.size( ) * xi_t.size( ) * i + xi_t.size( ) * xi_t.size( ) * I + j * xi_t.size( ) + J ] += Xi[ I ] * eye[ xi_t.size( ) * i + j ] * Xi[ J ];

                    }

                }

            }

            dLdX[ i ] -= 2 * lambda * Xi[ i ];

            d2LdXdX[ X.size( ) * i + i ] -= 2 * lambda;

            d2LdXdX[ X.size( ) * i + X.size( ) - 1 ] -= 2 * Xi[ i ];
            d2LdXdX[ X.size( ) * ( X.size( ) - 1 ) + i ] -= 2 * Xi[ i ];

        }

        dLdX[ Xi.size( ) ] -= ( vectorTools::dot( Xi, Xi ) - R_nl * R_nl );

        d2LdXdR_nl[ X.size( ) - 1 ] = 2 * R_nl;

        return;

    }

    void computeOverlapDistanceLagrangian( const floatVector &X, const floatVector &chi_nl, const floatVector &xi_t, const floatType &R_nl, floatType &L,
                                               floatVector &dLdX, floatVector &dLdchi_nl, floatVector &dLdxi_t, floatType &dLdR_nl,
                                               floatVector &d2LdXdX, floatVector &d2LdXdchi_nl, floatVector &d2LdXdxi_t, floatVector &d2LdXdR_nl,
                                               floatVector &d2Ldchi_nldchi_nl, floatVector &d2Ldchi_nldxi_t, floatVector &d2Ldchi_nldR_nl,
                                               floatVector &d2Ldxi_tdxi_t, floatVector &d2Ldxi_tdR_nl,
                                               floatType &d2LdR_nldR_nl,
                                               floatVector &d3LdXdXdX, floatVector &d3LdXdXdchi_nl, floatVector &d3LdXdchi_nldchi_nl,
                                               floatVector &d3LdXdXdxi_t, floatVector &d3LdXdchi_nldxi_t,
                                               floatVector &d3LdXdXdR_nl, floatVector &d3LdXdchi_nldR_nl,
                                               floatVector &d3LdXdxi_tdxi_t, floatVector &d3LdXdxi_tdR_nl,
                                               floatVector &d3LdXdR_nldR_nl ){
        /*!
         * Compute the Lagrangian for the computation of the amount that the point \f$\xi_t\f$ in the local particle
         * is overlapping it's non-local neighbor.
         * 
         * \f$ \mathcal{L} = \frac{1}{2} \left( \chi^{nl}_{iI} \Xi_I - \xi_i \right)\left( \chi^{nl}_{iJ} \Xi_J - \xi_i\right) - \lambda \left( \Xi_I \Xi_I - 1\right)\f$
         * 
         * \param &X: The vector of unknowns. The first values are the current estimate of \f$\Xi\f$ and the final value is the Lagrange multiplier \f$\lambda\f$.
         * \param &chi_nl: The non-local micro-deformation tensor
         * \param &xi_t: The target point in the local particle
         * \param &R_nl: The radius of the non-local particle in the reference configuration
         * \param &L: The value of the Lagrangian
         * \param &dLdX: The gradient of the Lagrangian w.r.t. the unknown vector
         * \param &dLdchi_nl: The gradient of the Lagrangian w.r.t. the non-local micro deformation tensor
         * \param &dLdxi_t: The gradient of the Lagrangian w.r.t. the target point
         * \param &dLdR_nl: The gradient of the Lagrangian w.r.t. the reference configuration non-local particle radius
         * \param &d2LdXdX: The second gradient of the Lagrangian w.r.t. the solution vector
         * \param &d2LdXdchi_nl: The second gradient of the Lagrangian w.r.t. the solution vector and the non-local micro deformation tensor
         * \param &d2LdXdxi_t: The second gradient of the Lagrangian w.r.t. the solution vector and the target point
         * \param &d2LdXdR_nl: The second gradient of the Lagrangina w.r.t. the solution vector and the non-local radius in the reference configuration
         * \param &d2Ldchi_nldchi_nl: The second gradient of the Lagrangian w.r.t. the non-local micro deformation tensor
         * \param &d2Ldchi_nldxi_t: The second gradient of the Lagrangian w.r.t. the non-local micro deformation tensor and the target point
         * \param &d2Ldchi_nldR_nl: The second gradient of the Lagrangina w.r.t. the non-local micro deformation tensor and the non-local radius in the reference configuration
         * \param &d2Ldxi_tdxi_t: The second gradient of the Lagrangian w.r.t. the target point
         * \param &d2Ldxi_tdR_nl: The second gradient of the Lagrangina w.r.t. the target point and the non-local radius in the reference configuration
         * \param &d2LdR_nldR_nl: The second gradient of the Lagrangina w.r.t. the non-local radius in the reference configuration
         * \param &d3LdXdXdX: The third gradient of the Lagrangian w.r.t. the unknown vector
         * \param &d3LdXdXdchi_nl: The third gradient of the Lagrangian w.r.t. the unknown vector twice and the micro-deformation tensor once
         * \param &d3LdXdchi_nldchi_nl: The third gradient of the Lagrangian w.r.t. the unknown vector once and the micro-deformation tensor twice
         * \param &d3LdXdXdxi_t: The third gradient of the Lagrangian w.r.t. the unknown vector twice and \f$\xi^t\f$ once
         * \param &d3LdXdchi_nldxi_t: The third gradient of the Lagrangian w.r.t. the unknown vector, the micro-deformation tensor, and \f$\xi^t\f$.
         * \param &d3LdXdXdR_nl: The third gradient of the Lagrangian w.r.t. the unknown vector twice and the non-local radius once
         * \param &d3LdXdchi_nldR_nl: The third gradient of the Lagrangian w.r.t. the unknown vector, the micro-deformation tensor, and the non-local radius
         * \param &d3LdXdxi_tdxi_t: The third gradient of the Lagrangian w.r.t. the unknown vector once and \f$\xi_t\f$ twice
         * \param &d3LdXdxi_tdxi_t: The third gradient of the Lagrangian w.r.t. the unknown vector,  f$\xi_t\f$, and the reference configuration non-local radius
         * \param &d3LdXdR_nldR_nl: The third gradient of the Lagrangian w.r.t. the unknown vector once and the non-local reference radius once
         */

        unsigned int Xsize = X.size( );

        if ( Xsize < ( xi_t.size( ) + 1 ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "X has a size of " + std::to_string( Xsize ) + " and should have a size of " + std::to_string( xi_t.size( ) + 1 ) ) );

        }

        if ( chi_nl.size( ) != ( xi_t.size( ) * xi_t.size( ) ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "chi_nl has a size of " + std::to_string( chi_nl.size( ) ) + " and should have a size of " + std::to_string( xi_t.size( ) * xi_t.size( ) ) ) );

        }

        floatVector Xi( X.begin( ), X.begin( ) + xi_t.size( ) );
        floatType lambda = X.back( );

        floatVector d = -xi_t;
        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                d[ i ] += chi_nl[ xi_t.size( ) * i + I ] * Xi[ I ];

            }

        }
        
        L = 0.5 * vectorTools::dot( d, d ) - lambda * ( vectorTools::dot( Xi, Xi ) - R_nl * R_nl );

        dLdX = floatVector( X.size( ), 0 );

        dLdchi_nl = floatVector( chi_nl.size( ), 0 );

        dLdxi_t = -d;

        dLdR_nl = 2 * lambda * R_nl;

        d2LdXdX = floatVector( X.size( ) * X.size( ), 0 );

        d2LdXdchi_nl = floatVector( X.size( ) * chi_nl.size( ), 0 );

        d2LdXdxi_t = floatVector( X.size( ) * xi_t.size( ), 0 );

        d2Ldchi_nldchi_nl = floatVector( chi_nl.size( ) * chi_nl.size( ), 0 );

        d2Ldchi_nldxi_t = floatVector( chi_nl.size( ) * xi_t.size( ), 0 );

        d2Ldxi_tdxi_t = floatVector( xi_t.size( ) * xi_t.size( ), 0 );

        d2LdXdR_nl = floatVector( X.size( ), 0 );

        d2Ldchi_nldR_nl = floatVector( chi_nl.size( ), 0 );

        d2Ldxi_tdR_nl = floatVector( xi_t.size( ), 0 );

        d2LdR_nldR_nl = 2 * lambda;

        d3LdXdXdX = floatVector( X.size( ) * X.size( ) * X.size( ), 0 );

        d3LdXdXdchi_nl = floatVector( X.size( ) * X.size( ) * chi_nl.size( ), 0 );

        d3LdXdchi_nldchi_nl = floatVector( X.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        d3LdXdXdxi_t = floatVector( X.size( ) * X.size( ) * xi_t.size( ), 0 );

        d3LdXdchi_nldxi_t = floatVector( X.size( ) * chi_nl.size( ) * xi_t.size( ), 0 );

        d3LdXdXdR_nl = floatVector( X.size( ) * X.size( ), 0 );

        d3LdXdchi_nldR_nl = floatVector( X.size( ) * chi_nl.size( ), 0 );

        d3LdXdxi_tdxi_t = floatVector( X.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        d3LdXdxi_tdR_nl = floatVector( X.size( ) * xi_t.size( ), 0 );

        d3LdXdR_nldR_nl = floatVector( X.size( ), 0 );

        floatVector eye( chi_nl.size( ), 0 );

        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            d2Ldxi_tdxi_t[ xi_t.size( ) * i + i ] = 1;

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                dLdX[ I ] += chi_nl[ xi_t.size( ) * i + I ] * d[ i ];

                dLdchi_nl[ xi_t.size( ) * i + I ] += Xi[ I ] * d[ i ];

                d2LdXdxi_t[ xi_t.size( ) * i + I ] -= chi_nl[ xi_t.size( ) * I + i ];

                d3LdXdXdX[ X.size( ) * X.size( ) * i + X.size( ) * I + ( X.size( ) - 1 ) ] = -2 * eye[ xi_t.size( ) * i + I ];

                d3LdXdXdX[ X.size( ) * X.size( ) * i + X.size( ) * ( X.size( ) - 1 ) + I ] = -2 * eye[ xi_t.size( ) * i + I ];

                d3LdXdXdX[ X.size( ) * X.size( ) * ( X.size( ) - 1 ) + X.size( ) * i + I ] = -2 * eye[ xi_t.size( ) * i + I ];

                for ( unsigned int J = 0; J < xi_t.size( ); J++ ){

                    d2LdXdX[ X.size( ) * I + J ] += chi_nl[ xi_t.size( ) * i + I ] * chi_nl[ xi_t.size( ) * i + J ];

                    d2LdXdchi_nl[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * i + J ] += d[ i ] * eye[ xi_t.size( ) * I + J ] + chi_nl[ xi_t.size( ) * i + I ] * Xi[ J ];

                    d2Ldchi_nldxi_t[ xi_t.size( ) * xi_t.size( ) * i + xi_t.size( ) * I + J ] -= Xi[ I ] * eye[ xi_t.size( ) * i + J ];

                    for ( unsigned int j = 0; j < xi_t.size( ); j++ ){

                        d2Ldchi_nldchi_nl[ xi_t.size( ) * xi_t.size( ) * xi_t.size( ) * i + xi_t.size( ) * xi_t.size( ) * I + j * xi_t.size( ) + J ] += Xi[ I ] * eye[ xi_t.size( ) * i + j ] * Xi[ J ];
                    
                        d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + xi_t.size( ) * i + j ] += eye[ xi_t.size( ) * I + j ] * chi_nl[ xi_t.size( ) * i + J ]
                                                                                                                      + chi_nl[ xi_t.size( ) * i + I ] * eye[ xi_t.size( )* J + j ];

                        d3LdXdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * i + xi_t.size( ) * J + j ] += -eye[ xi_t.size( ) * i + j ] * eye[ xi_t.size( ) * I + J ];

                        for ( unsigned int K = 0; K < xi_t.size( ); K++ ){

                            d3LdXdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * I + xi_t.size( ) * chi_nl.size( ) * i + chi_nl.size( ) * J + xi_t.size( ) * j + K ]
                                += eye[ xi_t.size( ) * i + j ] * Xi[ K ] * eye[ xi_t.size( ) * I + J ] + eye[ xi_t.size( ) * i + j ] * eye[ xi_t.size( ) * I + K ] * Xi[ J ];

                        }

                    }

                }

            }

            dLdX[ i ] -= 2 * lambda * Xi[ i ];

            d2LdXdX[ X.size( ) * i + i ] -= 2 * lambda;

            d2LdXdX[ X.size( ) * i + X.size( ) - 1 ] -= 2 * Xi[ i ];
            d2LdXdX[ X.size( ) * ( X.size( ) - 1 ) + i ] -= 2 * Xi[ i ];

        }

        dLdX[ Xi.size( ) ] -= ( vectorTools::dot( Xi, Xi ) - R_nl * R_nl );

        d2LdXdR_nl[ X.size( ) - 1 ] = 2 * R_nl;

        d3LdXdR_nldR_nl[ X.size( ) - 1 ] = 2;

        return;

    }

    void computeOverlapDistanceLagrangian( const floatVector &X, const floatVector &chi_nl, const floatVector &xi_t, const floatType &R_nl, floatType &L,
                                               floatVector &dLdX, floatVector &dLdchi_nl, floatVector &dLdxi_t, floatType &dLdR_nl,
                                               floatVector &d2LdXdX, floatVector &d2LdXdchi_nl, floatVector &d2LdXdxi_t, floatVector &d2LdXdR_nl,
                                               floatVector &d2Ldchi_nldchi_nl, floatVector &d2Ldchi_nldxi_t, floatVector &d2Ldchi_nldR_nl,
                                               floatVector &d2Ldxi_tdxi_t, floatVector &d2Ldxi_tdR_nl,
                                               floatType &d2LdR_nldR_nl,
                                               floatVector &d3LdXdXdX, floatVector &d3LdXdXdchi_nl, floatVector &d3LdXdchi_nldchi_nl,
                                               floatVector &d3LdXdXdxi_t, floatVector &d3LdXdchi_nldxi_t,
                                               floatVector &d3LdXdXdR_nl, floatVector &d3LdXdchi_nldR_nl,
                                               floatVector &d3LdXdxi_tdxi_t, floatVector &d3LdXdxi_tdR_nl,
                                               floatVector &d3LdXdR_nldR_nl,
                                               floatVector &d4LdXdXdchi_nldchi_nl ){
        /*!
         * Compute the Lagrangian for the computation of the amount that the point \f$\xi_t\f$ in the local particle
         * is overlapping it's non-local neighbor.
         * 
         * \f$ \mathcal{L} = \frac{1}{2} \left( \chi^{nl}_{iI} \Xi_I - \xi_i \right)\left( \chi^{nl}_{iJ} \Xi_J - \xi_i\right) - \lambda \left( \Xi_I \Xi_I - 1\right)\f$
         * 
         * \param &X: The vector of unknowns. The first values are the current estimate of \f$\Xi\f$ and the final value is the Lagrange multiplier \f$\lambda\f$.
         * \param &chi_nl: The non-local micro-deformation tensor
         * \param &xi_t: The target point in the local particle
         * \param &R_nl: The radius of the non-local particle in the reference configuration
         * \param &L: The value of the Lagrangian
         * \param &dLdX: The gradient of the Lagrangian w.r.t. the unknown vector
         * \param &dLdchi_nl: The gradient of the Lagrangian w.r.t. the non-local micro deformation tensor
         * \param &dLdxi_t: The gradient of the Lagrangian w.r.t. the target point
         * \param &dLdR_nl: The gradient of the Lagrangian w.r.t. the reference configuration non-local particle radius
         * \param &d2LdXdX: The second gradient of the Lagrangian w.r.t. the solution vector
         * \param &d2LdXdchi_nl: The second gradient of the Lagrangian w.r.t. the solution vector and the non-local micro deformation tensor
         * \param &d2LdXdxi_t: The second gradient of the Lagrangian w.r.t. the solution vector and the target point
         * \param &d2LdXdR_nl: The second gradient of the Lagrangina w.r.t. the solution vector and the non-local radius in the reference configuration
         * \param &d2Ldchi_nldchi_nl: The second gradient of the Lagrangian w.r.t. the non-local micro deformation tensor
         * \param &d2Ldchi_nldxi_t: The second gradient of the Lagrangian w.r.t. the non-local micro deformation tensor and the target point
         * \param &d2Ldchi_nldR_nl: The second gradient of the Lagrangina w.r.t. the non-local micro deformation tensor and the non-local radius in the reference configuration
         * \param &d2Ldxi_tdxi_t: The second gradient of the Lagrangian w.r.t. the target point
         * \param &d2Ldxi_tdR_nl: The second gradient of the Lagrangina w.r.t. the target point and the non-local radius in the reference configuration
         * \param &d2LdR_nldR_nl: The second gradient of the Lagrangina w.r.t. the non-local radius in the reference configuration
         * \param &d3LdXdXdX: The third gradient of the Lagrangian w.r.t. the unknown vector
         * \param &d3LdXdXdchi_nl: The third gradient of the Lagrangian w.r.t. the unknown vector twice and the micro-deformation tensor once
         * \param &d3LdXdchi_nldchi_nl: The third gradient of the Lagrangian w.r.t. the unknown vector once and the micro-deformation tensor twice
         * \param &d3LdXdXdxi_t: The third gradient of the Lagrangian w.r.t. the unknown vector twice and \f$\xi^t\f$ once
         * \param &d3LdXdchi_nldxi_t: The third gradient of the Lagrangian w.r.t. the unknown vector, the micro-deformation tensor, and \f$\xi^t\f$.
         * \param &d3LdXdXdR_nl: The third gradient of the Lagrangian w.r.t. the unknown vector twice and the non-local radius once
         * \param &d3LdXdchi_nldR_nl: The third gradient of the Lagrangian w.r.t. the unknown vector, the micro-deformation tensor, and the non-local radius
         * \param &d3LdXdxi_tdxi_t: The third gradient of the Lagrangian w.r.t. the unknown vector once and \f$\xi_t\f$ twice
         * \param &d3LdXdxi_tdxi_t: The third gradient of the Lagrangian w.r.t. the unknown vector, \f$\xi_t\f$, and the reference configuration non-local radius
         * \param &d3LdXdR_nldR_nl: The third gradient of the Lagrangian w.r.t. the unknown vector once and the non-local reference radius once
         * \param &d4LdXdXdchi_nldchi_nl: The fourth gradient of the Lagrangian w.r.t. the unknown vector twice and the non-local micro deformation tensor twice
         */

        unsigned int Xsize = X.size( );

        if ( Xsize < ( xi_t.size( ) + 1 ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "X has a size of " + std::to_string( Xsize ) + " and should have a size of " + std::to_string( xi_t.size( ) + 1 ) ) );

        }

        if ( chi_nl.size( ) != ( xi_t.size( ) * xi_t.size( ) ) ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "chi_nl has a size of " + std::to_string( chi_nl.size( ) ) + " and should have a size of " + std::to_string( xi_t.size( ) * xi_t.size( ) ) ) );

        }

        floatVector Xi( X.begin( ), X.begin( ) + xi_t.size( ) );
        floatType lambda = X.back( );

        floatVector d = -xi_t;
        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                d[ i ] += chi_nl[ xi_t.size( ) * i + I ] * Xi[ I ];

            }

        }
        
        L = 0.5 * vectorTools::dot( d, d ) - lambda * ( vectorTools::dot( Xi, Xi ) - R_nl * R_nl );

        dLdX = floatVector( X.size( ), 0 );

        dLdchi_nl = floatVector( chi_nl.size( ), 0 );

        dLdxi_t = -d;

        dLdR_nl = 2 * lambda * R_nl;

        d2LdXdX = floatVector( X.size( ) * X.size( ), 0 );

        d2LdXdchi_nl = floatVector( X.size( ) * chi_nl.size( ), 0 );

        d2LdXdxi_t = floatVector( X.size( ) * xi_t.size( ), 0 );

        d2Ldchi_nldchi_nl = floatVector( chi_nl.size( ) * chi_nl.size( ), 0 );

        d2Ldchi_nldxi_t = floatVector( chi_nl.size( ) * xi_t.size( ), 0 );

        d2Ldxi_tdxi_t = floatVector( xi_t.size( ) * xi_t.size( ), 0 );

        d2LdXdR_nl = floatVector( X.size( ), 0 );

        d2Ldchi_nldR_nl = floatVector( chi_nl.size( ), 0 );

        d2Ldxi_tdR_nl = floatVector( xi_t.size( ), 0 );

        d2LdR_nldR_nl = 2 * lambda;

        d3LdXdXdX = floatVector( X.size( ) * X.size( ) * X.size( ), 0 );

        d3LdXdXdchi_nl = floatVector( X.size( ) * X.size( ) * chi_nl.size( ), 0 );

        d3LdXdchi_nldchi_nl = floatVector( X.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        d3LdXdXdxi_t = floatVector( X.size( ) * X.size( ) * xi_t.size( ), 0 );

        d3LdXdchi_nldxi_t = floatVector( X.size( ) * chi_nl.size( ) * xi_t.size( ), 0 );

        d3LdXdXdR_nl = floatVector( X.size( ) * X.size( ), 0 );

        d3LdXdchi_nldR_nl = floatVector( X.size( ) * chi_nl.size( ), 0 );

        d3LdXdxi_tdxi_t = floatVector( X.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        d3LdXdxi_tdR_nl = floatVector( X.size( ) * xi_t.size( ), 0 );

        d3LdXdR_nldR_nl = floatVector( X.size( ), 0 );

        d4LdXdXdchi_nldchi_nl = floatVector( X.size( ) * X.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        floatVector eye( chi_nl.size( ), 0 );

        vectorTools::eye( eye );

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            d2Ldxi_tdxi_t[ xi_t.size( ) * i + i ] = 1;

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                dLdX[ I ] += chi_nl[ xi_t.size( ) * i + I ] * d[ i ];

                dLdchi_nl[ xi_t.size( ) * i + I ] += Xi[ I ] * d[ i ];

                d2LdXdxi_t[ xi_t.size( ) * i + I ] -= chi_nl[ xi_t.size( ) * I + i ];

                d3LdXdXdX[ X.size( ) * X.size( ) * i + X.size( ) * I + ( X.size( ) - 1 ) ] = -2 * eye[ xi_t.size( ) * i + I ];

                d3LdXdXdX[ X.size( ) * X.size( ) * i + X.size( ) * ( X.size( ) - 1 ) + I ] = -2 * eye[ xi_t.size( ) * i + I ];

                d3LdXdXdX[ X.size( ) * X.size( ) * ( X.size( ) - 1 ) + X.size( ) * i + I ] = -2 * eye[ xi_t.size( ) * i + I ];

                for ( unsigned int J = 0; J < xi_t.size( ); J++ ){

                    d2LdXdX[ X.size( ) * I + J ] += chi_nl[ xi_t.size( ) * i + I ] * chi_nl[ xi_t.size( ) * i + J ];

                    d2LdXdchi_nl[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * i + J ] += d[ i ] * eye[ xi_t.size( ) * I + J ] + chi_nl[ xi_t.size( ) * i + I ] * Xi[ J ];

                    d2Ldchi_nldxi_t[ xi_t.size( ) * xi_t.size( ) * i + xi_t.size( ) * I + J ] -= Xi[ I ] * eye[ xi_t.size( ) * i + J ];

                    for ( unsigned int j = 0; j < xi_t.size( ); j++ ){

                        d2Ldchi_nldchi_nl[ xi_t.size( ) * xi_t.size( ) * xi_t.size( ) * i + xi_t.size( ) * xi_t.size( ) * I + j * xi_t.size( ) + J ] += Xi[ I ] * eye[ xi_t.size( ) * i + j ] * Xi[ J ];
                    
                        d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + xi_t.size( ) * i + j ] += eye[ xi_t.size( ) * I + j ] * chi_nl[ xi_t.size( ) * i + J ]
                                                                                                                      + chi_nl[ xi_t.size( ) * i + I ] * eye[ xi_t.size( )* J + j ];


                        d3LdXdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * i + xi_t.size( ) * J + j ] += -eye[ xi_t.size( ) * i + j ] * eye[ xi_t.size( ) * I + J ];

                        for ( unsigned int K = 0; K < xi_t.size( ); K++ ){

                            d3LdXdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * I + xi_t.size( ) * chi_nl.size( ) * i + chi_nl.size( ) * J + xi_t.size( ) * j + K ]
                                += eye[ xi_t.size( ) * i + j ] * Xi[ K ] * eye[ xi_t.size( ) * I + J ] + eye[ xi_t.size( ) * i + j ] * eye[ xi_t.size( ) * I + K ] * Xi[ J ];

                            for ( unsigned int L = 0; L < xi_t.size( ); L++ ){

                                d4LdXdXdchi_nldchi_nl[ X.size( ) * chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * chi_nl.size( ) * J + xi_t.size( ) * chi_nl.size( ) * i + chi_nl.size( ) * j + xi_t.size( ) * K + L ]
                                    += eye[ xi_t.size( ) * I + j ] * eye[ xi_t.size( ) * i + K ] * eye[ xi_t.size( ) * J + L ]
                                     + eye[ xi_t.size( ) * i + K ] * eye[ xi_t.size( ) * I + L ] * eye[ xi_t.size( )* J + j ];


                            }

                        }

                    }

                }

            }

            dLdX[ i ] -= 2 * lambda * Xi[ i ];

            d2LdXdX[ X.size( ) * i + i ] -= 2 * lambda;

            d2LdXdX[ X.size( ) * i + X.size( ) - 1 ] -= 2 * Xi[ i ];
            d2LdXdX[ X.size( ) * ( X.size( ) - 1 ) + i ] -= 2 * Xi[ i ];

        }

        dLdX[ Xi.size( ) ] -= ( vectorTools::dot( Xi, Xi ) - R_nl * R_nl );

        d2LdXdR_nl[ X.size( ) - 1 ] = 2 * R_nl;

        d3LdXdR_nldR_nl[ X.size( ) - 1 ] = 2;

        return;

    }

    void solveOverlapDistance( const floatVector &chi_nl, const floatVector &xi_t, const floatType &R_nl, floatVector &d,
                                   const floatType tolr, const floatType tola, const unsigned int max_iteration,
                                   const unsigned int max_ls, const floatType alpha_ls ){
        /*!
         * Solve for the overlap distance where \f$\xi_t\f$ is known to be inside of the non-local particle
         * 
         * \param &chi_nl: The non-local micro-deformation tensor
         * \param &xi_t: The position inside of the non-local particle
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &d: The distance vector going from \f$\xi_t\f$ to the solved point on the surface of the non-local particle.
         * \param tolr: The relative tolerance
         * \param tola: The absolute tolerance
         * \param max_iteration: The maximum number of iterations
         * \param max_ls: The maximum number of line-search iterations
         * \param alpha_ls: The alpha parameter for the line-search
         */

        floatVector inv_chi_nl = vectorTools::inverse( chi_nl, xi_t.size( ), xi_t.size( ) );

        floatVector Xi_t( xi_t.size( ), 0 );

        for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

            for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

                Xi_t[ I ] += inv_chi_nl[ xi_t.size( ) * I + i ] * xi_t[ i ];

            }

        }

        floatVector lagrange( 1, 1 );

        floatVector X = vectorTools::appendVectors( { Xi_t, lagrange } );

        floatType L;

        floatVector dLdX, dLdchi_nl, dLdxi_t;

        floatType dLdR_nl;

        floatVector d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl, d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl, d2Ldxi_tdxi_t, d2Ldxi_tdR_nl;

        floatType d2LdR_nldR_nl;

        ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X, chi_nl, xi_t, R_nl, L,
                                                             dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                             d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                             d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                             d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                             d2LdR_nldR_nl ) );

        floatType R = vectorTools::l2norm( dLdX );

        floatType Rp = R;

        floatType tol = tolr * R + tola;

        unsigned int num_iteration = 0;

        floatVector dX;

        while ( ( num_iteration < max_iteration ) && ( R > tol ) ){

            unsigned int rank;

            dX = -vectorTools::solveLinearSystem( d2LdXdX, dLdX, dLdX.size( ), dLdX.size( ), rank );

            floatType lambda = 1;

            ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X + lambda * dX, chi_nl, xi_t, R_nl, L,
                                                                 dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                                 d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                                 d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                                 d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                                 d2LdR_nldR_nl ) );

            unsigned int num_ls = 0;

            R = vectorTools::l2norm( dLdX );

            while ( ( num_ls < max_ls ) && ( R > ( 1 - alpha_ls ) * Rp ) ){ 

                lambda *= 0.5;

                ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X + lambda * dX, chi_nl, xi_t, R_nl, L,
                                                                     dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                                     d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                                     d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                                     d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                                     d2LdR_nldR_nl ) );

                R = vectorTools::l2norm( dLdX );

                num_ls++;

            }

            if ( R > ( 1 - alpha_ls ) * Rp ){

                ERROR_TOOLS_CATCH( throw std::runtime_error( "Failure in linesearch" ) );

            }

            X += lambda * dX;

            Rp = R;

            num_iteration++;

        }

        if ( R > tol ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The optimizer did not converge" ) );

        }

        d = -xi_t;

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                d[ i ] += chi_nl[ xi_t.size( ) * i + I ] * X[ I ];

            }

        }

        return;

    }

    void solveOverlapDistance( const floatVector &chi_nl, const floatVector &xi_t, const floatType &R_nl, floatVector &d,
                                   floatMatrix &dddchi_nl, floatMatrix &dddxi_t, floatVector &dddR_nl,
                                   const floatType tolr, const floatType tola, const unsigned int max_iteration,
                                   const unsigned int max_ls, const floatType alpha_ls ){
        /*!
         * Solve for the overlap distance where \f$\xi_t\f$ is known to be inside of the non-local particle
         * 
         * \param &chi_nl: The non-local micro-deformation tensor
         * \param &xi_t: The position inside of the non-local particle
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &d: The distance vector going from \f$\xi_t\f$ to the solved point on the surface of the non-local particle.
         * \param &dddchi_nl: The gradient of the distance vector w.r.t. the non-local micro-deformation tensor
         * \param &dddxi_t: The gradient of the distance vector w.r.t. the target micro-relative position vector
         * \param &dddR_nl: The gradient of the distance vector w.r.t. the non-local particle radius
         * \param tolr: The relative tolerance
         * \param tola: The absolute tolerance
         * \param max_iteration: The maximum number of iterations
         * \param max_ls: The maximum number of line-search iterations
         * \param alpha_ls: The alpha parameter for the line-search
         */

        floatVector inv_chi_nl = vectorTools::inverse( chi_nl, xi_t.size( ), xi_t.size( ) );

        floatVector Xi_t( xi_t.size( ), 0 );

        for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

            for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

                Xi_t[ I ] += inv_chi_nl[ xi_t.size( ) * I + i ] * xi_t[ i ];

            }

        }

        floatVector lagrange( 1, 1 );

        floatVector X = vectorTools::appendVectors( { Xi_t, lagrange } );

        floatType L;

        floatVector dLdX, dLdchi_nl, dLdxi_t;

        floatType dLdR_nl;

        floatVector d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl, d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl, d2Ldxi_tdxi_t, d2Ldxi_tdR_nl;

        floatType d2LdR_nldR_nl;

        ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X, chi_nl, xi_t, R_nl, L,
                                                             dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                             d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                             d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                             d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                             d2LdR_nldR_nl ) );

        floatType R = vectorTools::l2norm( dLdX );

        floatType Rp = R;

        floatType tol = tolr * R + tola;

        unsigned int num_iteration = 0;

        floatVector dX;

        while ( ( num_iteration < max_iteration ) && ( R > tol ) ){

            unsigned int rank;

            dX = -vectorTools::solveLinearSystem( d2LdXdX, dLdX, dLdX.size( ), dLdX.size( ), rank );

            floatType lambda = 1;

            ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X + lambda * dX, chi_nl, xi_t, R_nl, L,
                                                                 dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                                 d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                                 d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                                 d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                                 d2LdR_nldR_nl ) );

            unsigned int num_ls = 0;

            R = vectorTools::l2norm( dLdX );

            while ( ( num_ls < max_ls ) && ( R > ( 1 - alpha_ls ) * Rp ) ){ 

                lambda *= 0.5;

                ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X + lambda * dX, chi_nl, xi_t, R_nl, L,
                                                                     dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                                     d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                                     d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                                     d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                                     d2LdR_nldR_nl ) );

                R = vectorTools::l2norm( dLdX );

                num_ls++;

            }

            if ( R > ( 1 - alpha_ls ) * Rp ){

                ERROR_TOOLS_CATCH( throw std::runtime_error( "Failure in linesearch" ) );

            }

            X += lambda * dX;

            Rp = R;

            num_iteration++;

        }

        if ( R > tol ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The optimizer did not converge" ) );

        }

        d = -xi_t;

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                d[ i ] += chi_nl[ xi_t.size( ) * i + I ] * X[ I ];

            }

        }

        // Compute the first order derivatives

         // Wrap the Jacobians of the Residual
        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdX( d2LdXdX.data( ), X.size( ), X.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdchi_nl( d2LdXdchi_nl.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdxi_t( d2LdXdxi_t.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdR_nl( d2LdXdR_nl.data( ), X.size( ), 1 );

        // Wrap the flattened versions of the Jacobians
        floatVector _flat_dXdchi_nl( X.size( ) * chi_nl.size( ), 0 );

        floatVector _flat_dXdxi_t( X.size( ) * xi_t.size( ), 0 );

        floatVector _flat_dXdR_nl( X.size( ), 0 );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _dXdchi_nl( _flat_dXdchi_nl.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _dXdxi_t( _flat_dXdxi_t.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _dXdR_nl( _flat_dXdR_nl.data( ), X.size( ), 1 );

        // Compute the Jacobians of the unknown vector

        vectorTools::solverType< floatType > linearSolver( _d2LdXdX );

        _dXdchi_nl = -linearSolver.solve( _d2LdXdchi_nl );

        _dXdxi_t = -linearSolver.solve( _d2LdXdxi_t );

        _dXdR_nl = -linearSolver.solve( _d2LdXdR_nl );

        // Construct the jacobians of the distance vector

        floatVector eye( chi_nl.size( ), 0 );

        vectorTools::eye( eye );

        dddchi_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ), 0 ) );

        dddxi_t   = floatMatrix( d.size( ), floatVector( xi_t.size( ), 0 ) );

        dddR_nl   = floatVector( d.size( ), 0 );

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            dddxi_t[ i ][ i ] = -1;

            for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                dddR_nl[ i ] += chi_nl[ xi_t.size( ) * i + a ] * _flat_dXdR_nl[ a ];

                for ( unsigned int A = 0; A < xi_t.size( ); A++ ){

                    dddchi_nl[ i ][ xi_t.size( ) * a + A ] += eye[ xi_t.size( ) * i + a ] * X[ A ];

                    dddxi_t[ i ][ a ] += chi_nl[ xi_t.size( ) * i + A ] * _flat_dXdxi_t[ xi_t.size( ) * A + a ];

                    for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                        dddchi_nl[ i ][ xi_t.size( ) * a + A ] += chi_nl[ xi_t.size( ) * i + I ] * _flat_dXdchi_nl[ chi_nl.size( ) * I + xi_t.size( ) * a + A ];

                    }

                }

            }

        }

        return;

    }

    void solveOverlapDistance( const floatVector &chi_nl, const floatVector &xi_t, const floatType &R_nl, floatVector &d,
                                   floatMatrix &dddchi_nl, floatMatrix &dddxi_t, floatVector &dddR_nl,
                                   floatMatrix &d2ddchi_nldchi_nl, floatMatrix &d2ddchi_nldxi_t, floatMatrix &d2ddchi_nldR_nl,
                                   floatMatrix &d2ddxi_tdxi_t, floatMatrix &d2ddxi_tdR_nl,
                                   floatVector &d2ddR_nldR_nl,
                                   const floatType tolr, const floatType tola, const unsigned int max_iteration,
                                   const unsigned int max_ls, const floatType alpha_ls ){
        /*!
         * Solve for the overlap distance where \f$\xi_t\f$ is known to be inside of the non-local particle
         * 
         * \param &chi_nl: The non-local micro-deformation tensor
         * \param &xi_t: The position inside of the non-local particle
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &d: The distance vector going from \f$\xi_t\f$ to the solved point on the surface of the non-local particle.
         * \param &dddchi_nl: The gradient of the distance vector w.r.t. the non-local micro-deformation tensor
         * \param &dddxi_t: The gradient of the distance vector w.r.t. the target micro-relative position vector
         * \param &dddR_nl: The gradient of the distance vector w.r.t. the non-local particle radius
         * \param &d2ddchi_nldchi_nl: The second gradient of the distance vector w.r.t. the non-local micro-deformation tensor
         * \param &d2ddchi_nldxi_t: The second gradient of the distance vector w.r.t. the non-local micro-deformation tensor and \f$\xi_t\f$
         * \param &d2ddchi_nldR_nl: The second gradient of the distance vector w.r.t. the non-local micro-deformation tensor the non-local reference radius
         * \param &d2ddxi_tdxi_t: The second gradient of the distance vector w.r.t. \f$\xi_t\f$
         * \param &d2ddxi_tdR_nl: The second gradient of the distance vector w.r.t. \f$\xi_t\f$ and the non-local reference radius
         * \param &d2ddR_nldR_nl: The second gradient of the distance vector w.r.t. the non-local reference radius
         * \param tolr: The relative tolerance
         * \param tola: The absolute tolerance
         * \param max_iteration: The maximum number of iterations
         * \param max_ls: The maximum number of line-search iterations
         * \param alpha_ls: The alpha parameter for the line-search
         */

        floatVector inv_chi_nl = vectorTools::inverse( chi_nl, xi_t.size( ), xi_t.size( ) );

        floatVector Xi_t( xi_t.size( ), 0 );

        for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

            for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

                Xi_t[ I ] += inv_chi_nl[ xi_t.size( ) * I + i ] * xi_t[ i ];

            }

        }

        floatVector lagrange( 1, 1 );

        floatVector X = vectorTools::appendVectors( { Xi_t, lagrange } );

        floatType L;

        floatVector dLdX, dLdchi_nl, dLdxi_t;

        floatType dLdR_nl;

        floatVector d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl, d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl, d2Ldxi_tdxi_t, d2Ldxi_tdR_nl;

        floatType d2LdR_nldR_nl;

        ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X, chi_nl, xi_t, R_nl, L,
                                                             dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                             d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                             d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                             d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                             d2LdR_nldR_nl ) );

        floatType R = vectorTools::l2norm( dLdX );

        floatType Rp = R;

        floatType tol = tolr * R + tola;

        unsigned int num_iteration = 0;

        floatVector dX;

        while ( ( num_iteration < max_iteration ) && ( R > tol ) ){

            unsigned int rank;

            dX = -vectorTools::solveLinearSystem( d2LdXdX, dLdX, dLdX.size( ), dLdX.size( ), rank );

            floatType lambda = 1;

            ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X + lambda * dX, chi_nl, xi_t, R_nl, L,
                                                                 dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                                 d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                                 d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                                 d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                                 d2LdR_nldR_nl ) );

            unsigned int num_ls = 0;

            R = vectorTools::l2norm( dLdX );

            while ( ( num_ls < max_ls ) && ( R > ( 1 - alpha_ls ) * Rp ) ){ 

                lambda *= 0.5;

                ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X + lambda * dX, chi_nl, xi_t, R_nl, L,
                                                                     dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                                     d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                                     d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                                     d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                                     d2LdR_nldR_nl ) );

                R = vectorTools::l2norm( dLdX );

                num_ls++;

            }

            if ( R > ( 1 - alpha_ls ) * Rp ){

                ERROR_TOOLS_CATCH( throw std::runtime_error( "Failure in linesearch" ) );

            }

            X += lambda * dX;

            Rp = R;

            num_iteration++;

        }

        if ( R > tol ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The optimizer did not converge" ) );

        }

        d = -xi_t;

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                d[ i ] += chi_nl[ xi_t.size( ) * i + I ] * X[ I ];

            }

        }

        // Evaluate the additional derivatives

        floatVector d3LdXdXdX, d3LdXdXdchi_nl, d3LdXdchi_nldchi_nl, d3LdXdXdxi_t, d3LdXdchi_nldxi_t,
                    d3LdXdXdR_nl, d3LdXdchi_nldR_nl, d3LdXdxi_tdxi_t, d3LdXdxi_tdR_nl, d3LdXdR_nldR_nl;

        ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X, chi_nl, xi_t, R_nl, L, dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                             d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                             d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                             d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                             d2LdR_nldR_nl,
                                                             d3LdXdXdX, d3LdXdXdchi_nl, d3LdXdchi_nldchi_nl,
                                                             d3LdXdXdxi_t, d3LdXdchi_nldxi_t,
                                                             d3LdXdXdR_nl, d3LdXdchi_nldR_nl,
                                                             d3LdXdxi_tdxi_t, d3LdXdxi_tdR_nl,
                                                             d3LdXdR_nldR_nl ) );

        // Compute the first order derivatives

         // Wrap the Jacobians of the Residual
        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdX( d2LdXdX.data( ), X.size( ), X.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdchi_nl( d2LdXdchi_nl.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdxi_t( d2LdXdxi_t.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdR_nl( d2LdXdR_nl.data( ), X.size( ), 1 );

        // Wrap the flattened versions of the Jacobians
        floatVector _flat_dXdchi_nl( X.size( ) * chi_nl.size( ), 0 );

        floatVector _flat_dXdxi_t( X.size( ) * xi_t.size( ), 0 );

        floatVector _flat_dXdR_nl( X.size( ), 0 );

        floatVector _flat_d2Xdchi_nldchi_nl( X.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        floatVector _flat_d2Xdchi_nldxi_t( X.size( ) * chi_nl.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d2Xdchi_nldR_nl( X.size( ) * chi_nl.size( ), 0 );

        floatVector _flat_d2Xdxi_tdxi_t( X.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d2Xdxi_tdR_nl( X.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d2XdR_nldR_nl( X.size( ), 0 );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _dXdchi_nl( _flat_dXdchi_nl.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _dXdxi_t( _flat_dXdxi_t.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _dXdR_nl( _flat_dXdR_nl.data( ), X.size( ), 1 );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2Xdchi_nldchi_nl( _flat_d2Xdchi_nldchi_nl.data( ), X.size( ), chi_nl.size( ) * chi_nl.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2Xdchi_nldxi_t( _flat_d2Xdchi_nldxi_t.data( ), X.size( ), chi_nl.size( ) * xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2Xdchi_nldR_nl( _flat_d2Xdchi_nldR_nl.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2Xdxi_tdxi_t( _flat_d2Xdxi_tdxi_t.data( ), X.size( ), xi_t.size( ) * xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2Xdxi_tdR_nl( _flat_d2Xdxi_tdR_nl.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2XdR_nldR_nl( _flat_d2XdR_nldR_nl.data( ), X.size( ), 1 );

        // Compute the Jacobians of the unknown vector

        vectorTools::solverType< floatType > linearSolver( _d2LdXdX );

        _dXdchi_nl = -linearSolver.solve( _d2LdXdchi_nl );

        _dXdxi_t = -linearSolver.solve( _d2LdXdxi_t );

        _dXdR_nl = -linearSolver.solve( _d2LdXdR_nl );

        floatVector D2XDCHIDCHI_RHS( X.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        floatVector D2XDCHIDXI_RHS( X.size( ) * chi_nl.size( ) * xi_t.size( ), 0 );

        floatVector D2XDCHIDR_RHS( X.size( ) * chi_nl.size( ), 0 );

        floatVector D2XDXIDXI_RHS( X.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        floatVector D2XDXIDR_RHS( X.size( ) * xi_t.size( ), 0 );

        floatVector D2XDRDR_RHS( X.size( ), 0 );

        for ( unsigned int I = 0; I < X.size( ); I++ ){

            D2XDRDR_RHS[ I ] += d3LdXdR_nldR_nl[ I ];

            for ( unsigned int J = 0; J < X.size( ); J++ ){

                D2XDRDR_RHS[ I ] += 2 * d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_dXdR_nl[ J ];

                for ( unsigned int K = 0; K < X.size( ); K++ ){

                    D2XDRDR_RHS[ I ] += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ J ] * _flat_dXdR_nl[ K ];

                }

            }

            for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                D2XDXIDR_RHS[ xi_t.size( ) * I + a ] += d3LdXdxi_tdR_nl[ xi_t.size( ) * I + a ];

                for ( unsigned int J = 0; J < X.size( ); J++ ){

                    D2XDXIDR_RHS[ xi_t.size( ) * I + a ] += d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ]
                                                          + d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + a ] * _flat_dXdR_nl[ J ];

                    for ( unsigned int K = 0; K < X.size( ); K++ ){

                        D2XDXIDR_RHS[ xi_t.size( ) * I + a ] += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ] * _flat_dXdR_nl[ K ];

                    }

                }

                for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                    D2XDXIDXI_RHS[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * a + b ]
                         += d3LdXdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * a + b ];

                    for ( unsigned int J = 0; J < X.size( ); J++ ){

                        D2XDXIDXI_RHS[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * a + b ]
                             += d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + b ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ]
                             + d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + a ] * _flat_dXdxi_t[ xi_t.size( ) * J + b ];

                        for ( unsigned int K = 0; K < X.size( ); K++ ){

                            D2XDXIDXI_RHS[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * a + b ]
                                 += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ] * _flat_dXdxi_t[ xi_t.size( ) * K + b ];

                        }

                    }

                }

                for ( unsigned int A = 0; A < xi_t.size( ); A++ ){

                    D2XDCHIDR_RHS[ chi_nl.size( ) * I + xi_t.size( ) * a + A ]
                         += d3LdXdchi_nldR_nl[ chi_nl.size( ) * I + xi_t.size( ) * a + A ];

                    for ( unsigned int J = 0; J < X.size( ); J++ ){

                        D2XDCHIDR_RHS[ chi_nl.size( ) * I + xi_t.size( ) * a + A ]
                             += d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ]
                              + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + xi_t.size( ) * a + A ] * _flat_dXdR_nl[ J ];

                        for ( unsigned int K = 0; K < X.size( ); K++ ){

                            D2XDCHIDR_RHS[ chi_nl.size( ) * I + xi_t.size( ) * a + A ]
                                 += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ] * _flat_dXdR_nl[ K ];

                        }

                    }

                    for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                        D2XDCHIDXI_RHS[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ]
                             += d3LdXdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ];

                        for ( unsigned int J = 0; J < X.size( ); J++ ){

                            D2XDCHIDXI_RHS[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ]
                                 += d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + b ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ]
                                  + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + xi_t.size( ) * a + A ] * _flat_dXdxi_t[ xi_t.size( ) * J + b ];

                            for ( unsigned int K = 0; K < X.size( ); K++ ){

                                D2XDCHIDXI_RHS[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ]
                                     += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ] * _flat_dXdxi_t[ xi_t.size( ) * K + b ];

                            }

                        }
            
                        for ( unsigned int B = 0; B < xi_t.size( ); B++ ){
        
                            D2XDCHIDCHI_RHS[ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                 += d3LdXdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ];

                            for ( unsigned int J = 0; J < X.size( ); J++ ){
            
                                D2XDCHIDCHI_RHS[ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                     += d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + xi_t.size( ) * b + B ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ]
                                      + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + xi_t.size( ) * a + A ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * b + B ];
            
                                for ( unsigned int K = 0; K < X.size( ); K++ ){
            
                                    D2XDCHIDCHI_RHS[ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                         += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * K + xi_t.size( ) * b + B ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ];
            
                                }
            
                            }
        
                        }

                    }

                }

            }

        }

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDCHIDCHI_RHS( D2XDCHIDCHI_RHS.data( ), X.size( ), chi_nl.size( ) * chi_nl.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDCHIDXI_RHS( D2XDCHIDXI_RHS.data( ), X.size( ), chi_nl.size( ) * xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDCHIDR_RHS( D2XDCHIDR_RHS.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDXIDXI_RHS( D2XDXIDXI_RHS.data( ), X.size( ), xi_t.size( ) * xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDXIDR_RHS( D2XDXIDR_RHS.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDRDR_RHS( D2XDRDR_RHS.data( ), X.size( ), 1 );

        _d2Xdchi_nldchi_nl = -linearSolver.solve( _D2XDCHIDCHI_RHS );

        _d2Xdchi_nldxi_t   = -linearSolver.solve( _D2XDCHIDXI_RHS );

        _d2Xdchi_nldR_nl   = -linearSolver.solve( _D2XDCHIDR_RHS );

        _d2Xdxi_tdxi_t     = -linearSolver.solve( _D2XDXIDXI_RHS );

        _d2Xdxi_tdR_nl     = -linearSolver.solve( _D2XDXIDR_RHS );

        _d2XdR_nldR_nl     = -linearSolver.solve( _D2XDRDR_RHS );

        // Construct the jacobians of the distance vector

        floatVector eye( chi_nl.size( ), 0 );

        vectorTools::eye( eye );

        dddchi_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ), 0 ) );

        dddxi_t   = floatMatrix( d.size( ), floatVector( xi_t.size( ), 0 ) );

        dddR_nl   = floatVector( d.size( ), 0 );

        d2ddchi_nldchi_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ) * chi_nl.size( ), 0 ) );

        d2ddchi_nldxi_t = floatMatrix( d.size( ), floatVector( chi_nl.size( ) * xi_t.size( ), 0 ) );

        d2ddchi_nldR_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ), 0 ) );

        d2ddxi_tdxi_t = floatMatrix( d.size( ), floatVector( xi_t.size( ) * xi_t.size( ), 0 ) );

        d2ddxi_tdR_nl = floatMatrix( d.size( ), floatVector( xi_t.size( ), 0 ) );

        d2ddR_nldR_nl = floatVector( d.size( ), 0 );

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            dddxi_t[ i ][ i ] = -1;

            for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                dddR_nl[ i ] += chi_nl[ xi_t.size( ) * i + a ] * _flat_dXdR_nl[ a ];
                d2ddR_nldR_nl[ i ] += chi_nl[ xi_t.size( ) * i + a ] * _flat_d2XdR_nldR_nl[ a ];

                for ( unsigned int A = 0; A < xi_t.size( ); A++ ){

                    dddchi_nl[ i ][ xi_t.size( ) * a + A ] += eye[ xi_t.size( ) * i + a ] * X[ A ];

                    d2ddchi_nldR_nl[ i ][ xi_t.size( ) * a + A ] += eye[ xi_t.size( ) * i + a ] * _flat_dXdR_nl[ A ];

                    for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                        d2ddchi_nldxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ] += eye[ xi_t.size( ) * i + a ] * _flat_dXdxi_t[ xi_t.size( ) * A + b ];

                        for ( unsigned int B = 0; B < xi_t.size( ); B++ ){

                            d2ddchi_nldchi_nl[ i ][ xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                += eye[ xi_t.size( ) * i + a ] * _flat_dXdchi_nl[ chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                 + eye[ xi_t.size( ) * i + b ] * _flat_dXdchi_nl[ chi_nl.size( ) * B + xi_t.size( ) * a + A ];

                        }

                    }

                    dddxi_t[ i ][ a ] += chi_nl[ xi_t.size( ) * i + A ] * _flat_dXdxi_t[ xi_t.size( ) * A + a ];

                    d2ddxi_tdR_nl[ i ][ a ] += chi_nl[ xi_t.size( ) * i + A ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * A + a ];

                    for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                        dddchi_nl[ i ][ xi_t.size( ) * a + A ] += chi_nl[ xi_t.size( ) * i + I ] * _flat_dXdchi_nl[ chi_nl.size( ) * I + xi_t.size( ) * a + A ];

                        d2ddchi_nldR_nl[ i ][ xi_t.size( ) * a + A ] += chi_nl[ xi_t.size( ) * i + I ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * I + xi_t.size( ) * a + A ];

                        d2ddxi_tdxi_t[ i ][ xi_t.size( ) * a + I ] += chi_nl[ xi_t.size( ) * i + A ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * A + xi_t.size( ) * a + I ];

                        for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                            d2ddchi_nldxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ]
                                += chi_nl[ xi_t.size( ) * i + I ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ];
    
                            for ( unsigned int B = 0; B < xi_t.size( ); B++ ){
    
                            d2ddchi_nldchi_nl[ i ][ xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                += chi_nl[ xi_t.size( ) * i + I ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * I + xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ];

                            }

                        }

                    }

                }

            }

        }

        return;

    }

    void solveOverlapDistance( const floatVector &chi_nl, const floatVector &xi_t, const floatType &R_nl, floatVector &d,
                                   floatMatrix &dddchi_nl, floatMatrix &dddxi_t, floatVector &dddR_nl,
                                   floatMatrix &d2ddchi_nldchi_nl, floatMatrix &d2ddchi_nldxi_t, floatMatrix &d2ddchi_nldR_nl,
                                   floatMatrix &d2ddxi_tdxi_t, floatMatrix &d2ddxi_tdR_nl,
                                   floatVector &d2ddR_nldR_nl,
                                   floatMatrix &d3ddchi_nldchi_nldchi_nl, floatMatrix &d3ddchi_nldchi_nldxi_t, floatMatrix &d3ddchi_nldchi_nldR_nl,
                                   floatMatrix &d3ddchi_nldxi_tdxi_t, floatMatrix &d3ddchi_nldxi_tdR_nl,
                                   floatMatrix &d3ddchi_nldR_nldR_nl,
                                   floatMatrix &d3ddxi_tdxi_tdxi_t, floatMatrix &d3ddxi_tdxi_tdR_nl,
                                   floatMatrix &d3ddxi_tdR_nldR_nl,
                                   floatVector &d3ddR_nldR_nldR_nl,
                                   const floatType tolr, const floatType tola, const unsigned int max_iteration,
                                   const unsigned int max_ls, const floatType alpha_ls ){
        /*!
         * Solve for the overlap distance where \f$\xi_t\f$ is known to be inside of the non-local particle
         * 
         * \param &chi_nl: The non-local micro-deformation tensor
         * \param &xi_t: The position inside of the non-local particle
         * \param &R_nl: The non-local particle radius in the reference configuration
         * \param &d: The distance vector going from \f$\xi_t\f$ to the solved point on the surface of the non-local particle.
         * \param &dddchi_nl: The gradient of the distance vector w.r.t. the non-local micro-deformation tensor
         * \param &dddxi_t: The gradient of the distance vector w.r.t. the target micro-relative position vector
         * \param &dddR_nl: The gradient of the distance vector w.r.t. the non-local particle radius
         * \param &d2ddchi_nldchi_nl: The second gradient of the distance vector w.r.t. the non-local micro-deformation tensor
         * \param &d2ddchi_nldxi_t: The second gradient of the distance vector w.r.t. the non-local micro-deformation tensor and \f$\xi_t\f$
         * \param &d2ddchi_nldR_nl: The second gradient of the distance vector w.r.t. the non-local micro-deformation tensor the non-local reference radius
         * \param &d2ddxi_tdxi_t: The second gradient of the distance vector w.r.t. \f$\xi_t\f$
         * \param &d2ddxi_tdR_nl: The second gradient of the distance vector w.r.t. \f$\xi_t\f$ and the non-local reference radius
         * \param &d2ddR_nldR_nl: The second gradient of the distance vector w.r.t. the non-local reference radius
         * \param &d3ddchi_nldchi_nldchi_nl: The third gradient of the distance vector w.r.t. the non-local micro-deformation tensor
         * \param &d3ddchi_nldchi_nldxi_t: The third gradient of the distance vector w.r.t. the non-local micro-deformation tensor twice and \f$\xi_t\f$ once
         * \param &d3ddchi_nldchi_nldR_nl: The third gradient of the distance vector w.r.t. the non-local micro-deformation tensor, \f$\xi_t\f$, and the non-local reference radius
         * \param &d3ddchi_nldxi_tdxi_t: The third gradient of the distance vector w.r.t. the non-local micro-deformation tensor once and \f$\xi_t\f$ twice
         * \param &d3ddchi_nldxi_tdR_nl: The third gradient of the distance vector w.r.t. the non-local micro-deformation tensor, \f$\xi_t\f$ twice, and the non-local reference radius
         * \param &d3ddchi_nldR_nldR_nl: The third gradient of the distance vector w.r.t. the non-local micro-deformation tensor once and the non-local reference radius twice
         * \param &d3ddxi_tdxi_tdxi_t: The third gradient of the distance vector w.r.t. \f$\xi_t\f$
         * \param &d3ddxi_tdxi_tdR_nl: The third gradient of the distance vector w.r.t. \f$\xi_t\f$ twice and the non-local reference radius once
         * \param &d3ddxi_tdR_nldR_nl: The third gradient of the distance vector w.r.t. \f$\xi_t\f$ once and the non-local reference radius twice
         * \param &d3ddR_nldR_nldR_nl: The third gradient of the distance vector w.r.t. the non-local reference radius
         * \param tolr: The relative tolerance
         * \param tola: The absolute tolerance
         * \param max_iteration: The maximum number of iterations
         * \param max_ls: The maximum number of line-search iterations
         * \param alpha_ls: The alpha parameter for the line-search
         */

        floatVector inv_chi_nl = vectorTools::inverse( chi_nl, xi_t.size( ), xi_t.size( ) );

        floatVector Xi_t( xi_t.size( ), 0 );

        for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

            for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

                Xi_t[ I ] += inv_chi_nl[ xi_t.size( ) * I + i ] * xi_t[ i ];

            }

        }

        floatVector lagrange( 1, 1 );

        floatVector X = vectorTools::appendVectors( { Xi_t, lagrange } );

        floatType L;

        floatVector dLdX, dLdchi_nl, dLdxi_t;

        floatType dLdR_nl;

        floatVector d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl, d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl, d2Ldxi_tdxi_t, d2Ldxi_tdR_nl;

        floatType d2LdR_nldR_nl;

        ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X, chi_nl, xi_t, R_nl, L,
                                                             dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                             d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                             d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                             d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                             d2LdR_nldR_nl ) );

        floatType R = vectorTools::l2norm( dLdX );

        floatType Rp = R;

        floatType tol = tolr * R + tola;

        unsigned int num_iteration = 0;

        floatVector dX;

        while ( ( num_iteration < max_iteration ) && ( R > tol ) ){

            unsigned int rank;

            dX = -vectorTools::solveLinearSystem( d2LdXdX, dLdX, dLdX.size( ), dLdX.size( ), rank );

            floatType lambda = 1;

            ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X + lambda * dX, chi_nl, xi_t, R_nl, L,
                                                                 dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                                 d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                                 d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                                 d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                                 d2LdR_nldR_nl ) );

            unsigned int num_ls = 0;

            R = vectorTools::l2norm( dLdX );

            while ( ( num_ls < max_ls ) && ( R > ( 1 - alpha_ls ) * Rp ) ){ 

                lambda *= 0.5;

                ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X + lambda * dX, chi_nl, xi_t, R_nl, L,
                                                                     dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                                     d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                                     d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                                     d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                                     d2LdR_nldR_nl ) );

                R = vectorTools::l2norm( dLdX );

                num_ls++;

            }

            if ( R > ( 1 - alpha_ls ) * Rp ){

                ERROR_TOOLS_CATCH( throw std::runtime_error( "Failure in linesearch" ) );

            }

            X += lambda * dX;

            Rp = R;

            num_iteration++;

        }

        if ( R > tol ){

            ERROR_TOOLS_CATCH( throw std::runtime_error( "The optimizer did not converge" ) );

        }

        d = -xi_t;

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                d[ i ] += chi_nl[ xi_t.size( ) * i + I ] * X[ I ];

            }

        }

        // Evaluate the additional derivatives

        floatVector d3LdXdXdX, d3LdXdXdchi_nl, d3LdXdchi_nldchi_nl, d3LdXdXdxi_t, d3LdXdchi_nldxi_t,
                    d3LdXdXdR_nl, d3LdXdchi_nldR_nl, d3LdXdxi_tdxi_t, d3LdXdxi_tdR_nl, d3LdXdR_nldR_nl,
                    d4LdXdXdchi_nldchi_nl;

        ERROR_TOOLS_CATCH( computeOverlapDistanceLagrangian( X, chi_nl, xi_t, R_nl, L, dLdX, dLdchi_nl, dLdxi_t, dLdR_nl,
                                                             d2LdXdX, d2LdXdchi_nl, d2LdXdxi_t, d2LdXdR_nl,
                                                             d2Ldchi_nldchi_nl, d2Ldchi_nldxi_t, d2Ldchi_nldR_nl,
                                                             d2Ldxi_tdxi_t, d2Ldxi_tdR_nl,
                                                             d2LdR_nldR_nl,
                                                             d3LdXdXdX, d3LdXdXdchi_nl, d3LdXdchi_nldchi_nl,
                                                             d3LdXdXdxi_t, d3LdXdchi_nldxi_t,
                                                             d3LdXdXdR_nl, d3LdXdchi_nldR_nl,
                                                             d3LdXdxi_tdxi_t, d3LdXdxi_tdR_nl,
                                                             d3LdXdR_nldR_nl,
                                                             d4LdXdXdchi_nldchi_nl ) );

        // Compute the first order derivatives

         // Wrap the Jacobians of the Residual
        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdX( d2LdXdX.data( ), X.size( ), X.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdchi_nl( d2LdXdchi_nl.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdxi_t( d2LdXdxi_t.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2LdXdR_nl( d2LdXdR_nl.data( ), X.size( ), 1 );

        // Wrap the flattened versions of the Jacobians
        floatVector _flat_dXdchi_nl( X.size( ) * chi_nl.size( ), 0 );

        floatVector _flat_dXdxi_t( X.size( ) * xi_t.size( ), 0 );

        floatVector _flat_dXdR_nl( X.size( ), 0 );

        floatVector _flat_d2Xdchi_nldchi_nl( X.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        floatVector _flat_d2Xdchi_nldxi_t( X.size( ) * chi_nl.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d2Xdchi_nldR_nl( X.size( ) * chi_nl.size( ), 0 );

        floatVector _flat_d2Xdxi_tdxi_t( X.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d2Xdxi_tdR_nl( X.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d2XdR_nldR_nl( X.size( ), 0 );

        floatVector _flat_d3Xdchi_nldchi_nldchi_nl( X.size( ) * chi_nl.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        floatVector _flat_d3Xdchi_nldchi_nldxi_t( X.size( ) * chi_nl.size( ) * chi_nl.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d3Xdchi_nldchi_nldR_nl( X.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        floatVector _flat_d3Xdchi_nldxi_tdxi_t( X.size( ) * chi_nl.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d3Xdchi_nldxi_tdR_nl( X.size( ) * chi_nl.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d3Xdchi_nldR_nldR_nl( X.size( ) * chi_nl.size( ), 0 );

        floatVector _flat_d3Xdxi_tdxi_tdxi_t( X.size( ) * xi_t.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d3Xdxi_tdxi_tdR_nl( X.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d3Xdxi_tdR_nldR_nl( X.size( ) * xi_t.size( ), 0 );

        floatVector _flat_d3XdR_nldR_nldR_nl( X.size( ), 0 );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _dXdchi_nl( _flat_dXdchi_nl.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _dXdxi_t( _flat_dXdxi_t.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _dXdR_nl( _flat_dXdR_nl.data( ), X.size( ), 1 );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2Xdchi_nldchi_nl( _flat_d2Xdchi_nldchi_nl.data( ), X.size( ), chi_nl.size( ) * chi_nl.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2Xdchi_nldxi_t( _flat_d2Xdchi_nldxi_t.data( ), X.size( ), chi_nl.size( ) * xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2Xdchi_nldR_nl( _flat_d2Xdchi_nldR_nl.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2Xdxi_tdxi_t( _flat_d2Xdxi_tdxi_t.data( ), X.size( ), xi_t.size( ) * xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2Xdxi_tdR_nl( _flat_d2Xdxi_tdR_nl.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d2XdR_nldR_nl( _flat_d2XdR_nldR_nl.data( ), X.size( ), 1 );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d3Xdchi_nldchi_nldchi_nl( _flat_d3Xdchi_nldchi_nldchi_nl.data( ), X.size( ), chi_nl.size( ) * chi_nl.size( ) * chi_nl.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d3Xdchi_nldchi_nldxi_t( _flat_d3Xdchi_nldchi_nldxi_t.data( ), X.size( ), chi_nl.size( ) * chi_nl.size( ) * xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d3Xdchi_nldchi_nldR_nl( _flat_d3Xdchi_nldchi_nldR_nl.data( ), X.size( ), chi_nl.size( ) * chi_nl.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d3Xdchi_nldxi_tdxi_t( _flat_d3Xdchi_nldxi_tdxi_t.data( ), X.size( ), chi_nl.size( ) * xi_t.size( ) * xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d3Xdchi_nldxi_tdR_nl( _flat_d3Xdchi_nldxi_tdR_nl.data( ), X.size( ), chi_nl.size( ) * xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d3Xdchi_nldR_nldR_nl( _flat_d3Xdchi_nldR_nldR_nl.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d3Xdxi_tdxi_tdxi_t( _flat_d3Xdxi_tdxi_tdxi_t.data( ), X.size( ), xi_t.size( ) * xi_t.size( ) * xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d3Xdxi_tdxi_tdR_nl( _flat_d3Xdxi_tdxi_tdR_nl.data( ), X.size( ), xi_t.size( ) * xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d3Xdxi_tdR_nldR_nl( _flat_d3Xdxi_tdR_nldR_nl.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _d3XdR_nldR_nldR_nl( _flat_d3XdR_nldR_nldR_nl.data( ), X.size( ), 1 );

        // Compute the Jacobians of the unknown vector

        vectorTools::solverType< floatType > linearSolver( _d2LdXdX );

        _dXdchi_nl = -linearSolver.solve( _d2LdXdchi_nl );

        _dXdxi_t = -linearSolver.solve( _d2LdXdxi_t );

        _dXdR_nl = -linearSolver.solve( _d2LdXdR_nl );

        floatVector D2XDCHIDCHI_RHS( X.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        floatVector D2XDCHIDXI_RHS( X.size( ) * chi_nl.size( ) * xi_t.size( ), 0 );

        floatVector D2XDCHIDR_RHS( X.size( ) * chi_nl.size( ), 0 );

        floatVector D2XDXIDXI_RHS( X.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        floatVector D2XDXIDR_RHS( X.size( ) * xi_t.size( ), 0 );

        floatVector D2XDRDR_RHS( X.size( ), 0 );

        for ( unsigned int I = 0; I < X.size( ); I++ ){

            D2XDRDR_RHS[ I ] += d3LdXdR_nldR_nl[ I ];

            for ( unsigned int J = 0; J < X.size( ); J++ ){

                D2XDRDR_RHS[ I ] += 2 * d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_dXdR_nl[ J ];

                for ( unsigned int K = 0; K < X.size( ); K++ ){

                    D2XDRDR_RHS[ I ] += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ J ] * _flat_dXdR_nl[ K ];

                }

            }

            for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                D2XDXIDR_RHS[ xi_t.size( ) * I + a ] += d3LdXdxi_tdR_nl[ xi_t.size( ) * I + a ];

                for ( unsigned int J = 0; J < X.size( ); J++ ){

                    D2XDXIDR_RHS[ xi_t.size( ) * I + a ] += d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ]
                                                          + d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + a ] * _flat_dXdR_nl[ J ];

                    for ( unsigned int K = 0; K < X.size( ); K++ ){

                        D2XDXIDR_RHS[ xi_t.size( ) * I + a ] += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ] * _flat_dXdR_nl[ K ];

                    }

                }

                for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                    D2XDXIDXI_RHS[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * a + b ]
                         += d3LdXdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * a + b ];

                    for ( unsigned int J = 0; J < X.size( ); J++ ){

                        D2XDXIDXI_RHS[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * a + b ]
                             += d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + b ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ]
                             + d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + a ] * _flat_dXdxi_t[ xi_t.size( ) * J + b ];

                        for ( unsigned int K = 0; K < X.size( ); K++ ){

                            D2XDXIDXI_RHS[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * a + b ]
                                 += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ] * _flat_dXdxi_t[ xi_t.size( ) * K + b ];

                        }

                    }

                }

                for ( unsigned int A = 0; A < xi_t.size( ); A++ ){

                    D2XDCHIDR_RHS[ chi_nl.size( ) * I + xi_t.size( ) * a + A ]
                         += d3LdXdchi_nldR_nl[ chi_nl.size( ) * I + xi_t.size( ) * a + A ];

                    for ( unsigned int J = 0; J < X.size( ); J++ ){

                        D2XDCHIDR_RHS[ chi_nl.size( ) * I + xi_t.size( ) * a + A ]
                             += d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ]
                              + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + xi_t.size( ) * a + A ] * _flat_dXdR_nl[ J ];

                        for ( unsigned int K = 0; K < X.size( ); K++ ){

                            D2XDCHIDR_RHS[ chi_nl.size( ) * I + xi_t.size( ) * a + A ]
                                 += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ] * _flat_dXdR_nl[ K ];

                        }

                    }

                    for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                        D2XDCHIDXI_RHS[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ]
                             += d3LdXdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ];

                        for ( unsigned int J = 0; J < X.size( ); J++ ){

                            D2XDCHIDXI_RHS[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ]
                                 += d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + b ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ]
                                  + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + xi_t.size( ) * a + A ] * _flat_dXdxi_t[ xi_t.size( ) * J + b ];

                            for ( unsigned int K = 0; K < X.size( ); K++ ){

                                D2XDCHIDXI_RHS[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ]
                                     += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ] * _flat_dXdxi_t[ xi_t.size( ) * K + b ];

                            }

                        }
            
                        for ( unsigned int B = 0; B < xi_t.size( ); B++ ){
        
                            D2XDCHIDCHI_RHS[ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                 += d3LdXdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ];

                            for ( unsigned int J = 0; J < X.size( ); J++ ){
            
                                D2XDCHIDCHI_RHS[ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                     += d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + xi_t.size( ) * b + B ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ]
                                      + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + xi_t.size( ) * a + A ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * b + B ];
            
                                for ( unsigned int K = 0; K < X.size( ); K++ ){
            
                                    D2XDCHIDCHI_RHS[ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                         += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * K + xi_t.size( ) * b + B ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + xi_t.size( ) * a + A ];
            
                                }
            
                            }
        
                        }

                    }

                }

            }

        }

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDCHIDCHI_RHS( D2XDCHIDCHI_RHS.data( ), X.size( ), chi_nl.size( ) * chi_nl.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDCHIDXI_RHS( D2XDCHIDXI_RHS.data( ), X.size( ), chi_nl.size( ) * xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDCHIDR_RHS( D2XDCHIDR_RHS.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDXIDXI_RHS( D2XDXIDXI_RHS.data( ), X.size( ), xi_t.size( ) * xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDXIDR_RHS( D2XDXIDR_RHS.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D2XDRDR_RHS( D2XDRDR_RHS.data( ), X.size( ), 1 );

        _d2Xdchi_nldchi_nl = -linearSolver.solve( _D2XDCHIDCHI_RHS );

        _d2Xdchi_nldxi_t   = -linearSolver.solve( _D2XDCHIDXI_RHS );

        _d2Xdchi_nldR_nl   = -linearSolver.solve( _D2XDCHIDR_RHS );

        _d2Xdxi_tdxi_t     = -linearSolver.solve( _D2XDXIDXI_RHS );

        _d2Xdxi_tdR_nl     = -linearSolver.solve( _D2XDXIDR_RHS );

        _d2XdR_nldR_nl     = -linearSolver.solve( _D2XDRDR_RHS );

        floatVector D3XDCHIDCHIDCHI_RHS( X.size( ) * chi_nl.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        floatVector D3XDCHIDCHIDXI_RHS( X.size( ) * chi_nl.size( ) * chi_nl.size( ) * xi_t.size( ), 0 );

        floatVector D3XDCHIDCHIDR_RHS( X.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 );

        floatVector D3XDCHIDXIDXI_RHS( X.size( ) * chi_nl.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        floatVector D3XDCHIDXIDR_RHS( X.size( ) * chi_nl.size( ) * xi_t.size( ), 0 );

        floatVector D3XDCHIDRDR_RHS( X.size( ) * chi_nl.size( ), 0 );

        floatVector D3XDXIDXIDXI_RHS( X.size( ) * xi_t.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        floatVector D3XDXIDXIDR_RHS( X.size( ) * xi_t.size( ) * xi_t.size( ), 0 );

        floatVector D3XDXIDRDR_RHS( X.size( ) * xi_t.size( ), 0 );

        floatVector D3XDRDRDR_RHS( X.size( ), 0 );

        for ( unsigned int I = 0; I < X.size( ); I++ ){

            for ( unsigned int aA = 0; aA < chi_nl.size( ); aA++ ){

                for ( unsigned int bB = 0; bB < chi_nl.size( ); bB++ ){
    
                    for ( unsigned int cC = 0; cC < chi_nl.size( ); cC++ ){
    
                        for ( unsigned int J = 0; J < X.size( ); J++ ){
        
                            D3XDCHIDCHIDCHI_RHS[ chi_nl.size( ) * chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * chi_nl.size( ) * aA + chi_nl.size( ) * bB + cC ]
                                += d4LdXdXdchi_nldchi_nl[ X.size( ) * chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * bB + cC ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + aA ]
                                 + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( )  * J + bB ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA  + cC ]
                                 + d4LdXdXdchi_nldchi_nl[ X.size( ) * chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + bB ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + cC ]
                                 + d4LdXdXdchi_nldchi_nl[ X.size( ) * chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + cC ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + bB ]
                                 + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + aA ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * bB + cC ]
                                 + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + cC ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + bB ];
        
                            for ( unsigned int K = 0; K < X.size( ); K++ ){
        
                                D3XDCHIDCHIDCHI_RHS[ chi_nl.size( ) * chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * chi_nl.size( ) * aA + chi_nl.size( ) * bB + cC ]
                                    += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + aA ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * K + chi_nl.size( ) * bB + cC ]
                                     + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * K + bB ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + cC ]
                                     + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * K + cC ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + bB ];
        
                            }
        
                        }
    
                    }
    
                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){
    
                        for ( unsigned int J = 0; J < X.size( ); J++ ){
        
                            D3XDCHIDCHIDXI_RHS[ chi_nl.size( ) * chi_nl.size( ) * xi_t.size( ) * I + chi_nl.size( ) * xi_t.size( ) * aA + xi_t.size( ) * bB + c ]
                                += d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + bB ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * aA + c ]
                                 + d4LdXdXdchi_nldchi_nl[ X.size( ) * chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + bB ] * _flat_dXdxi_t[ xi_t.size( ) * J + c ]
                                 + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + aA ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * bB + c ]
                                 + d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + c ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + bB ];
        
                            for ( unsigned int K = 0; K < X.size( ); K++ ){
        
                                D3XDCHIDCHIDXI_RHS[ chi_nl.size( ) * chi_nl.size( ) * xi_t.size( ) * I + chi_nl.size( ) * xi_t.size( ) * aA + xi_t.size( ) * bB + c ]
                                    += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + aA ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * K + xi_t.size( ) * bB + c ]
                                     + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * K + bB ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * aA + c ]
                                     + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * K + c ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + bB ];
        
                            }
        
                        }
    
                    }

                    for ( unsigned int J = 0; J < X.size( ); J++ ){
        
                        D3XDCHIDCHIDR_RHS[ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * aA + bB ]
                            += d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + bB ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * J + aA ]
                             + d4LdXdXdchi_nldchi_nl[ X.size( ) * chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + bB ] * _flat_dXdR_nl[ J ]
                             + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + aA ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * J + bB ]
                             + d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + bB ];

                        for ( unsigned int K = 0; K < X.size( ); K++ ){
        
                            D3XDCHIDCHIDR_RHS[ chi_nl.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * aA +  bB ]
                                += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + aA ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * K + bB ]
                                 + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * K + bB ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * J + aA ]
                                 + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ K ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * J + chi_nl.size( ) * aA + bB ];
                        }
        
                    }
    
                }

                for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                        for ( unsigned int J = 0; J < X.size( ); J++ ){
        
                            D3XDCHIDXIDXI_RHS[ chi_nl.size( ) * xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * aA + xi_t.size( ) * b + c ]
                                += d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + b ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * aA + c ]
                                 + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + aA ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * J + xi_t.size( ) * b + c ]
                                 + d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + c ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * aA + b ];
        
                            for ( unsigned int K = 0; K < X.size( ); K++ ){
        
                                D3XDCHIDXIDXI_RHS[ chi_nl.size( ) * xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * aA + xi_t.size( ) * b + c ]
                                    += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + aA ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * b + c ]
                                     + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * K + b ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * aA + c ]
                                     + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * K + c ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * aA + b ];
        
                            }
        
                        }

                    }

                    for ( unsigned int J = 0; J < X.size( ); J++ ){
        
                        D3XDCHIDXIDR_RHS[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * aA + b ]
                            += d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + b ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * J + aA ]
                             + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + aA ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * J + b ]
                             + d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * aA + b ];
        
                        for ( unsigned int K = 0; K < X.size( ); K++ ){
        
                            D3XDCHIDXIDR_RHS[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * aA + b ]
                                += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + aA ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * K + b ]
                                 + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * K + b ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * J + aA ]
                                 + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ K ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * J + xi_t.size( ) * aA + b ];
        
                        }
        
                    }

                }

                for ( unsigned int J = 0; J < X.size( ); J++ ){
        
                    D3XDCHIDRDR_RHS[ chi_nl.size( ) * I + aA ]
                        += d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * J + aA ]
                         + d3LdXdXdchi_nl[ X.size( ) * chi_nl.size( ) * I + chi_nl.size( ) * J + aA ] * _flat_d2XdR_nldR_nl[ J ]
                         + d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * J + aA ];
        
                    for ( unsigned int K = 0; K < X.size( ); K++ ){
        
                        D3XDCHIDRDR_RHS[ chi_nl.size( ) * I + aA ]
                            += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdchi_nl[ chi_nl.size( ) * J + aA ] * _flat_d2XdR_nldR_nl[ K ]
                             + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ K ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * J + aA ]
                             + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ K ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * J + aA ];
        
                    }
        
                }

            }

            for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                    for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                        for ( unsigned int J = 0; J < X.size( ); J++ ){

                            D3XDXIDXIDXI_RHS[ xi_t.size( ) * xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ]
                                += d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + b ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * J + xi_t.size( ) * a + c ]
                                 + d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + a ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * J + xi_t.size( ) * b + c ]
                                 + d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + c ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * J + xi_t.size( ) * a + b ];

                            for ( unsigned int K = 0; K < X.size( ); K++ ){

                                D3XDXIDXIDXI_RHS[ xi_t.size( ) * xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * b + c ]
                                    += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * K + xi_t.size( ) * b + c ]
                                     + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * K + b ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * J + xi_t.size( ) * a + c ]
                                     + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * K + c ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * J + xi_t.size( ) * a + b ];

                            }

                        }

                    }

                    for ( unsigned int J = 0; J < X.size( ); J++ ){

                        D3XDXIDXIDR_RHS[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * a + b ]
                            += d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + b ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * J + a ]
                             + d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + a ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * J + b ]
                             + d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * J + xi_t.size( ) * a + b ];

                        for ( unsigned int K = 0; K < X.size( ); K++ ){

                            D3XDXIDXIDR_RHS[ xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * a + b ]
                                += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * K + b ]
                                 + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * K + b ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * J + a ]
                                 + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ K ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * J + xi_t.size( ) * a + b ];

                        }

                    }

                }


                for ( unsigned int J = 0; J < X.size( ); J++ ){

                    D3XDXIDRDR_RHS[ xi_t.size( ) * I + a ]
                        += d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * J + a ]
                         + d3LdXdXdxi_t[ X.size( ) * xi_t.size( ) * I + xi_t.size( ) * J + a ] * _flat_d2XdR_nldR_nl[ J ]
                         + d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * J + a ];

                    for ( unsigned int K = 0; K < X.size( ); K++ ){

                        D3XDXIDRDR_RHS[ xi_t.size( ) * I + a ]
                            += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdxi_t[ xi_t.size( ) * J + a ] * _flat_d2XdR_nldR_nl[ K ]
                             + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ K ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * J + a ]
                             + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ K ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * J + a ];

                    }

                }

            }

            for ( unsigned int J = 0; J < X.size( ); J++ ){

                D3XDRDRDR_RHS[ I ]
                    += d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_d2XdR_nldR_nl[ J ]
                     + d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_d2XdR_nldR_nl[ J ]
                     + d3LdXdXdR_nl[ X.size( ) * I + J ] * _flat_d2XdR_nldR_nl[ J ];

                for ( unsigned int K = 0; K < X.size( ); K++ ){

                    D3XDRDRDR_RHS[ I ]
                        += d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ J ] * _flat_d2XdR_nldR_nl[ K ]
                         + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ K ] * _flat_d2XdR_nldR_nl[ J ]
                         + d3LdXdXdX[ X.size( ) * X.size( ) * I + X.size( ) * J + K ] * _flat_dXdR_nl[ K ] * _flat_d2XdR_nldR_nl[ J ];

                }

            }

        }

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D3XDCHIDCHIDCHI_RHS( D3XDCHIDCHIDCHI_RHS.data( ), X.size( ), chi_nl.size( ) * chi_nl.size( ) * chi_nl.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D3XDCHIDCHIDXI_RHS( D3XDCHIDCHIDXI_RHS.data( ), X.size( ), chi_nl.size( ) * chi_nl.size( ) * xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D3XDCHIDCHIDR_RHS( D3XDCHIDCHIDR_RHS.data( ), X.size( ), chi_nl.size( ) * chi_nl.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D3XDCHIDXIDXI_RHS( D3XDCHIDXIDXI_RHS.data( ), X.size( ), chi_nl.size( ) * xi_t.size( ) * xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D3XDCHIDXIDR_RHS( D3XDCHIDXIDR_RHS.data( ), X.size( ), chi_nl.size( ) * xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D3XDCHIDRDR_RHS( D3XDCHIDRDR_RHS.data( ), X.size( ), chi_nl.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D3XDXIDXIDXI_RHS( D3XDXIDXIDXI_RHS.data( ), X.size( ), xi_t.size( ) * xi_t.size( ) * xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D3XDXIDXIDR_RHS( D3XDXIDXIDR_RHS.data( ), X.size( ), xi_t.size( ) * xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D3XDXIDRDR_RHS( D3XDXIDRDR_RHS.data( ), X.size( ), xi_t.size( ) );

        Eigen::Map< const Eigen::Matrix< floatType, -1, -1, Eigen::RowMajor > >
            _D3XDRDRDR_RHS( D3XDRDRDR_RHS.data( ), X.size( ), 1 );

        _d3Xdchi_nldchi_nldchi_nl = -linearSolver.solve( _D3XDCHIDCHIDCHI_RHS );

        _d3Xdchi_nldchi_nldxi_t = -linearSolver.solve( _D3XDCHIDCHIDXI_RHS );

        _d3Xdchi_nldchi_nldR_nl = -linearSolver.solve( _D3XDCHIDCHIDR_RHS );

        _d3Xdchi_nldxi_tdxi_t = -linearSolver.solve( _D3XDCHIDXIDXI_RHS );

        _d3Xdchi_nldxi_tdR_nl = -linearSolver.solve( _D3XDCHIDXIDR_RHS );

        _d3Xdchi_nldR_nldR_nl = -linearSolver.solve( _D3XDCHIDRDR_RHS );

        _d3Xdxi_tdxi_tdxi_t = -linearSolver.solve( _D3XDXIDXIDXI_RHS );

        _d3Xdxi_tdxi_tdR_nl = -linearSolver.solve( _D3XDXIDXIDR_RHS );

        _d3Xdxi_tdR_nldR_nl = -linearSolver.solve( _D3XDXIDRDR_RHS );

        _d3XdR_nldR_nldR_nl = -linearSolver.solve( _D3XDRDRDR_RHS );

        // Construct the jacobians of the distance vector

        floatVector eye( chi_nl.size( ), 0 );

        vectorTools::eye( eye );

        dddchi_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ), 0 ) );

        dddxi_t   = floatMatrix( d.size( ), floatVector( xi_t.size( ), 0 ) );

        dddR_nl   = floatVector( d.size( ), 0 );

        d2ddchi_nldchi_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ) * chi_nl.size( ), 0 ) );

        d2ddchi_nldxi_t = floatMatrix( d.size( ), floatVector( chi_nl.size( ) * xi_t.size( ), 0 ) );

        d2ddchi_nldR_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ), 0 ) );

        d2ddxi_tdxi_t = floatMatrix( d.size( ), floatVector( xi_t.size( ) * xi_t.size( ), 0 ) );

        d2ddxi_tdR_nl = floatMatrix( d.size( ), floatVector( xi_t.size( ), 0 ) );

        d2ddR_nldR_nl = floatVector( d.size( ), 0 );

        d3ddchi_nldchi_nldchi_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ) * chi_nl.size( ) * chi_nl.size( ), 0 ) );

        d3ddchi_nldchi_nldxi_t = floatMatrix( d.size( ), floatVector( chi_nl.size( ) * chi_nl.size( ) * xi_t.size( ), 0 ) );

        d3ddchi_nldchi_nldR_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ) * chi_nl.size( ), 0 ) );

        d3ddchi_nldxi_tdxi_t = floatMatrix( d.size( ), floatVector( chi_nl.size( ) * xi_t.size( ) * xi_t.size( ), 0 ) );

        d3ddchi_nldxi_tdR_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ) * xi_t.size( ), 0 ) );

        d3ddchi_nldR_nldR_nl = floatMatrix( d.size( ), floatVector( chi_nl.size( ), 0 ) );

        d3ddxi_tdxi_tdxi_t = floatMatrix( d.size( ), floatVector( xi_t.size( ) * xi_t.size( ) * xi_t.size( ), 0 ) );

        d3ddxi_tdxi_tdR_nl = floatMatrix( d.size( ), floatVector( xi_t.size( ) * xi_t.size( ), 0 ) );

        d3ddxi_tdR_nldR_nl = floatMatrix( d.size( ), floatVector( xi_t.size( ), 0 ) );

        d3ddR_nldR_nldR_nl = floatVector( d.size( ), 0 );

        for ( unsigned int i = 0; i < xi_t.size( ); i++ ){

            dddxi_t[ i ][ i ] = -1;

            for ( unsigned int a = 0; a < xi_t.size( ); a++ ){

                dddR_nl[ i ] += chi_nl[ xi_t.size( ) * i + a ] * _flat_dXdR_nl[ a ];

                d2ddR_nldR_nl[ i ] += chi_nl[ xi_t.size( ) * i + a ] * _flat_d2XdR_nldR_nl[ a ];

                d3ddR_nldR_nldR_nl[ i ] += chi_nl[ xi_t.size( ) * i + a ] * _flat_d3XdR_nldR_nldR_nl[ a ];

                for ( unsigned int A = 0; A < xi_t.size( ); A++ ){

                    dddchi_nl[ i ][ xi_t.size( ) * a + A ] += eye[ xi_t.size( ) * i + a ] * X[ A ];

                    d2ddchi_nldR_nl[ i ][ xi_t.size( ) * a + A ] += eye[ xi_t.size( ) * i + a ] * _flat_dXdR_nl[ A ];

                    d3ddchi_nldR_nldR_nl[ i ][ xi_t.size( ) * a + A ] += eye[ xi_t.size( ) * i + a ] * _flat_d2XdR_nldR_nl[ A ];

                    for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                        d2ddchi_nldxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ] += eye[ xi_t.size( ) * i + a ] * _flat_dXdxi_t[ xi_t.size( ) * A + b ];

                        d3ddchi_nldxi_tdR_nl[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ] += eye[ xi_t.size( ) * i + a ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * A + b ];

                        for ( unsigned int B = 0; B < xi_t.size( ); B++ ){

                            d2ddchi_nldchi_nl[ i ][ xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                += eye[ xi_t.size( ) * i + a ] * _flat_dXdchi_nl[ chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                 + eye[ xi_t.size( ) * i + b ] * _flat_dXdchi_nl[ chi_nl.size( ) * B + xi_t.size( ) * a + A ];

                            d3ddchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * xi_t.size( ) * A + xi_t.size( ) * b + B ] += eye[ xi_t.size( ) * i + a ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * A + xi_t.size( ) * b + B ];

                            d3ddchi_nldchi_nldR_nl[ i ][ xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                += eye[ xi_t.size( ) * i + a ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                 + eye[ xi_t.size( ) * i + b ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * B + xi_t.size( ) * a + A ];

                            for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                d3ddchi_nldchi_nldxi_t[ i ][ xi_t.size( ) * chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * xi_t.size( ) * A + xi_t.size( ) * xi_t.size( ) * b + xi_t.size( ) * B + c ]
                                    += eye[ xi_t.size( ) * i + a ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * A + xi_t.size( ) * xi_t.size( ) * b + xi_t.size( ) * B + c ]
                                     + eye[ xi_t.size( ) * i + b ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * B + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + c ];

                                for ( unsigned int C = 0; C < xi_t.size( ); C++ ){

                                    d3ddchi_nldchi_nldchi_nl[ i ][ xi_t.size( ) * chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * chi_nl.size( ) * A + xi_t.size( ) * chi_nl.size( ) * b + chi_nl.size( ) * B + xi_t.size( ) * c + C ]
                                        += eye[ xi_t.size( ) * i + a ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * A + xi_t.size( ) * chi_nl.size( ) * b + chi_nl.size( ) * B + xi_t.size( ) * c + C ]
                                         + eye[ xi_t.size( ) * i + b ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * B + xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * c + C ];

                                }

                            }

                        }

                    }

                    dddxi_t[ i ][ a ] += chi_nl[ xi_t.size( ) * i + A ] * _flat_dXdxi_t[ xi_t.size( ) * A + a ];

                    d2ddxi_tdR_nl[ i ][ a ] += chi_nl[ xi_t.size( ) * i + A ] * _flat_d2Xdxi_tdR_nl[ xi_t.size( ) * A + a ];

                    d3ddxi_tdR_nldR_nl[ i ][ a ] += chi_nl[ xi_t.size( ) * i + A ] * _flat_d3Xdxi_tdR_nldR_nl[ xi_t.size( ) * A + a ];

                    for ( unsigned int I = 0; I < xi_t.size( ); I++ ){

                        dddchi_nl[ i ][ xi_t.size( ) * a + A ] += chi_nl[ xi_t.size( ) * i + I ] * _flat_dXdchi_nl[ chi_nl.size( ) * I + xi_t.size( ) * a + A ];

                        d2ddchi_nldR_nl[ i ][ xi_t.size( ) * a + A ] += chi_nl[ xi_t.size( ) * i + I ] * _flat_d2Xdchi_nldR_nl[ chi_nl.size( ) * I + xi_t.size( ) * a + A ];

                        d3ddchi_nldR_nldR_nl[ i ][ xi_t.size( ) * a + A ] += chi_nl[ xi_t.size( ) * i + I ] * _flat_d3Xdchi_nldR_nldR_nl[ chi_nl.size( ) * I + xi_t.size( ) * a + A ];

                        d2ddxi_tdxi_t[ i ][ xi_t.size( ) * a + I ] += chi_nl[ xi_t.size( ) * i + A ] * _flat_d2Xdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * A + xi_t.size( ) * a + I ];

                        d3ddxi_tdxi_tdR_nl[ i ][ xi_t.size( ) * a + I ] += chi_nl[ xi_t.size( ) * i + A ] * _flat_d3Xdxi_tdxi_tdR_nl[ xi_t.size( ) * xi_t.size( ) * A + xi_t.size( ) * a + I ];

                        for ( unsigned int b = 0; b < xi_t.size( ); b++ ){

                            d2ddchi_nldxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ]
                                += chi_nl[ xi_t.size( ) * i + I ] * _flat_d2Xdchi_nldxi_t[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ];
    
                            d3ddxi_tdxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * I + b ]
                                += chi_nl[ xi_t.size( ) * i + A ] * _flat_d3Xdxi_tdxi_tdxi_t[ xi_t.size( ) * xi_t.size( ) * xi_t.size( ) * A + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * I + b ];

                            d3ddchi_nldxi_tdR_nl[ i ][ xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ]
                                += chi_nl[ xi_t.size( ) * i + I ] * _flat_d3Xdchi_nldxi_tdR_nl[ chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * A + b ];

                            for ( unsigned int B = 0; B < xi_t.size( ); B++ ){
    
                                d2ddchi_nldchi_nl[ i ][ xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                    += chi_nl[ xi_t.size( ) * i + I ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * I + xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ];

                                d3ddchi_nldxi_tdxi_t[ i ][ xi_t.size( ) * xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * xi_t.size( ) * A + xi_t.size( ) * b + B ]
                                    += chi_nl[ xi_t.size( ) * i + I ] * _flat_d3Xdchi_nldxi_tdxi_t[ chi_nl.size( ) * xi_t.size( ) * xi_t.size( ) * I + xi_t.size( ) * xi_t.size( ) * xi_t.size( ) * a + xi_t.size( ) * xi_t.size( ) * A + xi_t.size( ) * b + B ];

                                d3ddchi_nldchi_nldR_nl[ i ][ xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                    += chi_nl[ xi_t.size( ) * i + I ] * _flat_d3Xdchi_nldchi_nldR_nl[ chi_nl.size( ) * chi_nl.size( ) * I + xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ];

                                for ( unsigned int c = 0; c < xi_t.size( ); c++ ){

                                    d3ddchi_nldchi_nldxi_t[ i ][ xi_t.size( ) * chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * xi_t.size( ) * A + xi_t.size( ) * xi_t.size( ) * b + xi_t.size( ) * B + c ]
                                        += chi_nl[ xi_t.size( ) * i + I ] * _flat_d3Xdchi_nldchi_nldxi_t[ chi_nl.size( ) * chi_nl.size( ) * xi_t.size( ) * I + xi_t.size( ) * chi_nl.size( ) * xi_t.size( ) * a + chi_nl.size( ) * xi_t.size( ) * A + xi_t.size( ) * xi_t.size( ) * b + xi_t.size( ) * B + c ];

                                    for ( unsigned int C = 0; C < xi_t.size( ); C++ ){

                                        d3ddchi_nldchi_nldchi_nl[ i ][ xi_t.size( ) * chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * chi_nl.size( ) * A + xi_t.size( ) * chi_nl.size( ) * b + chi_nl.size( ) * B + xi_t.size( ) * c + C ]
                                            += eye[ xi_t.size( ) * i + c ] * eye[ xi_t.size( ) * I + C ] * _flat_d2Xdchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * I + xi_t.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * A + xi_t.size( ) * b + B ]
                                             + chi_nl[ xi_t.size( ) * i + I ] * _flat_d3Xdchi_nldchi_nldchi_nl[ chi_nl.size( ) * chi_nl.size( ) * chi_nl.size( ) * I + xi_t.size( ) * chi_nl.size( ) * chi_nl.size( ) * a + chi_nl.size( ) * chi_nl.size( ) * A + xi_t.size( ) * chi_nl.size( ) * b + chi_nl.size( ) * B + xi_t.size( ) * c + C ];

                                    }

                                }

                            }

                        }

                    }

                }

            }

        }

        return;

    }

}
