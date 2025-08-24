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

    MicMac is an open source software specialized in image matching
    for research in geographic information. MicMac is built on the
    eLiSe image library. MicMac is governed by the  "Cecill-B licence".
    See below and http://www.cecill.info.

Header-MicMac-eLiSe-25/06/2007*/


#include "StdAfx.h"


void init_var();
void PIREMATR()  ;
void ECRICOMP()   ;
void ECRIBOOL() ;
void ECRICHEM();
void ALGOHONGR(Im2D_INT4,Im1D_INT4)  ;
void LECTURE(Im2D_INT4) ;
void ALGO1() ;
void ALGO2()   ;
void ALGO3() ;
void VERIF();
void INIMAT();
void LECMATRI() ;
void ECRIMAT()  ;
void INITECGRA();
void CHEMOPT() ;
void MODIAFFE();
void AFFECOPT1();
void LECAFFPRO(Im1D_INT4)  ;
void GENEAUTO();
void INIREC();
void AUGRECOBL();
void COMPLREC() ;
void RANGNONCOU();
void MINICOL() ;
void INIADD();
void MINIGLOB();
void NOUVZERIND();
void MAJMATRI();
void MAJREC() ;                  


                  

   /*         NOTATIONS :                                                                              */
   /*         ----------                                                                               */
   /*                    Un certain number de caracteres utlises in le document n' existant pas on  */
   /*        clavier utilise for creer ce file il a fallu modifier in les commentaires les       */
   /*        notations correspondantes,ainsi:                                                          */
   /*                       -if E designe un ensemble d'elements d'une matrix A then @[E] designera  */
   /*           l'ensemble des elements de A qui se trouvent a la fois on la meme line and la meme    */  
   /*           colonne que deux elements de E.                                                        */
   /*                       -#[E] designera l'ensemble des elements de A qui se trouvent on la meme   */
   /*           line or la meme colonne qu'un element de E ( "or" non exclusif).                      */
   /*                       -A^B designe l'intersection de A and B.                                     */
   /*                       -A~B designe l'union de A and B.                                            */


 
                                                          



double quelle_heure_est_il()
{
/*
extern int ftime (struct timeb *);
#include <sys/time.h>
#include <sys/timeb.h>
   struct timeb tloc;
   ftime(&tloc);
   return(  ((double)tloc.millitm)/1000.0 +  tloc.time);
*/
   return 0.0;
}


 



static double temps;             /* temps de computation for algo1,algo2,algo3 */
                                       
static int Taille;                                 /*taille effective des matrices*/

static int NbSom;                                  /*number effectif de sommets*/

static int ** matrix;              /*matrix des couts*/

static int ** EcGra;      /*        graphe "d'ecart"; for I appartient a   */
                                   /*  [ 1 , EcGra[0][J] ] :EcGra[I][J] contient les */  
                                   /*  machine telles que matrix[I][J]=0            */   


static int * Chemin;                       /*contient le chemin de S a P*/


static int Existe;                                 /*=1 if existe chemin de S a P */ 


static int * AffTache;                 /* contient eventuellement la machine */
                                       /*    a laquelle est affecte la tache; */
                                       /* =0 if aucune affectation           */                        

static int * AffMach;                  /* contient eventuellement la tache   */
                                       /* a laquelle est affectee la machine; */
                                       /* =0 if aucune affectation           */

static int  *GrInjMax[2];           /* graphe injectif maximal issu de */     
                                    /*            l'affectation simple */ 

static int * RecLigne;           /* contient les colonnes qui participent */
                                     /*     a un recouvrement               */


static int * RecColon;               /* conttient les lignes qui participent */
                                     /*  a un recouvrement                   */

static int AdrG2;                        /* adresse in GrInjMax de du premier */
                                  /*           element de G2             */

static int AdDeltGL;                     /* adresse in RecLigne de la premiere */
                                  /*        line recouvrant Delta-GRAPHE */

static int AdDeltGC;                     /* adresse in RecColon de la premiere */
                                  /*    colonnne recouvrant Delta-GRAPHE  */

static int * MemAdGIM;                 /* memorise les adresses in GrInjMax */
                                  /* des elements qui appartiennent a une */
                                  /* rangee de recouvrement */

static int FinRecObl;                    /* =1 if fin du recouvrement obligatoire */

static int NbProLig;                     /* Nb provisoire de line recouvrantes */
                                  /* (compte tenu des lignes facultatives) */

static int * MinDeCol;            /* contient for Chaque colonne non */
                                  /* couvrante le minimum de ces elements */
                                  /* non couverts */


static int MinGlob;                      /* = minimum de MinDeCol */

static int * AddCol;              /* Quantite a additionner a chaque colonne */
 
static int * AddLig;              /* Quantite a additionner a chaque line   */

static int AddEns;                       /* Quantite a additionner a chaque element */

static int NouvZero;                     /* =1 if le number de zeros independants a augmente */
                                  /* =0 else                                         */

static int NbNvCoOb;                     /* = number de nouvelles colonnes obligatoires */

static int NbIter;                       /* number d'iterations */

static int ** MemMatr;    /* memorise la matrix de depart */

static int MaxAbso;                      /* = Max des elements rencontres au cour de */
                                  /* l'algorithme                             */

static int CompHon1;                     /* compte le number d'appel a Algo1 */

static int CompArEx;                     /* compte le number d'arcs explores */

static int ComElEx2;                     /* compte le number d'elements explores in Algo2 */

static int ComElEx3;                     /* compte le number d'elements explores in Algo3 */


static  int * Affecte;

static  int * AdrPere;
              
static  int * PasEncMa;              /* PasEncMa[I]=1 if la machine I n'a pas encore ete exploree */

static  int * PasEncTa;              /* PasEncTa[J]=1 if la tache J n'a pas encore ete exploree  */
  
static  int * MemAdLig;    /* memorise les adresses in RecLigne des lignes */
                           /* qui ne sont plus obligatoires      */

static  int * TabProvL;

static  int * TabProvC;

static  int * MemAdCol;    /* memorise les adresses in RecColonne des */
                           /* colonnes qui deviennent obligatoires */

static  int * Arbre;           /* contient la list des sommmets explores ,cette list est ordonnee */
                                /* suivant l'ordre in lequel on a rencontre les sommets ; ce qui    */
                                /* implique que if K>K' le plus court Chemin de S a Arbre[K] est     */
                                /* plus long or egal au plus court chemin de S a Arbre[K']           */


void init_var()
{
  int i;

  MemMatr = NEW_TAB(1+Taille,int *);
  matrix = NEW_TAB(1+Taille,int *);
  EcGra   = NEW_TAB(1+Taille,int *);
  for (i = 0 ; i < 1 + Taille ; i++)
  {
     matrix[i] = NEW_TAB(1+Taille,int);
     EcGra[i] = NEW_TAB(1+Taille,int);
     MemMatr[i] = NEW_TAB(1+Taille,int);
  }

  Chemin = NEW_TAB(NbSom+1,int);
  AffTache = NEW_TAB(NbSom+1,int);

  AffMach = NEW_TAB(Taille+1,int); 

  GrInjMax[0] = NEW_TAB(Taille+1,int); 
  GrInjMax[1] = NEW_TAB(Taille+1,int); 

  RecLigne = NEW_TAB(Taille+1,int); 

  RecColon = NEW_TAB(Taille+1,int); 

  MemAdGIM = NEW_TAB(Taille+1,int); 


  MinDeCol = NEW_TAB(Taille+1,int); 

  AddCol = NEW_TAB(Taille+1,int); 


 
  AddLig = NEW_TAB(Taille+1,int); 

   Affecte = NEW_TAB(Taille+1,int); 

  AdrPere = NEW_TAB(NbSom+1,int); 
              
  PasEncMa = NEW_TAB(NbSom+1,int); 

  PasEncTa = NEW_TAB(NbSom+1,int); 
  
  MemAdLig = NEW_TAB(Taille+1,int); 

  TabProvL = NEW_TAB(Taille+1,int); 

  TabProvC = NEW_TAB(Taille+1,int); 

  MemAdCol = NEW_TAB(Taille+1,int); 


 Arbre = NEW_TAB(NbSom+1,int); 
}



