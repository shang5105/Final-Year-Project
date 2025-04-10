import numpy as np
import tensorflow.lite as tflite
from PIL import Image

# Load TFLite model
interpreter = tflite.Interpreter(model_path="D:/Final Year Project/CNN Model/Trained Model/model.tflite")
interpreter.allocate_tensors()

# Get input and output tensors
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# Load class names from labels.txt
def load_labels(label_path):
    with open(label_path, "r") as f:
        return [line.strip() for line in f.readlines()]

labels_path = "D:/Final Year Project/CNN Model/Trained Model/labels.txt"
class_names = load_labels(labels_path)  # ✅ Using labels.txt instead of .npy

def preprocess_image(image_path, target_size=(224, 224)):
    """Preprocess image to match model input format"""
    image = Image.open(image_path).convert("RGB")
    image = image.resize(target_size)
    image = np.array(image, dtype=np.float32) / 255.0  # Normalize
    image = np.expand_dims(image, axis=0)  # Add batch dimension
    return image

def run_inference(image_path):
    """Run inference on an image"""
    image = preprocess_image(image_path)

    # Set input tensor
    interpreter.set_tensor(input_details[0]['index'], image)

    # Run inference
    interpreter.invoke()

    # Get output tensor (only one output for classification)
    predictions = interpreter.get_tensor(output_details[0]['index'])
    predicted_class = np.argmax(predictions)  # Get highest probability class
    confidence = np.max(predictions)  # Get confidence score

    return class_names[predicted_class], confidence  # ✅ Uses labels.txt

# Test with an image
image_path = "D:/Final Year Project/CNN Model/img.JPG"  # Update with actual image path
predicted_label, confidence = run_inference(image_path)
print(f"Predicted: {predicted_label} (Confidence: {confidence:.2%})")
