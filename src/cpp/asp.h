/**
  ******************************************************************************
  * \file asp.h
  ******************************************************************************
  * A C++ library for printing messages to stdout. Used as a stub repo example.
  ******************************************************************************
  */

#ifndef ASP_H
#define ASP_H

#include<sstream>

#include<error_tools.h>
#define USE_EIGEN
#include<vector_tools.h>
#include<abaqus_tools.h>

namespace asp{

    namespace unit_test{
        class aspBaseTester;
    }

    constexpr const char* str_end(const char *str) {
        /*! Recursively search string for last character
         * \param *str: pointer to string START of UNIX path like string
         * \return *str: pointer to last character in string
         */
        return *str ? str_end(str + 1) : str;
    }
    constexpr bool str_slant(const char *str) {
        /*! Recursively search string for leftmost UNIX path separator from the left
         * \param *str: pointer to string START of UNIX path like string
         * \return bool: True if string contains UNIX path separator. Else false.
         */
        return *str == '/' ? true : (*str ? str_slant(str + 1) : false);
    }
    constexpr const char* r_slant(const char* str) {
        /*! Recursively search string for rightmost UNIX path separator from the right
         * \param *str: pointer to string END of UNIX path like string
         * \return *str: pointer to start of base name
         */
        return *str == '/' ? (str + 1) : r_slant(str - 1);
    }
    constexpr const char* file_name(const char* str) {
        /*! Return the current file name with extension at compile time
         * \param *str: pointer to string START of UNIX path like string
         * \return str: file base name
         */
        return str_slant(str) ? r_slant(str_end(str)) : str;
    }
    //Return filename for constructing debugging messages
    //https://stackoverflow.com/questions/31050113/how-to-extract-the-source-filename-without-path-and-suffix-at-compile-time
    const std::string __BASENAME__ = file_name(__FILE__);
    const std::string __FILENAME__ = __BASENAME__.substr(0, __BASENAME__.find_last_of("."));

    typedef double floatType; //!< Define the float values type.
    typedef std::vector< floatType > floatVector; //!< Define a vector of floats
    typedef std::vector< std::vector< floatType > > floatMatrix; //!< Define a matrix of floats
    typedef std::unordered_map< unsigned int, floatType > mapFloatType; //!< Define an unordered map of floats
    typedef std::unordered_map< unsigned int, floatVector > mapFloatVector; //!< Define an unordered map of float vectors
    typedef std::unordered_map< unsigned int, floatMatrix > mapFloatMatrix; //!< Define an unordered map of float matrices

    floatType _pi = 3.14159265;

    /// Say hello
    /// @param message The message to print
    void sayHello(std::string message);

    void abaqusInterface( double *STRESS,       double *STATEV,       double *DDSDDE,       double &SSE,          double &SPD,
                          double &SCD,          double &RPL,          double *DDSDDT,       double *DRPLDE,       double &DRPLDT,
                          const double *STRAN,  const double *DSTRAN, const double *TIME,   const double &DTIME,  const double &TEMP,
                          const double &DTEMP,  const double *PREDEF, const double *DPRED,  const char *CMNAME,   const int &NDI,
                          const int &NSHR,      const int &NTENS,     const int &NSTATV,    const double *PROPS,  const int &NPROPS,
                          const double *COORDS, const double *DROT,   double &PNEWDT,       const double &CELENT, const double *DFGRD0,
                          const double *DFGRD1, const int &NOEL,      const int &NPT,       const int &LAYER,     const int &KSPT,
                          const int *JSTEP,     const int &KINC );

    void dummyMaterialModel( floatVector &stress,             floatVector &statev,        floatMatrix &ddsdde,       floatType &SSE,            floatType &SPD,
                             floatType &SCD,                  floatType &RPL,             floatVector &ddsddt,       floatVector &drplde,       floatType &DRPLDT,
                             const floatVector &strain,       const floatVector &dstrain, const floatVector &time,   const floatType &DTIME,    const floatType &TEMP,
                             const floatType &DTEMP,          const floatVector &predef,  const floatVector &dpred,  const std::string &cmname, const int &NDI,
                             const int &NSHR,                 const int &NTENS,           const int &NSTATV,         const floatVector &props,  const int &NPROPS,
                             const floatVector &coords,       const floatMatrix &drot,    floatType &PNEWDT,         const floatType &CELENT,   const floatMatrix &dfgrd0,
                             const floatMatrix &dfgrd1,       const int &NOEL,            const int &NPT,            const int &LAYER,          const int &KSPT,
                             const std::vector< int > &jstep, const int &KINC );

    class dataBase{

        public:

