#############################################
#                                           #
#             Power Compost 2.0             #
#                                           #
#   Fichier : setup.py                      #
#                                           #
#   Florentin Delaine                       #
#   florentin.delaine@ens-cachan.fr         #
#                                           #
#############################################

# -*-coding:utf-8 -*

__author__ = 'florentindelaine'

import os

import getpass

import pymysql

#----------#
#   Main   #
#----------#

print("=== PowerCompost apps setup script ====")

host = input("Host: ")

user = input("Username: ")

password = getpass.getpass("Password: ")

print("Connexion...")
connexionDB = pymysql.connect(host=host, user=user, passwd=password)
cursorDB = connexionDB.cursor()
print("Connexion opened...")

cursorDB.execute("CREATE DATABASE PowerCompost CHARACTER SET 'utf8'")

cursorDB.execute("USE PowerCompost")

cursorDB.execute("CREATE TABLE Test_benches (id SMALLINT UNSIGNED AUTO_INCREMENT, name VARCHAR(30) NOT NULL, length SMALLINT UNSIGNED NOT NULL, width SMALLINT UNSIGNED NOT NULL, height SMALLINT UNSIGNED NOT NULL, insulation_thickness SMALLINT UNSIGNED, PRIMARY KEY (id)) ENGINE=INNODB")

cursorDB.execute("CREATE TABLE Sets_of_sensors (id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT, name VARCHAR(30) NOT NULL, nb_sensors SMALLINT NOT NULL, comments TEXT, PRIMARY KEY (id)) ENGINE=INNODB")

cursorDB.execute("CREATE TABLE Set_model(id SMALLINT UNSIGNED AUTO_INCREMENT, name VARCHAR(30) NOT NULL, x SMALLINT UNSIGNED, y SMALLINT UNSIGNED, z SMALLINT UNSIGNED, PRIMARY KEY (id)) ENGINE=INNODB")

cursorDB.execute("CREATE TABLE Experiments (id SMALLINT UNSIGNED AUTO_INCREMENT, name VARCHAR(30) NOT NULL, test_bench_id SMALLINT UNSIGNED, sensors_set_id SMALLINT UNSIGNED, started DATETIME, stopped DATETIME, volumetric_mass DOUBLE, thermal_capacity DOUBLE, thermal_conductivity DOUBLE, comments TEXT, PRIMARY KEY (id)) ENGINE=INNODB")

connexionDB.close()
print("Connexion closed...")
print("=== SETUP COMPLETED ===")
