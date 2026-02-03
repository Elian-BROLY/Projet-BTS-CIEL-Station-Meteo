#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // CONNAITRE CES 2 LIGNES MIEUX (STACK) [
    stack = new QStackedWidget(this);
    setCentralWidget(stack);
    // ]
    resize(400, 700);

    // --- PAGE IDENTIFICATION ---
    pageIdentification = new QWidget();
    layoutIdentification = new QVBoxLayout(pageIdentification); // Layout vertical lié au pageIdentification
    layoutIdentification->setSpacing(20); // Definit l'espace que les widgets ont dans le layout
    layoutIdentification->setContentsMargins(50, 100, 50, 100); // Marges larges pour centrer le formulaire

    // Création des Widgets de connexion
    QLabel *titre = new QLabel("Connexion Station Météo");
    editLogin = new QLineEdit();
    editPassword = new QLineEdit();
    btnConnexion = new QPushButton("Se connecter");

    // Styles des Widgets de connexion
    titre->setStyleSheet("font-size: 20px; font-weight: bold;");
    titre->setAlignment(Qt::AlignCenter);
    editLogin->setPlaceholderText("Nom d'utilisateur");
    editLogin->setStyleSheet("padding: 8px; border: 1px solid gray; border-radius: 4px;");
    editPassword->setPlaceholderText("Mot de passe");
    editPassword->setEchoMode(QLineEdit::Password); // Cache les caractères
    editPassword->setStyleSheet("padding: 8px; border: 1px solid gray; border-radius: 4px;");
    btnConnexion->setStyleSheet("QPushButton { border: 2px solid black; background-color: white; padding: 10px; border-radius: 5px; font-weight: bold; }" "QPushButton:hover { background-color: black; color: white; }");

    // Ajout des Widtgets de connexion au layout Identification
    layoutIdentification->addWidget(titre);
    layoutIdentification->addWidget(editLogin);
    layoutIdentification->addWidget(editPassword);
    layoutIdentification->addWidget(btnConnexion);
    layoutIdentification->addStretch();

    // --- PAGE MENU ---
    pageMenu = new QWidget();
    layoutMenu = new QVBoxLayout(pageMenu); // Layout vertical lié au pageMenu
    layoutUtilisateur = new QHBoxLayout();
    layoutBoutons = new QHBoxLayout();

    // Création des Widgets utilisateur
    btnDeconnexion = new QPushButton("➜]");
    labelUtilisateur = new QLabel(""); // Écriture de l'utilisateur (à changer avec le changement auto lors de la connexion aux utilisateurs de la base)

    // Styles des Widgets utilisateur
    labelUtilisateur->setStyleSheet("QLabel { font-weight: bold; font-size: 16px; }");
    btnDeconnexion->setFixedSize(25, 25);
    btnDeconnexion->setStyleSheet("QPushButton { border: 1px solid black; background-color: white; border-radius: 5px; }" "QPushButton:hover { background-color: black; color: white; }");

    // Ajout des Widtgets utilisateur au layout Utilisateur
    layoutUtilisateur->addWidget(labelUtilisateur);
    layoutUtilisateur->addWidget(btnDeconnexion);

    // Ligne de Sépration
    ligne = new QFrame(this);
    ligne->setFixedHeight(4);
    ligne->setStyleSheet("background-color: #ABABAB; border-radius: 2px;");

    // Création des Widgets période
    QDateTime maintenant = QDateTime::currentDateTime(); // Date du jour
    QDateTime hier = maintenant.addDays(-1);             // Date de hier
    labelDateDuJour = new QLabel("");
    labelPeriodeDeLaPrise = new QLabel();

    // pour que le graphique global les connaisse
    dateDebutSelectionnee = hier;
    dateFinSelectionnee = maintenant;

    // Styles des Widgets période
    labelDateDuJour->setAlignment(Qt::AlignCenter);
    labelDateDuJour->setStyleSheet("QLabel { font-size: 16px; }");
    labelPeriodeDeLaPrise->setText("La période de prise est du " +
                                   hier.toString("dd/MM/yyyy hh:mm") +
                                   " au " +
                                   maintenant.toString("dd/MM/yyyy hh:mm"));
    labelPeriodeDeLaPrise->setAlignment(Qt::AlignCenter);
    labelPeriodeDeLaPrise->setStyleSheet("QLabel { font-size: 16px; }");

    // Initialisation du Timer
    timerHeure = new QTimer(this);
    mettreAJourHeure();

    // Calcul du temps restant jusqu'à la minute suivante pour être bien synchro
    int msJusquaMinuteSuivante = (60 - QTime::currentTime().second()) * 1000;

    // Timer lancé une première fois pour qu'il se déclenche au début de la minute suivante
    QTimer::singleShot(msJusquaMinuteSuivante, this, [this]() {
    mettreAJourHeure();
    timerHeure->start(60000); // On vérifie ensuite toutes les minutes
    });

    // Création des Widgets données
    btnTemperature = new QPushButton("Température (°) :\n () °C");
    btnHumidite = new QPushButton("Humidité (%) :\n () %");
    btnVent = new QPushButton("Vent (Km/h) :\n () Km/h");

    // Styles des Widgets données
    btnTemperature->setStyleSheet("QPushButton { border: 1px solid black; background-color: white; border-radius: 5px; }" "QPushButton:hover { background-color: black; color: white; }");
    btnHumidite->setStyleSheet("QPushButton { border: 1px solid black; background-color: white; border-radius: 5px; }" "QPushButton:hover { background-color: black; color: white; }");
    btnVent->setStyleSheet("QPushButton { border: 1px solid black; background-color: white; border-radius: 5px; }" "QPushButton:hover { background-color: black; color: white; }");

    // Ajout des Widtgets données au layout Utilisateur
    layoutBoutons->addWidget(btnTemperature);
    layoutBoutons->addWidget(btnHumidite);
    layoutBoutons->addWidget(btnVent);

    // Création des Widgets date et redimension
    btnDate = new QPushButton("Date et/ou Heure");
    btnRedimension = new QPushButton("⤢");

    // Styles des Widgets date et redimension
    btnDate->setStyleSheet("QPushButton { border: 1px solid black; background-color: white; border-radius: 5px; }" "QPushButton:hover { background-color: black; color: white; }");
    btnRedimension->setStyleSheet("QPushButton { border: 1px solid black; background-color: white; border-radius: 5px; }" "QPushButton:hover { background-color: black; color: white; }");

    // Point de Rosée et Style des QLabel rosée
    labelCalculRosee = new QLabel("Point de rosée estimé : () °C"); // Calcul : LA FORMULE DE MAGNUS-TETENS
    labelRosee = new QLabel("Ressenti"); // Ajouter la fonction pour changer le texte en fonction de la température de la rosee

    labelCalculRosee->setAlignment(Qt::AlignCenter);
    labelCalculRosee->setStyleSheet("QLabel { font-size: 16px; }");
    labelRosee->setAlignment(Qt::AlignCenter);
    labelRosee->setStyleSheet("QLabel { font-size: 16px; }");

    // Ajout au layout Menu
    layoutMenu->addLayout(layoutUtilisateur);
    layoutMenu->addWidget(ligne);
    layoutMenu->addWidget(labelDateDuJour);
    layoutMenu->addWidget(labelPeriodeDeLaPrise);
    layoutMenu->addLayout(layoutBoutons);
    layoutMenu->addWidget(btnDate);
    layoutMenu->addWidget(btnRedimension);
    layoutMenu->addWidget(labelCalculRosee);
    layoutMenu->addWidget(labelRosee);
    layoutMenu->addStretch(); // Prend tout l'espace vide disponible

    // --- PAGE GRAPHIQUES ---
    pageGraphique = new QWidget();
    layoutGraph = new QVBoxLayout(pageGraphique); // Layout vertical lié au pageGraphique

    // Création du bouton Retour et du label Graph
    btnRetour = new QPushButton("<- Retour");
    chartView = new QChartView(); // Le conteneur du graphique
    chartView->setRenderHint(QPainter::Antialiasing); // Courbes lisses

    // Ajout au layout Graphique
    layoutGraph->addWidget(btnRetour);
    layoutGraph->addWidget(chartView);

    // --- AJOUT DE TOUTES LES PAGES AU STACK ---
    stack->addWidget(pageIdentification);   // Index 0
    stack->addWidget(pageMenu);             // Index 1
    stack->addWidget(pageGraphique);        // Index 2

    // A ENLEVER QUAND JAI FINI LAPP CEST QUE POUR LES TESTS POUR EVITER DE TAPER UN ID ET MDP A CHAQUE FOIS QUE JE LANCE
    stack->setCurrentIndex(1); // Force l'affichage sur le menu au demarrage

    // --- CONNEXIONS ---
    connect(btnConnexion, &QPushButton::clicked, this, &MainWindow::verifierConnexion);
    connect(btnDeconnexion, &QPushButton::clicked, this, &MainWindow::afficherIdentification);
    connect(btnTemperature, &QPushButton::clicked, this, &MainWindow::preparerGraphique);
    connect(btnHumidite, &QPushButton::clicked, this, &MainWindow::preparerGraphique);
    connect(btnVent, &QPushButton::clicked, this, &MainWindow::preparerGraphique);
    connect(btnDate, &QPushButton::clicked, this, &MainWindow::choisirPeriode);
    connect(btnRedimension, &QPushButton::clicked, this, &MainWindow::preparerGraphique);
    connect(btnRetour, &QPushButton::clicked, this, &MainWindow::afficherMenu);
    connect(timerHeure, &QTimer::timeout, this, &MainWindow::mettreAJourHeure);

    // A ENLEVER POUR L'APPLI ANDROID [
    connect(editPassword, &QLineEdit::returnPressed, btnConnexion, &QPushButton::click); // Si on appuie sur Entrée dans le mot de passe, ça déclenche le bouton
    // ]


    // A SUPPRIMER CAR CEST POUR LA SIMULATION
    QTimer *timerLecture = new QTimer(this);
    connect(timerLecture, &QTimer::timeout, this, &MainWindow::lireFichierSimulateur);
    timerLecture->start(300000);          // 5 minutes (300 000 ms)
    lireFichierSimulateur();
}


