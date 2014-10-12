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
    baliseDebutModele=sp.K(r'<MODELE>')
    baliseFinTas=sp.K(r'</TAS>')
    baliseFinCapteurs=sp.K(r'</CAPTEURS>')
    baliseFinDimensions=sp.K(r'</DIMENSIONS>')
    baliseFinIsolation=sp.K(r'</ISOLATION>')
    baliseFinTuyau=sp.K(r'</TUYAU>')
    baliseFinImage=sp.K(r'</IMAGE>')
    baliseFinModele=sp.K(r'</MODELE>')
#########################################################################################################
    with sp.Separator(blancs|commentaire):  #on fait abstraction des blancs et des commentaires
        tas=sp.Rule()
        blocDimensions=sp.Rule()
        blocCapteurs=sp.Rule()
        blocIsolation=sp.Rule()
        blocTuyau=sp.Rule()
        blocImage=sp.Rule()
        blocModele=sp.Rule()
        
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

        blocMod=sp.Rule()
        blocKrz=sp.Rule()
        blocKru=sp.Rule()
        blocEchim=sp.Rule()
        blocText=sp.Rule()
        blocTint=sp.Rule()
        
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

        mod=sp.Rule()
        krz=sp.Rule()
        kru=sp.Rule()
        echim=sp.Rule()
        text=sp.Rule()
        tint=sp.Rule()
##########################################################################################################
        tas|=baliseDebutTas & 'TAS Name="' & nomTas &'" >' & blocDimensions & blocCapteurs & blocIsolation & blocLambda & blocRho & blocC & blocDt & blocTech & blocDx & blocDuree & blocModele & blocTuyau & blocImage & baliseFinTas
        blocDimensions|=baliseDebutDimensions & dimension[0:] & baliseFinDimensions
        blocCapteurs|=baliseDebutCapteurs & capteur[0:] & baliseFinCapteurs
        blocIsolation|=baliseDebutIsolation & isolation[0:] & baliseFinIsolation
        blocTuyau|=baliseDebutTuyau & blocTentry & blocV & blocCeau & blocLambdpipe & blocRext & blocRint & blocHpipe & baliseFinTuyau
        blocImage|=baliseDebutImage & blocYN & blocPerpen & blocCote & baliseFinImage
        blocModele|=baliseDebutModele & blocMod & blocKrz & blocKru & blocEchim & blocText& blocTint & baliseFinModele
        
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

        blocMod|='modele =' & mod
        blocKrz|='kreac0 =' & krz
        blocKru|='kreac1 =' & kru
        blocEchim|='Echim =' & echim
        blocText|='Text =' & text
        blocTint|='Tint =' & tint
       
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

        mod|=nombre & ';'
        krz|=nbVirgule & ';'
        kru|=nbVirgule & ';'
        echim|=nbVirgule & ';'
        text|=nbVirgule & ';'
        tint|=nbVirgule & ';'

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

0.48; #gauche
0.3; #droite
1.8; #devant
1.2; #derrière
0.48; #bas
0.48; #haut

</ISOLATION>

lamb = 0.3 ;#lambda is the thermal conduction [Wm-1K-1]
rho = 800 ;#rho is the volumic mass [kgm-3]
c = 3300 ;#c is the thermal massic capacity [Jkg-1K-1]
dt = 30.0 ;#dt is the time quantum[s]
tech = 1200 ;#we take the values every 20 minutes
dx = 0.04 ;#dx is the space quantum[m]
duree = 300000 ;#duree is the time the study is during

<MODELE>

modele = 1 ; #you choose the order of the chemical reaction (0 or 1)
kreac0 = 90.0 ; #we want to identify this parameter[J.s-1.m-3] if modele=0
kreac1 = 0.0000000000000009 ; # [s-1.m-3] if modele=1
Echim = 100000000.0 ;#this is the concentration of chemical energy at the beginning : we want to identify it too [J/m^3]
Text = 11.0 ;#this is the temperature in the edge
Tint = 11.0 ; #The temperature at t=0 in the compost heap

</MODELE>

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
    donnees=traduire(texte1)        # ici, on choisit le texte entré en données
except SyntaxError as erreur :
    print("-Erreur à[ligne,colonne]")
    print('    ',erreur)"""
