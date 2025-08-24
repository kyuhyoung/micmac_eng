#ifndef CERO_MODELONEPAIRE_H
#define CERO_MODELONEPAIRE_H
#include "cimgeo.h"
#include "../../uti_phgrm/TiepTri/TiepTri.h"

/*Egalisation radiométrique d'Orthoimages (générées par Malt)
 * calcul du modele de correction radiométrique entre Im1 et Im2 tels que I1xy = a1 + b1 * I2xy
 * 
 */

class cERO_ModelOnePaire{
    
public:

    cERO_ModelOnePaire(int argc, char** argv);
    cERO_ModelOnePaire();
    ~cERO_ModelOnePaire();
    // Détermine la taille du dallage;
    Pt2di gridSize(); // retourne une taille x,y en pixel
    void saveIm2D(Im2D<float, double> * aIm,std::string aName); // sauver des images tout le long du process for vérifier que les traitements s'effectuent comme je le souhaite
    void applyRE();

private:
    cInterfChantierNameManipulateur * mICNM;
    cImGeo * mO1, * mO2; // Ortho 1 and Ortho 2
    Im2D_REAL4 mO1clip,mO2clip; // la zone de recouvrement
    Im2D_REAL4 mI1clip,mI2clip; // incidence
    std::string mNameOr1,mNameOr2,mNameOr1AndDir,mNameOr2AndDir,mDir;
    bool mAddCst; // ajout de la constante a in le modèle, default = true
    // les couples d'observation radiométrique sont stoquée in un vector
    std::vector<Pt2dr> mObsDallage;  // observation for une tuille donnée
    std::map<int, Pt2dr> mObsGlob; // observation globale for le modèle d'égalisation radiom à l'échelles globales
    std::map<int, double> mPondGlob; // for stoquer une value de pondération for chacune des observations des couples radiométriques
    Pt2di mSzTuile;
    Box2dr mBoxOverlapTerrain;
    Box2di mBoxOverO1, mBoxOverO2; // box en pixel de la zone de recouvrement between les 2 orthos
    int mNbObs; // number d'observation de couple radiométrique im1 im2 dont l'on souhaite disposer for ajuster le modèle
    int mNbCplRadio; // nb total de couple radiométrique
    bool mDebug;
    c2DLineModel mModelER1,mModelER2;
    int mPoid1,mPoid2;
    std::string mTmpDirEROS,mTmpDirERO;// un directory global for tout les modèles de chaques paires d'images, un local for la paire d'ortho
    bool mPondIncid, mAutoDZ;
    int mDeZoom;

    void ransacOnEachTiles();
    void ransacOn2Orthos();
    void L1On2Orthos();
    void saveModels();
    void chgResol();
    void autoDZ(); // determine appropriate level of dezoom

    void boxTer2Tfw(); // sauver une geom terrain comme tfw

    double pond(Pt2dr aPt);
    Pt2dr predQuantile(std::map<int, Pt2dr > *aObsMap, c2DLineModel aMod, int quantile=1, bool Y=false);// on donne une list d'observation and un modèle , renvoie le couple X(quantile n),Prediction(X)
    Pt2dr predQuantile(std::vector<Pt2dr> *aObs, c2DLineModel aMod, int quantile=1, bool Y=false);// on donne une list d'observation and un modèle , renvoie le couple X(quantile n),Prediction(X)

};

#endif // CERO_MODELONEPAIRE_H
