/*
#include <iostream>
#include <fstream>  // Pour gérer les fichiers
#include <string>
#include <thread>   // Pour la pause (sleep)
#include <chrono>   // Pour le temps
#include <iomanip>  // Pour arrondir les chiffres
#include <ctime>    // Pour le timestamp

using namespace std;

int main() {
	// UTILISE BIEN LE CHEMIN AVEC DES SLASHES / OU DOUBLES ANTI-SLASHES 
    string cheminFichier = "D:/Users/ebroly1/Documents/dataProjet/donnees_meteo.txt";

	cout << "=================================================" << endl;
	cout << "   SIMULATEUR STATION METEO (VISUAL STUDIO)      " << endl;
	cout << "=================================================" << endl;
	cout << "Enregistrement dans : " << cheminFichier << endl;
	cout << "Mode : Ajout (Append) - Les donnees ne sont plus ecrasees." << endl;
	cout << "Appuyez sur Ctrl+C pour arreter le simulateur." << endl;
	cout << "-------------------------------------------------" << endl;

	// Initialisation du generateur de nombres aleatoires
	srand(static_cast<unsigned int>(time(0)));

	while (true) {
		// --- GENERATION DES DONNEES ALEATOIRES ---
		float temp = -10.0f + (rand() % 351) / 10.0f;
		float hum = 0.0f + (rand() % 1001) / 10.0f;
		float vent = 0.0f + (rand() % 1501) / 10.0f;

		// Recuperation du temps actuel (Timestamp Unix)
		long long timestamp = time(0);

		// --- ECRITURE DANS LE FICHIER ---
		// ios::app permet d'ajouter a la fin sans supprimer le contenu existant
		ofstream fichier(cheminFichier, ios::app);

		if (fichier.is_open()) {
			// On force l'ecriture avec 1 seule decimale
			fichier << fixed << setprecision(1);

			// FORMAT : Temp Humidite Vent Timestamp (separes par des espaces)
			fichier << temp << " " << hum << " " << vent << " " << timestamp << endl;

			fichier.close();

			// Affichage console pour verifier que ca tourne
			cout << "Donnee envoyee : " << temp << "C | " << hum << "% | " << vent << "km/h | TS:" << timestamp << endl;
		}
		else {
			cerr << "ERREUR : Impossible d'ouvrir le fichier. Verifiez le chemin !" << endl;
		}

		// --- PAUSE DE 5 SECONDES ---
		// Tu peux changer cette valeur pour accelerer tes tests
		this_thread::sleep_for(chrono::minutes(5));
	}

	return 0;
} */

/*
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;

int main() {
	// Chemin vers ton fichier texte
	string cheminFichier = "D:/Users/ebroly1/Documents/dataProjet/donnees_meteo.txt";

	// ios::app pour ne pas effacer l'existant, ou ios::trunc si tu veux repartir de zéro
	ofstream fichier(cheminFichier, ios::app);

	if (!fichier.is_open()) {
		cerr << "Erreur d'ouverture ! Verifie que le dossier existe." << endl;
		return 1;
	}

	cout << "Generation de 1000 points (intervalle 5 min) en cours..." << endl;

	long long maintenant = time(0);
	srand(static_cast<unsigned int>(maintenant));

	for (int i = 1000; i >= 0; i--) {
		// --- CHANGEMENT ICI : 5 minutes = 300 secondes ---
		long long ts = maintenant - (i * 300);

		// Generation de valeurs selon tes plages (-5 a 30, 0 a 100, 0 a 150)
		float temp = -5.0f + (rand() % 351) / 10.0f;
		float hum = 0.0f + (rand() % 1001) / 10.0f;
		float vent = 0.0f + (rand() % 1501) / 10.0f;

		fichier << fixed << setprecision(1);
		fichier << temp << " " << hum << " " << vent << " " << ts << endl;
	}

	fichier.close();
	cout << "Termine ! 1000 points ajoutes (couvre environ 83 heures de donnees)." << endl;

	return 0;
}
*/
