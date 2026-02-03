#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QDate>
#include <QDateTimeEdit>
#include <QTimer>
#include <QDateTimeEdit>
#include <QtCharts>
#include <QWidget>
#include <QStackedWidget>
#include <cmath>

// A SUPPRIMER CAR CEST POUR LA SIMULATION
#include <QFile>
#include <QTextStream>
#include <QList>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *stack;                          // Gestionnaire de pages

    QWidget *centralWidget;
    QWidget *pageIdentification;                    // Conteneur page 0
    QWidget *pageMenu;                              // Conteneur page 1
    QWidget *pageGraphique;                         // Conteneur page 2

    QVBoxLayout *layoutIdentification;              // Layout vertical pour la page Identification
    QVBoxLayout *layoutMenu;                        // Layout vertical pour la page Menu
    QVBoxLayout *layoutGraph;                       // Layout vertical pour la page Graphiques

    QHBoxLayout *layoutUtilisateur;                 // Layout horizontal pour les Utilisateur
    QHBoxLayout *layoutBoutons;                     // Layout Horizontal pour les données

    QFrame *ligne;                                  // Affichage de la ligne de séparation (Nom utilisateur | Données / Graphiques)

    QTimer *timerHeure;                             // Timer pour les 5 minutes d'intervalle entre la récupération des données

    QDateTime dateDebutSelectionnee;                // Stocke le début choisi
    QDateTime dateFinSelectionnee;                  // Stocke la fin choisie

    QLabel *labelUtilisateur;                       // Affichage de l'utilisateur connecté
    QLabel *labelDateDuJour;                        // Affichage de la Date du jour
    QLabel *labelPeriodeDeLaPrise;                  // Affichage de la période souhaité
    QLabel *labelCalculRosee;                       // Affichage du point de rosée
    QLabel *labelRosee;                             // Affichage de l'air en fonction du point de rosée

    QString typeGrapheActuel;                       // Stocke "Température", "Humidité", "Vent" ou "Global"

    QLineEdit *editLogin;                           // Espace pour ecrire l'utilisateur
    QLineEdit *editPassword;                        // Espace pour ecrire le Mot de Passe

    QPushButton *btnConnexion;                      // Boutton pour se connecter
    QPushButton *btnDeconnexion;                    // Boutton pour se déconnecter ou changer d'utilisateur
    QPushButton *btnTemperature;                    // Boutton pour afficher le graphique température
    QPushButton *btnHumidite;                       // Boutton pour afficher le graphique d'humidité
    QPushButton *btnVent;                           // Boutton pour afficher le graphique du vent
    QPushButton *btnDate;                           // Boutton pour régler la date
    QPushButton *btnRedimension;                    // Boutton pour afficher le graphique global
    QPushButton *btnRetour;                         // Boutton pour revenir au Menu

    QChartView *chartView;                          // Afficheur de graphiques



    // A SUPPRIMER CAR CEST POUR LA SIMULATION
    QList<double> historiqueTemp;
    QList<double> historiqueHum;
    QList<double> historiqueVent;
    QList<qint64> historiqueTemps;

private slots:
    void afficherIdentification();                  // Affichage de la page Identification
    void afficherMenu();                            // Basculement en PORTRAIT et Affichage de la page Menu
    void creerGraphiqueIndividuel(QString type);    // Affiche et Créer les graphiques individuels
    void creerGraphiqueGlobal();                    // Affiche et Créer le graphique global
    void gererLegendeCliquee();
    void verifierConnexion();                       // Verifie que les Utilisateurs et Mot de Passe soient corrects
    void preparerGraphique();                       // Verifie quel boutons (température, humidité, vent) est cliqués pour que le bons noms soit données à chaque graphique
    void mettreAJourHeure();                        // Mettre a jour la date et l'heure du jour
    void choisirPeriode();                          // Chosis de la période pour l'affichage des graphiques
    void calculerPointDeRosee(double temp, double hum);     // Calcul du point de rosée

    // A SUPPRIMER CAR CEST POUR LA SIMULATION
    void lireFichierSimulateur();

};
#endif // MAINWINDOW_H