            virtual void clear( ){
                /*!
                 * The function to erase the current values stored
                 */

                ERROR_TOOLS_CATCH( throw std::runtime_error( "clear not implemented!" ) );

            }

    };

    template < typename T >
    class dataStorage : public dataBase{

        public:

            bool first = false; //!<The flag for whether the data has been stored

            T second; //!<The stored data

            dataStorage( ){ };

            dataStorage( const bool &_first, const T &_second ) : first( _first ), second( _second ){
                /*! Main constructor
                 * 
                 * \param &_first: The flag for whether the data has been stored
                 * \param &_second: The value of the data
                 */
            }

            virtual void clear( ){
                /*!
                 * The function to erase the current values stored by setting first to false and clearing second
                 */

                first = false;

                second.clear( );

            }

    };

    template <>
    inline void dataStorage< int >::clear( ){
                /*!
                 * The function to erase the current values stored by setting first to false and second to zero
                 */

        first = false;

        second = 0;

    }

    template <>
    inline void dataStorage< unsigned int >::clear( ){
                /*!
                 * The function to erase the current values stored by setting first to false and second to zero
                 */

        first = false;

        second = 0;

    }

    template <>
    inline void dataStorage< floatType >::clear( ){
                /*!
                 * The function to erase the current values stored by setting first to false and second to zero
                 */

        first = false;

        second = 0;

    }

    class aspBase{
        /*!
         * The base class for all Anisotropic Stochastic Particle (ASP) models.
         */

        public:

            // Public parameters

            // Constructors
            aspBase( );

            // Public member functions
            virtual void computeLocalParticleEnergyDensity( const floatType &previousTime, const floatType &deltaTime,
                                                            const floatVector &currentMicroDeformation, const floatVector &previousMicroDeformation,
                                                            const floatType &currentTemperature, const floatType &previousTemperature,
                                                            const floatVector &previousStateVariables,
                                                            const floatVector &parameters,
                                                            floatType &energyDensity, floatVector &cauchyStress, floatVector &stateVariables );

            virtual void computeLocalParticleEnergyDensity( const floatType &previousTime, const floatType &deltaTime,
                                                            const floatVector &currentMicroDeformation, const floatVector &previousMicroDeformation,
                                                            const floatType &currentTemperature, const floatType &previousTemperature,
                                                            const floatVector &previousStateVariables,
                                                            const floatVector &parameters,
                                                            floatType &energyDensity, floatVector &cauchyStress, floatVector &stateVariables, floatType &logProbabilityRatio );
       
            virtual void computeSurfaceAdhesionTraction( floatVector &surfaceAdhesionTraction );

            virtual void computeSurfaceAdhesionEnergyDensity( floatType &surfaceAdhesionEnergyDensity );

            virtual void computeSurfaceAdhesionEnergyDensity( floatType &surfaceAdhesionEnergyDensity,
                                                              floatVector &dSurfaceAdhesionEnergyDensitydLocalDeformationGradient,
                                                              floatVector &dSurfaceAdhesionEnergyDensitydLocalMicroDeformation,
                                                              floatVector &dSurfaceAdhesionEnergyDensitydGradientMicroDeformation );

            virtual void computeSurfaceOverlapTraction( mapFloatVector &surfaceOverlapTraction );

            virtual void computeSurfaceOverlapEnergyDensity( mapFloatType &surfaceOverlapEnergyDensity );

            // Getter functions
            const unsigned int* getDimension( );

            const unsigned int* getNumLocalParticles( );

            const floatType* getRadius( );

            const floatType* getPreviousTime( );

            const floatType* getDeltaTime( );

            const floatType* getPreviousTemperature( );

            const floatType* getTemperature( );

            const floatType* getLocalParticleEnergy( );

            const floatType* getLocalParticleEnergyDensity( );

            const floatType* getLocalParticleLogProbabilityRatio( );

            const floatType* getSurfaceAdhesionEnergyDensity( );

            const floatVector* getdSurfaceAdhesionEnergyDensitydLocalDeformationGradient( );

            const floatVector* getdSurfaceAdhesionEnergyDensitydLocalMicroDeformation( );

            const floatVector* getdSurfaceAdhesionEnergyDensitydGradientMicroDeformation( );

            const floatType* getLocalReferenceRadius( );

            const floatType* getNonLocalReferenceRadius( );

            const floatType* getLocalParticleReferenceVolume( );

            const floatType* getLocalParticleCurrentVolume( );

            const floatVector* getLocalParticleMicroCauchyStress( );

            const floatVector* getLocalParticleStateVariables( );

            const floatVector* getParticleParameters( );

            const floatVector* getLocalParticleParameters( );