void MainWindow::afficherIdentification() {
    // Vide les champs
    editLogin->clear();
    editPassword->clear();
    // Remettre les style normaux (au cas où il y avait une erreur en rouge)
    editPassword->setStyleSheet("padding: 8px; border: 1px solid gray; border-radius: 4px;");
    editPassword->setPlaceholderText("Mot de passe");
    // Basculement sur la page 0 (identification)
    stack->setCurrentIndex(0);
    // Basculement en PORTRAIT si nécessaire
    resize(400, 700);
    // Mettre le focus sur le login pour pouvoir taper directement
    editLogin->setFocus();
}

void MainWindow::afficherMenu() {
    stack->setCurrentIndex(1); // Retour à la page 1 (menu)
    resize(400, 700); // Passer en mode PORTRAIT
}

void MainWindow::preparerGraphique() {
    // QObject::sender() permet de savoir quel bouton a été cliqué
    QPushButton *boutonClique = qobject_cast<QPushButton*>(sender());

    if (boutonClique == btnRedimension) {
        creerGraphiqueGlobal();
    } else if (boutonClique == btnTemperature) {
        creerGraphiqueIndividuel("Température");
    } else if (boutonClique == btnHumidite) {
        creerGraphiqueIndividuel("Humidité");
    } else if (boutonClique == btnVent) {
        creerGraphiqueIndividuel("Vent");
    }
}