void free_var()
{
  int i;

  for (i = 0 ; i < 1 + Taille ; i++)
  {
      DELETE_TAB(matrix[i]);
      DELETE_TAB(EcGra[i]);
      DELETE_TAB(MemMatr[i]);
  }
   DELETE_TAB(MemMatr);
   DELETE_TAB(matrix);
   DELETE_TAB(EcGra);

   DELETE_TAB(Chemin);
   DELETE_TAB(AffTache);

   DELETE_TAB(AffMach);

   DELETE_TAB(GrInjMax[0]);
   DELETE_TAB(GrInjMax[1]);

   DELETE_TAB(RecLigne);

   DELETE_TAB(RecColon);

   DELETE_TAB(MemAdGIM);


   DELETE_TAB(MinDeCol);

   DELETE_TAB(AddCol);


 
   DELETE_TAB(AddLig);

   DELETE_TAB(Affecte);

   DELETE_TAB(AdrPere);
              
   DELETE_TAB(PasEncMa);

   DELETE_TAB(PasEncTa);
  
   DELETE_TAB(MemAdLig);

   DELETE_TAB(TabProvL);

   DELETE_TAB(TabProvC);

   DELETE_TAB(MemAdCol);

   DELETE_TAB(Arbre);
}


   
      /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/ 
      /*                                                */
      /*               MAIN                             */
      /*                                                */
      /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

 

 

     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
     /*                                  */
     /*          PIREMATR                */
     /*                                  */ 
     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
             
   
            /*  generation des "pires matrices" for Algo1,Algo2,Algo3 */

      /*     Algo1              Algo2              Algo3               */
   
      /* 0 0 0 0 0 0        0 0 0 0 0 0          0  0  0  0  0  0        */    
      /* 0 1 1 1 1 1        0 1 2 6 6 6          0  1  2  3  4  5        */      
      /* 0 2 2 2 2 2        0 6 2 3 6 6          0  2  4  6  8 10        */
      /* 0 3 3 3 3 3        0 6 6 3 4 6          0  3  6  9 12 15        */
      /* 0 4 4 4 4 4        0 6 6 6 4 5          0  4  8 12 16 20        */
      /* 0 5 5 5 5 5        0 1 6 6 6 5          0  5 10 15 20 25        */




void PIREMATR()

{int I,J;
  
 int Choix; 

 printf("\n   pire matrice pour algo 1,2 ou 3 ? \n");
 VoidScanf("%d",&Choix);

 for(I=1 ;I<=Taille ;I++ ){
    matrix[I][1]=0;
    matrix[1][I]=0;
 }

 if( Choix==2 ){
    for(I=2 ;I<=Taille ;I++ ){                       /*    Algo2 */
       for(J=2 ;J<=Taille ;J++ ){
          matrix[I][J]=Taille;
       }
    }
    matrix[2][2]=1;
    matrix[Taille][2]=1;
    for(I=3 ;I<=Taille ;I++ ){
       matrix[I][I]=I-1;
       matrix[I-1][I]=I-1;
    }
 }
   
 else{
     for(I=2 ;I<=Taille ;I++ ){
         for(J=2 ;J<=Taille ;J++ ){                                    
            if( Choix==3 )
               matrix[I][J]=(I-1)*(J-1);        /*   Algo3  */
            else
               matrix[I][J]=I-1;                /*   Algo1  */
         }
      }
  }

}



       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/                      
       /*                               */
       /*      ECRICOMP                 */
       /*                               */
       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


     /*    ecriture des diferrents compteurs    */



void ECRICOMP()

{     

  printf("\n CompHon1 %d \n",CompHon1);
  printf(" CompArEx %d \n",CompArEx);
  printf(" ComElEx2 %d \n",ComElEx2);
  printf(" ComElEx3 %d \n",ComElEx3);
  printf(" Temps  %4.3f \n",temps);
}
         

         /*$$$$$$$$$$$$$$$$$$$$$$$$$*/
         /*                         */
         /*     ECRIBOOL            */
         /*                         */
         /*$$$$$$$$$$$$$$$$$$$$$$$$$*/

      /* ecriture de la matrix under forme "booleenne"   */
      /* c'est a dire que  l'on remplace les zeros par   */
      /* des " * " and les aures elements par des " . "   */

      /*  44 0 777 0                 .   *   .   *       ,*/
      /*                                                 ,*/
      /*  444 6 0 7777               .   .   *   .       ,*/
      /*                  ---->                          ,*/
      /*  0 56789 0 7845             *   .   *   .       ,*/
      /*                                                 ,*/
      /*  0  543268 0 7              *   .   *   .       ,*/



void ECRIBOOL()

{int I,J;

  printf(" \n");
  for(I=1 ;I<=Taille ;I++ ){
     for(J=1 ;J<=Taille ;J++ ){
     if( matrix[I][J]==0 )
        printf(" * ");
     else
        printf(" . ");
     }
     printf(" \n");
  }
}

     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/      
     /*                            */     
     /*          ECRICHEM          */
     /*                            */
     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


   /*   ecriture du chemin de S a P  */


  
void ECRICHEM()

{int I;

  printf(" \n Chemin de 1 a 2 \n");
  for(I=1 ;I<=Chemin[0] ;I++ ) 
      printf(" %d ",Chemin[I]);
  printf(" \n");
}
                                       




        /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
        /*                                                 */
        /*            ALGOHONGR                            */
        /*                                                 */
        /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void ALGOHONGR(Im2D_INT4 cost,Im1D_INT4 res)  /* algorithme "hongrois": */
                                /* "assemble" tous les under-programmes */
                        

{
  ELISE_ASSERT(cost.tx()==res.tx(),"Hongr : incoherent sizes");
 
  double t2,t3;

  NbIter=0;          /* mise a zero des compteurs */

  MaxAbso=0;

  CompHon1=0;

  CompArEx=0;

  ComElEx2=0;

  ComElEx3=0;
    

           
  LECTURE(cost);                          /* lecture de la matrix */
   
  t2 =  quelle_heure_est_il();

  INIMAT();                           /*  on fait apparaitre quelques zeros */

  ALGO1();                            /*  premiere affectation */
                                      

  while( GrInjMax[0][0]<Taille ){      /* tant que il n'y a pas N(=Taille) affectation : */
                                       /*                                                */
       ALGO2();                        /*    %   -chercher les rangees obligatoires      */
                                       /*    %                                           */
       ALGO3();                        /*    %   -effectuer additions and soustractions   */   
                                       /*    %                                           */
       ALGO1();                        /*    %   -augmenter la taille de l'affectation    */

    
   }

   LECAFFPRO(res);                  /*   lire l'afectation "provisoire" (qui est maintenant  */
                                    /*   l'affectation definitive)                           */  


  t3 =  quelle_heure_est_il();

  temps= t3 - t2;
  //VERIF();
  free_var();
}

    
      /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
      /*                                               */
      /*           LECTURE                             */
      /*                                               */
      /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
                 

      /*  lecture de la matrix ,on choisit  between:        */
      /*         -LECMATRI <=> lecture element par element */
      /*         -PIREMATR <=> generation d'une des pires matrices */
      /*         -GENAUTO <=> geneartion d'une matrix "aleatoire" */




void LECTURE(Im2D_INT4 cost)                                              


{
  int I,J;

  ELISE_ASSERT
  (
     cost.tx() == cost.ty(),
     "Different size in hongrois"
  );
  Taille = cost.tx();
  INT4 ** c = cost.data();

  NbSom=Taille*2+2;
  init_var();
                                              
  for(I = 1 ; I <= Taille ; I++)
  {
     for ( J = 1; J<= Taille ; J++)
         matrix[I][J] =c[I-1][J-1];
     //  F_read(&(matrix[I][1]),sizeof(int),Taille,fp); 
  }

  MaxAbso=0;

     /*   mise en memoire de la matrix de depart in MemMatr */
  for(I=1 ;I<=Taille ;I++ ){
     for(J=1 ;J<=Taille ;J++ ){
        if( matrix[I][J]>MaxAbso )
           MaxAbso=matrix[I][J];
        MemMatr[I][J]=matrix[I][J];
     }
  }
}

     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
     /*                                             */
     /*         ALGO1                               */
     /*                                             */
     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void ALGO1()       /* algorithme d'affectation simple en considerant que la machine I peut realiser la  */
              /*  tache J if matrix[I][J]=0   ;    Algo1 est utilise:                    */
              /*        -soit tout seul in l'affectation simple                                  */
              /*        -soit en tant que under-programme de ALGOHONGR in l'affectation optimisee */