            const floatVector* getLocalReferenceNormal( );

            virtual void getLocalReferenceNormal( const unsigned int &index, floatVector &localReferenceNormal );

            const floatVector* getLocalSurfaceReferenceRelativePositionVector( );

            const floatVector* getNonLocalSurfaceReferenceRelativePositionVector( );

            const floatVector* getDeformationGradient( );

            const floatVector* getPreviousDeformationGradient( );

            const floatVector* getLocalDeformationGradient( );

            const floatVector* getPreviousLocalDeformationGradient( );

            const floatVector* getLocalMicroDeformation( );

            const floatVector* getPreviousLocalMicroDeformation( );

            const floatVector* getLocalReferenceParticleSpacingVector( );

            const floatVector* getMicroDeformation( );

            const floatVector* getPreviousMicroDeformation( );

            const floatVector* getGradientMicroDeformation( );

            const floatVector* getNonLocalMicroDeformationBase( );

            const floatVector* getNonLocalMicroDeformation( );

            const floatVector* getLocalGradientMicroDeformation( );

            const floatVector* getCurrentDistanceVector( );

            const floatVector* getLocalCurrentNormal( );

            virtual void getLocalCurrentNormal( const unsigned int &index, floatVector &normal );

            const floatVector* getSurfaceParameters( );

            const floatVector* getSurfaceOverlapParameters( );

            const floatVector* getReferenceDistanceVector( );

            const floatVector* getUnitSpherePoints( );

            const floatVector* getLocalReferenceSurfacePoints( );

            const floatVector* getNonLocalReferenceSurfacePoints( );

            const floatVector* getLocalCurrentSurfacePoints( );

            const floatVector* getNonLocalCurrentSurfacePoints( );

            const floatVector* getSurfaceAdhesionTraction( );

            const floatVector* getPreviousStateVariables( );

            const floatVector* getPreviousLocalStateVariables( );

            const std::unordered_map< unsigned int, floatVector>* getSurfaceOverlapTraction( );

            const floatMatrix* getLocalParticleCurrentBoundingBox( );

            const floatMatrix* getNonLocalParticleCurrentBoundingBox( );

            const mapFloatType* getSurfaceOverlapEnergyDensity( );

            const mapFloatVector* getParticlePairOverlap( );

            const std::vector< unsigned int >* getUnitSphereConnectivity( );

            const floatVector* getAssembledLocalParticleEnergies( );

            const floatMatrix* getAssembledLocalParticleMicroCauchyStresses( );

            const floatVector* getAssembledLocalParticleVolumes( );

            const floatVector* getAssembledLocalParticleLogProbabilityRatios( );

            const unsigned int* getLocalIndex( ){
                /*!
                 * Get the local particle index
                 */

                return &_localIndex;

            }

            const unsigned int* getNonLocalIndex( ){
                /*!
                 * Get the non-local particle index
                 */

                return &_nonLocalIndex;

            }

            const unsigned int* getLocalSurfaceNodeIndex( ){
                /*!
                 * Get the local surface node index
                 */

                return &_localSurfaceNodeIndex;

            }

            const floatType* getSurfaceAdhesionThickness( );

            const mapFloatType* getSurfaceOverlapThickness( );

            const std::vector< std::vector< std::vector< floatType > > >* getAssembledSurfaceAdhesionThicknesses( );

            const std::vector< std::vector< std::vector< floatType > > >* getAssembledSurfaceAdhesionEnergyDensities( );

            const std::vector< std::vector< std::vector< floatVector > > >* getAssembledSurfaceAdhesionTractions( );

            const std::vector< std::vector< std::vector< mapFloatType > > >* getAssembledSurfaceOverlapThicknesses( );

            const std::vector< std::vector< std::vector< mapFloatType > > >* getAssembledSurfaceOverlapEnergyDensities( );

            const std::vector< std::vector< std::vector< mapFloatVector > > >* getAssembledSurfaceOverlapTractions( );

            const floatMatrix* getdNonLocalMicroDeformationdLocalReferenceRelativePositionVector( );

            const floatMatrix* getdNonLocalMicroDeformationdNonLocalReferenceRelativePositionVector( );

            const floatMatrix* getdNonLocalMicroDeformationdLocalReferenceDistanceVector( );

            const floatMatrix* getdNonLocalMicroDeformationdNonLocalMicroDeformationBase( );

            const floatMatrix* getdNonLocalMicroDeformationdGradientMicroDeformation( );

            const floatMatrix* getdCurrentDistanceVectordLocalReferenceRelativePositionVector( );

            const floatMatrix* getdCurrentDistanceVectordNonLocalReferenceRelativePositionVector( );

