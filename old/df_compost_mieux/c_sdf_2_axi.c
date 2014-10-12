#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int main(int argc,char **argv)
{

//Variables entrantes :

// H  devient Hbig ; L devient Lbig
double F0,b,lamb,Text,dx, nech,dt,kreacz,kreacu,fpipe,Tentry,fpiperec = 0.0;
double Pi = 3.14159;

int n1ech = 0;

// dimensions de Matrices
int lignes , colonnes , altitude = 0;

//Variables sortantes :

// taille des chaines auxiliaires
int nbcar2=300;


// chaines auxiliaires
char ch1[nbcar2] ;
char ligne[nbcar2];
int ouiounon;

FILE *fp3; //donnees pour gnuplot
// variables internes
int nb, z, i, j , k, l, kk, lig, col, n, m, lpipe, lcapt, modele, lr = 0;

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


// entiers
fgets(ligne,nbcar2,fp);
n1ech= atoi(ligne);

fgets(ligne,nbcar2,fp);
lignes = atoi(ligne);

fgets(ligne,nbcar2,fp);
colonnes = atoi(ligne);

fgets(ligne,nbcar2,fp);
lr = atoi(ligne);

//Matrix :

//h,T0,C0
//float M[nlig][ncol];

float h[lignes][colonnes];
float T0[lignes][colonnes];
float C0[lignes][colonnes];

float Tp[lignes][colonnes];
float Cp[lignes][colonnes];

float Tc[lignes][colonnes];
float Cc[lignes][colonnes];

float r[lr];

// remplit les trois Matrices
for (lig = 0; lig < lignes; lig++) {
  for (col = 0; col < colonnes; col++) {
      fgets(ligne,nbcar2,fp);
      h[lig][col] = atof(ligne);

      fgets(ligne,nbcar2,fp);
      T0[lig][col] = atof(ligne);

      fgets(ligne,nbcar2,fp);
      C0[lig][col] = atof(ligne);
    }
  }

// le rayon
for (lig = 0; lig < lr; lig++){
    fgets(ligne,nbcar2,fp);
    r[lig] = atof(ligne);
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

int pipe[lpipe][2];
float Tpipe[lpipe];

for (i = 0; i < lpipe; i++) {
    for (j = 0; j < 2; j++) {
        fgets(ligne,nbcar2,fp);
        pipe[i][j] = atoi(ligne);
    }
}

//the captors
fgets(ligne,nbcar2,fp);
lcapt = atoi(ligne);

int captors[lcapt][2];

for (i = 0; i < lcapt; i++) {
    for (j = 0; j < 2; j++) {
        fgets(ligne,nbcar2,fp);
        captors[i][j] = atoi(ligne);
    }
}

fgets(ligne,nbcar2,fp);
ouiounon = atoi(ligne);

 // fin entree variables

 // fermer fichier
 fclose(fp);

 n=lignes-1;
 m=colonnes-1;
 kk=altitude-1;


// fonctions internes

// copie une matrice :  m2 = m1
void copymatrix(float m1[][colonnes],  float m2[][colonnes]) {
//int lig,col =0;
  for (lig = 0; lig < lignes; lig++) {
    for (col = 0; col < colonnes; col++) {
        m2[lig][col] = m1[lig][col];
        	}
      }
 } // fin de fonction Copie matrice


// double une matrice :   m2 = m1 * 2.0
void doublematrix(float m1[][colonnes],  float m2[][colonnes]) {
int lig,col =0;
  for (lig = 0; lig < lignes; lig++) {
    for (col = 0; col < colonnes; col++) {
         m2[lig][col] = m1[lig][col]* 2.0;
    }
  }
 } // fin de fonction double matrice

// fonctions de modèles bio-chimiques

void ordrezero(float a, float dt, float kreacz, float b){ // b=ordrezero(a)
    Cc[i][j] = Cp[i][j] - dt*kreacz;
    if (Cc[i][j] <= 0.) {
        Cc[i][j] = 0.;
        }
}

void ordreun(float a, float dt, float kreacu, float b){
    Cc[i][j] = Cp[i][j]*(1- dt*kreacu);
}

void ordrecomb(float a, float dt, float kreacz, float kreacu, float b){
    Cc[i][j] = Cp[i][j]*(1- dt*kreacu) - dt * kreacz;
    if (Cc[i][j] <= 0.) {
        Cc[i][j] = 0.;
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
	sprintf(ch1, "%f", T0[captors[i][0]][captors[i][1]]);
    fprintf(fp2,ch1);
  }
    strcpy(ch1,"  concentration 0  ");
    fprintf(fp2,ch1);
	sprintf(ch1, "%f", C0[captors[0][0]][captors[0][1]]);
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
                        if (modele == 0){
                            ordrezero(Cp[i][j],dt,kreacz,Cc[i][j]);
                        }
                        if (modele == 1) {
                            ordreun(Cp[i][j],dt,kreacu,Cc[i][j]);
                        }
                        if (modele == 2) {
                            ordrecomb(Cp[i][j],dt,kreacz,kreacu,Cc[i][j]);
                        }
            }
		}
  //  #lets write the edge conditions for temperature:

        Tc[0][0] = F0*(Tp[0][1]+(r[0]+dx/2)*Tp[1][0]) + Tp[0][0]*(1-(1+r[0]+dx/2)*F0) + b*(Cp[0][0]-Cc[0][0])/dt + (Text-Tp[0][0])*h[0][0]*r[0]*2*Pi*F0/lamb  ;// ce coin est du cote de l'axe : pas de 2
        Tc[n][0] = F0*(Tp[n][1]+(r[n]-dx/2)*Tp[n-1][0]) + Tp[n][0]*(1-(1+r[n]-dx/2)*F0) + b*(Cp[n][0]-Cc[n][0])/dt + 2*(Text-Tp[n][0])*h[n][0]*r[n]*2*Pi*F0/lamb ;
        Tc[0][m] = F0*(Tp[0][m-1]+(r[0]+dx/2)*Tp[1][m]) + Tp[0][m]*(1-(1+r[0]+dx/2)*F0) + b*(Cp[0][m]-Cc[0][m])/dt + (Text-Tp[0][m])*h[0][m]*r[0]*2*Pi*F0/lamb ; // ce coin est du cote de l'axe : pas de 2
        Tc[n][m] = F0*(Tp[n][m-1]+(r[n]-dx/2)*Tp[n-1][m]) + Tp[n][m]*(1-(1+r[n]-dx/2)*F0) + b*(Cp[n][m]-Cc[n][m])/dt + 2*(Text-Tp[n][m])*h[n][m]*r[n]*2*Pi*F0/lamb ;

        //for k in range (n-1):
		for ( k = 0 ; k < n-1 ; k++ ) {
          i = k+1;
          Tc[i][0] = F0*(Tp[i][1]+(r[i]+dx/2)*Tp[i+1][0]+(r[i]-dx/2)*Tp[i-1][0]) + Tp[i][0]*(1-(1+2*r[i])*F0) + b*(Cp[i][0]-Cc[i][0])/dt + (Text-Tp[i][0])*h[i][0]*r[i]*2*Pi*F0/lamb;
          Tc[i][m] = F0*(Tp[i][m-1]+(r[i]+dx/2)*Tp[i+1][m]+(r[i]-dx/2)*Tp[i-1][m]) + Tp[i][m]*(1-(1+2*r[i])*F0) + b*(Cp[i][m]-Cc[i][m])/dt + (Text-Tp[i][m])*h[i][m]*r[i]*2*Pi*F0/lamb;

        }

        //for l in range (m-1):
		for ( l = 0 ; l < m-1 ; l++ ) {
            j = l+1;
            Tc[0][j] = F0*((r[0]+dx/2)*Tp[1][j]+Tp[0][j+1]+Tp[0][j-1]) + Tp[0][j]*(1-(2+r[0]+dx/2)*F0) + b*(Cp[0][j]-Cc[0][j])/dt; // pas de perte de chaleur par l'axe
            Tc[n][j] = F0*(Tp[n][j+1]+(r[n]-dx/2)*Tp[n-1][j]+Tp[n][j-1]) + Tp[n][j]*(1-(2+r[n]-dx/2)*F0) + b*(Cp[n][j]-Cc[n][j])/dt + (Text-Tp[n][j])*h[n][j]*2*Pi*r[n]*F0/lamb;

        }


        //for i in range (n-1):
		for ( i = 0 ; i < n-1 ; i++ ) {

            //for j in range (m-1): //#lets write the inside of the matrix
			for ( j = 0 ; j < m-1 ; j++ )  {
                        k = i+1;
                        l = j+1;
                        Tc[k][l] = F0*((r[k]-dx/2)*Tp[k-1][l]+(r[k]+dx/2)*Tp[k+1][l]+Tp[k][l-1]+Tp[k][l+1]) + Tp[k][l]*(1-(2+2*r[k])*F0) + b*(Cp[k][l]-Cc[k][l])/dt;
			}
        }
        //And now, let's fill the pipe
        Tpipe[0] = Tentry;

        for (i = 1 ; i < lpipe ; i++){
            j = pipe[i][0];
            k = pipe[i][1];
            l = pipe[i][2];
            Tpipe[i] = (1-fpipe)*Tpipe[i-1] + fpipe*Tc[j][k];
            Tc[j][k] = (1-fpiperec)*Tc[j][k] + fpiperec*Tpipe[i];
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
                sprintf(ch1, "%f", Tc[captors[i][0]][captors[i][1]]);
                fprintf(fp2,ch1);
            }
            strcpy(ch1,"  concentration 0  ");
            fprintf(fp2,ch1);
            sprintf(ch1, "%f", Cc[captors[0][0]][captors[0][1]]);
            fprintf(fp2,ch1);
            strcpy(ch1,"  Tsortie  ");
            fprintf(fp2,ch1);
            sprintf(ch1, "%f", Tpipe[lpipe-1]);
            fprintf(fp2,ch1);
            //chaine.extend ("\n")
            strcpy(ch1,"\n");
            fprintf(fp2,ch1);

            //remplissage du fichier température capteur 0

            sprintf(ligne, "%f", Tc[captors[0][0]][captors[0][1]]);
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

    //écriture du fichier.dat

if (ouiounon == 1){  //si il faut faire un dessin
    fp3 = fopen("data.dat","w");

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
                sprintf(ligne, "%f", Tc[i][j]);
                fprintf(fp3,ligne);
                sprintf(ligne, "\n");
                fprintf(fp3,ligne);
            }
            sprintf(ligne, "\n");
            fprintf(fp3,ligne);
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
