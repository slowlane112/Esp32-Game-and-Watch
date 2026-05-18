from PIL import Image

input_files = [
	"ball.png",
	"vermin.png",
	"fire_silver.png",
	"helmet.png",
	"parachute.png",
	"octopus.png",
	"popeye.png",
	"chef.png",
	"fire.png",
	"turtle_bridge.png",
	"tropical_fish.png",
	"mario_the_juggler.png"
]	

output_raw = "menu.raw"

rgb565_bytes = bytearray()

for input_png in input_files:

    img = Image.open(input_png).convert("RGB")
    
    width, height = img.size
    print(f"{input_png}: {width}, {height}")

    # Convert each image to RGB565 and append
    for r, g, b in img.getdata():
        r5 = (r >> 3) & 0x1F
        g6 = (g >> 2) & 0x3F
        b5 = (b >> 3) & 0x1F

        value = (r5 << 11) | (g6 << 5) | b5

        # Little-endian
        rgb565_bytes.append(value & 0xFF)
        rgb565_bytes.append((value >> 8) & 0xFF)


with open(output_raw, "wb") as f:
    f.write(rgb565_bytes)

print("Done. Wrote", len(rgb565_bytes), "bytes total.")