            const floatMatrix* getdCurrentDistanceVectordLocalReferenceDistanceVector( );

            const floatMatrix* getdCurrentDistanceVectordLocalDeformationGradient( );

            const floatMatrix* getdCurrentDistanceVectordLocalMicroDeformation( );

            const floatMatrix* getdCurrentDistanceVectordNonLocalMicroDeformationBase( );

            const floatMatrix* getdCurrentDistanceVectordGradientMicroDeformation( );

            const floatMatrix* getd2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordLocalReferenceRelativePositionVector( );

            const floatMatrix* getd2CurrentDistanceVectordLocalDeformationGradientdLocalReferenceRelativePositionVector( );

            const floatMatrix* getd2CurrentDistanceVectordLocalMicroDeformationdLocalReferenceRelativePositionVector( );

            const floatMatrix* getd2CurrentDistanceVectordGradientMicroDeformationdLocalReferenceRelativePositionVector( );

            const floatMatrix* getd2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordLocalReferenceDistanceVector( );

            const floatMatrix* getd2CurrentDistanceVectordLocalDeformationGradientdLocalReferenceDistanceVector( );

            const floatMatrix* getd2CurrentDistanceVectordGradientMicroDeformationdLocalReferenceDistanceVector( );

            const floatMatrix* getd2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordNonLocalReferenceRelativePositionVector( );

            const floatMatrix* getd2CurrentDistanceVectordLocalDeformationGradientdNonLocalReferenceRelativePositionVector( );

            const floatMatrix* getd2CurrentDistanceVectordNonLocalMicroDeformationBasedNonLocalReferenceRelativePositionVector( );

            const floatMatrix* getd2CurrentDistanceVectordGradientMicroDeformationdNonLocalReferenceRelativePositionVector( );

            const floatMatrix* getdLocalCurrentNormaldLocalReferenceNormal( );

            const floatMatrix* getdLocalCurrentNormaldLocalMicroDeformation( );

            //! Get the value of the absolute tolerance
            const floatType* getAbsoluteTolerance( ){ return &_absoluteTolerance; }

            //! Get the value of the relative tolerance
            const floatType* getRelativeTolerance( ){ return &_relativeTolerance; }

            const floatMatrix* getd2NonLocalMicroDeformationdLocalReferenceRelativePositionVectordGradientMicroDeformation( );

            const floatMatrix* getd2NonLocalMicroDeformationdNonLocalReferenceRelativePositionVectordGradientMicroDeformation( );

            const floatMatrix* getd2NonLocalMicroDeformationdLocalReferenceDistanceVectordGradientMicroDeformation( );

            // Add functions
            void addLocalParticleData( dataBase *data ){
                /*!
                 * Add the pointer to the data object to the local-particle data garbage collection vector
                 * 
                 * \param *data: The pointer to the object to be cleaned when the local particle index changes
                 */

                _localParticleData.push_back( data );

            }

            void addSurfacePointData( dataBase *data ){
                /*!
                 * Add the pointer to the data object to the surface point data garbage collection vector
                 * 
                 * \param *data: The pointer to the object to be cleaned when the surface point's index changes
                 */

                _surfacePointData.push_back( data );

            }

            void addInteractionPairData( dataBase *data ){
                /*!
                 * Add the pointer to the data object to the interaction pair data garbage collection vector
                 * 
                 * \param *data: The pointer to the object to be cleaned when the interaction pair's index changes
                 */

                _interactionPairData.push_back( data );

            }

        protected:

            // Protected parameters
            unsigned int _dimension = 3; //!< The spatial dimension

            unsigned int _surfaceElementCount = 1; //!< The base number of surface elements. Will result in the unit sphere having eight elements.

            bool pointInBoundingBox( const floatVector &point, const floatMatrix &boundingBox );

            void formBoundingBox( const floatVector &points, floatMatrix &boundingBox );

            void idBoundingBoxContainedPoints( const floatVector &points, const floatMatrix &boundingBox, std::vector< unsigned int > &containedPoints );

            // Setter functions
            void setLocalReferenceRadius( const floatType &value );

            void setNonLocalReferenceRadius( const floatType &value );

            void setLocalReferenceNormal( const floatVector &value );

            void setLocalSurfaceReferenceRelativePositionVector( const floatVector &value );

            void setNonLocalSurfaceReferenceRelativePositionVector( const floatVector &value );

            void setReferenceDistanceVector( const floatVector &value );

            void setLocalDeformationGradient( const floatVector &value );

            void setPreviousLocalDeformationGradient( const floatVector &value );

            void setLocalMicroDeformation( const floatVector &value );

