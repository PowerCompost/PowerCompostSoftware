#############################################
#                                           #
#             Power Compost 2.0             #
#                                           #
#   Fichier : scriptDecoupe.py              #
#                                           #
#   Florentin Delaine                       #
#   florentin.delaine@ens-cachan.fr         #
#                                           #
#############################################

# -*-coding:utf-8 -*

__author__ = 'florentindelaine'

import os


def decoupe(path, fileName):
    '''Découpe le fichier .txt au nom et au path indiqué en plusieurs fichiers'''
    # Ouverture du fichier
    file = open("{}/{}.txt".format(path, fileName), "r", encoding="utf-8")
    print("Découpe : Fichier ouvert...")

    enTete = file.readline()  # Récupération de l'en-tête

    os.mkdir('Decoupe/{}'.format(fileName))  # Création du répertoire qui va contenir les fichiers

    # Paramètres boucle
    ligneLue = '0'  # Condition (tant qu'on a pas un readline() vide)
    intervalle = range(10000)  # On va découper les fichiers en paquets de 10 000
    passages = 0  # Pour la numérotation des fichiers découpés

    while ligneLue != '':
        passages += 1

        # donnees contiendra le contenu du fichier d'index passages
        donnees = enTete

        for i in intervalle:
            ligneLue = file.readline()
            if ligneLue == '':  # On casse la boucle si la ligne lue est vide => fichiers continus !
                break
            else:
                donnees += ligneLue

        # On créé le nouveau fichier, on écrit, on ferme
        decoupe = open("Decoupe/{}/{}_{}.txt".format(fileName, fileName, passages), 'w', encoding='utf-8')
        decoupe.write(donnees)
        decoupe.close()

    # On referme le fichier de base et on retourne le nombre
    file.close()
    print("Découpe : Fichier fermé...")
    return passages


def suppressionFichiersDecoupe(nomFichier):
    '''Supprime les fichiers de découpe dans le répertoire nommé nomFichier'''

    # On liste le contenu du dossier
    contenuDossier = os.listdir("Decoupe/{}".format(nomFichier))

    # On retire tous les fichiers
    for fichier in contenuDossier:
        os.remove("Decoupe/{}/{}".format(nomFichier, fichier))

    # On retire le dossier
    os.rmdir("Decoupe/{}".format(nomFichier))