void MainWindow::creerGraphiqueIndividuel(QString type) {
    typeGrapheActuel = type;
    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();

    QDateTime maintenant = QDateTime::currentDateTime();
    QDateTime ilYaDeuxHeures = maintenant.addSecs(-7200);
    qint64 limiteTimestamp = ilYaDeuxHeures.toMSecsSinceEpoch();

    QValueAxis *axisY = new QValueAxis();

    // ----- ADAPTATION DE L'ÉCHELLE Y SELON LE TYPE -----
    if (type == "Température") {
        series->setName("Graphique de la Température en °C sur 2 heures");
        series->setColor(Qt::red);
        axisY->setRange(-10, 40); // Pour voir le gel et les fortes chaleurs
        axisY->setTitleText("Température (°C)");
    } else if (type == "Humidité") {
        series->setName("Graphique de l'Humidité en % sur 2 heures");
        series->setColor(Qt::blue);
        axisY->setRange(0, 100);  // Toujours entre 0 et 100%
        axisY->setTitleText("Humidité (%)");
    } else if (type == "Vent") {
        series->setName("Graphique du Vent en km/h sur 2 heures");
        series->setColor(Qt::green);
        axisY->setRange(0, 160);  // Jusqu'à 150 km/h + marge
        axisY->setTitleText("Vent (km/h)");
    }
    // ---------------------------------------------------

    // ----- RÉCUPÉRATION DES DONNÉES (LES 1000 POINTS SERONT FILTRÉS ICI) -----
    for(int i = 0; i < historiqueTemps.size(); i++) {
        qint64 t = historiqueTemps.at(i);
        if (t >= limiteTimestamp) {
            if (type == "Température") series->append(t, historiqueTemp.at(i));
            else if (type == "Humidité") series->append(t, historiqueHum.at(i));
            else if (type == "Vent") series->append(t, historiqueVent.at(i));
        }
    }

    chart->addSeries(series);

    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("HH:mm");
    axisX->setRange(ilYaDeuxHeures, maintenant);
    axisX->setTickCount(9); // Graduation toutes les 15 min
    axisX->setTitleText("Fenêtre glissante (2 heures)");
    chart->addAxis(axisX, Qt::AlignBottom);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chartView->setChart(chart);
    stack->setCurrentIndex(2);
    resize(800, 500);
}

