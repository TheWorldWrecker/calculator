from PIL import Image

# Load the image
image = Image.open('score.png').convert('L')  # Convert to grayscale

# Apply threshold
threshold = 128
binary_image = image.point(lambda p: p > threshold and 255)

# Convert to binary (0, 1) and then to hexadecimal
hex_data = []
for y in range(binary_image.height):
    row = []
    for x in range(binary_image.width):
        pixel = binary_image.getpixel((x, y))
        row.append(1 if pixel == 255 else 0)
    # Convert row of binary data to a single byte
    byte = 0
    for bit in row:
        byte = (byte << 1) | bit
    hex_data.append(f"\\x{byte:02X}")

# Print hexadecimal data
print(''.join(hex_data))
input()
