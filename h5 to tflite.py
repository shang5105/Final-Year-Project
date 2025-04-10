import tensorflow as tf
import numpy as np
import os

# Define paths
base_path = r"D:\Final Year Project\CNN Model\Trained Model"
h5_path = os.path.join(base_path, "Trained_Model_MobileNetV2.h5")  # Your trained model
npy_path = os.path.join(base_path, "Class_Names.npy")  # Your class labels
tflite_path = os.path.join(base_path, "model.tflite")
txt_path = os.path.join(base_path, "labels.txt")  # Text file for Flutter

# **Step 1: Convert .h5 to .tflite**
# Load model
model = tf.keras.models.load_model(h5_path)

# Convert model to TFLite format
converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

# Save TFLite model
with open(tflite_path, "wb") as f:
    f.write(tflite_model)

print("✅ TFLite model saved successfully!")

# **Step 2: Convert .npy to .txt**
# Load class names
class_names = np.load(npy_path, allow_pickle=True)

# Save labels as a text file (one class per line)
with open(txt_path, "w") as f:
    for label in class_names:
        f.write(f"{label}\n")

print("✅ Labels saved as labels.txt")
