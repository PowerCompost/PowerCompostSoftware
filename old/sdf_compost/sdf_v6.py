#ps6 : simulation diff finies 3D

from numpy import *
from math import exp
import subprocess
from sdf_parser import parser
import sp

def approximation(x):
    if x-int(x) >= 0.5:
        return int(x+1)
    else :
        return int(x)

def lecture(donnees):
    
    g = open(donnees,'r')

    texte = ''

    for i in g:
        texte = texte + i

    g.close

    traduire = parser()

    try :
        donnees=traduire(texte)        # ici, on choisit le texte entré en données (texte1 = données de base pour l'exercice)
    except SyntaxError as erreur :
        print("-Erreur à[ligne,colonne]")
        print('    ',erreur)
    
    nameOfTheHeap = donnees[0]

    kreac = 90 #we want to identify this parameter[J.s-1.m-3]
    Echim = 1e8 #this is the concentration of chemical energy at the beginning : we want to identify it too [J/m^3]
    Text = 11. #this is the temperature in the edge


    en = donnees[1][0] #
    em = donnees[1][1] # kk, n and m are the dimensions of the heap
    kaka = donnees[1][2]#
    capteurs = donnees[2] #without captors, you can't see anything !

    lamb = donnees[4] #lambda is the thermal conduction [Wm-1K-1]
    rho = donnees[5] #rho is the volumic mass [kgm-3]
    c = donnees[6] #c is the thermal massic capacity [Jkg-1K-1]
    dt = donnees[7] #dt is the time quantum[s]
    tech = donnees[8]  #we take the values every 20 minutes
    dx = donnees[9] #dx is the space quantum[m]
    duree = donnees[10] #duree is the time the study is during

    n = approximation(en/dx)-1 #
    m = approximation(em/dx)-1 # kk, n and m are the dimensions of the matrix T0 (and of every T[z])
    kk = approximation(kaka/dx)-1#

    T0 = [mat([[11.]*(m+1)]*(n+1))]*(kk+1)  #T0 is the temperature of the compost at the beginning of the study  112*76*75
    #T0 and C0 are lists of matrix : the third dimension is the cote (sorry, I didn't found better)
    
    C0 = [mat([[Echim]*(m+1)]*(n+1))]*(kk+1)       #C0 is the concentration of reagent

    R = 8.314 #the gaz constant [JK-1mol-1]
    hth = [mat([[1.]*(m+1)]*(n+1))]*(kk+1) # h is the edge thermic impedance it's a matrix so that it will be possible to make a non uniform insulation [WK-1m-2]
    hr = 4*5.67*0.00000001*0.9*(273.15+Text)**3
    hc = 2.8
    h = [mat([[1.]*(m+1)]*(n+1))]*(kk+1)

    #lets build the matrix hth
    a0 = donnees[3][0]
    a1 = donnees[3][1]
    a2 = donnees[3][2]
    a3 = donnees[3][3]
    a4 = donnees[3][4]
    a5 = donnees[3][5]

    for i in range(n+1):
        for k in range(kk+1):
            hth[k][i,0] = a2
            hth[k][i,m] = a3

    for j in range(m+1):
        for k in range(kk+1):
            hth[k][0,j] = a0
            hth[k][n,j] = a1

    for i in range(n+1):
        for j in range(m+1):
            hth[0][i,j] = a4
            hth[kk][i,j] = a5
        
    #lets build the matrix h of thermic impedence with the folowing formula : htot = 1/(1/hth+1/hrhc)

    for i in range(n+1):
        for j in range (m+1):
            for k in range(kk+1):
                h[k][i,j] = 1/(1/hth[k][i,j]+1/(hr+hc))
    pipe = []
    for i in range(n+1):
        pipe.append([i,5,5])


    nech = duree/dt      #it is the number of time quantums
    n1ech = int(tech/dt)  #it is the number of values taken
    a = lamb/(rho*c) #[m2/s]
    b = dt/(rho*c)   #[Km3W-1]
    F0 = a*dt/(dx*dx)   #[1] a necessary condition is that 4F0<1
    print(6*F0,3*h[0][0,0]*F0*dx/lamb)