            void setPreviousLocalMicroDeformation( const floatVector &value );

            void setNonLocalMicroDeformation( const floatVector &value );

            void setNonLocalMicroDeformationBase( const floatVector &value );

            void setLocalGradientMicroDeformation( const floatVector &value );

            void setLocalCurrentNormal( const floatVector &value );

            void setLocalReferenceParticleSpacingVector( const floatVector &value );

            void setCurrentDistanceVector( const floatVector &value );

            void setSurfaceParameters( const floatVector &value );

            void setSurfaceOverlapParameters( const floatVector &value );

            void setLocalParticleEnergyDensity( const floatType &value );

            void setLocalParticleMicroCauchyStress( const floatVector &value );

            void setLocalParticleStateVariables( const floatVector &value );

            void setLocalParticleLogProbabilityRatio( const floatType &value );

            void setLocalParticleEnergy( const floatType &value );

            void setSurfaceAdhesionEnergyDensity( const floatType &value );

            void setdSurfaceAdhesionEnergyDensitydLocalDeformationGradient( const floatVector &value );

            void setdSurfaceAdhesionEnergyDensitydLocalMicroDeformation( const floatVector &value );

            void setdSurfaceAdhesionEnergyDensitydGradientMicroDeformation( const floatVector &value );

            void setSurfaceOverlapEnergyDensity( const mapFloatType &value );

            void setSurfaceAdhesionTraction( const floatVector &value );

            void setSurfaceOverlapTraction( const mapFloatVector &value );

            void setLocalParticleCurrentBoundingBox( const floatMatrix &value );

            void setNonLocalParticleCurrentBoundingBox( const floatMatrix &value );

            void setLocalReferenceSurfacePoints( const floatVector &value );

            void setNonLocalReferenceSurfacePoints( const floatVector &value );

            void setLocalCurrentSurfacePoints( const floatVector &value );

            void setNonLocalCurrentSurfacePoints( const floatVector &value );

            void setParticlePairOverlap( const mapFloatVector &value );

            void setLocalParticleReferenceVolume( const floatType &value );

            void setLocalParticleCurrentVolume( const floatType &value );

            void setLocalParticleParameters( const floatVector &value );

            void setSurfaceAdhesionThickness( const floatType &value );

            void setSurfaceOverlapThickness( const mapFloatType &value );

            void setdNonLocalMicroDeformationdLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setdNonLocalMicroDeformationdNonLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setdNonLocalMicroDeformationdLocalReferenceDistanceVector( const floatMatrix &value );

            void setdNonLocalMicroDeformationdNonLocalMicroDeformationBase( const floatMatrix &value );

            void setdNonLocalMicroDeformationdGradientMicroDeformation( const floatMatrix &value );

            void setd2NonLocalMicroDeformationdLocalReferenceRelativePositionVectordGradientMicroDeformation( const floatMatrix &value );

            void setd2NonLocalMicroDeformationdNonLocalReferenceRelativePositionVectordGradientMicroDeformation( const floatMatrix &value );

            void setd2NonLocalMicroDeformationdLocalReferenceDistanceVectordGradientMicroDeformation( const floatMatrix &value );

            void setdCurrentDistanceVectordLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setdCurrentDistanceVectordNonLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setdCurrentDistanceVectordLocalReferenceDistanceVector( const floatMatrix &value );

            void setdCurrentDistanceVectordLocalDeformationGradient( const floatMatrix &value );

            void setdCurrentDistanceVectordLocalMicroDeformation( const floatMatrix &value );

            void setdCurrentDistanceVectordNonLocalMicroDeformationBase( const floatMatrix &value );

            void setdCurrentDistanceVectordGradientMicroDeformation( const floatMatrix &value );

            void setd2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setd2CurrentDistanceVectordLocalDeformationGradientdLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setd2CurrentDistanceVectordLocalMicroDeformationdLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setd2CurrentDistanceVectordGradientMicroDeformationdLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setd2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordLocalReferenceDistanceVector( const floatMatrix &value );

            void setd2CurrentDistanceVectordLocalDeformationGradientdLocalReferenceDistanceVector( const floatMatrix &value );

            void setd2CurrentDistanceVectordGradientMicroDeformationdLocalReferenceDistanceVector( const floatMatrix &value );

            void setd2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordNonLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setd2CurrentDistanceVectordLocalDeformationGradientdNonLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setd2CurrentDistanceVectordNonLocalMicroDeformationBasedNonLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setd2CurrentDistanceVectordGradientMicroDeformationdNonLocalReferenceRelativePositionVector( const floatMatrix &value );

            void setdLocalCurrentNormaldLocalReferenceNormal( const floatMatrix &value );