{




  CompHon1=CompHon1+1;


  INITECGRA();           /* on cree le graphe d'ecart  */
                                                         


  Existe=1;

  while( Existe==1 ){          /* tant qu'on a trouve un chemin de S a P :      */
                               /*                                               */
     CHEMOPT();                /*     %  -rechercher un nouveau chemin de S a P */
                               /*     %                                         */
     MODIAFFE();               /*     %  -modifier l'affectation en consequence */

  }

  AFFECOPT1();                  /*  en function de l'affectation simple optimale on  recree            */
                                /*  GrInjMax                                                            */
 


}


    /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
    /*                                               */
    /*          ALGO2                                */
    /*                                               */
    /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


         /*  Algorithme de recherche des rangees qui participent obligatoirement au recouvrement */
                                                                                                       

void ALGO2()
 
{  
 
              



  FinRecObl=0;
       

  INIREC();       /*  on recherche une premiere serie de rangees obligatoires:    */
                  /*     celles qui contiennent un element de G(=GrInjMax) and un  */
                  /*     zero ne se trouvant pas on @[G]                         */

                           

  while( FinRecObl==0 ){       /* tant qu'on trouve de nouvelles rangees obligatoires :         */
                               /*                                                               */
     AUGRECOBL();              /*       -rechercher les zeros de ( #[delta-g] )^( #[g2] ); les */
                               /* rangees qui couvrent ces zeros and qui coupent g2 sont des     */
                               /* rangees obligatoires                                          */
  }
                                                                                                                
      

  COMPLREC();                 /* on complete le recouvrement with des lignes */


  RANGNONCOU();               /* on reoordonne les rangees non couvrantes   */
  


}


      /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
      /*                                               */
      /*                   ALGO3                       */
      /*                                               */
      /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void ALGO3()     
                 /* on va soustraire le minimum des elements non couverts a tous   */
                 /* les elements and le rajouter on les rangees couvrantes,puis    */
                 /* modifier le recouvrement ( afin de tenir compte des nouveau    */
                 /* zeros ) jusqu'a ce que le number de zeros independants ait     */
                 /* augmente; tant que ceci n'est pas fait les additions and        */
                 /* soustractions ne sont pas reellement effectuees on la matrix */
                 /* but on memorise ce qu'il convient d'additonner on chaque     */
                 /* line,chaque colonne and a l'ensemble des elements              */


{
                               



  MINICOL();          /* recherche for chaque colonne non couvrante du minimum des elements */
                      /* ne se trouvant pas on une line couvrante                          */


  INIADD();           /* mise a zero des quantites a additionner */     


  MINIGLOB();         /* -recherche (a partir de MinDeCol )du minimum des elements non couverts de matrix */
                      /* -soustraction de ce minimum au array MinDeCol                                   */
                      /* -mise a jour des quantites a additionner ( AddEns,AddCol,AddLig )                 */


  NOUVZERIND();       /* on regarde if le number de zeros independants a augmente */



  while( NouvZero==0 ){     /*  tant que le number de zeros independant n'augmente pas :        */
                            /*                                                                  */
       MAJREC();            /*   %    -mise a jour du recouvrement , modification de MinDeCol   */
                            /*   %   for tenir compte des lignes qui ne sont plus couvrantes   */
                            /*   %                                                              */
       MINIGLOB();          /*   %    -recherche du minimum global etc...(voir juste au dessus) */
                            /*   %                                                              */   
       NOUVZERIND();        /*   %    -on regarde if le number de zeros independants a augmente */
  }    
                     

       MAJMATRI();           /* a ce niveau le number de zeros independants a augmente donc on */
                             /* on met a jour matrix a partir de AddEns,AddLig,AddCol         */
                



}



     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
     /*                                                   */
     /*                 VERIF                             */
     /*                                                   */
     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void VERIF()            
                        /*   verification du result annonce on va regarder if :               */
                        /*       -les affectation correspondent a des zeros de matrix (Exact1) */
                        /*       -les elements de matrices sont positifs (Exact2)               */
                        /*       -on passe de la matrix de depart a la matrix reduite en      */
                        /*      ajoutant des termes contants on les lignes and les colonnes (Exact3) */
                        /*       -toutes les machines sont bien affectees                       */


{int I,J;

 int Aux;

 int Exact1;                                                                       

 int Exact2;

 int Exact3;

 int Exact4;

                     




         /* on verifie que les affectaion donnees correspondent bien */
         /*           a des zeros de la matrix reduite              */
                               
  Exact1=1;
  for(J=1 ;J<=Taille ;J++ ){
     I=AffTache[J];
     if( matrix[I][J]!=0 )
        Exact1=0;
  }
  if( Exact1==0)
     printf("\n les affectations ne correspondent pas a des zeros \n ");
                                                      







       /* on verifie que la matrix reduite  ne contient que */
       /* des elements positifs                              */

 
  Exact2=1;
  for(I=1 ;I<=Taille ;I++ ){
      for(J=1 ;J<=Taille ;J++ ){
         if( matrix[I][J]<0 )
            Exact2=0;
     } 
  }
  if( Exact2==0 )
     printf("\n la matrice reduite contient des elements negatifs ");





                                                             


    /* on verifie qu'il existe deux suite de real Lig[K] and Col[K] */
    /* telles que  :  MemMatr[I][J]-matrix[I][J]=Lig[I]+Col[J] */


  Exact3=1;
  for(I=1 ;I<=Taille ;I++){                               /* on rentre MemMatr-matrix in matrix */
      for(J=1 ;J<=Taille ;J++ )
         matrix[I][J]=MemMatr[I][J]-matrix[I][J];
  }
  for(I=1 ;I<=Taille ;I++ ){                             /* on soustrait matrix[I][1] a toute  */
      Aux=matrix[I][1];                                 /* line I                             */
      for(J=1 ;J<=Taille ;J++ )
        matrix[I][J]=matrix[I][J]-Aux;
  }
  for(J=1 ;J<=Taille ;J++ ){                             /* on soustrait matrix[1][J] a toute */
     Aux=matrix[1][J];                                  /* colonne J                          */
     for(I=1 ;I<=Taille ;I++ )
        matrix[I][J]=matrix[I][J]-Aux;
  }
  for(I=1 ;I<=Taille ;I++ ){                            /* on verifie que le result obtenu est */
      for(J=1 ;J<=Taille ;J++ ){                        /* la matrix nulle                      */

         if( matrix[I][J]!=0 )
            Exact3=0;
      }
  }
  if( Exact3==0 ){
     printf("\n (matrix des cout) - (matrix reduite) n'est pas une ");
     printf("\n matrice ligne-colonne \n ");
  }






      
  /* on verifie que toutes les machines sont contenues in AffTache   */


  Exact4=1;
  for(I=1 ;I<=Taille ;I++ )
      Affecte[I]=0;
  for(J=1 ;J<=Taille ;J++){
     I=AffTache[J];
     Affecte[I]=1;
  }
  for(I=1 ;I<=Taille ;I++){
     if( Affecte[I]==0 ){
        Exact4=0;
        printf("\n machine numero %d non affectee \n ",I);
     }
  }




  /* on verifie que Exaxc1=Exact2=Exact3=Exact4=1    */


  Exact1=Exact1+Exact2+Exact3+Exact4;
  ELISE_ASSERT(Exact1==4,"VERIF HONGR" );

}

      /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
      /*                                               */
      /*             INIMAT                            */
      /*                                               */
      /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void INIMAT()    /* on va faire apparaitre autant de zeros que possible */
            /* and on initialise les tableaux Afftache and affMach   */  

{int Min;     /* minimum de line or de colonne */
  
 int I,J;


        /* for chaque line on recherche le minimum and */
        /* on le soustrait a chaque element de la line */

  for(I=1 ;I<=Taille ;I++ ){
     Min=matrix[I][1];
     for(J=1 ;J<=Taille ;J++ ){
        if( matrix[I][J]<Min )
           Min=matrix[I][J];
     }
     for(J=1 ;J<=Taille ;J++ )
        matrix[I][J]=matrix[I][J]-Min;
  }
     
    
         /* for chaque colonne on recherche le minimum and */
         /* on le soustrait a chaque element de la colonne */

  for(J=1 ;J<=Taille ;J++ ){
     Min=matrix[1][J];
     for(I=1 ;I<=Taille ;I++ ){
        if( matrix[I][J]<Min )
           Min=matrix[I][J];
     }
     for(I=1 ;I<=Taille ;I++ )
        matrix[I][J]=matrix[I][J]-Min;
  }

         /* initialistion de AffTache and AffMach  */

  for(I=0 ;I<=Taille ;I++ ){
     AffTache[I]=0;
     AffMach[I]=0;
  }
}




          /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
          /*                                            */
          /*            LECMATRI                        */
          /*                                            */
          /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void LECMATRI()          /* lecture "a la main" de la matrix */

