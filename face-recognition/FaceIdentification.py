import sys
import os
import cv2
from deepface import DeepFace
import tensorflow as tf

def faceIdentification(input_image_path):
    user_images_directory = 'G:/Object-Detection/images'
    images = os.listdir(user_images_directory)
    #print("Les images disponibles: ", images)
   
    # Charger l'image d'entrée
    input_image = cv2.imread(input_image_path)
    if input_image is None:
       
        return "Invalid Image"

    for image in images:
        image_path = os.path.join(user_images_directory, image)
        
        img = cv2.imread(image_path)
        if img is None:
            continue
        try:
            dictionnaty = DeepFace.verify(input_image, img)
            #print(f"Résultat pour {image}: ", dictionnaty)
            
            if dictionnaty['verified']:
                
                return image.split('.')[0]

        except:
            return "Recognition error"
        
            
    return "No match"


if __name__ == "__main__":
    image_path = sys.argv[1]  
    with tf.device("/CPU:0"):
        result = faceIdentification(image_path)
        print(result)
