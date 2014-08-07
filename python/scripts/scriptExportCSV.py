#######################################
#                                     #
#          Power Compost 2.0          #
#                                     #
#   Fichier : scriptExportCSV.py      #
#                                     #
#   Florentin Delaine                 #
#   florentin.delaine@ens-cachan.fr   #
#                                     #
#######################################

# -*-coding:utf-8 -*

# À optimiser pour pas dépasser la limite en ligne d'Excel, Matlab and co
# Mettre des noms de variable plus clairs
# Si on exporte une seule table et qu'elle commence pas à zéro, on a la valeur de la ligne de départ de 0 au temps de cette ligne

__author__ = 'florentindelaine'

import informationsBDD
import pymysql
import math

#-----------#
# Fonctions #
#-----------#


def interpolationLineaire(temps, tableauValeurs):
    # tableauValeurs contient deux lignes dont la première colonne correspond aux abcisses (ie temps) des colonnes suivantes
    # On interpole de façon linéaire en les deux temps de ce tableau au temps indiqué en paramètre pour toutes les colonnes
    valeursInterpolees = [temps]

    print(tableauValeurs)

    # Sauf si les deux lignes sont au même temps, dans ce cas on retourne la première ligne
    if tableauValeurs[0][0] == tableauValeurs[1][0]:
        return tableauValeurs[0]

    else:
        nombreOrdonnees = range(1, len(tableauValeurs[0]))
        for ordonnee in nombreOrdonnees:
            valeursInterpolees.append((tableauValeurs[1][ordonnee] - tableauValeurs[0][ordonnee])*(temps-tableauValeurs[0][0])/(tableauValeurs[1][0]-tableauValeurs[0][0])+tableauValeurs[0][ordonnee])

        return valeursInterpolees


def determinerTableAEtudier(temps, nomsTables, dicoTempsDeFin, dicoEcartsAvecTablePrécédente, dicoOriginesTables):
    # Objectif : déterminer suivant un temps donné la ou les tables à utiliser pour récupérer les données nécessaires à l'interpolation
    # On retourne une liste avec la table contenant la ligne à t inférieur qui nous intéresse et la table contenant la ligne à t supérieur

    dicoTempsDeFinAbsolus = {} # dictionnaire pour établir un "axe des temps"
    # On parcourt les tables à exporter
    for nomTable in nomsTables:

        # On remplit le dico des temps absolus
        dicoTempsDeFinAbsolus[nomTable] = dicoTempsDeFin[nomTable]+dicoEcartsAvecTablePrécédente[nomTable]
        if nomsTables.index(nomTable) != 0:
            dicoTempsDeFinAbsolus[nomTable] += dicoTempsDeFinAbsolus[nomsTables[nomsTables.index(nomTable)-1]]

        # Si on étudie la première table et que temps est dans l'intervalle couvert par celle-ci
        if nomsTables.index(nomTable) == 0:
            if temps <= dicoTempsDeFin[nomTable]:
                return [nomTable, nomTable]

        else:
            # Si temps est dans une table
            if temps <= dicoTempsDeFinAbsolus[nomTable] and temps >= dicoTempsDeFinAbsolus[nomsTables[nomsTables.index(nomTable)-1]]+dicoEcartsAvecTablePrécédente[nomTable]+dicoOriginesTables[nomTable]:
                return [nomTable, nomTable]
            # Sinon il est entre deux tables
            elif temps >= dicoTempsDeFinAbsolus[nomsTables[nomsTables.index(nomTable)-1]] and temps <= dicoTempsDeFinAbsolus[nomsTables[nomsTables.index(nomTable)-1]]+dicoEcartsAvecTablePrécédente[nomTable]+dicoOriginesTables[nomTable]:
                return [nomsTables[nomsTables.index(nomTable)-1], nomTable]

#----------#
#   Main   #
#----------#

