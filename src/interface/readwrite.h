/* readwrite.h and readwrite.cpp regroupent les fonctions de lecture and d'écriture des différents fichiers xml utilisés */

#ifndef READWRITE_H
#define READWRITE_H

#include "all.h"

/* lecture and écriture des différents fichiers xml utilisés */


class XmlTag
{
	public :
		XmlTag();
		XmlTag(const QString& n, XmlTag* const p);
		XmlTag(const QString& n, const QString& c, XmlTag* const p);
		XmlTag(const QString& n, const QString& c, XmlTag* const p, const QList<XmlTag>& e, const QList<std::pair<QString,QString> >& a);
		XmlTag(const XmlTag& xmlTag);
		~XmlTag();

		void addEnfant(const XmlTag& child);
		void addAttribut(const QString& attNom, const QString& attValue);
		const XmlTag& getEnfant(const QString& childName, bool* ok=0) const;
		const XmlTag& getEnfant(const char* childName, bool* ok=0) const;
		const QList<const XmlTag*> getEnfants(const QString& childName, bool* ok=0) const;
		const QList<const XmlTag*> getEnfants(const char* childName, bool* ok=0) const;
		const QString& getAttribut(const QString& attName, bool* ok=0) const;
		const QString& getAttribut(const char* attName, bool* ok=0) const;

		XmlTag& operator=(const XmlTag& xmlTag);

		const QString& getNom() const;
		const QString& getContenu() const;
		XmlTag* const getParent() const;
		const QList<XmlTag>& getEnfants() const;
		QList<XmlTag>& modifEnfants();
		const QList<std::pair<QString,QString> >& getAttributs() const;
		QList<std::pair<QString,QString> >& modifAttributs();
		void setNom(const QString& n);
		void setContenu(const QString& c);
		void setParent(XmlTag* const p);
		void setEnfants(const QList<XmlTag>& e);
		void setAttributs(const QList<std::pair<QString,QString> >& a);

	private :
		void copie(const XmlTag& xmlTag);

		QString nom;
		QString contenu;
		XmlTag* parent;
		QList<XmlTag> enfants;
		QList<std::pair<QString,QString> > attributs;
};


class XmlTree
{
	public :
		XmlTree();
		XmlTree(const QString& f);
		XmlTree(const QString& f, const XmlTag& m, const QList<XmlTag>& o);
		XmlTree(const XmlTree& xmlTree);
		~XmlTree();

		QString lire(bool all=true);
		bool ecrire();

		XmlTree& operator=(const XmlTree& xmlTree);

		const QString& getFichier() const;
		const XmlTag& getMainTag() const;
		XmlTag& modifMainTag();
		const QList<XmlTag>& getOtherTags() const;
		void setFichier(const QString& f);
		void setMainTag(const XmlTag& m);
		void setOtherTags(const QList<XmlTag>& o);

	private :
		void copie(const XmlTree& xmlTree);
		void ecrireListe(QXmlStreamWriter& xmlWriter, const QList<XmlTag>* currentList);