void MainWindow::creerGraphiqueGlobal() {
    typeGrapheActuel = "Global";
    QChart *chart = new QChart();
    chart->setTitle("Archives : Comparaison Globale");

    // --- CRÉATION DES SÉRIES (Bien mettre les noms pour la légende) ---
    QLineSeries *sTemp = new QLineSeries();
    sTemp->setName("Température (°C)");
    sTemp->setColor(Qt::red);

    QLineSeries *sHum = new QLineSeries();
    sHum->setName("Humidité (%)");
    sHum->setColor(Qt::blue);

    QLineSeries *sVent = new QLineSeries();
    sVent->setName("Vent (km/h)");
    sVent->setColor(Qt::green);

    // // ----- RÉCUPÉRATION DES POINTS SIMULATION -----
    qint64 debut = dateDebutSelectionnee.toMSecsSinceEpoch();
    qint64 fin = dateFinSelectionnee.toMSecsSinceEpoch();

    for(int i = 0; i < historiqueTemps.size(); i++) {
        qint64 t = historiqueTemps.at(i);
        if (t >= debut && t <= fin) {
            sTemp->append(t, historiqueTemp.at(i));
            sHum->append(t, historiqueHum.at(i));
            sVent->append(t, historiqueVent.at(i));
        }
    }
    // // ----------------------------------------------

    chart->addSeries(sTemp);
    chart->addSeries(sHum);
    chart->addSeries(sVent);

    // --- CONFIGURATION DES AXES ---
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("dd/MM HH:mm");
    axisX->setRange(dateDebutSelectionnee, dateFinSelectionnee);
    axisX->setTitleText("Période sélectionnée"); // Remet le titre en bas
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(-10, 150);
    axisY->setTitleText("Mesures");
    chart->addAxis(axisY, Qt::AlignLeft);

    sTemp->attachAxis(axisX); sTemp->attachAxis(axisY);
    sHum->attachAxis(axisX);  sHum->attachAxis(axisY);
    sVent->attachAxis(axisX); sVent->attachAxis(axisY);

    // --- CONFIGURATION DE LA LÉGENDE INTERACTIVE ---
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // On récupère tous les marqueurs de la légende
    const auto markers = chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        // On déconnecte d'abord les connexions existantes pour éviter les doublons
        QObject::disconnect(marker, &QLegendMarker::clicked, this, &MainWindow::gererLegendeCliquee);
        // On connecte le clic sur le nom au nouveau Slot
        QObject::connect(marker, &QLegendMarker::clicked, this, &MainWindow::gererLegendeCliquee);
    }

    chartView->setChart(chart);
    stack->setCurrentIndex(2);
    resize(800, 500);
}