{int I,J;

  for(I=1 ;I<=Taille ;I++ ){
     for(J=1 ;J<=Taille ;J++ ){
        printf("\n valeur de matrice( %d , %d )? \n" ,I,J);
        VoidScanf("%d",&matrix[I][J]);
     }
  }
}


       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
       /*                                                 */
       /*             ECRIMAT                             */
       /*                                                 */
       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/



void ECRIMAT()                    /* ecriture de la matrix des couts (matrix de */
                             /* depart or matrix reduite)                   */

{int I,J;

 int Aux;

 int Choix;


  printf("\n 0=matrice de depart sinon matrice reduite \n ");
  VoidScanf("%d",&Choix);

  printf("\n\n  matrix  \n");
  for(I=1 ;I<=Taille ;I++ ){
     for(J=1 ;J<=Taille ;J++ ){
        if( Choix==0 )
           Aux=MemMatr[I][J];
        else
           Aux=matrix[I][J];
        printf(" %2d",Aux);      
     }
     printf("\n");
  }
}



  /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
  /*                                                                */
  /*                      INITECGRA                                 */
  /*                                                                */
  /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/



void INITECGRA()      /* initialisation du graphe d'ecart  */                      
                                                                            


{int I,J;

 int Compteur;              /* compte le number de successeur d'un sommet */



 for(J=1 ;J<=Taille ;J++ ){         /*   for les colonnes de 1 a Taille faire :                 */
    Compteur=0;                     /*    %  initialiser le number de successeur de Tache J      */
    for(I=1 ;I<=Taille ;I++ ){      /*    %  for les lignes de 1 a Taille faire:                */
       if( matrix[I][J]==0 ){      /*    %   % if matrix[I][J]=0 then:                        */
          Compteur=Compteur+1;      /*    %   %   % Tache J a un succeseur de plus               */
          EcGra[Compteur][J]=I;     /*    %   %   % ce successeur est Machine I                  */
       }                            /*    %                                                      */  
    }                               /*    %                                                      */
    EcGra[0][J]=Compteur;           /*    %  enregistrer le number de succeseur de Tache J       */
 }
}


    /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
    /*                                                      */
    /*                 CHEMOPT                              */
    /*                                                      */
    /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/



void CHEMOPT()                  /* recherche du plus court chemin de S a P */
                           /*                par des arcs de flot nul */


  { int I,J,K;

   int I1;




 int AdrDeb,AdrFin;               /* adresse in Arbre de debut and fin de */
                                  /*                            generation */

 int NbDesc;                     /* number de descandants d'une generation */

 int NumGen;                        /* numero de la generation */

 int AdrMac1=1;   /* Warn gcc */  /* adresse de la premiere machine non affectee rencontree */


 int Auxil;                          /* variable auxiliaire  */


 int ContRech;                  /* =1 s'il faut continuer la recherche d'un */ 
                                /*                          chemin de S a P */

 int GenrObj;                  /* =0 sila generation est une generation de tache ,1 else */


         /* initialisation des variables  */
         
  NumGen=1;
  ContRech=1;
  Existe=0;
  NbDesc=0; 
  for(K=1 ;K<=Taille ;K++ ){
     PasEncTa[K]=1;  
     PasEncMa[K]=1;
  }


   /* recherche de la generation 1 qui contient toutes */
   /* les machines non encore affectees                */

  for(J=1 ;J<=Taille ;J++ ){
     if( AffTache[J]==0 ){
        NbDesc=NbDesc+1;
        Arbre[NbDesc]=J;
        PasEncTa[J]=0;
     }
  }
  AdrDeb=1;
  AdrFin=NbDesc;
  GenrObj=0;            




      /* creation de l'arbre */




  while( ContRech==1 ){                         /*   tant qu'il faut continuer la recheche faire :         */             
     NumGen=NumGen+1;                           /*    1  -il y a une generation de plus (generation N)     */
     NbDesc=0;                                  /*    1  -qui est vide for l'instant                      */
     GenrObj=(GenrObj+1)%2;                     /*    1  -dont le genre est inverse de generation N-1      */   
     for(K=AdrDeb ;K<=AdrFin ;K++ ){            /*    1  -for chaque sommet de generation N-1 faire:      */
        if( GenrObj==1 ){                       /*    1  2  if generation N=generation de machine          */
           J=Arbre[K];                          /*    1  2  3  -J=sommet dont on cherche les fils          */
           for(I1=1 ;I1<=EcGra[0][J] ;I1++ ){   /*    1  2  3  -for tout les successeurs I de J a         */
              CompArEx=CompArEx+1;
              I=EcGra[I1][J];                   /*    1  2  3   travers EcGra faire :                      */
              if( PasEncMa[I]==1 ){             /*    1  2  3  4  -if la machine I n'a pas ete exploree and */
                 if( AffTache[J]!=I ){          /*    1  2  3  4   que mach I n'est pas affecte a tacheJ : */
                    NbDesc=NbDesc+1;            /*    1  2  3  4  5  -la generation s'aggrandit            */
                    Arbre[AdrFin+NbDesc]=I;     /*    1  2  3  4  5  -I appartient a generation N          */
                    AdrPere[AdrFin+NbDesc]=K;   /*    1  2  3  4  5  -J(adresse K in Arbre)=pere de I    */
                    PasEncMa[I]=0;              /*    1  2  3  4  5  -machine I vient d'etre rencontree    */
                    if( AffMach[I]==0 ){        /*    1  2  3  4  5  -if machine I non affectee then      */
                       Existe=1;                /*    1  2  3  4  5  6  -il existe un chemin de S a P      */
                       ContRech=0;              /*    1  2  3  4  5  6  -arreter la recherche              */
                       AdrMac1=AdrFin+NbDesc;   /*    1  2  3  4  5  6  -memoriser l'adresse de I          */
                    }                           /*    1  2                                                 */
                 }                              /*    1  2                                                 */
              }                                 /*    1  2                                                 */
           }                                    /*    1  2                                                 */
        }                                       /*    1  2                                                 */
        else{                                   /*    1  2  else:      (generation de tache )             */
           I=Arbre[K];                          /*    1  2  3  -soit I la machine exploree                 */ 
           J=AffMach[I];                        /*    1  2  3  -soit J la tache que realise I              */
           if( PasEncTa[J]==1 ){                /*    1  2  3  if la tache J n'a pas encore ete exploree:  */
              NbDesc=NbDesc+1;                  /*    1  2  3  4 -la generation s'aggrandit                */
              Arbre[AdrFin+NbDesc]=J;           /*    1  2  3  4 -J appartient a la generation N           */
              AdrPere[AdrFin+NbDesc]=K;         /*    1  2  3  4 -I(adresse K in Arbre ) est  pere de J  */
              PasEncTa[J]=0;                    /*    1  2  3  4-la machine J vient d'etre rencontree      */
           }                                    /*    1                                                    */
        }                                       /*    1                                                    */
     }                                          /*    1                                                    */
     if( NbDesc==0 )                            /*    1  if generation N est vide :                        */
        ContRech=0;                             /*       2-abandonner la recherche                         */
     AdrDeb=AdrFin+1;                           /*    1 -debut de generation N:after fin de generation N-1 */
     AdrFin=AdrFin+NbDesc;                      /*    1 -fin  generation decalee de taille de  generation  */
  }



         /* remontee in l'arbre */


  if( Existe==1 ){                  /*   if le chemin existe                  */
     Chemin[0]=NumGen;              /*    1-stocker la longueur du chemin     */
     Auxil=AdrMac1;                 /*    1-Auxiliaire=Adresse de Mac 1       */
     for(K=NumGen ;K>=1 ;K-- ){     /*    1-for les generation de N a 1:     */
        Chemin[K]=Arbre[Auxil];     /*    1  2-le chemin passe par Arbre[aux] */
        Auxil=AdrPere[Auxil];       /*    1  2-Aux=Adresse du pere de Aux     */
     }
  }
}

 


       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
       /*                                                  */
       /*               MODIAFFE                           */
       /*                                                  */
       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void MODIAFFE()   /* modification de l'affectation provisoire */       
             /* en function du chemin de S a P           */           
                                                                      

