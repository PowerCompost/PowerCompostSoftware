#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int main(int argc,char **argv)
{

//Variables entrantes :

// H  devient Hbig ; L devient Lbig
double F0,b,lamb,Text,dx, nech,dt,kreacz,kreacu,tech,fpipe,Tentry,fpiperec = 0.0;

int n1ech = 0;

// dimensions de Matrices
int lignes , colonnes , altitude = 0;

//Variables sortantes :

// taille des chaines auxiliaires
int nbcar2=300;


// chaines auxiliaires
char ch1[nbcar2] ;
char ligne[nbcar2];
char nomfichier[nbcar2];
int ouiounon;
int perpen;

FILE *fp3; //donnees pour gnuplot
// variables internes
int z, i, j , k, l, ll, kk, kkk, lig, col, al, n, m, lpipe, lcapt, coupur, modele = 0;

int nb = 1;

// test presence d' un seul parametre en ligne commande
if (argc != 2)  {
    printf("\nErreur : il faut un parametre ! exemple :\n c_sim_diff.exe  fsortie.txt");
    exit(0);
}

// le fichier texte d entree
FILE *fp; // pointeur fichier entree
FILE *fp2; // pointeur fichier sortie
FILE *fp4;

// ouverture fichier lu
fp = fopen("fich_echangev6_sdf.txt","r");
fp4 = fopen("capteur_zero.txt","w");
// lecture des parmetres d entree

// variables singulieres
// nom fichier sortie
//fgets(nomfichier,nbcar2,fp);

// flottants
fgets(ligne,nbcar2,fp);
F0= atof(ligne);

fgets(ligne,nbcar2,fp);
b= atof(ligne);

fgets(ligne,nbcar2,fp);
lamb= atof(ligne);

fgets(ligne,nbcar2,fp);
Text= atof(ligne);

fgets(ligne,nbcar2,fp);
dx= atof(ligne);

fgets(ligne,nbcar2,fp);
nech= atof(ligne);

fgets(ligne,nbcar2,fp);
dt= atof(ligne);

fgets(ligne,nbcar2,fp);
modele= atoi(ligne);

fgets(ligne,nbcar2,fp);
kreacz= atof(ligne);

fgets(ligne,nbcar2,fp);
kreacu= atof(ligne);

fgets(ligne,nbcar2,fp);
tech= atof(ligne);

// entiers
fgets(ligne,nbcar2,fp);
n1ech= atoi(ligne);

fgets(ligne,nbcar2,fp);
lignes = atoi(ligne);

fgets(ligne,nbcar2,fp);
colonnes = atoi(ligne);

fgets(ligne,nbcar2,fp);
altitude = atoi(ligne);

//Matrix :

//h,T0,C0
//float M[nlig][ncol];

float h[lignes][colonnes][altitude];
float T0[lignes][colonnes][altitude];
float C0[lignes][colonnes][altitude];

float Tp[lignes][colonnes][altitude];
float Cp[lignes][colonnes][altitude];

float Tc[lignes][colonnes][altitude];
float Cc[lignes][colonnes][altitude];


// remplit les trois Matrices
for (lig = 0; lig < lignes; lig++) {
  for (col = 0; col < colonnes; col++) {
    for (al = 0; al < altitude; al++) {
      fgets(ligne,nbcar2,fp);
      h[lig][col][al] = atof(ligne);

      fgets(ligne,nbcar2,fp);
      T0[lig][col][al] = atof(ligne);

      fgets(ligne,nbcar2,fp);
      C0[lig][col][al] = atof(ligne);
    }
  }
}

//the pipe

fgets(ligne,nbcar2,fp);
fpipe = atof(ligne);

fgets(ligne,nbcar2,fp);
lpipe = atoi(ligne);

fgets(ligne,nbcar2,fp);
fpiperec = atof(ligne);

fgets(ligne,nbcar2,fp);
Tentry = atof(ligne);

int pipe[lpipe][3];
float Tpipe[lpipe];

for (i = 0; i < lpipe; i++) {
    for (j = 0; j < 3; j++) {
        fgets(ligne,nbcar2,fp);
        pipe[i][j] = atoi(ligne);
    }
}

//the captors
fgets(ligne,nbcar2,fp);
lcapt = atoi(ligne);

int captors[lcapt][3];

for (i = 0; i < lcapt; i++) {
    for (j = 0; j < 3; j++) {
        fgets(ligne,nbcar2,fp);
        captors[i][j] = atoi(ligne);
    }
}

fgets(ligne,nbcar2,fp);
ouiounon = atoi(ligne);
fgets(ligne,nbcar2,fp);
perpen = atoi(ligne);
fgets(ligne,nbcar2,fp);
coupur = atof(ligne);

 // fin entree variables

 // fermer fichier
 fclose(fp);

 n=lignes-1;
 m=colonnes-1;
 kk=altitude-1;


// fonctions internes


// copie un tenseur d'ordre 3 :  m2 = m1
void copymatrix(float m1[][colonnes][altitude],  float m2[][colonnes][altitude]) {
//int lig,col =0;
  for (lig = 0; lig < lignes; lig++) {
    for (col = 0; col < colonnes; col++) {
      for (al = 0; al < altitude; al++) {
         m2[lig][col][al] = m1[lig][col][al];
      }
        	}
      }
 } // fin de fonction Copie tenseur d'ordre 3


// double un tenseur d'ordre 3 :   m2 = m1 * 2.0
void doublematrix(float m1[][colonnes][altitude],  float m2[][colonnes][altitude]) {
int lig,col,al =0;
  for (lig = 0; lig < lignes; lig++) {
    for (col = 0; col < colonnes; col++) {
      for (al = 0; al < altitude; al++) {
         m2[lig][col][al] = m1[lig][col][al]* 2.0;
      }
    }
  }
 } // fin de fonction double tenseur d'ordre 3

// fonctions de modèles bio-chimiques

void ordrezero(float a, float dt, float kreacz, float b){ // b=ordrezero(a)
    Cc[i][j][k] = Cp[i][j][k] - dt*kreacz;
    if (Cc[i][j][k] <= 0.) {
        Cc[i][j][k] = 0.;
        }
}

void ordreun(float a, float dt, float kreacu, float b){
    Cc[i][j][k] = Cp[i][j][k]*(1- dt*kreacu);
}

void ordrecomb(float a, float dt, float kreacz, float kreacu, float b){
    Cc[i][j][k] = Cp[i][j][k]*(1- dt*kreacu) - dt * kreacz;
    if (Cc[i][j][k] <= 0.) {
        Cc[i][j][k] = 0.;
        }
}

// fin de fonctions internes

// affiche matrice(s)
//affmat(h);

//                         Code :

// ouvre fichier sortie
//fp2 = fopen("fichsortie.txt","w");

// ouvre par ligne de commande en parametre
fp2 = fopen(argv[1],"w");


    nb = 0; //nb is the number of time quantums for the last value taking


    // ecrit dans fichier
    sprintf(ch1, "ENS Cachan R.FERRIER PS6_2013");
    fprintf(fp2,ch1);
  strcpy(ch1,"\n");
  fprintf(fp2,ch1);
  strcpy(ch1,"  Temps(s) : 0000.000000");
    fprintf(fp2,ch1);

  for (i = 0; i < lcapt ; i++) {
    strcpy(ch1,"  capteur ");
    fprintf(fp2,ch1);
	sprintf(ch1, "%d", i);
    fprintf(fp2,ch1);
    strcpy(ch1," ");
    fprintf(fp2,ch1);
	sprintf(ch1, "%f", T0[captors[i][0]][captors[i][1]][captors[i][2]]);
    fprintf(fp2,ch1);
  }
    strcpy(ch1,"  concentration 0  ");
    fprintf(fp2,ch1);
	sprintf(ch1, "%f", C0[captors[0][0]][captors[0][1]][captors[0][2]]);
    fprintf(fp2,ch1);
    strcpy(ch1,"  Tsortie  ");
    fprintf(fp2,ch1);
	sprintf(ch1, "%f", Tentry);
    fprintf(fp2,ch1);
  //  chaine.extend ("\n")
  strcpy(ch1,"\n");
  fprintf(fp2,ch1);


   // copies de matrices entrees :
   // Tp = T0;
   copymatrix(T0,Tp);

   // Cp = C0;
	copymatrix(C0,Cp);


// boucle principale de calculs
//for z in range(int(nech)-1):
for ( z = 0 ; z < nech-1 ; z++ )
{

        // multiplication de toutes les cases par 2.0
       // Cc = 2*C0       #Cc has the same size as C0
	   doublematrix(C0,Cc);
      //  Tc = 2*T0       #Tc has the same size as T0
      doublematrix(T0,Tc);
   // #lets build the matrix of concentration

        //for i in range (n+1):
		for ( i = 0 ; i < n+1 ; i++ ) {
            //for j in range (m+1):
			for ( j = 0 ; j < m+1 ; j++ )  {
                    for (k = 0 ; k < kk+1 ; k++ ) {
                        if (modele == 0){
                            ordrezero(Cp[i][j][k],dt,kreacz,Cc[i][j][k]);
                        }
                        if (modele == 1) {
                            ordreun(Cp[i][j][k],dt,kreacu,Cc[i][j][k]);
                        }
                        if (modele == 2) {
                            ordrecomb(Cp[i][j][k],dt,kreacz,kreacu,Cc[i][j][k]);
                        }
                    }
            }
		}
  //  #lets write the edge conditions for temperature:

        Tc[0][0][0]= F0*(Tp[0][1][0]+Tp[1][0][0]+Tp[0][0][1]) + Tp[0][0][0]*(1-3*F0) + b*(Cp[0][0][0]-Cc[0][0][0])/dt + 3*(Text-Tp[0][0][0])*h[0][0][0]*dx*F0/lamb;
        Tc[n][0][0]= F0*(Tp[n][1][0]+Tp[n-1][0][0]+Tp[n][0][1]) + Tp[n][0][0]*(1-3*F0) + b*(Cp[n][0][0]-Cc[n][0][0])/dt + 3*(Text-Tp[n][0][0])*h[n][0][0]*dx*F0/lamb;
        Tc[0][m][0]= F0*(Tp[0][m-1][0]+Tp[1][m][0]+Tp[0][m][1]) + Tp[0][m][0]*(1-3*F0) + b*(Cp[0][m][0]-Cc[0][m][0])/dt + 3*(Text-Tp[0][m][0])*h[0][m][0]*dx*F0/lamb;
        Tc[n][m][0]= F0*(Tp[n][m-1][0]+Tp[n-1][m][0]+Tp[n][m][1]) + Tp[n][m][0]*(1-3*F0) + b*(Cp[n][m][0]-Cc[n][m][0])/dt + 3*(Text-Tp[n][m][0])*h[n][m][0]*dx*F0/lamb;

        Tc[0][0][kk]= F0*(Tp[0][1][kk]+Tp[1][0][kk]+Tp[0][0][kk-1]) + Tp[0][0][kk]*(1-3*F0) + b*(Cp[0][0][kk]-Cc[0][0][kk])/dt + 3*(Text-Tp[0][0][kk])*h[0][0][kk]*dx*F0/lamb;
        Tc[n][0][kk]= F0*(Tp[n][1][kk]+Tp[n-1][0][kk]+Tp[n][0][kk-1]) + Tp[n][0][kk]*(1-3*F0) + b*(Cp[n][0][kk]-Cc[n][0][kk])/dt + 3*(Text-Tp[n][0][kk])*h[n][0][kk]*dx*F0/lamb;
        Tc[0][m][kk]= F0*(Tp[0][m-1][kk]+Tp[1][m][kk]+Tp[0][m][kk-1]) + Tp[0][m][kk]*(1-3*F0) + b*(Cp[0][m][kk]-Cc[0][m][kk])/dt + 3*(Text-Tp[0][m][kk])*h[0][m][kk]*dx*F0/lamb;
        Tc[n][m][kk]= F0*(Tp[n][m-1][kk]+Tp[n-1][m][kk]+Tp[n][m][kk-1]) + Tp[n][m][kk]*(1-3*F0) + b*(Cp[n][m][kk]-Cc[n][m][kk])/dt + 3*(Text-Tp[n][m][kk])*h[n][m][kk]*dx*F0/lamb;

        //for k in range (n-1):
		for ( k = 0 ; k < n-1 ; k++ ) {
          i = k+1;
          Tc[i][0][0]= F0*(Tp[i][1][0]+Tp[i][0][1]+Tp[i+1][0][0]+Tp[i-1][0][0]) + Tp[i][0][0]*(1-4*F0) + b*(Cp[i][0][0]-Cc[i][0][0])/dt + 2*(Text-Tp[i][0][0])*h[i][0][0]*dx*F0/lamb;
          Tc[i][m][0]= F0*(Tp[i][m-1][0]+Tp[i][m][1]+Tp[i+1][m][0]+Tp[i-1][m][0]) + Tp[i][m][0]*(1-4*F0) + b*(Cp[i][m][0]-Cc[i][m][0])/dt + 2*(Text-Tp[i][m][0])*h[i][m][0]*dx*F0/lamb;
          Tc[i][0][kk]= F0*(Tp[i][1][kk]+Tp[i][0][kk-1]+Tp[i+1][0][kk]+Tp[i-1][0][kk]) + Tp[i][0][kk]*(1-4*F0) + b*(Cp[i][0][kk]-Cc[i][0][kk])/dt + 2*(Text-Tp[i][0][kk])*h[i][0][kk]*dx*F0/lamb;
          Tc[i][m][kk]= F0*(Tp[i][m-1][kk]+Tp[i][m][kk-1]+Tp[i+1][m][kk]+Tp[i-1][m][kk]) + Tp[i][m][kk]*(1-4*F0) + b*(Cp[i][m][kk]-Cc[i][m][kk])/dt + 2*(Text-Tp[i][m][kk])*h[i][m][kk]*dx*F0/lamb;
          for (kkk = 0 ; kkk < kk-1 ; kkk++) {
            ll = kkk+1; //J'ai failli être coincé dans les notations ! Je sais, c'est dégeu, mais j'y peux rien...
            Tc[i][0][ll] = F0*(Tp[i][1][ll]+Tp[i+1][0][ll]+Tp[i-1][0][ll]+Tp[i][0][ll-1]+Tp[i][0][ll+1]) + Tp[i][0][ll]*(1-5*F0) + b*(Cp[i][0][ll]-Cc[i][0][ll])/dt + (Text-Tp[i][0][ll])*h[i][0][ll]*dx*F0/lamb;
            Tc[i][m][ll] = F0*(Tp[i][m-1][ll]+Tp[i+1][m][ll]+Tp[i-1][m][ll]+Tp[i][m][ll-1]+Tp[i][m][ll+1]) + Tp[i][m][ll]*(1-5*F0) + b*(Cp[i][m][ll]-Cc[i][m][ll])/dt + (Text-Tp[i][m][ll])*h[i][m][ll]*dx*F0/lamb;
          }
        }

        //for l in range (m-1):
		for ( l = 0 ; l < m-1 ; l++ ) {
            j = l+1;
            Tc[0][j][0]= F0*(Tp[1][j][0]+Tp[0][j-1][0]+Tp[0][j+1][0]+Tp[0][j][1]) + Tp[0][j][0]*(1-4*F0) + b*(Cp[0][j][0]-Cc[0][j][0])/dt + 2*(Text-Tp[0][j][0])*h[0][j][0]*dx*F0/lamb;
            Tc[n][j][0]= F0*(Tp[n-1][j][0]+Tp[n][j-1][0]+Tp[n][j+1][0]+Tp[n][j][1]) + Tp[n][j][0]*(1-4*F0) + b*(Cp[n][j][0]-Cc[n][j][0])/dt + 2*(Text-Tp[n][j][0])*h[n][j][0]*dx*F0/lamb;
            Tc[0][j][kk]= F0*(Tp[1][j][kk]+Tp[0][j-1][kk]+Tp[0][j+1][kk]+Tp[0][j][kk-1]) + Tp[0][j][kk]*(1-4*F0) + b*(Cp[0][j][kk]-Cc[0][j][kk])/dt + 2*(Text-Tp[0][j][kk])*h[0][j][kk]*dx*F0/lamb;
            Tc[n][j][kk]= F0*(Tp[n-1][j][kk]+Tp[n][j-1][kk]+Tp[n][j+1][kk]+Tp[n][j][kk-1]) + Tp[n][j][kk]*(1-4*F0) + b*(Cp[n][j][kk]-Cc[n][j][kk])/dt + 2*(Text-Tp[n][j][kk])*h[n][j][kk]*dx*F0/lamb;
            for (kkk = 0 ; kkk < kk-1 ; kkk++) {
              ll = kkk+1;
              Tc[0][j][ll] = F0*(Tp[1][j][ll]+Tp[0][j-1][ll]+Tp[0][j+1][ll]+Tp[0][j][ll+1]+Tp[0][j][ll-1]) + Tp[0][j][ll]*(1-5*F0) + b*(Cp[0][j][ll]-Cc[0][j][ll])/dt + (Text-Tp[0][j][ll])*h[0][j][ll]*dx*F0/lamb;
              Tc[n][j][ll] = F0*(Tp[n-1][j][ll]+Tp[n][j-1][ll]+Tp[n][j+1][ll]+Tp[n][j][ll+1]+Tp[n][j][ll-1]) + Tp[n][j][ll]*(1-5*F0) + b*(Cp[n][j][ll]-Cc[n][j][ll])/dt + (Text-Tp[n][j][ll])*h[n][j][ll]*dx*F0/lamb;
          }
        }
        for  ( kkk = 0 ; kkk < kk-1 ; kkk++){
            ll = kkk+1;
            Tc[0][0][ll]= F0*(Tp[1][0][ll]+Tp[0][1][ll]+Tp[0][0][ll-1]+Tp[0][0][ll+1]) + Tp[0][0][ll]*(1-4*F0) + b*(Cp[0][0][ll]-Cc[0][0][ll])/dt + 2*(Text-Tp[0][0][ll])*h[0][0][ll]*dx*F0/lamb;
            Tc[n][0][ll]= F0*(Tp[n-1][0][ll]+Tp[n][1][ll]+Tp[n][0][ll+1]+Tp[n][0][ll-1]) + Tp[n][0][ll]*(1-4*F0) + b*(Cp[n][0][ll]-Cc[n][0][ll])/dt + 2*(Text-Tp[n][0][ll])*h[n][0][ll]*dx*F0/lamb;
            Tc[0][m][ll]= F0*(Tp[1][m][ll]+Tp[0][m-1][ll]+Tp[0][m][ll-1]+Tp[0][m][ll+1]) + Tp[0][m][ll]*(1-4*F0) + b*(Cp[0][m][ll]-Cc[0][m][ll])/dt + 2*(Text-Tp[0][m][ll])*h[0][m][ll]*dx*F0/lamb;
            Tc[n][m][ll]= F0*(Tp[n-1][m][ll]+Tp[n][m-1][ll]+Tp[n][m][ll-1]+Tp[n][m][ll+1]) + Tp[n][m][ll]*(1-4*F0) + b*(Cp[n][m][ll]-Cc[n][m][ll])/dt + 2*(Text-Tp[n][m][ll])*h[n][m][ll]*dx*F0/lamb;

        }

		for ( k = 0 ; k < n-1 ; k++ ) {
            i = k+1;
           for (l = 0 ; l < m-1 ; l++) {
              j = l+1;
              Tc[i][j][0] = F0*(Tp[i-1][j][0]+Tp[i+1][j][0]+Tp[i][j-1][0]+Tp[i][j+1][0]+Tp[i][j][1]) + Tp[i][j][0]*(1-5*F0) + b*(Cp[i][j][0]-Cc[i][j][0])/dt + (Text-Tp[i][j][0])*h[i][j][0]*dx*F0/lamb;
              Tc[i][j][kk] = F0*(Tp[i-1][j][kk]+Tp[i+1][j][kk]+Tp[i][j-1][kk]+Tp[i][j+1][kk]+Tp[i][j][kk-1]) + Tp[i][j][kk]*(1-5*F0) + b*(Cp[i][j][kk]-Cc[i][j][kk])/dt + (Text-Tp[i][j][kk])*h[i][j][kk]*dx*F0/lamb;
          }
        }

        //for i in range (n-1):
		for ( i = 0 ; i < n-1 ; i++ ) {

            //for j in range (m-1): //#lets write the inside of the matrix
			for ( j = 0 ; j < m-1 ; j++ )  {
                    for (kkk = 0 ; kkk < kk-1 ; kkk++){
                        k = i+1;
                        l = j+1;
                        ll = kkk+1;
                        Tc[k][l][ll] = F0*(Tp[k-1][l][ll]+Tp[k+1][l][ll]+Tp[k][l-1][ll]+Tp[k][l+1][ll]+Tp[k][l][ll-1]+Tp[k][l][ll+1]) + Tp[k][l][ll]*(1-6*F0) + b*(Cp[k][l][ll]-Cc[k][l][ll])/dt;
                    }
			}
        }
        //And now, let's fill the pipe
        Tpipe[0] = Tentry;

        for (i = 1 ; i < lpipe ; i++){
            j = pipe[i][0];
            k = pipe[i][1];
            l = pipe[i][2];
            Tpipe[i] = (1-fpipe)*Tpipe[i-1] + fpipe*Tc[j][k][l];
            Tc[j][k][l] = (1-fpiperec)*Tc[j][k][l] + fpiperec*Tpipe[i];
        }


        if (nb == n1ech) {


      // chaine.extend ('temps(s) : ')
		   strcpy(ch1,"  Temps(s) : ");
           fprintf(fp2,ch1);

            //chaine.extend (str(z*dt))
			sprintf(ch1, "%f", z*dt);
            fprintf(fp2,ch1);

            for (i = 0; i < lcapt ; i++) {
                strcpy(ch1,"  capteur ");
                fprintf(fp2,ch1);
                sprintf(ch1,"%d" , i);
                fprintf(fp2,ch1);
                strcpy(ch1," ");
                fprintf(fp2,ch1);
                sprintf(ch1, "%f", Tc[captors[i][0]][captors[i][1]][captors[i][2]]);
                fprintf(fp2,ch1);
            }
            strcpy(ch1,"  concentration 0  ");
            fprintf(fp2,ch1);
            sprintf(ch1, "%f", Cc[captors[0][0]][captors[0][1]][captors[0][2]]);
            fprintf(fp2,ch1);
            strcpy(ch1,"  Tsortie  ");
            fprintf(fp2,ch1);
            sprintf(ch1, "%f", Tpipe[lpipe-1]);
            fprintf(fp2,ch1);
            //chaine.extend ("\n")
            strcpy(ch1,"\n");
            fprintf(fp2,ch1);

            //remplissage du fichier température capteur 0

            sprintf(ligne, "%f", Tc[captors[0][0]][captors[0][1]][captors[0][2]]);
            fprintf(fp4,ligne);
            strcpy(ch1,"\n");
            fprintf(fp4,ch1);


            nb = 1;
			}  // fin de if nb ==n1ech
        else {
            nb = nb+1;
			}  // fin du else

       // Cp = Cc+C0-C0  // Cp = copie de Cc
       copymatrix(Cc,Cp);

      //  Tp = Tc+T0-T0   // Tp = copie de Tc
      copymatrix(Tc,Tp);

 //printf("%i   " , z );

  }  // fin de for   z	: boucle principale de calculs

  // fermer fichier 2
   fclose(fp2);
   fclose(fp4);

    //écriture du fichier.dat

if (ouiounon == 1){  //si il faut faire un dessin
    fp3 = fopen("data.dat","w");
    if (perpen == 1){
        for (j = 1; j < colonnes; j++){
            for (k = 0; k < altitude; k++){
                sprintf(ligne, "%d", j);
                fprintf(fp3,ligne);
                sprintf(ligne, " ");
                fprintf(fp3,ligne);
                sprintf(ligne, "%d", k);
                fprintf(fp3,ligne);
                sprintf(ligne, " ");
                fprintf(fp3,ligne);
                sprintf(ligne, "%f", Tc[coupur][j][k]);
                fprintf(fp3,ligne);
                sprintf(ligne, "\n");
                fprintf(fp3,ligne);
            }
            sprintf(ligne, "\n");
            fprintf(fp3,ligne);
        }
    }
    if (perpen == 2){
        for (i = 1 ; i < lignes; i++){
            for (k = 0; k < altitude; k++){
                sprintf(ligne, "%d", i);
                fprintf(fp3,ligne);
                sprintf(ligne, " ");
                fprintf(fp3,ligne);
                sprintf(ligne, "%d", k);
                fprintf(fp3,ligne);
                sprintf(ligne, " ");
                fprintf(fp3,ligne);
                sprintf(ligne, "%f", Tc[i][coupur][k]);
                fprintf(fp3,ligne);
                sprintf(ligne, "\n");
                fprintf(fp3,ligne);
            }
            sprintf(ligne, "\n");
            fprintf(fp3,ligne);
        }
    }
    if (perpen == 3){
        for (i = 1 ; i < lignes; i++){
            for (j= 0; j < colonnes; j++){
                sprintf(ligne, "%d", i);
                fprintf(fp3,ligne);
                sprintf(ligne, " ");
                fprintf(fp3,ligne);
                sprintf(ligne, "%d", j);
                fprintf(fp3,ligne);
                sprintf(ligne, " ");
                fprintf(fp3,ligne);
                sprintf(ligne, "%f", Tc[i][j][coupur]);
                fprintf(fp3,ligne);
                sprintf(ligne, "\n");
                fprintf(fp3,ligne);
            }
            sprintf(ligne, "\n");
            fprintf(fp3,ligne);
        }
    }
    fclose(fp3); //fermeture du fichier
    fclose(fp4);
}

// sortie de chaine
//printf("%s \n" , chaine );

// fin du programme

printf("\n---------------F  I  N  ---------------------\n");
printf("\n\n");

return 0;
}
