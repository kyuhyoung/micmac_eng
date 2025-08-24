/*
		Methode des gradients conjugues

*/
#define VECT_IN_LINE
#include "vect.h"

class Grad_Conj {
public:
	double echelle;		//	dimension en dessous de laquelle on s'attend a un comportement quadratique
	int Dim;			//	Dimension du Pb
	Vect 		Sol		//	Solution courante
				, GradC	//	Gradient Conjugue
				, Grad;	//	Gradient courant
	double	modifmax,	//	Modification maximale apportee aux parameters
			potsol;		//	Potentiel de la solution courante
	int		iterations;	//	number d'iterations effectuees
	Grad_Conj( int d, double e ) : echelle( e ), Dim(d), Sol(d), GradC(d), Grad(d) {}
	virtual ~Grad_Conj(){}
//		function fournie par la librairie : minimisation
	void Minimize();
	void Descente();
	void CalcDer(double &G, double &C, double &gm );
//	Fonctions a fournir par le client :
//	Initialisation de Sol
	virtual void Init() = 0;
//	computation du gradient en une solution possible, retourne la value du potentiel en Sol
	virtual double Gradient() = 0;

//	compute le potentiel en Sol
	virtual double Potent( const Vect &t) = 0;

//	Communication du result courant au prog principal.
//	if celui-ci decide d'arreter la function retourne 1, else 0.
	virtual int Controle() = 0;
};