{int I,J,K;

 int Prov,Auxi;             /* variable "provisoire" */

  if( Existe==0 ){
     ; ;                         
  }
  else{
    
     /* le Chemin est de la forme T1 M1 T2 M2 .... Tq Mq */
     /* with Chemin[0]=2*q                               */
     /* on va affecter T1 a M1; T2 a M2;..... ;Tq a Mq   */

     Auxi=Chemin[0]/2;                
     for(K=1 ;K<=Auxi ;K++ ){    /*  for K de 1 a q           */
        Prov=2*K;                /*   1-                       */
        I=Chemin[Prov];          /*   1-I=Machine K            */
        Prov=Prov-1;             /*   1-                       */
        J=Chemin[Prov];          /*   1-J=Tache K              */
        AffTache[J]=I;           /*   1-c'est la machine K qui */
        AffMach[I]=J;            /*     realisera la tache K   */
     }
  }
}


       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
       /*                                                 */
       /*               AFFECOPT1                         */
       /*                                                 */
       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void AFFECOPT1()    /*  creation de GrInjMax a partir des tableaux */              
               /*  AffTache and AffMach                        */



{int J,Compteur;



 
  /* on va d'abord rentrer in GrInjMax les taches and les machines  */
  /* de l'affectation provisoire :GrInjMax[0][K] designera une tache */
  /* de cette affectation and GrInjMax[1][K] sera la machine qui la   */   
  /* realise; GrInjMax[0][0] contiendra la taille de l'affectation   */
  /* provisoire                                                      */

     Compteur=0;
     for(J=1 ;J<=Taille ;J++ ){                 /*  for les tache de 1 a N:             */
        if( AffTache[J]!=0 ){                   /*   1-if la tache J set affectee        */
           Compteur=Compteur+1;                 /*   1  2-GrInjMax s'agrandit            */
           GrInjMax[0][Compteur]=J;             /*   1  2-Tache J appartient a GrInjMax  */
           GrInjMax[1][Compteur]=AffTache[J];   /*   1  2-la machine qui realise J aussi */
        }
     }

     /* on va completer GrInjMax[0][] with les taches non affectees */


     GrInjMax[0][0]=Compteur;
     AffTache[0]=Compteur;
     AffMach[0]=Compteur;
     for(J=1 ;J<=Taille ;J++ ){
        if( AffTache[J]==0){
           Compteur=Compteur+1;
           GrInjMax[0][Compteur]=J;
        }
     }
 

  /* on va completer GrInjMax[1][] with les machines non affectees */

     Compteur=GrInjMax[0][0];  
     for(J=1 ;J<=Taille ;J++ ){
        if( AffMach[J]==0 ){
           Compteur=Compteur+1;
           GrInjMax[1][Compteur]=J;
        }
     }
}


     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
     /*                                                  */
     /*               LECAFFPRO                          */
     /*                                                  */
     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void LECAFFPRO(Im1D_INT4 res)  /* lecture d'une affectation provisoiremnt optimale */


{


   INT4 * r = res.data();
   for (INT J=1; J<=Taille ; J++)
      r[J-1] = AffTache[J]-1;


#if(0)
  FILE * fp;
  fp = F_open("commande/HONGR/resultat","w");
  fwrite(&(AffTache[1]),sizeof(int),Taille,fp);
  F_close(fp);


  printf(" \n\n AFFECTATION OPTIMALE \n ");
  printf("\n   %d taches affectees \n \n ",AffTache[0]);
  for(J=1 ;J<=Taille ;J++ ){
     if( AffTache[J]!=0)
        printf("tache %d effectuee par machine %d \n ",J,AffTache[J]);
  }
  printf("\n GrInjMax machine :\n");
  for(J=0 ;J<=Taille ;J++ )
      printf(" %d ;",GrInjMax[1][J]);
  printf("\n\n GrInjMax Tache \n ");
  for(J=0 ;J<=Taille ;J++)
      printf(" %d;",GrInjMax[0][J]);
#endif
}

     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
     /*                                           */
     /*                GENEAUTO                   */
     /*                                           */
     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void GENEAUTO()       /* generation automatique de matrix */
 

{int I,J,K,L,Densite;


 int ValMaxim;

 int MatBool;

 int para[4];

 int aux1,aux2;


  printf("\n ");
  for(J=1 ;J<=3 ;J++ ){
     printf(" parametre numero %d =? entre 1 et 100 \n ",J);
     VoidScanf("%d",&para[J]);
  }
  for(J=1 ;J<=Taille ;J++ ){
     for(I=1 ;I<=Taille ;I++ ){
        matrix[I][J]=J+Taille*(I-1);
        for(K=1 ;K<=6 ;K++){
           aux1=matrix[I][J];
           aux2=aux1+(aux1%para[1])+(aux1%para[2])*(aux1%para[3]);
           aux1=aux2%5000;
           aux1=(aux1+100+para[1]*para[2])%5000;
           matrix[I][J]=aux1;
           for(L=1 ;L<=3 ;L++ ){
               para[L]=(para[L]+14+2*L)%100;
               para[L]=para[L]+2;
           }
        }
     }  
  }  
  printf("\n valeur  maximale des elements ? ");
  VoidScanf("%d",&ValMaxim);
  printf(" \n Valeur de mise a zero    \n ");
  VoidScanf("%d",&Densite);
  printf(" \n Si matrice de 1 et 0 rentrer valeur nulle ");
  VoidScanf("%d",&MatBool);
  for(I=1 ;I<=Taille ;I++ ){
     for(J=1 ;J<=Taille ;J++){
        matrix[I][J]=matrix[I][J]%ValMaxim;
     }
  }
  for(I=1 ;I<=Taille ;I++ ){
     for(J=1 ;J<=Taille ;J++ ){
        if( matrix[I][J]<Densite )
           matrix[I][J]=0;
        else{
           if( MatBool==0 ){
              matrix[I][J]=1;
           }
        }
     }
  }
}



       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
       /*                                                  */
       /*                INIREC                            */
       /*                                                  */
       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/



void INIREC()         /* initialisation des donnees for l'algorithme de  */
                 /* recherche d'un recouvrement obligatoire maximal  */
                 /* ce qui revient a chercher les zeros de #[G]-@[G] */
                 /* and les rangees qui contiennent ces zeros tout en */
                 /* coupant @[G]                                     */




