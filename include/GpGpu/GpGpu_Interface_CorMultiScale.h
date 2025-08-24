#ifndef GPGPU_INTERFACE_CORMULTISCALE_H
#define GPGPU_INTERFACE_CORMULTISCALE_H
/** @addtogroup GpGpuDoc */
/*@{*/

#include "GpGpu/GpGpu.h"
#include "GpGpu/GpGpu_Data.h"
#include "GpGpu/GpGpu_MultiThreadingCpu.h"
#include <string>


/// \cond

#define SIZEWIN(rayonWin) (rayonWin*2+1)*(rayonWin*2+1)
#define NBSCALE 3
#define NBEPIIMAGE 2

struct dataCorrelMS;
struct const_Param_Cor_MS;

extern "C" textureReference&  texture_ImageEpi(int nEpi);
extern "C" textureReference* pTexture_ImageEpi(int nEpi);
extern "C" textureReference* ptexture_Masq_Erod(int nEpi);
extern "C" void LaunchKernelCorrelationMultiScalePreview(dataCorrelMS &data,const_Param_Cor_MS &param);
extern "C" void paramCorMultiScale2Device( const_Param_Cor_MS &param );
extern "C" void LaunchKernel__Correlation_MultiScale(dataCorrelMS &data, const_Param_Cor_MS &parCMS);
/// \endcond


///
/// \brief The const_Param_Cor_MS struct
///
struct const_Param_Cor_MS
{
    //constantParameterCorMultiScale():_NBScale(NBSCALE){}

    ///
    /// \brief aVV
    /// parcours des vignettes
    short2  aVV[NBSCALE][SIZEWIN(NBSCALE)];

    ///
    /// \brief size_aVV
    /// taille des vignettes
    ushort  size_aVV[NBSCALE];

    ///
    /// \brief aVPds
    /// poid des vignettes
    float   aVPds[NBSCALE];

    ///
    /// \brief anOff0
    /// offset terrain image epipolaire 0
    int2    anOff0;

    ///
    /// \brief anOff1
    /// offset terrain image epipolaire 1
    int2    anOff1;

	///
	/// \brief mSIg0
	/// Taille de l'image 0
	uint2	mSIg0;

	///
	/// \brief mSIg1
	/// Taille de l'image 1
	uint2	mSIg1;

	///
	/// \brief _zoneTerrain
	/// Rectangle de la zone du terrain
    Rect    _zoneTerrain;

	///
	/// \brief _dimTerrain
	/// taille du terrain
    uint2   _dimTerrain;

	///	number d'scale for un image
    ushort  aNbScale;

	///
	/// \brief anEpsilon
	/// Espilon
    float   anEpsilon;

	///
	/// \brief mAhDefCost
	/// Cout intrinsque par dfaut
    float   mAhDefCost;

	///
	/// \brief maxDeltaZ
	/// Delta Z maximum de la nappe
    uint    maxDeltaZ;

	///
	/// \brief aSeuilHC option
	///
    float   aSeuilHC;

	///
	/// \brief aSeuilBC option
	///
    float   aSeuilBC;

	///
	/// \brief aModeMax
	/// Option mode maximum
    bool    aModeMax;

	///
	/// \brief DoMixte
	/// Option mode mixte
    bool    DoMixte;

	///
	/// \brief mDyRegGpu
	/// Indique if la rgularisation est aussi compute par gpugpu
	bool	mDyRegGpu;

    ///
    /// \brief mNbByPix
    /// number de phase par pixel
    ushort  mNbByPix;

    ///
    /// \brief aStepPix
    /// Pas sub-pixelaire
    float   aStepPix;

    ///
    /// \brief mDim3Cache
    /// dimension du cache preparatoire au computation de correlation multi-scale
    uint3   mDim3Cache;

