#############################################
#                                           #
#             Power Compost 2.0             #
#                                           #
#   Fichier : scriptExtractionDonneesOld.py #
#                                           #
#   Florentin Delaine                       #
#   florentin.delaine@ens-cachan.fr         #
#                                           #
#############################################

# -*-coding:utf-8 -*

__author__ = 'florentindelaine'

import informationsBDD,scriptDecoupe

import os

import pymysql

import sp

import math

import datetime

#-----------#
# Fonctions #
#-----------#


def parser():
    # Définition des éléments
    date = sp.R(r'\d\d/\d\d/\d\d') # JJ/MM/AA
    nomCapteur = sp.R(r'TC\d') # Syntaxe à vérif : TCX
    mesure = sp.R(r'\d+.\d+') / (lambda mes: float(mes.replace(',', '.')))  # séparateur ,
    blancs = sp.R('\t') #tabulation
    temps = sp.R(r'\d+.\d+') / (lambda tps: float(tps.replace(',', '.')))

    # Définition de la grammaire
    with sp.Separator(blancs):
        donnees = sp.Rule()
        enTete = sp.Rule()
        mesures = sp.Rule()

        donnees |= enTete & mesures[:]
        enTete |= date & nomCapteur[:] & '\n'
        mesures |= temps & mesure[:] & '\n'

    return donnees

decoder = parser()


def extractionDonnees(path, fileName, originDate):
    print("Extraction des donnéees de {}/{}.txt".format(path,fileName))
    with open("{}/{}.txt".format(path, fileName), "r", encoding="utf-8") as file:
        donneesLues = file.read()
        #donneesLues+='\n' # pour éviter une erreur du parseur... # pas utile dans ce cas apparemment...

        # Décodage des donnees lues
        try:
            donneesExtraites = decoder(donneesLues)

        except SyntaxError as erreur:
            print("- Erreur détectée par le parseur 'sp' à [ligne,colonne]:")
            print('   ', erreur)

        try:
            # On créé la table si elle n'existe pas déjà (préparation de la requète)
            requete = "CREATE TABLE IF NOT EXISTS {} (id INT UNSIGNED NOT NULL AUTO_INCREMENT, DateHeure DATETIME, temps DECIMAL(20,6)".format(nomFichier)
            for nomCapteur in donneesExtraites[0][1]:
                requete += ", {} DECIMAL(20,6)".format(nomCapteur)

            requete += ", PRIMARY KEY (id))"

            curseurBDD.execute(requete)

            nombreMesures = len(donneesExtraites[1])

            print("Nombre de mesures à insérer : {}".format(nombreMesures))

            # Préparation de l'en-tête des requètes d'insertion
            requeteInsertionEnTete = "INSERT INTO {} (DateHeure,temps".format(nomFichier)
            for nomCapteur in donneesExtraites[0][1]:
                requeteInsertionEnTete += ", {}".format(nomCapteur)
            requeteInsertionEnTete += ") VALUES("

            # On ajoute tout ce qu'on a lu
            for mesure in donneesExtraites[1]:

                date = originDate+datetime.timedelta(seconds=math.floor(mesure[0]))

                # Préparation de la requète
                requete = requeteInsertionEnTete[:]
                requete += "'{}','{}'".format(date, mesure[0])

                for releve in mesure[1]:
                    requete += ", {}".format(releve)

                requete += ")"

                curseurBDD.execute(requete)

            connexionBDD.commit()
            print("Données insérées dans la table.")


        except:
            print('*** Requête SQL incorrecte ***')

#----------#
#   Main   #
#----------#

print("=== SCRIPT D'EXTRACTION DE DONNÉES ET D'INSERTION DANS UNE BASE SQL ====")

# On récupère le chemin et le nom du fichier à importer
cheminFichier = input("Chemin du fichier .txt à importer issu de données anciennes (en relatif par rapport au script ou en absolu) : ")
nomFichier = input("Nom du fichier (sans l'extension) ? ")

# On demande la date de début (conversion format)
dateOrigine = input("Date de début de la table (%Y/%m/%d %H:%M:%S) : ")

dateOrigine = datetime.datetime.strptime(dateOrigine, '%Y/%m/%d %H:%M:%S')

# On demande si les infos BDD sont correctes (libre à chacun de foutre la merde dans ses données)
print("=== Informations BDD ===\n- Hôte : {}\n- Port : {}\n- Utilisateur : {}\n- Base de données : {}\n".format(informationsBDD.adresse, informationsBDD.port, informationsBDD.utilisateur, informationsBDD.baseDeDonnees))
if input("Informations correctes ? (O/N) ") != 'O':
    exit(0)

# On se connecte à la BDD et on créé un curseur (gestion exceptions SQL ?)
print("Connexion à la base de données...")
#connexionBDD = pymysql.connect(host=informationsBDD.adresse, port=informationsBDD.port, user=informationsBDD.utilisateur, passwd=informationsBDD.motDePasse, db=informationsBDD.baseDeDonnees)
connexionBDD = pymysql.connect(host=informationsBDD.adresse, user=informationsBDD.utilisateur, passwd=informationsBDD.motDePasse, db=informationsBDD.baseDeDonnees)
curseurBDD = connexionBDD.cursor()
print("Connexion ouverte...")

# On découpe le fichier si on l'estime trop volumineux
if os.path.getsize("{}/{}.txt".format(cheminFichier, nomFichier)) > 100000:
    nombreFichiers = scriptDecoupe.decoupe(cheminFichier, nomFichier)
    intervalleExtraction = range(1, nombreFichiers+1)
    for indexFichier in intervalleExtraction:
        print("Fichier {}/{}".format(indexFichier, nombreFichiers))
        extractionDonnees("Decoupe/{}".format(nomFichier), "{}_{}".format(nomFichier, indexFichier), dateOrigine)
    scriptDecoupe.suppressionFichiersDecoupe(nomFichier)
else:
    extractionDonnees(cheminFichier, nomFichier, dateOrigine)

connexionBDD.close()
print("Connexion fermée...")
print("=== FIN DU SCRIPT ===")