		QString fichier;
		XmlTag mainTag;
		QList<XmlTag> otherTags;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class ParamCalcul
	//enregistrement du computation
{
	public:
		static bool ecrire(const ParamMain& paramMain, const QString& fichier);
		static QString lire(ParamMain& paramMain, const QString& fichier);
};

///////////////////////////////////////////////////////////////////////////////////
//fichiers for Pastis


class BDCamera
	//list des caméras (décomposition du nom des images)
{
	public :
		BDCamera();

		bool ecrire (const QList<std::pair<QString,double> >& imgNames);
		QString lire (QList<std::pair<QString,double> >& imgNames);

	private :
		QString BDfile;
};

class DicoCameraMM
	//list des caméras (décomposition du nom des images)
{
	public :
		DicoCameraMM(const ParamMain* pMain);

		QString ecrire (const CalibCam& calibCam, const QString& img);

	private :
		QString dicoFile;
		const ParamMain* paramMain;
};

class FichierCalibCam
	//écriture des paramètres de calibration interne
{
	public :
		FichierCalibCam();

                static bool ecrire (const QString& dossier, const CalibCam& calibCam);
                static QString lire (const QString& dossier, const QString& fichier, CalibCam& calibCam, int focalemm=0);
};

class FichierParamImage
	//list des images
{
	public:
		FichierParamImage();
		static bool ecrire(const QString& fichier, const QVector<ParamImage>& lstImg);
		static QString lire(const QString& fichier, QVector<ParamImage>& lstImg);
};

class FichierCouples
	//écriture des couples d'images
{
	public :
		FichierCouples();

		static bool ecrire (const QString& fichier, const QList<std::pair<QString, QString> >& couples, const QVector<ParamImage>& rawToTif=QVector<ParamImage>());
		static QString convertir (const QString& fichier, const QVector<ParamImage>& rawToTif);
		static QString lire (const QString& fichier, QList<std::pair<QString, QString> >& couples, const QVector<ParamImage>& tifToRaw, bool raw=true);

	private :
		static QString traduire(const QString& image, const QVector<ParamImage>& rawToTif, bool toTif);
};

class FichierAssocCalib
	//écriture des associations image - calibration interne
{
	public :
		FichierAssocCalib();

		static bool ecrire (const QString& fichier, const QVector<ParamImage>& assoc);
		static QString lire (const QString& fichier, QVector<ParamImage>& assoc);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//fichiers for Apero


class FichierFiltrage
	//écriture des associations image - calibration interne
{
	public :
		FichierFiltrage();

		static bool ecrire (QString fichier, QString dossier, QSize imgSize);
		static void renameFiles (QString dossier);
	private :
		static QString extensionSortie;
};

class FichierMaitresse
	//écriture des associations image - calibration interne
{
	public :
		FichierMaitresse();

		static bool ecrire (QString fichier, QString maitresse, QString calibFile, bool sommetsGPS);
		static QString lire (QString fichier, QString & maitresse);
};

class FichierExportPly
	//export des points homologues 3D and des caméras in un file ply
{
	public :
		FichierExportPly();

		static bool ecrire (QString fichier);
};

class FichierImgToOri
	//list des images à orienter
{
	public :
		FichierImgToOri();

		static bool ecrire (const QString& fichier, const QStringList& imgOri, const QVector<ParamImage>& assoc, const QString& maitresse, const QList<std::pair<QString, int> >& calibFiles, bool withGPSSummit=false, bool monoechelle=true, int etape1=0, const QList<int>& posesFigees=QList<int>());
		static bool ecrire (const QString& fichier, const QStringList& imgOri, const QString& maitresse, const QString& calibFile, int focale);
		static QString lire (const QString& fichier, QStringList& imgOri, const QVector<ParamImage>& assoc);
};

class FichierDefCalibration
	//définition des calibration
{
	public :
		FichierDefCalibration();

		static bool ecrire (const QString& fichier, const QList<std::pair<QString, int> >& calibFiles, bool monoechelle = true, int etape=0, const QList<int>& calibFigees=QList<int>());
		static bool ecrire (const QString& fichier, const QList<std::pair<QString, int> >& calibFiles, const QVector<bool>& calibDissoc);
};

class FichierCleCalib
	//clés des calibrations (PatterNameApply)
{
	public :
		FichierCleCalib();
		static bool ecrire (const QString& fichier, const QList<int>& calib);
};

class FichierContraintes
	//contraintes inconnues initiales
{
	public :
		FichierContraintes();
		static bool ecrire (const QString& fichier, bool classique, bool fisheye);
};

//fichiers for le géoréferencement///////////////////////////////////////////////

class FichierBasculOri
	//basculement de l'orientation absolue
{
	public :
		FichierBasculOri();

		static bool ecrire (const QString& fichier, const UserOrientation& param, const QStringList& images=QStringList());
		static QString lire (const QString& fichier, UserOrientation& param, const ParamMain& paramMain);
};

class FichierAppuiGPS
	//file de points d'appui terrain
{
	public :
		FichierAppuiGPS();

		static bool convert (const QString& fichierold, const QString& fichiernew);
		static bool format(const QString& fichier, bool& texte);
		static QString lire(const QString& fichier, QList<QString>& points);
		static QString lireFichierTexte(QFile& fichier, QList<QString>& points, QList<QVector<double> >& coordonnees);
};

class FichierAppuiImage
	//file de points d'appui with leurs coordonnées in les images
{
	public :
		FichierAppuiImage();

		static bool convert (const QString& fichierold, const QString& fichiernew);
		static bool format(const QString& fichier, bool& texte);
		static QString lire(const QString& fichier, const ParamMain* paramMain, const QList<QString>& points, QVector<QVector<QPoint> >& pointsAppui);
		static QString lireFichierTexte(QFile& fichier, const ParamMain* paramMain, const QList<QString>& points, QVector<QVector<QPoint> >& pointsAppui);
		static bool ecrire(const QString& fichier, const ParamMain* paramMain, const QList<QString>& points, const QVector<QVector<QPoint> >& pointsAppui);
};

class FichierObsGPS
	//file pointant vers le file des mesures image des points GPS
{
	public :
		FichierObsGPS();

		static bool ecrire (const QString& fichier, const UserOrientation& param);
};

class FichierIncGPS
	//file pointant vers le file des points GPS
{
	public :
		FichierIncGPS();

		static bool ecrire (const QString& fichier, const UserOrientation& param);
};

class FichierPondGPS
	//file for la pondération des points GPS
{
	public :
		FichierPondGPS();

		static bool ecrire (const QString& fichier, int orientMethode);
};

class FichierSommetsGPS
	//file de coordonnées GPS de sommets
{
	public :
		FichierSommetsGPS();

		static bool convert (const QString& fichierold, const ParamMain& paramMain, QString& resultDir, QStringList& images);
};

class FichierOriInit
	//file pointant vers le file des mesures image des points GPS
{
	public :
		FichierOriInit();

		static bool ecrire (const QString& fichier);
};

//fichiers for le multi-échelle///////////////////////////////////////////////
class FichierPosesFigees
	//étape 2 : écriture des poses figées (courtes focales)
	//étape 3 : écriture des poses non dissociées
{
	public :
		FichierPosesFigees();
		static bool ecrire (const QString& fichier, const QList<int>& calibFigees, const QList<std::pair<QString, int> >& calibFiles, const QVector<ParamImage>& imgs, const QStringList& imgOri, const QString& maitresse, bool figees);
		static bool ecrire (const QString& fichier, const QVector<bool>& calibDissoc, const QList<std::pair<QString, int> >& calibFiles, const QVector<ParamImage>& imgs, const QStringList& imgOri, const QString& maitresse);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////
//fichiers for Micmac


class FichierCartes
	//list des images utilisées for créer les cartes de profondeur
{
	public :
		FichierCartes ();
		static QString lire (const QString& fichier, CarteDeProfondeur& carte);
		static bool ecrire(const QString& fichier, const CarteDeProfondeur& carte);
};

class FichierMasque
	//file xml de référencement du masque saisi for MicMac
{
	public :
		FichierMasque ();
		static bool ecrire(const QString& fichier, const ParamMasqueXml& paramMasqueXml);
		static QString lire (const QString& fichier, ParamMasqueXml& paramMasqueXml);
};

class FichierDefMasque
	//définition du masque and de son file de référencement
{
	public :
		FichierDefMasque ();
		static bool ecrire(QString dossier, QString fichier, QString masque, QString ref);
		static QString lire (QString dossier, QString fichier, QString & masque, QString & refmasque);
};

class FichierRepere
	//repère de corrélation
{
	public :
		FichierRepere ();
		static bool ecrire(const QString& fichier, double profondeur);	//repère par défaut, else Bascule est utilisé
		static QString lire (const QString& fichier);
};

class FichierIntervalle
	//intervalle de recherche for la corrélation
{
	public :
		FichierIntervalle ();
		static bool ecrire(const QString& fichier, float pmin, float pmax);	//for le TA, voir MicmacThread
};

class FichierDiscontinuites
	//filtrage des discontinuités and fortes pentes
{
	public :
		FichierDiscontinuites ();
		static bool ecrire(const QString& fichier, float seuil, float coeff);	//for le TA, voir MicmacThread
};

class FichierNomCarte
	//définit le nom des cartes de profondeur
{
	public :
		FichierNomCarte ();
		static bool ecrire(const QString& fichier, const QString& numCarte, bool TA, bool repereImg);	//for le TA, voir MicmacThread
};

class FichierOrtho
	////paramètres des orthoimages
{
	public :
		FichierOrtho ();
		static bool ecrire(const QString& fichier, const CarteDeProfondeur& carte, const ParamMain& paramMain);
};

///////////////////////////////////////////////////////////////////////////////////////////
//fichiers for Porto

class FichierPorto
	//file xml de référencement du masque saisi for MicMac
{
	public :
		FichierPorto ();
		static bool ecrire(const QString& fichier, const QString& num);
};

///////////////////////////////////////////////////////////////////////////////////////////

class FichierGeorefMNT
//file de géoréférencement des cartes, MNT, TA and orthoimages
{
	public :
		FichierGeorefMNT ();
		static QString lire(GeorefMNT& georefMNT);
};

///////////////////////////////////////////////////////////////////////////////////////////

/*std::ostream& operator<<(std::ostream& outstream, const QString& msg) {
	//outstream << msg.toStdString().c_str();
	return outstream;
}*/

struct MetaDataRaw {
	QString camera;
	float focale;
	float feq35;
	QSize imgSize;
};
QString focaleRaw(const QString& image, const ParamMain& paramMain, MetaDataRaw& metaDataRaw);	//métadonnées d'une image raw ; renvoie false if pb
bool focaleTif(const QString& image, const QString& micmacDir, int* focale, QSize* taille);	//focale en mm d'une image tif ; renvoie 0 if pb
// get focal and image size for non-TIFF, non-raw images
// returns if values have been found
bool focaleOther( const string &i_directory, const string &i_image, int &o_focal, QSize &o_size );
bool createEmptyFile(const QString& fichier);	//crée un file vide

#endif
