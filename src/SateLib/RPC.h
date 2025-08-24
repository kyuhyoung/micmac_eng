/*Header-MicMac-eLiSe-25/06/2007

MicMac : Multi Image Correspondances par Methodes Automatiques de Correlation
eLiSe  : ELements of an Image Software Environnement

www.micmac.ign.fr


Copyright : Institut Geographique National
Author : Marc Pierrot Deseilligny
Contributors : Gregoire Maillet, Didier Boldo.

[1] M. Pierrot-Deseilligny, N. Paparoditis.
"A multiresolution and optimization-based image matching approach:
An application to surface reconstruction from SPOT5-HRS stereo imagery."
In IAPRS vol XXXVI-1/W41 in ISPRS Workshop On Topographic Mapping From Space
(With Special Emphasis on Small Satellites), Ankara, Turquie, 02-2006.

[2] M. Pierrot-Deseilligny, "MicMac, un lociel de mise en correspondance
d'images, adapte au contexte geograhique" to appears in
Bulletin d'information de l'Institut Geographique National, 2007.

Francais :

MicMac est un logiciel de mise en correspondance d'image adapte
au contexte de recherche en information geographique. Il s'appuie sur
la bibliotheque de manipulation d'image eLiSe. Il est distibue sous la
licences Cecill-B.  Voir en bas de fichier et  http://www.cecill.info.


English :

MicMa cis an open source software specialized in image matching
for research in geographic information. MicMac is built on the
eLiSe image library. MicMac is governed by the  "Cecill-B licence".
See below and http://www.cecill.info.

Header-MicMac-eLiSe-25/06/2007*/

//Important note:
//pt.x is either the column in image space or the longitude in geographic coordinates or the easting  in projected coordinates
//pt.y is either the row    in image space or the latitude  in geographic coordinates or the northing in projected coordinates
#ifndef _CRPC_H_
#define _CRPC_H_

class RPC2D
{
public:

        //Constructors and destructors
        RPC2D(){};
        ~RPC2D(){};

        //Elements of RPC
        std::vector<double> inverse_line_num_coef;
        std::vector<double> inverse_line_den_coef;
        std::vector<double> inverse_samp_num_coef;
        std::vector<double> inverse_samp_den_coef;
        //Offsets and scale for ground space
        double lat_off, lat_scale, long_off, long_scale, height_off, height_scale;
        //Offsets and scale for image space
        double line_off, line_scale, samp_off, samp_scale;

        //Boundaries of RPC validity for image space
        double first_row, first_col, last_row, last_col;
        //Boundaries of RPC validity for geo space
        double first_lon, first_lat, last_lon, last_lat;


        // From geocentric to image
        Pt2dr InverseRPC2DNorm(Pt2dr PgeoNorm)const;
        Pt2dr InverseRPC2D(Pt3dr Pgeo, double aAngle, double aFactor)const;

        //Compute the 2D transfo between geodetic and image lattice points
        void ComputeRPC2D(vector<vector<Pt2dr> > aMatPtsIm, vector<vector<Pt3dr> > aMatPtsGeo, double aHMax, double aHMin);

        //Compute image position of a 3D point
        Pt3dr InversePreRPCNorm(Pt3dr aPtGeoNorm, vector<vector<Pt3dr> > aMatPtsGeo, vector<vector<Pt3dr> > aMatSatPos);

        //Filter points out of the normalized lattice grid
        vector<Pt3dr> filterOutOfBound(vector<Pt3dr> aVectorGeoNorm, vector<vector<Pt3dr> > aMatPtsGeo);

        //Showing Info
        void info()
        {
                std::cout << "RPC2D info:" << std::endl;
                std::cout << "===========================================================" << std::endl;
                std::cout << "long_scale   : " << long_scale << " | long_off   : " << long_off << std::endl;
                std::cout << "lat_scale    : " << lat_scale << " | lat_off    : " << lat_off << std::endl;
                std::cout << "first_lon    : " << first_lon << " | last_lon   : " << last_lon << std::endl;
                std::cout << "first_lat    : " << first_lat << " | last_lat   : " << last_lat << std::endl;
                std::cout << "samp_scale   : " << samp_scale << " | samp_off   : " << samp_off << std::endl;
                std::cout << "line_scale   : " << line_scale << " | line_off   : " << line_off << std::endl;
                std::cout << "first_col    : " << first_col << " | last_col   : " << last_col << std::endl;
                std::cout << "first_row    : " << first_row << " | last_row   : " << last_row << std::endl;
                std::cout << "inverse_samp_num_coef : " << inverse_samp_num_coef << std::endl;
                std::cout << "inverse_samp_den_coef : " << inverse_samp_den_coef << std::endl;
                std::cout << "inverse_line_num_coef : " << inverse_line_num_coef << std::endl;
                std::cout << "inverse_line_den_coef : " << inverse_line_den_coef << std::endl;
                std::cout << "===========================================================" << std::endl;
        }

};