void MainWindow::gererLegendeCliquee() {
    // On récupère le marqueur qui a été cliqué
    QLegendMarker* marker = qobject_cast<QLegendMarker*>(sender());

    if (!marker) return;

    // Selon le type de marqueur, on change la visibilité de la série
    switch (marker->type()) {
    case QLegendMarker::LegendMarkerTypeXY: {
        // Inverser la visibilité de la série (si affichée -> cacher, si cachée -> afficher)
        marker->series()->setVisible(!marker->series()->isVisible());

        // On change l'opacité du marqueur pour montrer qu'il est désactivé
        marker->setVisible(true);
        qreal alpha = marker->series()->isVisible() ? 1.0 : 0.5;

        // Appliquer l'opacité au texte et au carré de couleur
        QBrush brush = marker->labelBrush();
        QColor color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);
        break;
    }
    default: break;
    }
}

void MainWindow::verifierConnexion() {
    QString user = editLogin->text();
    QString pass = editPassword->text();
    QString nomAffiche;

    // Simulation de la base de données utilisateurs
    if (user == "admin" && pass == "admin") {
        nomAffiche = "Administrateur";
    }
    else if (user == "ciel" && pass == "ciel") {
        nomAffiche = "BTS CIEL";
    }

    if (!nomAffiche.isEmpty()) {
        // Succès
        labelUtilisateur->setText(nomAffiche);
        afficherMenu(); // On utilise ta fonction existante pour changer de page
    }
    else {
        // Échec : on applique le style d'erreur
        editPassword->setStyleSheet("padding: 8px; border: 2px solid red; border-radius: 4px;");
        editPassword->setPlaceholderText("Identifiants incorrects !");
        editPassword->clear();
        editPassword->setFocus();
    }
}

void MainWindow::mettreAJourHeure() {
    QDateTime maintenant = QDateTime::currentDateTime();
    int minutes = maintenant.time().minute();

    // On calcule l'arrondi aux 5 minutes inférieures
    int minutesArrondies = (minutes / 5) * 5;

    // On crée une heure modifiée avec ces minutes arrondies
    QTime heureAffichage(maintenant.time().hour(), minutesArrondies);

    // On affiche le texte immédiatement
    QString texte = "Aujourd'hui on est le : " + maintenant.toString("dd/MM/yyyy") +
                    " à " + heureAffichage.toString("hh:mm");

    labelDateDuJour->setText(texte);
}

