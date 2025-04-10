import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import numpy as np
import os
import matplotlib.pyplot as plt
from PIL import Image, UnidentifiedImageError

# Check if an image file is valid
def is_image_valid(filepath):
    try:
        with Image.open(filepath) as img:
            img.verify()  # Verifies that it's an image
        return True
    except (UnidentifiedImageError, OSError):
        return False

# Function to remove corrupt images from dataset directory
def remove_corrupted_images(folder_path):
    corrupted = 0
    for root, dirs, files in os.walk(folder_path):
        for file in files:
            if file.lower().endswith(('.jpg', '.jpeg')):
                file_path = os.path.join(root, file)
                try:
                    img = Image.open(file_path)
                    img.verify()  # Will raise an exception if corrupt
                except (IOError, SyntaxError):
                    print(f"Removing corrupted image: {file_path}")
                    os.remove(file_path)
                    corrupted += 1
    print(f"Removed {corrupted} corrupted images.")

# Set dataset directory path
dataset_path = "D:/Final Year Project/CNN Model/Dataset"
img_size = (224, 224)
batch_size = 32

# Step 1: Clean dataset from corrupt images
remove_corrupted_images(dataset_path)

# Step 2: Load dataset from directories
train_ds = tf.keras.utils.image_dataset_from_directory(
    dataset_path,
    validation_split=0.2,
    subset="training",
    seed=42,
    image_size=img_size,
    batch_size=batch_size
)

val_ds = tf.keras.utils.image_dataset_from_directory(
    dataset_path,
    validation_split=0.2,
    subset="validation",
    seed=42,
    image_size=img_size,
    batch_size=batch_size
)

# Get class names dynamically
class_names = np.array(train_ds.class_names)
num_classes = len(class_names)
print(f"Classes: {class_names}")

# Save class names for future use
np.save("D:/Final Year Project/CNN Model/Class_Names.npy", class_names)

# Normalize pixel values
normalization_layer = layers.Rescaling(1./255)
train_ds = train_ds.map(lambda x, y: (normalization_layer(x), y))
val_ds = val_ds.map(lambda x, y: (normalization_layer(x), y))

# Prefetching for performance
AUTOTUNE = tf.data.AUTOTUNE
train_ds = train_ds.prefetch(buffer_size=AUTOTUNE)
val_ds = val_ds.prefetch(buffer_size=AUTOTUNE)

# OPTIONAL: This will ignore remaining errors silently
train_ds = train_ds.apply(tf.data.experimental.ignore_errors())
val_ds = val_ds.apply(tf.data.experimental.ignore_errors())

# Build model using MobileNetV2
base_model = tf.keras.applications.MobileNetV2(
    input_shape=(224, 224, 3),
    include_top=False,
    weights='imagenet'
)
base_model.trainable = False

model = keras.Sequential([
    base_model,
    layers.GlobalAveragePooling2D(),
    layers.Dense(128, activation='relu'),
    layers.Dropout(0.3),
    layers.Dense(num_classes, activation='softmax')
])

# Compile Model
model.compile(
    optimizer=keras.optimizers.Adam(),
    loss='sparse_categorical_crossentropy',
    metrics=['accuracy']
)

# Train Model
epochs = 15
history = model.fit(train_ds, validation_data=val_ds, epochs=epochs)

# Print final accuracies
train_accuracy = history.history['accuracy'][-1]
val_accuracy = history.history['val_accuracy'][-1]
print(f"Final Training Accuracy: {train_accuracy:.2%}")
print(f"Final Validation Accuracy: {val_accuracy:.2%}")

# Save Model
model.save("D:/Final Year Project/CNN Model/Trained_Model_MobileNetV2.h5")
print("Model Saved")

# Plot Training & Validation Accuracy
plt.plot(history.history['accuracy'], label="Train Accuracy")
plt.plot(history.history['val_accuracy'], label="Val Accuracy")
plt.xlabel("Epochs")
plt.ylabel("Accuracy")
plt.legend()
plt.title("Training vs Validation Accuracy")
plt.show()