            void setdLocalCurrentNormaldLocalMicroDeformation( const floatMatrix &value );

        private:
            // Friend classes
            friend class unit_test::aspBaseTester;

            // Private parameters
            unsigned int _localIndex = 0;

            unsigned int _nonLocalIndex = 0;

            unsigned int _localSurfaceNodeIndex = 0;

            unsigned int _numLocalParticles = 1;

            floatType   _absoluteTolerance = 1e-9;

            floatType   _relativeTolerance = 1e-9;

            floatType   _radius;

            floatType   _previousTime;

            floatType   _deltaTime;

            floatType   _temperature;

            floatType   _previousTemperature;

            floatVector _previousDeformationGradient;

            floatVector _previousMicroDeformation;

            floatVector _previousGradientMicroDeformation;

            floatVector _previousStateVariables;

            floatVector _deformationGradient;

            floatVector _microDeformation;

            floatVector _gradientMicroDeformation;

            floatVector _particleParameters;

            dataStorage< floatMatrix > _localParticleCurrentBoundingBox;

            dataStorage< floatVector > _localReferenceSurfacePoints;

            dataStorage< floatVector > _localCurrentSurfacePoints;

            // ALL OF THESE MUST BE CLEARED AFTER EACH SURFACE INTEGRAND CALCULATION
            dataStorage< floatType > _localReferenceRadius;

            dataStorage< floatType > _nonLocalReferenceRadius;

            dataStorage< floatType > _localParticleEnergyDensity;

            dataStorage< floatType > _localParticleLogProbabilityRatio;

            dataStorage< std::vector< unsigned int > > _unitSphereConnectivity;

            dataStorage< floatVector > _unitSpherePoints;

            dataStorage< floatVector > _localReferenceNormal;

            dataStorage< floatVector > _localSurfaceReferenceRelativePositionVector;

            dataStorage< floatVector > _nonLocalSurfaceReferenceRelativePositionVector;

            dataStorage< floatVector > _referenceDistanceVector;

            dataStorage< floatVector > _localReferenceParticleSpacing;

            dataStorage< floatVector > _localDeformationGradient;

            dataStorage< floatVector > _previousLocalDeformationGradient;

            dataStorage< floatVector > _localMicroDeformation;

            dataStorage< floatVector > _previousLocalMicroDeformation;

            dataStorage< floatVector > _nonLocalMicroDeformation;

            dataStorage< floatVector > _nonLocalMicroDeformationBase;

            dataStorage< floatVector > _localGradientMicroDeformation;

            dataStorage< floatVector > _currentDistanceVector;

            dataStorage< floatVector > _localCurrentNormal;

            dataStorage< floatVector > _surfaceParameters;

            dataStorage< floatVector > _surfaceOverlapParameters;

            dataStorage< floatType > _surfaceAdhesionEnergyDensity;

            dataStorage< mapFloatType > _surfaceOverlapEnergyDensity;

            dataStorage< floatVector > _nonLocalReferenceSurfacePoints;

            dataStorage< floatVector > _nonLocalCurrentSurfacePoints;

            dataStorage< floatMatrix > _nonLocalParticleCurrentBoundingBox;

            dataStorage< mapFloatVector > _particlePairOverlap;

            dataStorage< floatVector > _surfaceAdhesionTraction;

            dataStorage< mapFloatVector > _surfaceOverlapTraction;

            dataStorage< floatVector > _allParticleSurfaceAdhesionEnergy;

            dataStorage< floatMatrix > _allParticleSurfaceAdhesionTraction;

            dataStorage< std::vector< mapFloatType > > _allParticleSurfaceOverlapEnergy;

            dataStorage< std::vector< mapFloatVector > > _allParticleSurfaceOverlapTraction;

            dataStorage< floatVector > _allParticleSurfaceConstraintEnergy;

            dataStorage< floatType > _localParticleEnergy;

            dataStorage< floatVector > _localParticleEnergies;

            dataStorage< floatVector > _localParticleMicroCauchyStress;

            dataStorage< floatVector > _localParticleStateVariables;

            dataStorage< floatType > _localParticleReferenceVolume;

            dataStorage< floatType > _localParticleCurrentVolume;

            dataStorage< floatVector > _localParticleParameters;

            dataStorage< floatVector > _assembledLocalParticleEnergies;

            dataStorage< floatMatrix > _assembledLocalParticleMicroCauchyStress;

            dataStorage< floatVector > _assembledLocalParticleVolumes;

            dataStorage< floatVector > _assembledLocalParticleLogProbabilityRatios;

            dataStorage< floatType > _surfaceAdhesionThickness;