void MainWindow::choisirPeriode() {
    QDialog *fenetreDate = new QDialog(this);
    fenetreDate->setWindowTitle("Choisir la période de mesure");
    QVBoxLayout *diagLayout = new QVBoxLayout(fenetreDate);

    QDateTime maintenant = QDateTime::currentDateTime();

    // SECTION DÉBUT
    diagLayout->addWidget(new QLabel("<b>Début de la période :</b>"));
    QHBoxLayout *layoutDebut = new QHBoxLayout();
    QDateEdit *dateDebut = new QDateEdit(maintenant.date().addDays(-1));
    dateDebut->setCalendarPopup(true);
    dateDebut->setMaximumDate(maintenant.date());
    QTimeEdit *heureDebut = new QTimeEdit(maintenant.time());
    layoutDebut->addWidget(new QLabel("Date :"));
    layoutDebut->addWidget(dateDebut);
    layoutDebut->addWidget(new QLabel("Heure :"));
    layoutDebut->addWidget(heureDebut);
    diagLayout->addLayout(layoutDebut);

    // SECTION FIN
    diagLayout->addWidget(new QLabel("<b>Fin de la période :</b>"));
    QHBoxLayout *layoutFin = new QHBoxLayout();
    QDateEdit *dateFin = new QDateEdit(maintenant.date());
    dateFin->setCalendarPopup(true);
    dateFin->setMaximumDate(maintenant.date());
    QTimeEdit *heureFin = new QTimeEdit(maintenant.time());
    layoutFin->addWidget(new QLabel("Date :"));
    layoutFin->addWidget(dateFin);
    layoutFin->addWidget(new QLabel("Heure :"));
    layoutFin->addWidget(heureFin);
    diagLayout->addLayout(layoutFin);

    // Logique anti-futur
    auto verifierHeure = [=]() {
        if (dateFin->date() >= maintenant.date()) {
            heureFin->setMaximumTime(maintenant.time());
        } else {
            heureFin->setMaximumTime(QTime(23, 59));
        }
    };
    connect(dateFin, &QDateEdit::dateChanged, verifierHeure);
    verifierHeure();

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, fenetreDate);
    diagLayout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, fenetreDate, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, fenetreDate, &QDialog::reject);

    if (fenetreDate->exec() == QDialog::Accepted) {
        // --- MISE À JOUR DES VARIABLES MEMBRES ---
        dateDebutSelectionnee = QDateTime(dateDebut->date(), heureDebut->time());
        dateFinSelectionnee = QDateTime(dateFin->date(), heureFin->time());

        QString debutStr = dateDebutSelectionnee.toString("dd/MM/yyyy HH:mm");
        QString finStr = dateFinSelectionnee.toString("dd/MM/yyyy HH:mm");
        labelPeriodeDeLaPrise->setText("La période de prise est du " + debutStr + " au " + finStr);
    }
}

void MainWindow::calculerPointDeRosee(double temp, double hum) {
    double b = 17.27;
    double c = 237.7;

    double a = log(hum / 100.0) + (b * temp) / (c + temp);
    double pointDeRosee = (c * a) / (b - a);

    labelCalculRosee->setText("Point de rosée estimé : " + QString::number(pointDeRosee, 'f', 1) + " °C");

    QString ressenti;
    if (pointDeRosee < 10) {
        ressenti = "Air sec";
    } else if (pointDeRosee < 15) {
        ressenti = "Air agréable";
    } else if (pointDeRosee < 20) {
        ressenti = "Air humide";
    } else {
        ressenti = "Air très humide";
    }

    labelRosee->setText("Ressenti : " + ressenti);
}

MainWindow::~MainWindow() { }





// A SUPPRIMER CAR CEST POUR LA SIMUALTION
void MainWindow::lireFichierSimulateur() {
    QFile fichier("D:/Users/ebroly1/Documents/dataProjet/donnees_meteo.txt");
    if (fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flux(&fichier);

        // On vide l'historique actuel pour ne pas avoir de doublons
        historiqueTemp.clear();
        historiqueHum.clear();
        historiqueVent.clear();
        historiqueTemps.clear();

        // // ----- PARTIE SIMULATION : LECTURE DE TOUT LE FICHIER -----
        while (!flux.atEnd()) {
            QString ligne = flux.readLine();
            QStringList valeurs = ligne.split(" ");
            if (valeurs.size() == 4) {
                double t = valeurs[0].toDouble();
                double h = valeurs[1].toDouble();
                double v = valeurs[2].toDouble();
                qint64 ts = valeurs[3].toLongLong() * 1000; // Conversion secondes vers millisecondes

                historiqueTemp.append(t);
                historiqueHum.append(h);
                historiqueVent.append(v);
                historiqueTemps.append(ts);

                // On garde les dernières valeurs lues pour les boutons
                btnTemperature->setText(QString("Température :\n %1 °C").arg(t, 0, 'f', 1));
                btnHumidite->setText(QString("Humidité :\n %1 %").arg(h, 0, 'f', 1));
                btnVent->setText(QString("Vent :\n %1 Km/h").arg(v, 0, 'f', 1));
                calculerPointDeRosee(t, h);
            }
        }
        // // -----------------------------------------------------------
        fichier.close();

        // Rafraîchissement auto du graphique si on est dessus
        if (stack->currentIndex() == 2) {
            if (typeGrapheActuel == "Global") creerGraphiqueGlobal();
            else creerGraphiqueIndividuel(typeGrapheActuel);
        }
    }
}