{int I,J,Compteur;

 int Aux;             /* variable auxiliaire */

 int Prov;            /* variable provisoire utilisee for intervertion */

 int PartRec;          /* =0 if une rangee ne participe pas au recouvrement */
  
 int K,K1;


  AdrG2=1;
  Compteur=0;

           /* recherche des lignes participant obligatoirement au */
           /*     recouvrement (1  niveau )                      */

  for(I=1 ;I<=GrInjMax[0][0] ;I++ ){                       /* for toutes les lignes coupant GrInjMax         */
     PartRec=0;                                            /*  1-a priori la line n'est pas obligatoire      */
     for(J=GrInjMax[0][0]+1 ;J<=Taille ;J++ ){             /*  1-for les colonnes ne coupant pas GrInjMax    */
        ComElEx2=ComElEx2+1;                               /*  1  2- (mise a jour compteur ComElEx2 )         */
        if( matrix[GrInjMax[1][I]][GrInjMax[0][J]]==0 ){  /*  1  2-if l'element est nul                      */
           PartRec=1;                                      /*  1  2  3-la line est obligatoire               */
        }                                                  /*  1                                              */
     }                                                     /*  1                                              */
     if( PartRec==1 ){                                     /*  1-if la line est obligatoire                  */
        Compteur=Compteur+1;                               /*  1  2-il y a une line obligatoire de plus      */
        RecLigne[Compteur]=GrInjMax[1][I];                 /*  1  2-on la range a la fin de RecLigne          */  
        MemAdGIM[Compteur]=I;                              /*  1  2-on stocke l'adresse correspondante in   */
     }                                                     /*  1  2   GrInjMax                                */
  }
  RecLigne[0]=Compteur;       /* on stocke le number de line obligatoires */


    
       /*   on reordonne G de telle maniere que les element de g (c'est a dire */
       /*   ceux qui se trouvent on une line obligatoire ) soit au debut and  */
       /*   ceux de g2 a la fin                                                */


  for(K=1 ;K<=RecLigne[0] ;K++ ){                /* for toutes les lignes obligatoires:        */
     Aux=MemAdGIM[K];                            /*  1-soit Aux l'adresse correspondante in G */
     for(K1=0 ;K1<=1 ;K1++ ){                    /*  1-for K1 =0 and K1 =1:                     */
        Prov=GrInjMax[K1][Aux];                  /*  1  2-intervertir GrInjMax[K1][Aux] with    */
        GrInjMax[K1][Aux]=GrInjMax[K1][AdrG2];   /*  1  2 GrInjMax[K1][Adrg2](qui correspond    */
        GrInjMax[K1][AdrG2]=Prov;                /*  1  2 au premier element de G2)             */
     }                                           /*  1                                          */
     AdrG2=AdrG2+1;                              /*  1-G2 diminue                               */
  }

        
       /* on effectue exatement la meme operation but with les colonnes */

               /*recherche des colonnes obligatoires */  

  Compteur=0;
  for(J=1 ;J<=GrInjMax[0][0] ;J++ ){
      PartRec=0;
      for(I=GrInjMax[0][0]+1 ;I<=Taille ;I++ ){
          ComElEx2=ComElEx2+1;
          if( matrix[GrInjMax[1][I]][GrInjMax[0][J]]==0 )
             PartRec=1;
       }
       if( PartRec==1 ){
          Compteur=Compteur+1;
          RecColon[Compteur]=GrInjMax[0][J];
          MemAdGIM[Compteur]=J;
      }
   }
   RecColon[0]=Compteur;
 
 
     /* Reoordonnemnt de G */                                              
          
  for(K=1 ;K<=RecColon[0] ;K++ ){
     Aux=MemAdGIM[K];
     for(K1=0 ;K1<=1 ;K1++ ){
        Prov=GrInjMax[K1][Aux];
        GrInjMax[K1][Aux]=GrInjMax[K1][AdrG2];
        GrInjMax[K1][AdrG2]=Prov;         
     }
     AdrG2=AdrG2+1;
  }
  Aux=RecColon[0]+RecLigne[0];
  if( Aux==0 )
    FinRecObl=1;
                                       





                                   
   /* a ce stade delta-g=g and donc : */


  AdDeltGL=1;
  AdDeltGC=1;

}

       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
       /*                                                  */
       /*         AUGRECOBL                                */
       /*                                                  */
       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/



void AUGRECOBL()             /* recherche de nouvelles rangees participant */
                        /* au recouvrement obligatoire                */


{int I,J;

 int PartRec;

 
 int Aux;           /* auxiliaire de computation */

 int Prov;          /* variable utilisee for intervertir deux variables */

 int I1,J1;

 int K,K1;

 int I2,J2;

 int CompNelC;     /* compte le number de nouvelles colonnes */

 int CompNelL;     /* compte le number de nouvelles lignes */



   
        /* recherche des zeros non couverts de #[delta-g]^#[g2] en deux etapes : */
                                                              

       /* ETAPE 1 les zeros non couverts qui coupent delta-g  selon une line and */
       /* g2 selon une colonne                                                   */



  CompNelC=0;                                      /* au depart il n'y a aucune nouvelle colonne    */
  for(J1=AdrG2 ;J1<=GrInjMax[0][0] ;J1++ ){        /* for tout les element de g2 :                 */
     J=GrInjMax[0][J1];                            /*  1-soit J la colonne de l'element             */
     PartRec=0;                                    /*  1-a priori J n'est pas obligatoire           */
     for(J2=AdDeltGC ;J2<=RecColon[0] ;J2++ ){     /*  1-for les colonnes R[J2] couvrant delta-g   */      
        ComElEx2=ComElEx2+1;                       /*  1  2-(mise a jour compteur )                 */
        I=AffTache[RecColon[J2]];                  /*  1  2-soit I la line coupant R[J2] en G      */
        if( matrix[I][J]==0 )                     /*  1  2-if matrix[I][J]=0:                     */
           PartRec=1;                              /*  1  2   3-J devient obligatoire               */
      }                                            /*  1                                            */
      if( PartRec==1 ){                            /*  1-if J est obligatoire :                     */
         CompNelC=CompNelC+1;                      /*  1  2-il y a une colonne obligatoire en plus  */
         Aux=RecColon[0]+CompNelC;                 /*  1  2-soit aux l'adresse or il faut la ranger */
         RecColon[Aux]=J;                          /*  1  2-on la range                             */
         MemAdGIM[CompNelC]=J1;                    /*  1  2-on memorise l'adresse correspondante    */
      }                                            /*  1  2     GrInjMax                            */
  }                                                                                                           
  
     /* on reordonne G for tenir compte des nouvelles colonne obligatoire */
    
 
  for(K=1 ;K<=CompNelC ;K++ ){                  /* for toute les nouvelles colonnes :                */
     Aux=MemAdGIM[K];                           /*  1-soit aux l'adresse correspondante in GrInjMax */
     for(K1=0 ;K1<=1 ;K1++ ){                   /*  1-for K1=0 and K1=1 :                             */
        Prov=GrInjMax[K1][Aux];                 /*  1  2-intervertir GrInjMax[K1][aux] with           */
        GrInjMax[K1][Aux]=GrInjMax[K1][AdrG2];  /*  1  2 GrInjMax[K1][AdrG2](qui corespond au         */
        GrInjMax[K1][AdrG2]=Prov;               /*  1  2 premier element de G2)                       */
     }                                          /*  1                                                 */
     AdrG2=AdrG2+1;                             /*  1-G2 diminue                                      */  
  }


                                                                             





   /* ETAPE 2 les zeros non couverts qui coupent delta-g selon une colonne */  
   /* and g2 selon une line                                                */


  CompNelL=0;
  for(I1=AdrG2 ;I1<=GrInjMax[0][0] ;I1++ ){
     I=GrInjMax[1][I1];
     PartRec=0;
     for(I2=AdDeltGL ;I2<=RecLigne[0] ;I2++ ){
        ComElEx2=ComElEx2+1;
        J=AffMach[RecLigne[I2]];
        if( matrix[I][J]==0 )
           PartRec=1;
     }
     if( PartRec==1){
        CompNelL=CompNelL+1;
        Aux=RecLigne[0]+CompNelL;
        RecLigne[Aux]=I;
        MemAdGIM[CompNelL]=I1;
     }
  }     
      
    /* on reordonne G  */

  for(K=1 ;K<=CompNelL ;K++){
     Aux=MemAdGIM[K];
     for(K1=0 ;K1<=1 ;K1++ ){
        Prov=GrInjMax[K1][AdrG2];
        GrInjMax[K1][AdrG2]=GrInjMax[K1][Aux];
        GrInjMax[K1][Aux]=Prov;
     }
     AdrG2=AdrG2+1;
  }








       /* mise a jour des donnes permettant d'acceder a */
       /* Delta-GRAPHE and de RecLigne and RecColon */
   
  AdDeltGL=RecLigne[0]+1;              /* -delta-g commence a la fin */
  AdDeltGC=RecColon[0]+1;              /* g precedent                */
  RecColon[0]=RecColon[0]+CompNelC;    /* -g=                        */
  RecLigne[0]=RecLigne[0]+CompNelL;    /*   g~delta-g                */



   /* if le number de nouvelles rangees obligatoires est   */
   /* egal a zero then il va falloir arreter la recherche */

  Aux=CompNelL+CompNelC;
  if (Aux==0 )
    FinRecObl=1;
}


     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
     /*                                                  */
     /*             COMPLREC                             */
     /*                                                  */
     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void COMPLREC()    /* une fois selectionnees les rangees qui doivent */
              /* imperativement participer au recouvrement minimal */
              /* on va completer ce recouvrement par des lignes */ 
 