###########################################################################################################

    nb = 0 #nb is the number of time quantums for the last value taking

    print ('début de la simulation')

    f = open('fich_echangev6_sdf.txt','w')

    #let's write the floats for C
    f.write (str(F0))
    f.write ('\n')
    f.write (str(b))
    f.write ('\n')
    f.write (str(lamb))
    f.write ('\n')
    f.write (str(Text))
    f.write ('\n')
    f.write (str(dx))
    f.write ('\n')
    f.write (str(nech))
    f.write ('\n')
    f.write (str(dt))
    f.write ('\n')
    f.write (str(kreac))
    f.write ('\n')
    f.write (str(tech))
    f.write ('\n')

    # The integers
    f.write (str(n1ech))
    f.write ('\n')

    #and the matrix
    f.write(str(shape(T0)[1]))#n+1
    f.write ('\n')
    f.write(str(shape(T0)[2]))#m+1
    f.write ('\n')
    f.write(str(shape(T0)[0]))#kk+1
    f.write ('\n')
    for i in range(n+1):
        for j in range(m+1):
            for k in range(kk+1):
                f.write(str([h[k][i,j]][0]))
                f.write ('\n')
                f.write(str([T0[k][i,j]][0]))
                f.write ('\n')
                f.write(str([C0[k][i,j]][0]))
                f.write ('\n')

    #the pipe is coming !

    Tentry = donnees[11][0] #Tentry is the température of the incoming water [°C]
    v = donnees[11][1] #v is the speed of the water in the pipe [m/s]
    ceau = donnees[11][2] #ceau is the thermal capacity of water [J/K/kg]
    lambdpipe = donnees[11][3] #lambdpipe is the thermal conduction of the pipe [W/K/m]
    rext = donnees[11][4]
    rint = donnees[11][5]
    hpipe = donnees[11][6] #hpipe is the convection coefficient

    Zpipe = log(rext/rint)/(2*3.14159*lambdpipe*dx) #this is the thermal resistance of one elementary segment of the pipe [K/W]
    S = 3.14159*rint**2 #the section of the pipe[m^2]
    meau = 1000*S*v*dt #meau is the mass of water between two time quanta [kg]
    if v == 0:
        texposition = 0 #because something/0 = error
        texposurzmcv = 0 #because 0/0 = error
        fpiperec = 0 #because error = error
    else :
        texposition = dx/v #texposition is the time during which the water is in contact with every little block.
        texposurzmcv = texposition/(Zpipe*meau*ceau) #those four are always together ! [1]
        fpiperec = dt/(Zpipe*rho*dx**3*c) #I need it [1]

    print(texposurzmcv) #it must be < 1
    print(fpiperec) #it must be < 1

    f.write(str(texposurzmcv))
    f.write('\n')
    f.write(str(len(pipe)))
    f.write ('\n')
    f.write(str(fpiperec))
    f.write ('\n')
    f.write(str(Tentry))
    f.write ('\n')
    for i in pipe :
        for j in range(3):
            f.write(str(i[j]))
            f.write ('\n')

    #and the captors :
    f.write(str(len(capteurs)))
    f.write ('\n')
    for i in capteurs :
        for j in range(3):
            f.write(str(approximation(i[1][j]/dx))) #don't forget the dx
            f.write ('\n')

    #The picture :

    YN = donnees[12][0]
    perpen = donnees[12][1]
    cote = donnees[12][2]
    if YN == 'y':
        f.write(str(1))
    else :
        f.write(str(0))
    f.write ('\n')
    if perpen == 'x':
        f.write(str(1))
    if perpen == 'y':
        f.write(str(2))
    if perpen == 'z':
        f.write(str(3))
    f.write ('\n')
    f.write(str(approximation(cote/dx)))
    f.write ('\n')
    f.write
            
    #number of characters the programm.c will write : 15*number of writings = 15*duree/tech = 12*15*dt*nech/tech pour 

    f.close()
    
    chainecar = 'résultat3D_Echim' + str(Echim) +'kreac'+ str(kreac) + 'Veau' + str(v) + '.txt'

    chaine = subprocess.check_output(["c_sdf_2.exe",chainecar])

    print("fin")

lecture('donnees_sdf.txt')