class RPC
{
public:
// MPD :  Declaration forgotten by someone ...
        double ComputeDenomApprox(double ab[20], double aU[20]);
        void ComputeEq(double aB, double aDenomApprox, double aU[20], double(&aEq)[39]);


        //Elements of RPC
        std::vector<double> direct_line_num_coef;
        std::vector<double> direct_line_den_coef;
        std::vector<double> direct_samp_num_coef;
        std::vector<double> direct_samp_den_coef;
        std::vector<double> inverse_line_num_coef;
        std::vector<double> inverse_line_den_coef;
        std::vector<double> inverse_samp_num_coef;
        std::vector<double> inverse_samp_den_coef;
        //Offsets and scale for ground space
        double lat_off, lat_scale, long_off, long_scale, height_off, height_scale;
        //Offsets and scale for image space
        double line_off, line_scale, samp_off, samp_scale;

        //Boundaries of RPC validity for image space
        double first_row, first_col, last_row, last_col;
        //Boundaries of RPC validity for geo space
        double first_lon, first_lat, last_lon, last_lat, first_height, last_height;

        //Errors indicated in DIMAP files
        RPC() :
		first_row(0),
		first_col(0),
		last_row(0),
		last_col(0),
		first_lon(0),
		first_lat(0),
		last_lon(0),
		last_lat(0),
		first_height(0),
		last_height(0),
                indirErrBiasRow(0),
                indirErrBiasCol(0),
                dirErrBiasX(0),
                dirErrBiasY(0),
		IS_DIR_INI(false),
		IS_INV_INI(false),
        IS_UNIT_m(false),
        mRecGrid(50,50,10)
        {
        }
        double indirErrBiasRow;
        double indirErrBiasCol;
        double dirErrBiasX;
        double dirErrBiasY;

        bool IS_DIR_INI;
	    bool IS_INV_INI;
        bool IS_UNIT_m;
    
        Pt3di mRecGrid;

        Pt3dr DirectRPCNorm(Pt3dr)const;
        Pt3dr InverseRPCNorm(Pt3dr)const;
        Pt3dr DirectRPC(Pt3dr)const;
        Pt3dr InverseRPC(Pt3dr, std::vector<double> vRefineCoef=std::vector<double>()) const;

        int RPC2Grid(int nbLayers, double altiMin, double  altiMax, std::string refineCoef, std::string aNameFile, double stepPixel, double stepCarto, std::string targetSyst, std::string inputSyst, bool binaire);

        //Computing cartographic validity zone
        vector<Pt2dr> empriseCarto(vector<Pt2dr> Pgeo, std::string targetSyst, std::string inputSyst)const;

        // Applying the grid affinity from coefficient from vRefineCoef to a point
        Pt3dr ptRefined(Pt3dr Pimg, std::vector<double> vRefineCoef)const;
        ///
        /// \brief dtermination des sommets de la grille en coordonnes image en function du pas (en pixels) puis conversion en coordonnes gographiques
        /// \param ulcSamp colonne du coin suprieur gauche de la grille en coordonnes image
        /// \param ulcLine line du coin suprieur gauche de la grille en coordonnes image
        /// \param stepPixel pas en pixels for la grille en coordonnes image
        /// \param nbSamp number de colonnes de la grille en coordonnes image
        /// \param nbLine number de lignes de la grille en coordonnes image
        /// \param vAltitude vector contenant les altitudes de chaque  layer 
        /// \param vPtCarto vector de structures de points Pt2dr qui contient les sommets de la grille directe (for l'ensemble des  layers )
        /// \param targetSyst systme de projection cible, suivant la nomenclature proj4
        /// \param vRefineCoef vector contenant les six coefficients de l'affinit servant  affiner la grille
        ///
        void createDirectGrid(double ulcSamp, double ulcLine,
                double stepPixel,
                int nbSamp, int  nbLine,
                std::vector<double> const &vAltitude,
                std::vector<Pt2dr> &vPtCarto, std::string targetSyst, std::string inputSyst,
                std::vector<double> vRefineCoef)const;