{int I,I1;


  NbProLig=RecLigne[0];                     /* for l'instant le number provisoire de line est */    
                                            /* egal au number de line obligatoire              */

  for(I1=AdrG2 ;I1<=GrInjMax[0][0] ;I1++ ){  /*  for tous les elements de g2 :             */
     I=GrInjMax[1][I1];                      /*    1-soit I la line de cet element         */
     NbProLig=NbProLig+1;                    /*    1-le number provisoire de line augmente */
     RecLigne[NbProLig]=I;                   /*    1-la line I est une line provisoire    */
  }
}


        /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
        /*                                            */
        /*          RANGNONCOU                        */
        /*                                            */
        /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void RANGNONCOU()    /* on va classer les lignes and les colonnes non */
                /* couvrantes a la fin de RecLigne and RecColon  */



{int I,J;

 int I1,J1;

 int Compteur;

 int K;      


         /* recherche des lignes non couvrantes coupant GrInjMax */

  Compteur=NbProLig;
  for(J1=1 ;J1<=RecColon[0] ;J1++ ){    /* for toute les colonne couvrantes         */
     J=RecColon[J1];                    /*  1-soit J le numero de colonne            */
     I=AffTache[J];                     /*  1-soit I la line coupant J on G        */
     Compteur=Compteur+1;               /*  1-il y a une line non couvrante de plus */
     RecLigne[Compteur]=I;              /*  1-la ranger a la fin de RecLigne         */
  }
  
        /* recherche des colonnes non couvrantes coupant GrInjMax */


   Compteur=RecColon[0];
   for(I1=1 ;I1<=NbProLig ;I1++ ){       
       I=RecLigne[I1];
       J=AffMach[I];
       Compteur=Compteur+1;
       RecColon[Compteur]=J;
  }
  
          /* recherche des rangees ne coupant pas GrInjMax */
    
  for(K=GrInjMax[0][0]+1 ;K<=Taille ;K++ ){  /* for tous les element de GrInjMax en dehors de G */
      J=GrInjMax[0][K];                      /*  1-soit J la colonne                             */
      I=GrInjMax[1][K];                      /*  1-soit I la line                               */
      RecColon[K]=J;                         /*  1-ranger J a la fin de RecColon                 */
      RecLigne[K]=I;                         /*  1-ranger I a la fin de RecLigne                 */
  }
}

      /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
      /*                                                */
      /*           MINICOL                              */
      /*                                                */
      /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void MINICOL()       /* recherche des elements minimum non couverts de */
                 /* chaque colonne non couvrante                   */


{int I,J;

 int I1,J1;

 int Min;

 
  for(J1=RecColon[0]+1 ;J1<=Taille ;J1++ ){     /* for toute les colonnes non couvrantes :  */
     J=RecColon[J1];                            /*  1-soit J la colonne                      */
     I=RecLigne[NbProLig+1];                    /*  1-soit I la premiere line non couvrante */
     Min=matrix[I][J];                         /*  1-soit Min=matrix[I][J]                 */
     for(I1=NbProLig+1 ;I1<=Taille ;I1++ ){     /*  1-for toutes les lignes non couuvrantes */
        ComElEx3=ComElEx3+1;                    /*  1  2- ( mise a jour compteur )           */          
        I=RecLigne[I1];                         /*  1  2-soit I la line                     */
        if( matrix[I][J]<Min )                 /*  1  2-if matrix[I][J] > Min              */
           Min=matrix[I][J];                   /*  1  2  3-Min = matrix[I][J]              */   
     }                                          /*  1                                        */
     MinDeCol[J1]=Min;                          /*  1-enregistrer Min in MinDeCol          */
  }
}


     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
     /*                                                */
     /*              INIADD                            */ 
     /*                                                */
     /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/



void INIADD()          /* intialisation de AddCol,AddLig and AddEns */

{int K;     

  
  for(K=1 ;K<=Taille ;K++ ){
     AddCol[K]=0;
     AddLig[K]=0;
  }
  AddEns=0;  
}




        /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
        /*                                              */
        /*            MINIGLOB                          */
        /*                                              */
        /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void MINIGLOB()          /* recherche de l'element non couvert   */
                    /* globalement minimal and actualisation */
                    /* de AddEns,AddCol and AddLig           */


{int J1,I1;

 int I,J;



   /* computation de MinGlob ; MinGlob est egal au minimum de MinDeCol */

  MinGlob=MinDeCol[RecColon[0]+1];
  for(J1=RecColon[0]+1 ;J1<=Taille ;J1++ ){
     if( MinGlob>MinDeCol[J1] )
        MinGlob=MinDeCol[J1];
  }

    /* soustraction de MinGlob a MinDeCol  */

  for(J1=RecColon[0]+1 ;J1<=Taille ;J1++ )
     MinDeCol[J1]=MinDeCol[J1]-MinGlob;
                          

   /* mise a jour de AdddEns,AddLig,AddCol */

  AddEns=AddEns-MinGlob;

  for(J1=1 ;J1<=RecColon[0] ;J1++ ){
     J=RecColon[J1];
     AddCol[J]=AddCol[J]+MinGlob;
  }

  for(I1=1 ;I1<=NbProLig ;I1++ ){
     I=RecLigne[I1];
     AddLig[I]=AddLig[I]+MinGlob;
  }
}



       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
       /*                                                */
       /*              NOUVZERIND                        */
       /*                                                */
       /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void NOUVZERIND()        /* recherche s'il y a de nouveaux zeros independants */


{int J1;
 
  


   /* recheche on les colonnes coupant G on des lignes obligatoires */

  NouvZero=0;
  for(J1=RecColon[0]+1 ;J1<=RecColon[0]+RecLigne[0] ;J1++ ){
      if( MinDeCol[J1]==0 )
         NouvZero=1;
  }
 
    
  /* recherche on les colonnes ne coupant pas G */

  for(J1=GrInjMax[0][0]+1 ;J1<=Taille ;J1++ ){
      if(MinDeCol[J1]==0)
         NouvZero=1;
  }
}
  
         /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
         /*                                                */
         /*            MAJMATRI                            */
         /*                                                */
         /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


void MAJMATRI()        /* mise a jour de matrix une fois que l'on a trouve */
                  /*      un zero independant supplementaire           */


{int I,J;

 int MaxLoc;


  MaxLoc=0;
  for(I=1 ;I<=Taille ;I++ ){
     for(J=1 ;J<=Taille ;J++){
        matrix[I][J]=matrix[I][J]+AddEns+AddLig[I]+AddCol[J];
        if( matrix[I][J]>MaxLoc ) 
           MaxLoc=matrix[I][J];
     }
  }
  if( MaxLoc>MaxAbso )
     MaxAbso=MaxLoc;
}


         /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
         /*                                              */
         /*             MAJREC                           */
         /*                                              */
         /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

 
void MAJREC()         /* mise a jour des parameters de recouvrement and des minimum de coloonnes for tenir */
                      /* compte des nouvelles lignes non couvrantes in le cas or tout les zeros se       */
                      /* trouvent on des colonnes coupant g2                                              */