            dataStorage< mapFloatType > _surfaceOverlapThickness;

            dataStorage< std::vector< std::vector< std::vector< floatType > > > > _assembledSurfaceAdhesionThicknesses;

            dataStorage< std::vector< std::vector< std::vector< floatType > > > > _assembledSurfaceAdhesionEnergyDensities;

            dataStorage< std::vector< std::vector< std::vector< floatVector > > > > _assembledSurfaceAdhesionTractions;

            dataStorage< std::vector< std::vector< std::vector< mapFloatType > > > > _assembledSurfaceOverlapThicknesses;

            dataStorage< std::vector< std::vector< std::vector< mapFloatType > > > > _assembledSurfaceOverlapEnergyDensities;

            dataStorage< std::vector< std::vector< std::vector< mapFloatVector > > > > _assembledSurfaceOverlapTractions;

            dataStorage< floatMatrix > _dNonLocalMicroDeformationdNonLocalMicroDeformationBase;

            dataStorage< floatMatrix > _dNonLocalMicroDeformationdGradientMicroDeformation;

            dataStorage< floatMatrix > _dNonLocalMicroDeformationdLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _dNonLocalMicroDeformationdLocalReferenceDistanceVector;

            dataStorage< floatMatrix > _dNonLocalMicroDeformationdNonLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _d2NonLocalMicroDeformationdLocalReferenceRelativePositionVectordGradientMicroDeformation;

            dataStorage< floatMatrix > _d2NonLocalMicroDeformationdLocalReferenceDistanceVectordGradientMicroDeformation;

            dataStorage< floatMatrix > _d2NonLocalMicroDeformationdNonLocalReferenceRelativePositionVectordGradientMicroDeformation;

            dataStorage< floatMatrix > _dCurrentDistanceVectordLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _dCurrentDistanceVectordNonLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _dCurrentDistanceVectordLocalReferenceDistanceVector;

            dataStorage< floatMatrix > _dCurrentDistanceVectordLocalDeformationGradient;

            dataStorage< floatMatrix > _dCurrentDistanceVectordLocalMicroDeformation;

            dataStorage< floatMatrix > _dCurrentDistanceVectordNonLocalMicroDeformationBase;

            dataStorage< floatMatrix > _dCurrentDistanceVectordGradientMicroDeformation;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordLocalDeformationGradientdLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordLocalMicroDeformationdLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordGradientMicroDeformationdLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordLocalReferenceDistanceVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordLocalDeformationGradientdLocalReferenceDistanceVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordLocalMicroDeformationdLocalReferenceDistanceVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordGradientMicroDeformationdLocalReferenceDistanceVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordNonLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordLocalDeformationGradientdNonLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordNonLocalMicroDeformationBasedNonLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _d2CurrentDistanceVectordGradientMicroDeformationdNonLocalReferenceRelativePositionVector;

            dataStorage< floatMatrix > _dLocalCurrentNormaldLocalReferenceNormal;

            dataStorage< floatMatrix > _dLocalCurrentNormaldLocalMicroDeformation;

            dataStorage< floatVector > _dSurfaceAdhesionEnergyDensitydLocalDeformationGradient;

            dataStorage< floatVector > _dSurfaceAdhesionEnergyDensitydLocalMicroDeformation;

            dataStorage< floatVector > _dSurfaceAdhesionEnergyDensitydGradientMicroDeformation;

            std::vector< dataBase* > _localParticleData; //! A vector of pointers to quantities required for a local particle

            std::vector< dataBase* > _surfacePointData; //! A vector of pointers to quantities required for a local surface point

            std::vector< dataBase* > _interactionPairData; //! A vector of pointers to quantities required for a particle interaction

            // END OF MEMBERS WHICH MUST BE CLEARED AFTER EACH SURFACE INTEGRAND CALCULATION

            // Private member functions
            virtual void initializeUnitSphere( );

            virtual void setLocalReferenceRadius( );

            virtual void setNonLocalReferenceRadius( );

            virtual void setLocalReferenceNormal( );

            virtual void setLocalSurfaceReferenceRelativePositionVector( );

            virtual void setNonLocalSurfaceReferenceRelativePositionVector( );

            virtual void setReferenceDistanceVector( );

            virtual void setLocalDeformationGradient( );

            virtual void setPreviousLocalDeformationGradient( );

            virtual void setLocalMicroDeformation( );

            virtual void setPreviousLocalMicroDeformation( );

            virtual void setNonLocalMicroDeformation( );

            virtual void setNonLocalMicroDeformationBase( );

            virtual void setLocalGradientMicroDeformation( );

