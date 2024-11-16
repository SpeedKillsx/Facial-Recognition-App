#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Face.hpp"
#include <Python.h>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <sys/stat.h>  // Pour la fonction de vérification de fichier

std::mutex fileMutex;
using namespace std;
using namespace cv;

Rect corrdFace;
std::string temp_image_path;
std::string identifiedFace = "Recognizing...";

Face::Face(cv::Mat frame) {
    this->frame = frame;
    this->haarCascadeFace.load("G:/Object-Detection/haarcascade_frontalface_alt.xml");
}

// Fonction pour vérifier si un fichier existe
bool isFileAvailable(const std::string& filePath) {
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);  // Retourne vrai si le fichier existe
}

void Face::faceDetection() {
    // Vérifier si le fichier Haar Cascade est chargé correctement
    if (this->haarCascadeFace.empty()) {
        std::cout << "--(!)Erreur de chargement du fichier cascade\n";
        return;
    }

    Mat frame_gray;
    resize(this->frame, this->frame, Size(200, 200));

    cvtColor(this->frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    std::vector<Rect> faces;
    this->haarCascadeFace.detectMultiScale(frame_gray, faces);

    if (!faces.empty()) {
        // Si des visages sont détectés, capturez le visage une seule fois
        if (!this->faceCaptured) {
            std::cout << "captured" << std::endl;
            this->faceCaptured = true;  // Définir le drapeau à true pour empêcher la nouvelle sauvegarde
            this->setFace(this->frame);
            temp_image_path = "G:/Object-Detection/build/Release/temp_image.jpg";

            // Vérifier si le fichier est disponible avant d'écrire
            if (!isFileAvailable(temp_image_path)) {
                std::lock_guard<std::mutex> lock(fileMutex);  // Verrouiller pour éviter l'accès concurrent
                cv::imwrite(temp_image_path, this->face);  // Sauvegarder l'image une seule fois
            }
        }

        // Exécuter la reconnaissance faciale une seule fois
        if (!this->isRecognitionStarted) {
            this->launchFaceRecognition();  // Lancer la reconnaissance faciale en arrière-plan
            this->isRecognitionStarted = true;  // Définir le drapeau à true
        }
    }

    // Dessiner le rectangle et afficher le nom autour du visage
    for (const Rect& faceRect : faces) {
        corrdFace.x = faceRect.x;
        corrdFace.y = faceRect.y;
        rectangle(this->frame, faceRect, Scalar(255, 0, 0), 2); // Rectangle bleu, épaisseur 2
        putText(this->frame, identifiedFace, Point(corrdFace.x, corrdFace.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1, 2);
    }
}

std::string Face::faceIdentification() {
    std::string temp_output_path = "G:/Object-Detection/build/Release/output.txt";

    // Créer la commande Python pour exécuter le script
    std::string pythonCommand = "C:/Users/Altea/.conda/envs/ten-gpu/python.exe G:/Object-Detection/face-recognition/FaceIdentification.py " + temp_image_path + " > " + temp_output_path;

    // Utiliser un verrou pour éviter un accès simultané au fichier
    std::lock_guard<std::mutex> lock(fileMutex);  // Verrouiller le mutex ici

    // Exécuter le script Python de manière asynchrone
    int result = std::system(pythonCommand.c_str());
    if (result != 0) {
        std::cerr << "Échec de l'exécution du script Python!" << std::endl;
        return "Erreur d'exécution";
    }

    // Lire le résultat depuis le fichier de sortie temporaire
    std::ifstream inputFile(temp_output_path);
    if (inputFile) {
        if (!identifiedFace._Equal("No match") && !identifiedFace._Equal("Invalid Image") && !identifiedFace._Equal("Recognition error")) {
            std::getline(inputFile, identifiedFace);
            std::cout << "output = " << identifiedFace << std::endl;
            inputFile.close();
        } else {
            std::cerr << identifiedFace << std::endl;
        }
    } else {
        std::cerr << "Erreur: Impossible de lire le fichier de sortie." << std::endl;
        return "Erreur de résultat";
    }

    return identifiedFace;
}

// Lancer la reconnaissance faciale dans un thread en arrière-plan
void Face::launchFaceRecognition() {
    // Démarrer la reconnaissance faciale dans un thread en arrière-plan
    std::thread recognitionThread([this] {
        identifiedFace = this->faceIdentification();
    });

    // Détacher le thread pour qu'il s'exécute indépendamment
    recognitionThread.detach();
}

// Setters et Getters pour le frame et le visage
void Face::setFrame(cv::Mat& frame) {
    this->frame = frame;
}

void Face::setFace(cv::Mat& face) {
    this->face = face;
}

cv::Mat Face::getFrame() {
    return this->frame;
}