        ///
        /// \brief computation des sommets de la grille en coordonnes terrain (cartographiques) en function du pas puis conversion en coordonnes gographiques and enfin image
        /// \param ulcX longitude du coin suprieur gauche de la grille en coordonnes cartographiques
        /// \param ulcY latitude du coin suprieur gauche de la grille en coordonnes cartographiques
        /// \param nbrSamp number de colonnes de la grille en coordonnes cartographiques
        /// \param nbrLine number de lignes de la grille en coordonnes cartographiques
        /// \param stepCarto pas en mtres for la grille en coordonnes cartographiques
        /// \param vAltitude vector contenant les altitudes de chaque  layer 
        /// \param vPtImg vector de sommets de la grille inverse (for l'ensemble des  layers )
        /// \param targetSyst systme de projection cible, suivant la nomenclature proj4
        /// \param vRefineCoef vector contenant les six coefficients de l'affinit servant  affiner la grille
        ///
        void createInverseGrid(double ulcX, double ulcY,
                int nbrSamp, int nbrLine,
                double stepCarto,
                std::vector<double> const &vAltitude,
                std::vector<Pt3dr> &vPtImg, std::string targetSyst, std::string inputSyst,
                std::vector<double> vRefineCoef)const;

        ///
        /// \brief creation du file XML and calculs intermediaires
        /// \param nomGrid nom du file Grid en sortie
        /// \param nomImage nom de l'image concerne
        /// \param stepPixel pas en pixels for la grille en coordonnes image
        /// \param stepCarto pas en mtres for la grille en coordonnes cartographiques
        /// \param vAltitude vector contenant les altitudes de chaque  layer 
        /// \param targetSyst systme de projection cible, suivant la nomenclature proj4
        /// \param vRefineCoef vector contenant les six coefficients de l'affinit servant  affiner la grille
        ///
        void createGrid(std::string const &nomGrid, std::string const &nomImage,
                double stepPixel, double stepCarto,
                std::vector<double> vAltitude, std::string targetSyst, std::string inputSyst,
                std::vector<double> vRefineCoef);

        ///
        /// \brief effacement des fichiers relatifs  la creation des grilles ssi le modle n'est pas affin
        /// \param nomGrid nom du file Grid en sortie
        /// \param refine la grille est-elle affine
        ///
        void clearing(std::string const &nomGrid, bool refine)
        {
                if (refine == false)
                {
                        if (ifstream("processing/conv_ptGeo.txt"))       ELISE_fp::RmFile("processing/conv_ptGeo.txt");
                        if (ifstream("processing/conv_ptCarto.txt"))     ELISE_fp::RmFile("processing/conv_ptCarto.txt");
                        if (ifstream("processing/direct_ptGeo.txt"))     ELISE_fp::RmFile("processing/direct_ptGeo.txt");
                        if (ifstream("processing/direct_ptCarto.txt"))   ELISE_fp::RmFile("processing/direct_ptCarto.txt");
                        if (ifstream("processing/inverse_ptGeo.txt"))   ELISE_fp::RmFile("processing/inverse_ptGeo.txt");
                        if (ifstream("processing/inverse_ptCarto.txt")) ELISE_fp::RmFile("processing/inverse_ptCarto.txt");
                        if (ELISE_fp::IsDirectory("processing"))         ELISE_fp::RmDir("processing");
                }
                //effacement de la grille affinee + grilles GRC and binaire
                std::string gridGRC = nomGrid;
                std::string refGridGRC2 = nomGrid;
                refGridGRC2.append("Bin");

                if (ifstream(nomGrid.c_str()))     ELISE_fp::RmFile(nomGrid.c_str());
                if (ifstream(gridGRC.c_str()))     ELISE_fp::RmFile(gridGRC.c_str());
                if (ifstream(refGridGRC2.c_str())) ELISE_fp::RmFile(refGridGRC2.c_str());
        }

