from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from time import sleep
import os
import requests

# Setup website and search query
website = "https://www.google.com/"
crop_disease = "tomato leaf spot disease"

driver = webdriver.Chrome()
driver.maximize_window()
driver.get(website)
sleep(1)

# Search for the crop disease
search_box = driver.find_element(By.NAME, "q")
search_box.send_keys(crop_disease)
search_box.send_keys(Keys.RETURN)
sleep(3)

# Go to the Images tab
images_tab = driver.find_element(By.XPATH, '//*[@id="hdtb-sc"]/div/div[1]/div[1]/div/div[2]/a/div')
images_tab.click()
sleep(3)

# Define the base folder where you want to save the images
base_folder = "D:\\Final Year Project\\Web Scraping"  # Example path, modify this as per your requirement

# Create the main folder and subfolder based on the crop disease name
main_folder = os.path.join(base_folder, "crop_disease_downloaded_images")
sub_folder = os.path.join(main_folder, crop_disease.replace(" ", "_"))

# Create directories if they do not exist
if not os.path.exists(sub_folder):
    os.makedirs(sub_folder)

# Print the full path to the folder
folder_path = os.path.abspath(sub_folder)
print(f"Images will be downloaded to: {folder_path}")

# Get the image elements with only class 'YQ4gaf' (filter out 'zr758c' class)
image_count = 10  # Number of high-quality images to download
thumbnail_elements = driver.find_elements(By.XPATH, "//img[contains(@class, 'YQ4gaf') and not(contains(@class, 'zr758c'))]")[:image_count]

# To avoid downloading duplicate or low-resolution images
downloaded_urls = set()

# Click on each thumbnail to load the high-quality image and download it
for index, thumbnail_elem in enumerate(thumbnail_elements):
    try:
        # Click on the thumbnail to open high-res image in a new tab
        thumbnail_elem.click()
        sleep(5)  # Wait for the high-res image to load
        
        # Get the high-res image URL
        high_res_image = driver.find_element(By.XPATH, "//img[contains(@class, 'n3VNCb') or contains(@class, 'sFlh5c')]")
        img_url = high_res_image.get_attribute("src")
        
        # Skip if the URL has already been downloaded
        if img_url in downloaded_urls:
            print(f"Skipped image {index+1}: duplicate URL.")
            continue

        # Only download images with 'http' in the URL to avoid base64 data URLs
        if img_url and img_url.startswith('http'):
            # Open the image in a new tab
            driver.execute_script(f"window.open('{img_url}', '_blank');")
            sleep(5)  # Wait for the new tab to open

            # Switch to the new tab
            driver.switch_to.window(driver.window_handles[-1])

            # Download the image
            img_data = requests.get(img_url).content
            
            # Check image resolution to ensure it's high quality
            img_size_kb = len(img_data) / 1024  # Calculate size in KB
            if img_size_kb < 50:  # Skip if image size is too small (indicating low quality)
                print(f"Skipped image {index+1}: too small ({img_size_kb:.2f} KB).")
                driver.close()  # Close the tab
                driver.switch_to.window(driver.window_handles[0])  # Switch back to the original tab
                continue

            # Save the image
            img_filename = os.path.join(sub_folder, f"image_{index+1}.jpg")
            with open(img_filename, "wb") as img_file:
                img_file.write(img_data)
            downloaded_urls.add(img_url)

            print(f"Downloaded {img_filename} ({img_size_kb:.2f} KB)")

        else:
            print(f"Skipped image {index+1} due to invalid URL.")

        # Close the high-res view by closing the tab
        driver.close()  # Close the new tab
        driver.switch_to.window(driver.window_handles[0])  # Switch back to the original tab
        sleep(1)

    except Exception as e:
        print(f"Failed to download image {index+1}: {e}")

# Close the browser
driver.quit()