	///
	/// \brief init Initialidation des paramtres
	/// \param aVV
	/// \param aVPds
	/// \param offset0
	/// \param offset1
	/// \param sIg0
	/// \param sIg1
	/// \param NbByPix
	/// \param StepPix
	/// \param nEpsilon
	/// \param AhDefCost
	/// \param aSeuilHC
	/// \param aSeuilBC
	/// \param aModeMax
	/// \param DoMixte
	/// \param dynRegulGpu
	/// \param nbscale
	///
	void init(const std::vector<std::vector<Pt2di> >  &aVV,
			const std::vector<double >              &aVPds,
			int2    offset0,
			int2    offset1,
			uint2	sIg0,
			uint2	sIg1,
			ushort  NbByPix,
			float   StepPix,
			float   nEpsilon,
			float   AhDefCost,
			float   aSeuilHC,
			float   aSeuilBC,
			bool    aModeMax,
			bool    DoMixte,
			bool	dynRegulGpu,
			//InterfOptimizGpGpu* interOpt,
			ushort  nbscale = NBSCALE );

	///
	/// \brief setTerrain Definir la zone du terrain
	/// \param zoneTerrain
	///
    void setTerrain(Rect    zoneTerrain);

	///
	/// \brief dealloc
	/// Dsallocation de la mmoire
    void dealloc();

};



///
/// \brief The dataCorrelMS struct
///
struct dataCorrelMS
{
    dataCorrelMS();

    ~dataCorrelMS();

    ///
    /// \brief _HostImage
    /// Images epipolaires
    CuHostData3D<float>         _HostImage[NBEPIIMAGE];

    ///
    /// \brief _HostMaskErod
    /// Masque
    CuHostData3D<pixel>         _HostMaskErod[NBEPIIMAGE];


    ///
    /// \brief _uInterval_Z
	///
    CuUnifiedData3D<short2>        _uInterval_Z;

    ///
    /// \brief _uCost
    ///
	CuUnifiedData3D<float>         _uCostf;

	///
	/// \brief _uCost
	///
	CuUnifiedData3D<ushort>         _uCostu;

	///
	/// \brief _uPit
	///
	CuUnifiedData3D<uint>			_uPit;

	///
	/// \brief _uCost
	///
	CuUnifiedData3D<pixel>         _uCostp;


	template<class T>
	///
	/// \brief pDeviceCost
	/// \return Le pointeur on le volume des couts de correlation
	///
	T* pDeviceCost(){return NULL;}

	///
	/// \brief _dt_MaskErod
	/// Texture du masque erode
    ImageGpGpu<pixel,cudaContext>           _dt_MaskErod[NBEPIIMAGE];
	///
	/// \brief _dt_Image
	/// Groupe de textures des images pipolaries
    ImageLayeredGpGpu<float,cudaContext>    _dt_Image[NBEPIIMAGE];

	///
	/// \brief _texImage
	/// Rdrence on les textures d'images
    textureReference*           _texImage[NBEPIIMAGE];

	///
	/// \brief _texMaskErod
	/// Rfrences on les textures des masques rodes
    textureReference*           _texMaskErod[NBEPIIMAGE];

	///
	/// \brief transfertImage Transfert des images de l'hote vers le device
	/// \param sizeImage taille des images
	/// \param dataImage Pointeur on les donnes des images in l'hote
	/// \param id
	///
    void    transfertImage(uint2 sizeImage, float ***dataImage , int id);



	/// \brief transfertMask Transfert des masques de l'hote vers le device
	/// \param dimMask0 taille du masque 0
	/// \param dimMask1 taille du masque 1
	/// \param mImMasqErod_0 Pointeur source des donnes du masque 0
	/// \param mImMasqErod_1 Pointeur source des donnes du masque 1
	///
	void    transfertMask(uint2 dimMask0, uint2 dimMask1, pixel **mImMasqErod_0, pixel **mImMasqErod_1);

	///
	/// \brief transfertNappe Transfert de la nappe
	/// \param mX0Ter Dimension terrain
	/// \param mX1Ter Dimension terrain
	/// \param mY0Ter Dimension terrain
	/// \param mY1Ter Dimension terrain
	/// \param mTabZMin Z Minimum de la nappe
	/// \param mTabZMax Z Maximum de la nappe
	/// \param dynGpu
	///
	void    transfertNappe(int  mX0Ter, int  mX1Ter, int  mY0Ter, int  mY1Ter, short **mTabZMin, short **mTabZMax, bool dynGpu);