        //Showing Info
        void info()
        {
                std::cout << "RPC info:" << std::endl;
                std::cout << "===========================================================" << std::endl;
                std::cout << "long_scale   : " << long_scale << " | long_off   : " << long_off << std::endl;
                std::cout << "lat_scale    : " << lat_scale << " | lat_off    : " << lat_off << std::endl;
                std::cout << "height_scale : " << height_scale << " | height_off : " << height_off << std::endl;
                std::cout << "samp_scale   : " << samp_scale << " | samp_off   : " << samp_off << std::endl;
                std::cout << "line_scale   : " << line_scale << " | line_off   : " << line_off << std::endl;
                std::cout << "first_row    : " << first_row << " | last_row   : " << last_row << std::endl;
                std::cout << "first_col    : " << first_col << " | last_col   : " << last_col << std::endl;
                std::cout << "first_lon    : " << first_lon << " | last_lon   : " << last_lon << std::endl;
				std::cout << "first_lat    : " << first_lat << " | last_lat   : " << last_lat << std::endl;
				std::cout << "first_height : " << first_height << " | last_height   : " << last_height << std::endl;
                std::cout << "direct_samp_num_coef : " << direct_samp_num_coef << std::endl;
                std::cout << "direct_samp_den_coef : " << direct_samp_den_coef << std::endl;
                std::cout << "direct_line_num_coef : " << direct_line_num_coef << std::endl;
                std::cout << "direct_line_den_coef : " << direct_line_den_coef << std::endl;
                std::cout << "inverse_samp_num_coef : " << inverse_samp_num_coef << std::endl;
                std::cout << "inverse_samp_den_coef : " << inverse_samp_den_coef << std::endl;
                std::cout << "inverse_line_num_coef : " << inverse_line_num_coef << std::endl;
                std::cout << "inverse_line_den_coef : " << inverse_line_den_coef << std::endl;
                std::cout << "===========================================================" << std::endl;
        }

        //For Dimap v2
    void ReadDimap(std::string const &filename);
    void WriteAirbusRPC(std::string aFileOut);
	void ReconstructValidity();
	void ReconstructValidity2D();
	void ReconstructValidity3D();
    void UpdateValidity();

        //For DigitalGlobe data
    void ReadRPB(std::string const &filename);
    void ReadXML(std::string const &filename);

	//For IKONOS/CartoSat
	void ReadASCII(std::string const &filename);
    int  ReadASCIIMetaData(std::string const &metafilename, std::string const &filename);
        
	//For Aster

		std::vector<Pt2dr> ASTERPtsIm;
		std::vector<Pt3dr> ASTERPtsECEF;
		std::vector<Pt3dr> ASTERSatPos;
		void AsterMetaDataXML(std::string filename);

	//For all but Dimap
	void InverseToDirectRPC();

    //Construction of RPCs
    vector<vector<Pt3dr> > GenerateNormLineOfSightGrid(int nbLayers, double aHMin, double aHMax);
    // Old version, not to be used ----- vector<Pt3dr> GenerateRandNormGrid(u_int gridSize);
    vector<Pt3dr> GenerateRandNormGrid(const Pt2di &aGridSz);
    vector<Pt3dr> GenerateNormGrid(const Pt3di &aGridSz);
	void GCP2Direct(vector<Pt3dr> aGridGeoNorm, vector<Pt3dr> aGridImNorm);
    void GCP2Inverse(vector<Pt3dr> aGridGeoNorm, vector<Pt3dr> aGridImNorm);
	void ComputeNormFactors(double aHMin, double aHMax);
    void Validity2Dto3D(RPC2D aRPC2D);

	void TestDirectRPCGen();

	void ChSysRPC(const cSystemeCoord &);
    void SetRecGrid();   


private:

    void NormR2(std::vector<Pt3dr> & aPts) const;
    void NormR3(std::vector<Pt3dr> & aPts) const;

    void UnNormR2(std::vector<Pt3dr> & aPts) const;
    void UnNormR3(std::vector<Pt3dr> & aPts) const;
   
    
    void SetNewScaleOffsetR2(const std::vector<Pt3dr> & aGrid);
    void SetNewScaleOffsetR3(const std::vector<Pt3dr> & aGrid);
    void SetNewFirstLastR3(double& aLongMin,
                           double& aLongMax,
                           double& aLatMin,
                           double& aLatMax,
                           double& aHMin,
                           double& aHMax);

    void ReconstructValidityLong();
    void ReconstructValidityLat();
	void ReconstructValidityH();

    void GetGridExtent(const std::vector<Pt3dr> & aGrid,
                             Pt3dr & aExtMin, 
                             Pt3dr & aExtMax,
                             Pt3dr & aSumXYZ ) const;

};
#endif
/*Footer-MicMac-eLiSe-25/06/2007

Ce logiciel est un programme informatique servant   la mise en
correspondances d'images pour la reconstruction du relief.

Ce logiciel est régi par la licence CeCILL-B soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL-B telle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,    l'utilisation,    la modification et/ou au
développement et   la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe 
manipuler et qui le réserve donc   des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités   charger  et  tester  l'adéquation  du
logiciel   leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
  l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder   cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL-B, et que vous en avez accepté les
termes.
Footer-MicMac-eLiSe-25/06/2007*/