# On se connecte à la BDD et on créé un curseur (gestion exceptions SQL ?)
print("Connexion à la base de données...")
connexionBDD = pymysql.connect(host=informationsBDD.adresse, user=informationsBDD.utilisateur, passwd=informationsBDD.motDePasse, db=informationsBDD.baseDeDonnees)
curseurBDD = connexionBDD.cursor()
print("Connexion ouverte...")

try:
    # On affiche les tables de la base
    curseurBDD.execute("SHOW TABLES")
    print("Tables dans la base de données : \n")
    listeTables = []
    for (nomTable,) in curseurBDD:
        listeTables.append(nomTable)
        print("- {}\n".format(nomTable))

    # On récupère les tables qu'on veut exporter d'un seul tenant
    nomsTablesAExporter = [input("Choix ? ")]
    tableSupplémentaire = ''
    while tableSupplémentaire != 'fin':
        if nomsTablesAExporter[-1] not in listeTables:
            raise ValueError("Le nom de table saisi n'est pas dans la base de données")  # Gérer pour pas sortir directement ?
        tableSupplémentaire = input("Choix d'une table supplémentaire (les tables seront raccordées) ou fin ? ")
        nomsTablesAExporter.append(tableSupplémentaire)

    nomsTablesAExporter.pop()  # On vire le 'fin' des tables à exporter

    # On demande les écarts temporels entre les tables et on récupère aussi les origines (tables commençant parfois pas à 0)
    correctionsOrigines = {}
    originesTables = {}
    for nomTableAExporter in nomsTablesAExporter:
        correctionsOrigines[nomTableAExporter] = float(input("Écart temporel entre la fin de la table précédente et la table {} (s) ? ".format(nomTableAExporter)))
        curseurBDD.execute("SELECT temps FROM {} WHERE id=(SELECT MIN(id) FROM {})".format(nomTableAExporter, nomTableAExporter))
        (origine,) = curseurBDD.fetchone()
        originesTables[nomTableAExporter] = float(origine)

    # On demande le pas de temps
    dt = int(input("dt (s) ? "))

    idsMax = {}
    tFinTable = {}
    Ttot = 0
    nomTablesAExporter = ''
    
    for nomTableAExporter in nomsTablesAExporter:
        correctionsOrigines[nomTableAExporter] += dt*nomsTablesAExporter.index(nomTableAExporter)  # Pour éviter un petit soucis dans les raccords (mais pas très fondé scientifiquement)

        # On récupérère les id de fin de table
        curseurBDD.execute("SELECT MAX(id) FROM {}".format(nomTableAExporter))
        (idMax,) = curseurBDD.fetchone()
        idsMax[nomTableAExporter] = idMax

        # On récupère les temps de fin de table avec conversion en float
        curseurBDD.execute("SELECT temps FROM {} WHERE id={}".format(nomTableAExporter, idsMax[nomTableAExporter]))  # Max dernière table BDD
        (tFin,) = curseurBDD.fetchone()
        tFin = float(tFin)
        tFinTable[nomTableAExporter] = tFin
        Ttot += tFin+correctionsOrigines[nomTableAExporter]

        # On créé le nom du fichier exporté sous la forme table1_table2_..._tablen
        nomTablesAExporter += nomTableAExporter
        if nomTableAExporter != nomsTablesAExporter[-1]:
            nomTablesAExporter += "_"

    # Calcul du nombre d'itérations pour effectuer l'export
    nDemandés = math.floor(Ttot/dt)  # vérifier nombre des points pas trop important (limite excel)
    intervalleDemandé = range(0, nDemandés+1)

    print("Longueur de la table récupérée, nombre de points déterminé...")

    # On commence à balancer la sauce...
    fichierCSV = open("Export/{}.csv".format(nomTablesAExporter), "w", encoding="utf-8")
    print("Fichier d'export créé...")

    # On récupère la structure de la table
    curseurBDD.execute("SHOW COLUMNS FROM {}".format(nomsTablesAExporter[0]))  # Attention si on rajoute une colonne pour préciser les caractéristiques de l'essai à la fin, ça va foutre la merde
    print("Structure de table récupérée...")

    # On convertit tout ça et on adapte pour créer l'en-tête
    structure = list(curseurBDD.fetchall())
    structure.pop(0)

    enTete = ""
    for colonne in structure:
        enTete += colonne[0]+';'
    enTete += '\n'

    fichierCSV.write(enTete)
    print("En-tête écrit...")

    # Et c'est parti pour ajouter tous les points
    for indicePoint in intervalleDemandé:
        # Petit indicateur pour savoir où on en est (Python a tendance à ralentir l'affichage graphique mais ça tourne quand même derrière hein ;) )
        print("{}/{}".format(indicePoint, nDemandés))

        # On détermine si à dt*indicePoint on est dans une table ou entre deux tables
        [nomTableInf, nomTableSup] = determinerTableAEtudier(indicePoint*dt, nomsTablesAExporter, tFinTable, correctionsOrigines, originesTables)

        # Si on est dans une table
        if nomTableInf == nomTableSup:
            # Si c'est la première table, pas besoin de corriger le dt*indicePoint pour être bien en relatif
            if nomsTablesAExporter.index(nomTableSup) == 0:
                # On trouve la ligne qui a le premier temps supérieur à dt*indicePoint
                curseurBDD.execute("SELECT * FROM {} WHERE temps>={} LIMIT 1".format(nomTableSup, indicePoint*dt))
            # Sinon
            else:
                # Ben faut retirer toutes les corrections et les temps de fin des tables précédentes...
                temps = indicePoint*dt-correctionsOrigines[nomTableSup]
                for indexTable in range(nomsTablesAExporter.index(nomTableSup)):
                    temps -= tFinTable[nomsTablesAExporter[indexTable]]+correctionsOrigines[nomsTablesAExporter[indexTable]]

                # On trouve la ligne qui a le premier temps supérieur à temps
                curseurBDD.execute("SELECT * FROM {} WHERE temps>={} LIMIT 1".format(nomTableSup, temps))

            # On récupère cette ligne
            lignesInterpolation = [list(curseurBDD.fetchone())]

            # On convertit tout en float (format par défaut c'est du Decimal ou un truc dans le genre)
            for indice in range(len(lignesInterpolation[0])):
                lignesInterpolation[0][indice] = float(lignesInterpolation[0][indice])

            # Si c'est pas la toute première ligne de la table qu'on vient de récupérer... (vérif sur le temps car j'ai pas stocké les id)
            if lignesInterpolation[0][1] != originesTables[nomTableSup]:
                # On va chercher la ligne juste en dessous
                curseurBDD.execute("SELECT * FROM {} WHERE id={}".format(nomTableInf, lignesInterpolation[0][0]-1))
                # On la met en position 0 après l'avoir convertit en liste
                lignesInterpolation.insert(0, list(curseurBDD.fetchone()))
                # Et on convertit les valeurs en float
                for indice in range(len(lignesInterpolation[0])):
                    lignesInterpolation[0][indice] = float(lignesInterpolation[0][indice])

                # On vire les premiers items (id)
                lignesInterpolation[0].pop(0)
                lignesInterpolation[1].pop(0)

            # Sinon
            else:
                # Ben on recopie cette ligne
                lignesInterpolation.append(lignesInterpolation[0])
                # Et on vire les premiers éléments (apparemment lors du append, Python garde le lien entre les deux lignes)
                lignesInterpolation[0].pop(0)

        # Sinon on est entre deux tables
        else:
            # On va chercher la première ligne de la table supérieure
            curseurBDD.execute("SELECT * FROM {} WHERE id=(SELECT MIN(id) FROM {})".format(nomTableSup, nomTableSup))
            lignesInterpolation = [list(curseurBDD.fetchone())]

            # Conversion float...
            for indice in range(len(lignesInterpolation[0])):
                lignesInterpolation[0][indice] = float(lignesInterpolation[0][indice])

            ## changer exception... on peut commencer ailleurs qu'à 0...
            #if indicePoint!=0:

            # On va chercher la ligne de fin de la table précédente
            curseurBDD.execute("SELECT * FROM {} WHERE id={}".format(nomTableInf, idsMax[nomTableInf]))
            # On la met en position 0 après l'avoir convertit en liste
            lignesInterpolation.insert(0, list(curseurBDD.fetchone()))
            # Et on convertit les valeurs en float
            for indice in range(len(lignesInterpolation[0])):
                lignesInterpolation[0][indice] = float(lignesInterpolation[0][indice])

            # On vire les premiers items (id)
            lignesInterpolation[0].pop(0)
            lignesInterpolation[1].pop(0)

            #else:
            #    lignesInterpolation.append(lignesInterpolation[0])
            #    lignesInterpolation[0].pop(0)

        # Maintenant on va corriger les temps pour que l'interpolation se fasse bien
        # Si c'est la première table, pas de soucis
        if nomsTablesAExporter.index(nomTableSup) == 0:
            donnees = interpolationLineaire(indicePoint*dt, lignesInterpolation)

        # Si on est entre deux tables
        elif nomTableSup != nomTableInf:
            # On corrige le temps de la première ligne de sorte à ajouter les temps de fin des tables d'avant et les corrections
            for indexTable in range(nomsTablesAExporter.index(nomTableInf)):
                lignesInterpolation[0][0] += tFinTable[nomsTablesAExporter[indexTable]]+correctionsOrigines[nomsTablesAExporter[indexTable]]
            lignesInterpolation[0][0] += correctionsOrigines[nomTableInf]

            # De même avec la seconde ligne (on pourrait réduire cette boucle en incluant une partie dans la précédente mais bon...
            for indexTable in range(nomsTablesAExporter.index(nomTableSup)):
                lignesInterpolation[1][0] += tFinTable[nomsTablesAExporter[indexTable]]+correctionsOrigines[nomsTablesAExporter[indexTable]]
            lignesInterpolation[1][0] += correctionsOrigines[nomTableSup]

            # On interpole
            donnees = interpolationLineaire(indicePoint*dt, lignesInterpolation)

        # Sinon on est dans une table
        else:
            # On va corriger le temps auquel on est
            temps = indicePoint*dt

            # On retire tous les temps de fin et les corrections des tables précédentes
            for indexTable in range(nomsTablesAExporter.index(nomTableSup)):
                temps -= tFinTable[nomsTablesAExporter[indexTable]]+correctionsOrigines[nomsTablesAExporter[indexTable]]
            temps -= correctionsOrigines[nomTableSup]

            # On interpole
            donnees = interpolationLineaire(temps, lignesInterpolation)

        # On va ajouter les données une à une (à améliorer car il doit y avoir plus efficace
        for indiceColonne in range(len(donnees)):
            # On corrige le temps donné par le résultat de l'interpolation
            if indiceColonne == 0:
                # Si on était dans une table et que c'était pas la première
                if nomTableSup != nomsTablesAExporter[0] and nomTableInf == nomTableSup:
                    # On rajoute au temps les temps de fin et les corrections des tables précédentes
                    for indexTable in range(nomsTablesAExporter.index(nomTableSup)):
                        donnees[indiceColonne] += tFinTable[nomsTablesAExporter[indexTable]]+correctionsOrigines[nomsTablesAExporter[indexTable]]
                    donnees[indiceColonne] += correctionsOrigines[nomTableSup]

            fichierCSV.write("{};".format(donnees[indiceColonne]))

        fichierCSV.write("\n")

    fichierCSV.close()

    print("Table exportée dans le fichier {}.csv dans le répertoire Export où est le script. Enjoy !".format(nomTablesAExporter))

#except:
#     print('*** Requête SQL incorrecte ***')

except ValueError as error:
    print(error)

except pymysql.MySQLError as error:
    print(error)

connexionBDD.close()