            virtual void setLocalCurrentNormal( );

            virtual void setLocalReferenceParticleSpacingVector( );

            virtual void setCurrentDistanceVector( );

            virtual void setdNonLocalMicroDeformationdLocalReferenceRelativePositionVector( );

            virtual void setdNonLocalMicroDeformationdNonLocalReferenceRelativePositionVector( );

            virtual void setdNonLocalMicroDeformationdLocalReferenceDistanceVector( );

            virtual void setdNonLocalMicroDeformationdNonLocalMicroDeformationBase( );

            virtual void setdNonLocalMicroDeformationdGradientMicroDeformation( );

            virtual void setd2NonLocalMicroDeformationdLocalReferenceRelativePositionVectordGradientMicroDeformation( );

            virtual void setd2NonLocalMicroDeformationdNonLocalReferenceRelativePositionVectordGradientMicroDeformation( );

            virtual void setd2NonLocalMicroDeformationdLocalReferenceDistanceVectordGradientMicroDeformation( );

            virtual void setdCurrentDistanceVectordLocalReferenceRelativePositionVector( );

            virtual void setdCurrentDistanceVectordNonLocalReferenceRelativePositionVector( );

            virtual void setdCurrentDistanceVectordLocalReferenceDistanceVector( );

            virtual void setdCurrentDistanceVectordLocalDeformationGradient( );

            virtual void setdCurrentDistanceVectordLocalMicroDeformation( );

            virtual void setdCurrentDistanceVectordNonLocalMicroDeformationBase( );

            virtual void setdCurrentDistanceVectordGradientMicroDeformation( );

            virtual void setd2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordLocalReferenceRelativePositionVector( );

            virtual void setd2CurrentDistanceVectordLocalDeformationGradientdLocalReferenceRelativePositionVector( );

            virtual void setd2CurrentDistanceVectordLocalMicroDeformationdLocalReferenceRelativePositionVector( );

            virtual void setd2CurrentDistanceVectordGradientMicroDeformationdLocalReferenceRelativePositionVector( );

            virtual void setd2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordLocalReferenceDistanceVector( );

            virtual void setd2CurrentDistanceVectordLocalDeformationGradientdLocalReferenceDistanceVector( );

            virtual void setd2CurrentDistanceVectordGradientMicroDeformationdLocalReferenceDistanceVector( );

            virtual void setd2CurrentDistanceVectordNonLocalReferenceRelativePositionVectordNonLocalReferenceRelativePositionVector( );

            virtual void setd2CurrentDistanceVectordLocalDeformationGradientdNonLocalReferenceRelativePositionVector( );

            virtual void setd2CurrentDistanceVectordNonLocalMicroDeformationBasedNonLocalReferenceRelativePositionVector( );

            virtual void setd2CurrentDistanceVectordGradientMicroDeformationdNonLocalReferenceRelativePositionVector( );

            virtual void setdLocalCurrentNormaldLocalReferenceNormal( );

            virtual void setdLocalCurrentNormaldLocalMicroDeformation( );

            virtual void setSurfaceParameters( );

            virtual void setSurfaceOverlapParameters( );

            virtual void setLocalParticleQuantities( );

            virtual void setLocalParticleEnergy( );

            virtual void setSurfaceAdhesionEnergyDensity( );

            virtual void setdSurfaceAdhesionEnergyDensitydLocalDeformationGradient( );

            virtual void setdSurfaceAdhesionEnergyDensitydLocalMicroDeformation( );

            virtual void setdSurfaceAdhesionEnergyDensitydGradientMicroDeformation( );

            virtual void setSurfaceOverlapEnergyDensity( );

            virtual void setSurfaceAdhesionTraction( );

            virtual void setSurfaceOverlapTraction( );

            virtual void setLocalParticleCurrentBoundingBox( );

            virtual void setNonLocalParticleCurrentBoundingBox( );

            virtual void setLocalReferenceSurfacePoints( );

            virtual void setNonLocalReferenceSurfacePoints( );

            virtual void setLocalCurrentSurfacePoints( );

            virtual void setNonLocalCurrentSurfacePoints( );

            virtual void setParticlePairOverlap( );

            virtual void setLocalParticleReferenceVolume( );

            virtual void setLocalParticleCurrentVolume( );

            virtual void setLocalParticleParameters( );

            virtual void setSurfaceAdhesionThickness( );

            virtual void setSurfaceOverlapThickness( );

            virtual void resetInteractionPairData( );

            virtual void resetSurfacePointData( );

            virtual void resetLocalParticleData( );

            virtual void assembleLocalParticles( );

            virtual void assembleSurfaceResponses( );

    };

}

#endif