{int I,J;

 int K;

 int Compteur;

 int I2;


 int Min;

 int Aux;

 int I1,J1;




    /* recherche des colonnes devenant obligatoires and des lignes qui */
    /* les coupent en GrInjMax ,stockage de leurs adresses */



 NbNvCoOb=0;
 for(J1=RecLigne[0]+RecColon[0]+1 ;J1<=GrInjMax[0][0] ;J1++ ){ /* for toutes les colonnes coupant g2  :   */
    if( MinDeCol[J1]==0 ){                                     /*  1-if la colonne contient un zero        */
       NbNvCoOb=NbNvCoOb+1;                                    /*  1-il y a une colonne obligatoire en +   */
       J=RecColon[J1];                                         /*  1-soit J cette colonne                  */ 
       I=AffTache[J];                                          /*  1-soit I la line coupant J on G       */  
       MemAdCol[NbNvCoOb]=J1;                                  /*  1-memoriser l'adresse de J in RecCol  */
       for(I1=1 ;I1<=GrInjMax[0][0] ;I1++){                    /*  1-for toutes les lignes coupant G      */
           if( RecLigne[I1]==I ){                              /*  1  2-if la line est la line I         */
              MemAdLig[NbNvCoOb]=I1;                           /*  1  2  3-memoriser adresse in GrInjMax */
          }
      }
    }
 }


    /* mise a jour des minimum */
   

  for(J1=RecColon[0]+1 ;J1<=Taille ;J1++ ){                /* for toutes les colonnes non couvrantes       */
     J=RecColon[J1];                                       /*  1-soit J la colonne                          */
     Min=MinDeCol[J1];                                     /*  1-soit Min=MinDeCol                          */
     for(I2=1 ;I2<=NbNvCoOb ;I2++ ){                       /*  1-for toutes les lignes qui ne sont plus    */
        ComElEx3=ComElEx3+1;                               /*  1          couvrantes                        */
        I1=MemAdLig[I2];                                   /*  1  2-soit I1 l'adresse in MemAdLig         */
        I=RecLigne[I1];                                    /*  1  2-soit I la line                         */
        Aux=matrix[I][J]+AddEns+AddLig[I]+AddCol[J];      /*  1  2-soit Aux la value "reelle" de matrix  */
        if( Aux<Min )                                      /*  1  2-if Aux plus petit que Min               */
           Min=Aux;                                        /*  1  2  3-then Min vaut Aux                   */
     }                                                     /*  1                                            */
     MinDeCol[J1]=Min;                                     /*  1-stocker la vleur du minimum                */ 
   }


     /* mise a jour de RecLigne and RecColon ; rapellons la facon dont il doivent etre ordonnes                    */
     /*                                                                                                           */
     /*                                                                                                           */
     /*               RecLigne[0]             NbProlig                        GrInjMax[0][0]                      */
     /*                         |                    |                             |                              */
     /*   RECLIGNE  ----------------------------------------------------------------------------------------      */
     /*         lignes          |    lignes          |  lignes coupant colonne     |    line ne                  */
     /*       obligatoires      |    provisoires     |     obligatoire on G       |   coupant pas G              */
     /*                                                                                                           */
     /*                                                                                                           */
     /*               RecColon[0]       RecColon[0]+RecLigne[0]                GrInjMax[0][0]                     */
     /*                         |                            |                        |                           */
     /*  RECCOLON   -----------------------------------------------------------------------------------------     */
     /*        colonnes         |    colonnes coupant lignes | colonne coupant lignes |     colonnes ne coupant   */
     /*      obligatoires       |       obligatoires on G   |  provisoires on G     |              pas G        */
              




  for(K=1 ;K<=NbNvCoOb ;K++ ){            /* for les line and les colonnes qui changent d'etat */
     TabProvL[K]=RecLigne[MemAdLig[K]];   /*  1-memoriser la line                              */
     TabProvC[K]=RecColon[MemAdCol[K]];   /*  1-memoriser la colonne                            */
  }



  for(K=1 ;K<=NbNvCoOb ;K++ ){     /* for les lignes and les colonnes qui changent d'etat */
      RecLigne[MemAdLig[K]]=0;     /*  1-marquer les lignes                               */
      RecColon[MemAdCol[K]]=0;     /*  1-marquer les colonnes                             */ 
  }

  Compteur=0;                                                     

  for(K=RecLigne[0]+1 ;K<=NbProLig-NbNvCoOb ;K++ ){   /*  on va reordonner Recligne comme on l'exemple suivant */
      while( RecLigne[K+Compteur]==0 )                /*  au depart: $15  9  0  3  7  8  0  2  0  6             */
         Compteur=Compteur+1;                         /*  puis     :  15  9  3  7  8$ 8  0  2  0  6             */
      RecLigne[K]=RecLigne[K+Compteur];               /*  puis     :  15  9  3  7  8  2  6$ 2  0  6             */ 
  }                                                   /*    $ indique le dernier element modifie                */
                                                      /*  sens de deplacement de $:  -->                        */

  for(K=1 ;K<=NbNvCoOb ;K++ )                         /* les line qui ne sont plus couvrantes sont rangees a la fin */
     RecLigne[K+NbProLig-NbNvCoOb]=TabProvL[K];       /*   par exemple    15  9  3  7  8  2  6     4  5  13          */



  Compteur=0;
  for(K=GrInjMax[0][0] ;K>=RecColon[0]+NbNvCoOb+1 ;K-- ){    /* on va reordonner RecColon and MinDeCol comme ceci:      */
     while( RecColon[K-Compteur]==0 )                        /* au depart: 15  6  0  24  0  7  0  8  9  3 $            */ 
        Compteur=Compteur+1;                                 /* puis     : 15  6  0  24  0  7 $7  8  9  3              */
     RecColon[K]=RecColon[K-Compteur];                       /* puis     : 15  6  0 $15  6 24  7  8  9  3              */
     MinDeCol[K]=MinDeCol[K-Compteur];                       /*  sens de deplacement de $ :  <--                       */ 
  }
  Aux=RecColon[0];
  for(K=1 ;K<=NbNvCoOb ;K++ )              /* les colonnes qui deviennent obligatoires sont rangees au debut  */ 
     RecColon[Aux+K]=TabProvC[K];



  RecColon[0]=RecColon[0]+NbNvCoOb;
  NbProLig=NbProLig-NbNvCoOb;
}



cComputecKernelGraph::cComputecKernelGraph() :
    mPdsArc  (1,1),
    mCostArc (1,1),
    mPdsSom  (1),
    mCostSom (1)
{
}

void cComputecKernelGraph::AddCost(int aK1,int aK2,double aPds1,double aPds2,double aDist)
{
     mDPdsArc[aK1][aK2]  += aPds1;
     mDPdsArc[aK2][aK1]  += aPds2;
     mDPdsSom[aK1]       += aPds1;
     mDPdsSom[aK2]       += aPds2;

     mDCostArc[aK1][aK2] += aDist*aPds1;
     mDCostArc[aK2][aK1] += aDist*aPds2;
     mDCostSom[aK1]      += aDist*aPds1;
     mDCostSom[aK2]      += aDist*aPds2;
}

int  cComputecKernelGraph::GetKernelGen()
{
   if (mNb>=3) return GetKernel();
   if (mNb==1) return 0;

   if (mNb==2)
   {
       return (mDPdsSom[0] > mDPdsSom[1]) ? 0 : 1;
   }
   ELISE_ASSERT(false,"cComputecKernelGraph::GetKernelGen");
   return -1;
}

int  cComputecKernelGraph::GetKernel()
{
    ELISE_ASSERT(mNb>=3,"cComputecKernelGraph::GetKernel");
    int aKBest=-1;
    for (int anIter=2 ; anIter < mNb ; anIter++)
    {
         int aKWorst=-1;
         double aBestCost = 1e9;
         double aWortCost = -1e9;
         for (int aK=0 ; aK<mNb ; aK++)
         {
             if (mDPdsSom[aK] >0)
             {
                double aCost =  mDCostSom[aK]  / mDPdsSom[aK];
                if (aCost>aWortCost)
                {
                     aWortCost = aCost;
                     aKWorst = aK;
                }
                if (aCost< aBestCost)
                {
                     aBestCost = aCost;
                     aKBest = aK;
                }
             }
         }
         ELISE_ASSERT((aKWorst>=0) && (aKBest>=0),"cComputecKernelGraph::GetKernel");
         for (int aK=0 ; aK<mNb ; aK++)
         {
              mDPdsSom[aK] -=  mDPdsArc [aK][aKWorst];
              mDCostSom[aK] -= mDCostArc[aK][aKWorst];
         }
         mDPdsSom[aKWorst] = -1;


    }
    return aKBest;
}


void cComputecKernelGraph::SetN(int aN)
{
    // ELISE_ASSERT(aN>=3,"cComputecKernelGraph::SetN");
    mNb = aN;
    mCostArc.Resize(Pt2di(aN,aN));
    mPdsArc.Resize(Pt2di(aN,aN));
    mPdsSom.Resize(aN);
    mCostSom.Resize(aN);

    mDPdsArc = mPdsArc.data();
    mDCostArc = mCostArc.data();
    mDPdsSom = mPdsSom.data();
    mDCostSom = mCostSom.data();

    for (int anX=0 ; anX < aN ; anX++)
    {
       mDPdsSom[anX] = 0.0;
       mDCostSom[anX] = 0.0;
       for (int anY=0 ; anY < aN ; anY++)
       {
           mDCostArc[anY][anX] = 0.0;
           mDPdsArc[anY][anX] = 0.0;
       }
    }
}



/*Footer-MicMac-eLiSe-25/06/2007

Ce logiciel est un programme informatique servant à la mise en
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
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant 
donné sa spécificité de logiciel libre, qui peut le rendre complexe à 
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement, 
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité. 

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez 
pris connaissance de la licence CeCILL-B, et que vous en avez accepté les
termes.
Footer-MicMac-eLiSe-25/06/2007*/
