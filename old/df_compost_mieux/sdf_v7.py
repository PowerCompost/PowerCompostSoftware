#ps6 : simulation diff finies 3D

from numpy import *
from math import *
import subprocess
from sdf_parser import parser
from sdf_ax_parser import ax_parser
import sp
from tkinter import *
import os

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
        donnees=traduire(texte)        # ici, on choisit le texte entré en données
    except SyntaxError as erreur :
        print("-Erreur à[ligne,colonne]")
        print('    ',erreur)
    
    nameOfTheHeap = donnees[0]

    modele = donnees[11][0]  #this decides the model chosen (0 or 1)
    kreac0 = donnees[11][1] #90 #we want to identify this parameter[J.s-1.m-3]
    kreac1 = donnees[11][2] #8e-7 #[s-1.m-3]
    Echim = donnees[11][3] #1e8 #this is the concentration of chemical energy at the beginning : we want to identify it too [J/m^3]
    Text = donnees[11][4] #this is the temperature in the edge
    Tint = donnees[11][5] #this is the temperature in the heap at the beginning
    
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

    T0 = [mat([[Tint]*(m+1)]*(n+1))]*(kk+1)  #T0 is the temperature of the compost at the beginning of the study  112*76*75

    #T0 and C0 are lists of matrix : the third dimension is the cote (sorry, I didn't found better)
    
    C0 = [mat([[Echim]*(m+1)]*(n+1))]*(kk+1)       #C0 is the concentration of reagent

    R = 8.314 #the gaz constant [JK-1mol-1]
    hth = [mat([[1.]*(m+1)]*(n+1))] # h is the edge thermic impedance it's a matrix so that it will be possible to make a non uniform insulation [WK-1m-2]
    for k in range(kk):
        hth.append(mat([[1.]*(m+1)]*(n+1)))
    hr = 4*5.67*0.00000001*0.9*(273.15+Text)**3 # = 5.089829465910104
    hc = 2.8
    h = [mat([[1.]*(m+1)]*(n+1))]
    for k in range(kk):
        h.append(mat([[1.]*(m+1)]*(n+1)))

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

        
    #lets build the matrix h of thermic impedence with the following formula : htot = 1/(1/hth+1/hrhc)

    for i in range(n+1):
        for j in range (m+1):
            for k in range(kk+1):
                h[k][i,j] = 1/(1/hth[k][i,j]+1/(hr+hc))

    pipe = []
    # a vertical pipe in the center:
    """for k in range(kk+1):
        pipe.append([approximation((n+1)/2),approximation((m+1)/2),k])"""

    #a spiral pipe :

    rspir = 5
    paspir = 0.5
    for k in range ((kk+1)*10):
        toto = approximation((n+1)/2+rspir*cos(k/paspir/10))
        lucy = approximation((m+1)/2+rspir*sin(k/paspir/10))
        pipe.append([toto,lucy,approximation(k/10)])

    while i < (len(pipe)-1): #now, we don't want any object to be twice in the list.
        if pipe[i+1] == pipe[i]:
            pipe.remove(pipe[i+1])
        i += 1
            
    #print(pipe)

    nech = duree/dt      #it is the number of time quantums
    n1ech = int(tech/dt)  #it is the number of values taken
    print(n1ech)
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
    f.write (str(modele))
    f.write ('\n')
    f.write (str(kreac0))
    f.write ('\n')
    f.write (str(kreac1))
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

    Tentry = donnees[12][0] #Tentry is the température of the incoming water [°C]
    v = donnees[12][1] #v is the speed of the water in the pipe [m/s]
    ceau = donnees[12][2] #ceau is the thermal capacity of water [J/K/kg]
    lambdpipe = donnees[12][3] #lambdpipe is the thermal conduction of the pipe [W/K/m]
    rext = donnees[12][4]
    rint = donnees[12][5]
    hpipe = donnees[12][6] #hpipe is the convection coefficient

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
        for j in range(3): #list of the coords of the pipe
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

    YN = donnees[13][0]
    perpen = donnees[13][1]
    cote = donnees[13][2]
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
    
    chainecar = 'résult_Ech' + str(Echim) + 'mod' + str(modele) + 'krz' + str(kreac0) + 'kru' + str(kreac1) + 'Veau' + str(v) + '.txt'

    chaine = subprocess.check_output(["c_sdf_2.exe",chainecar])  #let's call the C programm !

    os.startfile(chainecar)  # now, we start the file 'result_Ech'

    print("fin")
