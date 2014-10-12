#parser

import sp

def parser():
    commentaire=sp.R(r'#.*')
    blancs=sp.R(r'\s+')
    nom=sp.R(r'[a-zA-Z]')
    nomTas=sp.R(r'\w+')
    nombre=sp.R(r'[0-9]+') / int  #convertit le nombre en entier
    nbVirgule=sp.R(r'[0-9]+'+'.'+'[0-9]+') /float
    baliseDebutTas=sp.K(r'<')
    baliseDebutCapteurs=sp.K(r'<CAPTEURS>')
    baliseDebutDimensions=sp.K(r'<DIMENSIONS>')
    baliseDebutIsolation=sp.K(r'<ISOLATION>')
    baliseDebutTuyau=sp.K(r'<TUYAU>')
    baliseDebutImage=sp.K(r'<IMAGE>')
    baliseFinTas=sp.K(r'</TAS>')
    baliseFinCapteurs=sp.K(r'</CAPTEURS>')
    baliseFinDimensions=sp.K(r'</DIMENSIONS>')
    baliseFinIsolation=sp.K(r'</ISOLATION>')
    baliseFinTuyau=sp.K(r'</TUYAU>')
    baliseFinImage=sp.K(r'</IMAGE>')
#########################################################################################################
    with sp.Separator(blancs|commentaire):  #on fait abstraction des blancs et des commentaires
        tas=sp.Rule()
        blocDimensions=sp.Rule()
        blocCapteurs=sp.Rule()
        blocIsolation=sp.Rule()
        blocTuyau=sp.Rule()
        blocImage=sp.Rule()
        
        blocTentry=sp.Rule()
        blocV=sp.Rule()
        blocCeau=sp.Rule()
        blocLambdpipe=sp.Rule()
        blocRext=sp.Rule()
        blocRint=sp.Rule()
        blocHpipe=sp.Rule()
        
        blocLambda=sp.Rule()
        blocRho=sp.Rule()
        blocC=sp.Rule()
        blocDt=sp.Rule()
        blocTech=sp.Rule()
        blocDx=sp.Rule()
        blocDuree=sp.Rule()

        blocYN=sp.Rule()
        blocPerpen=sp.Rule()
        blocCote=sp.Rule()
        
        dimension=sp.Rule()
        capteur=sp.Rule()
        isolation=sp.Rule()
        lamb=sp.Rule()
        rho=sp.Rule()
        c=sp.Rule()
        dt=sp.Rule()
        tech=sp.Rule()
        dx=sp.Rule()
        duree=sp.Rule()

        tentry=sp.Rule()
        v=sp.Rule()
        ceau=sp.Rule()
        lambdpipe=sp.Rule()
        rext=sp.Rule()
        rint=sp.Rule()
        hpipe=sp.Rule()

        YN=sp.Rule()
        perpen=sp.Rule()
        cote=sp.Rule()
##########################################################################################################
        tas|=baliseDebutTas & 'TAS Name="' & nomTas &'" >' & blocDimensions & blocCapteurs & blocIsolation & blocLambda & blocRho & blocC & blocDt & blocTech & blocDx & blocDuree & blocTuyau & blocImage & baliseFinTas
        blocDimensions|=baliseDebutDimensions & dimension[0:] & baliseFinDimensions
        blocCapteurs|=baliseDebutCapteurs & capteur[0:] & baliseFinCapteurs
        blocIsolation|=baliseDebutIsolation & isolation[0:] & baliseFinIsolation
        blocTuyau|=baliseDebutTuyau & blocTentry & blocV & blocCeau & blocLambdpipe & blocRext & blocRint & blocHpipe & baliseFinTuyau
        blocImage|=baliseDebutImage & blocYN & blocPerpen & blocCote & baliseFinImage
        
        blocTentry|='Tentry =' & tentry
        blocV|='v =' & v
        blocCeau|='ceau =' & ceau
        blocLambdpipe|='lambdpipe =' & lambdpipe
        blocRext|='rext =' & rext
        blocRint|='rint =' & rint
        blocHpipe|='hpipe =' & hpipe
        
        blocLambda|='lamb =' & lamb
        blocRho|='rho =' & rho
        blocC|='c =' & c
        blocDt|='dt =' & dt
        blocTech|='tech =' & tech
        blocDx|='dx =' & dx
        blocDuree|='duree =' & duree

        blocYN|='YN =' & YN
        blocPerpen|='_|_' & perpen
        blocCote|='cote =' & cote
       
        capteur|=nom & ':' & (nbVirgule & ',')[0:] & ';'
        dimension|=nbVirgule & ';'
        isolation|=nbVirgule & ';'
        lamb|=nbVirgule & ';'
        rho|=nbVirgule & ';'
        c|=nbVirgule & ';'
        dt|=nbVirgule & ';'
        tech|=nbVirgule & ';'
        dx|=nbVirgule & ';'
        duree|=nbVirgule & ';'

        tentry|=nbVirgule & ';'
        v|=nbVirgule & ';'
        ceau|=nbVirgule & ';'
        lambdpipe|=nbVirgule & ';'
        rext|=nbVirgule & ';'
        rint|=nbVirgule & ';'
        hpipe|=nbVirgule & ';'

        YN|=nom & ';'
        perpen|=nom & ';'
        cote|=nbVirgule & ';'

    return tas

"""traduire=parser()

texte1=""""""
<TAS Name="Tas1" >

# Description textuelle cohérente

<DIMENSIONS>
1.12 ;
0.70 ;
0.70 ;
</DIMENSIONS>

<CAPTEURS>
a : 0.1 , 0.1 , 0.1 ,;
</CAPTEURS>

<ISOLATION>

0.2;
0.3;
0.1;
0.2;
0.2;
0.2;

</ISOLATION>

lamb = 0.3 ;#lambda is the thermal conduction [Wm-1K-1]
rho = 800 ;#rho is the volumic mass [kgm-3]
c = 3300 ;#c is the thermal massic capacity [Jkg-1K-1]
dt = 30.0 ;#dt is the time quantum[s]
tech = 1200 ;#we take the values every 20 minutes
dx = 0.04 ;#dx is the space quantum[m]
duree = 300000 ;#duree is the time the study is during

<TUYAU>
Tentry = 11.0 ;#Tentry is the température of the incoming water [°C]
v = 0.01 ;#v is the speed of the water in the pipe [m/s]
ceau = 4185.0 ;#ceau is the thermal capacity of water [J/K/kg]
lambdpipe = 0.1 ;#lambdpipe is the thermal conduction of the pipe [W/K/m]
rext = 0.02 ;
rint = 0.018 ;
hpipe = 2.8 ;#hpipe is the convection coefficient
</TUYAU>

<IMAGE>

YN = y ; #do you want a picture ?
_|_ x ;#le plan de l'image est perpendiculaire à x
cote = 0.60 ;

</IMAGE>

</TAS>
"""

"""try :
    donnees=traduire(texte1)        # ici, on choisit le texte entré en données (texte1 = données de base pour l'exercice)
except SyntaxError as erreur :
    print("-Erreur à[ligne,colonne]")
    print('    ',erreur)

print(donnees)"""