	///
	/// \brief syncDeviceData
	/// Synchronisation des donnes between hote and device
    void    syncDeviceData();


	///
	/// \brief dealloc
	/// Dsallocation de la mmoire hote and device
    void    dealloc();

	///
	/// \brief _maxDeltaZ
	/// Delta Z maximum de la nappe
    uint    _maxDeltaZ;

	///
	/// \brief getInterOpt
	/// \return Interface de optimisateur GpGpu
	///
	InterfOptimizGpGpu* getInterOpt();

	///
	/// \brief setInterOpt
	/// \param InterOpt
	///
	void setInterOpt(InterfOptimizGpGpu* InterOpt);

private:
	void unitT__CopyCoordInColor(uint2 sizeImage, float *dest);

	InterfOptimizGpGpu* mInterOpt;
};

///
/// \brief The GpGpu_Interface_Cor_MS class
/// class de gestion des processus for le lancement de computation GPGPU for la corrlation multiscale
class GpGpu_Interface_Cor_MS : public CSimpleJobCpuGpu< bool>
{
public:

    GpGpu_Interface_Cor_MS();
    ~GpGpu_Interface_Cor_MS();

	///
	/// \brief freezeCompute
	/// Suspendre le computation
    virtual void    freezeCompute(){}

	///
	/// \brief Job_Correlation_MultiScale
	/// Lancer une correlation par pair en gomtrie pipolaire
    void            Job_Correlation_MultiScale();

	///
	/// \brief transfertImageAndMask Transfert des images  and des masques
	/// \param sI0
	/// \param sI1
	/// \param dataImg0
	/// \param dataImg1
	/// \param mask0
	/// \param mask1
	///
	void			transfertImageAndMask(
			uint2 sI0,
			uint2 sI1,
			float ***dataImg0,
			float ***dataImg1,
			pixel **mask0,
			pixel **mask1);

	///
	/// \brief init Initialisation des paramtres
	/// \param terrain
	/// \param aVV
	/// \param aVPds
	/// \param offset0
	/// \param offset1
	/// \param sIg0
	/// \param sIg1
	/// \param mTabZMin
	/// \param mTabZMax
	/// \param NbByPix
	/// \param StepPix
	/// \param nEpsilon
	/// \param AhDefCost
	/// \param aSeuilHC
	/// \param aSeuilBC
	/// \param aModeMax
	/// \param DoMixte
	/// \param dynRegulGpu
	/// \param interOpt
	/// \param nbscale
	///
	void init(Rect terrain,
			  const std::vector<std::vector<Pt2di> >  &aVV,
			  const std::vector<double >              &aVPds,
			  int2      offset0,
			  int2      offset1,
			  uint2		sIg0,
			  uint2		sIg1,
			  short   **mTabZMin,
			  short   **mTabZMax,
			  ushort    NbByPix,
			  float     StepPix,
			  float     nEpsilon,
			  float     AhDefCost,
			  float     aSeuilHC,
			  float     aSeuilBC,
			  bool      aModeMax,
			  bool      DoMixte,
			  bool		dynRegulGpu,
			  InterfOptimizGpGpu* interOpt,
			  ushort nbscale = NBSCALE );

	template<class T>
	///
	/// \brief getCost
	/// \param pt Position Terrain
	/// \return Le cout d'une position terrain
	///
	T getCost(uint3 pt);

	template<class T>
	///
	/// \brief getCost
	/// \param pt
	/// \return  Le cout d'une position terrain
	///
	T* getCost(uint2 pt);

	/// Dsallocation de la mmoire
    void dealloc();

	///
	/// \brief param
	/// \return Les paramtres de corrlation
	///
	const_Param_Cor_MS& param(){return _cDataCMS;}

private:

    virtual void    simpleWork(){}

    dataCorrelMS    _dataCMS;

    const_Param_Cor_MS _cDataCMS;


};
/*@}*/
#endif // GPGPU_INTERFACE_CORMULTISCALE_H