#
##########################################################################################################################
#

def lectureax(donnees):
    
    g = open(donnees,'r')

    texte = ''

    for i in g:
        texte = texte + i

    g.close

    traduire = ax_parser()

    try :
        donnees=traduire(texte)        # ici, on choisit le texte entré en données
    except SyntaxError as erreur :
        print("-Erreur à[ligne,colonne]")
        print('    ',erreur)
    
    nameOfTheHeap = donnees[0]

    modele = donnees[11][0]  #this decides the model chosen (0 or 1)
    kreac0 = donnees[11][1] #90 #we want to identify this parameter[J.s-1.m-3]
    kreac1 = donnees[11][2] #8e-7 #[s-1.m-3]
    Echim = donnees[11][3] #1e8 #this is the concentration of chemical energy at the beginning : we want to identify it too [J/m^3]
    Text = donnees[11][4] #this is the temperature in the edge
    Tint = donnees[11][5] #this is the temperature in the heap at the beginning
    
    en = donnees[1][0] #
    em = donnees[1][1] # n and m are the dimensions of the heap
    capteurs = donnees[2] #without captors, you can't see anything !

    lamb = donnees[4] #lambda is the thermal conduction [Wm-1K-1]
    rho = donnees[5] #rho is the volumic mass [kgm-3]
    c = donnees[6] #c is the thermal massic capacity [Jkg-1K-1]
    dt = donnees[7] #dt is the time quantum[s]
    tech = donnees[8]  #we take the values every 20 minutes
    dx = donnees[9] #dx is the space quantum[m]
    duree = donnees[10] #duree is the time the study is during

    n = approximation(en/dx)-1 #
    m = approximation(em/dx)-1 # n and m are the dimensions of the matrix T0 (and of every T[z])

    T0 = mat([[Tint]*(m+1)]*(n+1)) #T0 is the temperature of the compost at the beginning of the study  112*76
    
    C0 = mat([[Echim]*(m+1)]*(n+1))      #C0 is the concentration of reagent

    R = 8.314 #the gaz constant [JK-1mol-1]
    hth = mat([[1.]*(m+1)]*(n+1)) # h is the edge thermic impedance it's a matrix so that it will be possible to make a non uniform insulation [WK-1m-2]
    hr = 4*5.67*0.00000001*0.9*(273.15+Text)**3
    hc = 2.8
    h = mat([[1.]*(m+1)]*(n+1))

    #lets build the matrix hth
    a0 = donnees[3][0]
    a1 = donnees[3][1]
    a2 = donnees[3][2]
    a3 = donnees[3][3]

    for i in range(n+1):
        hth[i,0] = a2
        hth[i,m] = a3

    for j in range(m+1):
        hth[0,j] = a0
        hth[n,j] = a1
        
    #lets build the matrix h of thermic impedence with the folowing formula : htot = 1/(1/hth+1/hrhc)

    for i in range(n+1):
        for j in range (m+1):
            h[i,j] = 1/(1/hth[i,j]+1/(hr+hc))
    pipe = []
    # a vertical pipe in the center:
    """for k in range(kk+1):
        pipe.append([approximation((n+1)/2),approximation((m+1)/2),k])"""

    #a spiral pipe :

    rspir = 5
    paspir = 0.5
    for k in range (10):
        toto = approximation((n+1)/2+rspir*cos(k/paspir/10))
        lucy = approximation((m+1)/2+rspir*sin(k/paspir/10))
        pipe.append([toto,lucy])

    while i < (len(pipe)-1): #now, we don't want any object to be twice in the list.
        if pipe[i+1] == pipe[i]:
            pipe.remove(pipe[i+1])
        i += 1
            
    #print(pipe)

    nech = duree/dt      #it is the number of time quantums
    n1ech = int(tech/dt)  #it is the number of values taken
    print(n1ech)
    a = lamb/(rho*c) #[m2/s]
    b = dt/(rho*c)   #[Km3W-1]
    F0 = a*dt/(dx*dx)   #[1] a necessary condition is that 4F0<1

    r = [1]*shape(T0)[0]  #we need the ray
    for i in range (shape(T0)[0]):
        r[i] = dx*i
    print(4*r[n]*F0)

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
    f.write (str(modele))
    f.write ('\n')
    f.write (str(kreac0))
    f.write ('\n')
    f.write (str(kreac1))
    f.write ('\n')

    # The integers
    f.write (str(n1ech))
    f.write ('\n')

    #and the matrix
    f.write(str(shape(T0)[0]))#n+1
    f.write ('\n')
    f.write(str(shape(T0)[1]))#m+1
    f.write ('\n')

    # the radius
    f.write(str(shape(r)[0]))
    f.write ('\n')

    for i in range(n+1):
        for j in range(m+1):
            f.write(str([h[i,j]][0]))
            f.write ('\n')
            f.write(str([T0[i,j]][0]))
            f.write ('\n')
            f.write(str([C0[i,j]][0]))
            f.write ('\n')


    for i in range(shape(r)[0]):
        f.write(str(r[i]))
        f.write ('\n')

    #the pipe is coming !

    Tentry = donnees[12][0] #Tentry is the température of the incoming water [°C]
    v = donnees[12][1] #v is the speed of the water in the pipe [m/s]
    ceau = donnees[12][2] #ceau is the thermal capacity of water [J/K/kg]
    lambdpipe = donnees[12][3] #lambdpipe is the thermal conduction of the pipe [W/K/m]
    rext = donnees[12][4]
    rint = donnees[12][5]
    hpipe = donnees[12][6] #hpipe is the convection coefficient

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
        for j in range(2): #list of the coords of the pipe
            f.write(str(i[j]))
            f.write ('\n')

    #and the captors :
    f.write(str(len(capteurs)))
    f.write ('\n')
    for i in capteurs :
        for j in range(2):
            f.write(str(approximation(i[1][j]/dx))) #don't forget the dx
            f.write ('\n')

    #The picture :

    YN = donnees[13][0]
    if YN == 'y':
        f.write(str(1))
    else :
        f.write(str(0))
            
    #number of characters the programm.c will write : 15*number of writings = 15*duree/tech = 12*15*dt*nech/tech pour 

    f.close()
    
    chainecar = 'résult_Ech' + str(Echim) + 'mod' + str(modele) + 'krz' + str(kreac0) + 'kru' + str(kreac1) + 'Veau' + str(v) + '.txt'

    chaine = subprocess.check_output(["c_sdf_2_axi.exe",chainecar])  #let's call the C programm !

    os.startfile(chainecar)  # now, we start the file 'result_Ech'

    print("fin")

#lecture('donnees_sdf.txt')


##############################################################################################################
##########################################################################################################

def lecture0() :
    lecture('donnees0_sdf.txt')

def lecture1() :
    lecture('donnees1_sdf.txt')

def lecture2() :
    lecture('donnees2_sdf.txt')

def lecture3() :
    lecture('donnees3_sdf.txt')

def lecture4() :
    lectureax('donnees4_sdf.txt')


fenetre = Tk()
fenetre.title('SDF_Compost')
#
#les boutons 'lancer donnees'


Button(fenetre,text="Lancer 'donnees0'",font="Arial 18",width=15, command=lecture0).pack()
Button(fenetre,text="Lancer 'donnees1'",font="Arial 18",width=15, command=lecture1).pack()
Button(fenetre,text="Lancer 'donnees2'",font="Arial 18",width=15, command=lecture2).pack()
Button(fenetre,text="Lancer 'donnees3'",font="Arial 18",width=15, command=lecture3).pack()
Button(fenetre,text="Lancer 'donnees4' en axysymétrique",font="Arial 18",width=30, command=lecture4).pack()

fenetre.mainloop()
