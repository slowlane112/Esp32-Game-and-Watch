from PIL import Image

input_files = [
	"top_oil_panic.png",
	"bottom_oil_panic.png",
	"top_donkey_kong.png",
	"bottom_donkey_kong.png",
	"top_mickey_and_donald.png",
	"bottom_mickey_and_donald.png",
	"top_green_house.png",
	"bottom_green_house.png",
	"top_donkey_kong_ii.png",
	"bottom_donkey_kong_ii.png",
	"top_pinball.png",
	"bottom_pinball.png",
	"top_black_jack.png",
	"bottom_black_jack.png",
	"top_squish.png",
	"bottom_squish.png",
	"top_bomb_sweeper.png",
	"bottom_bomb_sweeper.png",
	"top_safe_buster.png",
	"bottom_safe_buster.png",
	"top_gold_cliff.png",
	"bottom_gold_cliff.png",
	"top_zelda.png",
	"bottom_zelda.png"

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
